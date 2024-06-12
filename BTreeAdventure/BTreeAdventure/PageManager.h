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

        LoadDataToBTree(tree);

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