#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "BTree.h"
#include "Person.h"

class PageManager : protected std::fstream // FStream methods and attributes are going to be protected
{

public:
    // Constructor
    PageManager(const char* filename)
    {
        this->filename = filename;
        open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!is_open()) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        }
        else {
            std::cout << "archivo abierto good " << filename << std::endl;
        }
    };

    // Destructor
    ~PageManager()
    {
        if (is_open()) {
            close();
        }
    }

    /// @brief Get the size of a the file
    /// @return the size of a file
    long GetFileSize() {
        clear();
        seekg(0, std::ios::end);
        return tellg();
    }

    Personita ReadGetObjectByPageID(const long& pageID)
    {
        Personita person;
        clear();
        seekg(pageID * sizeof(person), std::ios::beg);
        read(reinterpret_cast<char*>(&person), sizeof(person));
        if (!good()) {
            std::cerr << "Error al leer page: " << pageID << std::endl;
        }
        return person;
    };

    void ReadFileAndLoadToBtree(BTree& tree) {
        if (!is_open()) {
            std::cerr << "Error: el archivo no esta abierto." << std::endl;
        }

        clear(); // Limpiar cualquier bandera de error anterior
        seekg(0, std::ios::beg); // Mover el puntero al inicio del archivo

        //LoadDataToBTree(tree);
        
        Personita person;
        const size_t chunkSize = 10000; // Leer 10000 registros a la vez
        std::vector<Personita> buffer(chunkSize);

        while (true) {
            read(reinterpret_cast<char*>(buffer.data()), chunkSize * sizeof(Personita));
            std::streamsize bytesRead = gcount();

            if (bytesRead == 0) {
                break;
            }

            size_t recordsRead = bytesRead / sizeof(Personita);

            for (size_t i = 0; i < recordsRead; ++i) {
                person = buffer[i];
                tree.Insert(person.dni, person.pageID);
            }

            if (bytesRead < static_cast<std::streamsize>(chunkSize * sizeof(Personita))) {
                break;
            }
        }
        

        if (fail() && !eof()) {
            std::cerr << "Error al leer el archivo." << std::endl;
        }
    }

    void LoadDataToBTree(BTree& tree)
    {
        Personita person;
        while (read(reinterpret_cast<char*>(&person), sizeof(Personita))) {
            if (good()) {
                tree.Insert(person.dni, person.pageID);
            }
            else {
                std::cerr << "Error al leer una entrada en el archivo." << std::endl;
                break;
            }
        }
    }

    void AddNewPerson(BTree& tree, Personita& person) {
        // Verificar el tamaño del archivo para determinar el nuevo pageID
        long newPageID = GetFileSize() / sizeof(Personita);

        // Asignar el nuevo pageID a la persona
        person.pageID = newPageID;

        WriteObjectInDisk(newPageID, person);

        if (!good()) {
            std::cerr << "Error al escribir el nuevo registro en el archivo." << std::endl;
            return;
        }

        // Insertar la nueva persona en el B-Tree
        tree.Insert(person.dni, person.pageID);
    }

    void WriteObjectInDisk(long newPageID, Personita& person)
    {
        // Escribir el nuevo registro en el archivo
        clear();
        seekp(newPageID * sizeof(Personita), std::ios::beg);
        write(reinterpret_cast<char*>(&person), sizeof(person));
    }

    void DeleteRecordFromDisk(long pageID, BTree& tree) {
        // Leer el registro del archivo para obtener el DNI antes de eliminarlo
        Personita personToRemove = ReadGetObjectByPageID(pageID);
        std::string dniToRemove = personToRemove.dni;

        // Nombre del archivo temporal
        const char* tempFilename = "temp.bin";

        // Crear archivo temporal
        std::fstream tempFile(tempFilename, std::ios::out | std::ios::binary);
        if (!tempFile.is_open()) {
            std::cerr << "Error al abrir el archivo temporal." << std::endl;
            return;
        }

        // Leer registros del archivo original y copiar los que no se van a eliminar
        clear();
        seekg(0, std::ios::beg); // Mover el puntero al inicio del archivo
        Personita person;
        long currentID = 0;

        while (read(reinterpret_cast<char*>(&person), sizeof(Personita))) {
            if (currentID != pageID) {
                tempFile.write(reinterpret_cast<char*>(&person), sizeof(person));
            }
            currentID++;
        }

        if (fail() && !eof()) {
            std::cerr << "Error al leer el archivo original." << std::endl;
            tempFile.close();
            std::remove(tempFilename); // Eliminar el archivo temporal en caso de error
            return;
        }

        // Cerrar archivos
        tempFile.close();
        close();

        // Eliminar el archivo original y renombrar el archivo temporal
        if (std::remove(filename) != 0) {
            std::cerr << "Error al eliminar el archivo original." << std::endl;
            std::remove(tempFilename); // Eliminar el archivo temporal si falla
            return;
        }

        if (std::rename(tempFilename, filename) != 0) {
            std::cerr << "Error al renombrar el archivo temporal." << std::endl;
            return;
        }

        // Reabrir el archivo original como binario
        open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!is_open()) {
            std::cerr << "Error al reabrir el archivo: " << filename << std::endl;
        }

        // Eliminar el registro del B-Tree utilizando el DNI obtenido
        tree.Remove(dniToRemove.c_str());
    }

    /* page manager Version 1.0.0 methods
    template <typename T>
    T readPage(const long& n, const T& object)
    {
        T object;
        clear();
        seekg(n * sizeof(T), std::ios::beg);
        read(reinterpret_cast<char*>(&object), sizeof(T));
        if (!good()) {
            std::cerr << "Error al leer page: " << n << std::endl;
        }
        return object;
    }

    template <typename T>
    void writePage(const long& n, std::vector<T>& p)
    {
        clear();
        for (size_t i = 0; i < p.size(); i++)
        {
            seekp(i * sizeof(p[i]), std::ios::beg);
            p[i].id = i;
            write(reinterpret_cast<char*>(&p[i]), sizeof(p[i]));
            if (!good()) {
                std::cerr << "Error al escribir en page: " << n << std::endl;
            }
        }
    }
    */

private:
    // Private member variable to hold the value
    const char* filename;
};