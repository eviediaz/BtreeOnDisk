#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "BTree.h"
#include "Person.h"
#include <iomanip>

class PageManager : protected std::fstream // FStream methods and attributes are going to be protected
{

public:
    // Constructor
    PageManager(const char* filename)
    {
        this->filename = filename;
        this->file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!this->file.is_open()) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        }
        else {
            std::cout << "archivo abierto good " << filename << std::endl;
        }
    };

    // Destructor
    ~PageManager()
    {
        if (this->file.is_open()) {
            this->file.close();
        }
    }

    /// <summary>
    /// open the file and get the syze of the file in bytes 
    /// </summary>
    /// <returns> returns the size of the file </returns>
    std::uintmax_t GetFileSize() {
        std::ifstream in(this->filename, std::ios::binary | std::ios::ate);
        if (!in.is_open()) {
            std::cerr << "Error al abrir el archivo para obtener el tamanio." << std::endl;
            return -1;
        }
        std::uintmax_t fileSize = in.tellg();
        in.close();
        std::cout << "Tamanio del archivo: " << fileSize << " bytes" << std::endl;
        return fileSize;
    }

    /// <summary>
    /// read the file and search it's location in disk by it's pageID
    /// </summary>
    /// <param name="pageID"></param>
    /// <returns> returns an 'Personita' object </returns>
    Personita ReadFileGetPersonByPageID(const long& pageID)
    {
        Personita person;
        this->file.clear();
        this->file.seekg(pageID * sizeof(person), std::ios::beg);
        this->file.read(reinterpret_cast<char*>(&person), sizeof(person));
        if (!this->file.good()) {
            std::cerr << "Error al leer page: " << pageID << std::endl;
        }
        return person;
    };

    /// <summary>
    /// Read the file by a given chunk, reads the chunk and insert the data in the BTree
    ///  and load it to RAM
    /// </summary>
    /// <param name="tree"></param>
    void ReadFileAndLoadToBtree(BTree& tree) {
        if (!this->file.is_open()) {
            std::cerr << "Error: el archivo no esta abierto." << std::endl;
        }

        this->file.clear(); // Limpiar cualquier bandera de error anterior
        this->file.seekg(0, std::ios::beg); // Mover el puntero al inicio del archivo        
        
        Personita person;
        const size_t chunkSize = 10000; // Leer 10000 registros a la vez
        std::vector<Personita> buffer(chunkSize);

        while (true) {
            this->file.read(reinterpret_cast<char*>(buffer.data()), chunkSize * sizeof(Personita));
            std::streamsize bytesRead = this->file.gcount();

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

        if (this->file.fail() && !this->file.eof()) {
            std::cerr << "Error al leer el archivo." << std::endl;
        }
    }
    
    void SerializeBTree(BTree& tree, const char* outputFilename) {
        std::ofstream outFile(outputFilename, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Error al abrir el archivo de salida: " << outputFilename << " - " << strerror(errno) << std::endl;
            return;
        }
        SerializeBTreeNode(tree.GetRoot(), outFile);
        outFile.close();
        std::cout << "\nB-Tree serializado en: " << outputFilename << std::endl;
    }
    

    void DeserializeBTree(BTree& tree, const char* inputFilename) {
        std::ifstream inFile(inputFilename, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Error al abrir el archivo de entrada: " << inputFilename << " - " << strerror(errno) << std::endl;
            return;
        }
        BTreeNode* root = DeserializeBTreeNode(inFile, tree.GetMinimunDegree());
        tree.SetRoot(root);
        inFile.close();
        std::cout << "\nB-Tree deserializado desde: " << inputFilename << std::endl;
    }

    /// <summary>
    /// Write a 'Personita' object (478 bytes) in the file
    /// </summary>
    /// <param name="newPageID"></param>
    /// <param name="person"></param>
    void WritePersonitaInDisk(long newPageID, Personita& person)
    {
        // Escribir el nuevo registro en el archivo
        this->file.clear();
        this->file.seekp(newPageID * sizeof(Personita), std::ios::beg);
        this->file.write(reinterpret_cast<char*>(&person), sizeof(person));

        if (!this->file.good()) {
            std::cerr << "Error al escribir en el archivo " << filename << std::endl;
        }
    }

    /// <summary>
    /// Writes a given person in disk and insert it to BTree
    /// </summary>
    /// <param name="tree"></param>
    /// <param name="person"></param>
    /// <param name="outputFilename"></param>
    void InsertNewRecordInDiskAndBTree(BTree& tree, Personita& person, const char* outputFilename) {
        // Verificar el tamaño del archivo para determinar el nuevo pageID
        long newPageID = GetFileSize() / sizeof(Personita);

        // Asignar el nuevo pageID a la persona
        person.pageID = newPageID;

        WritePersonitaInDisk(newPageID, person);

        if (!this->file.good()) {
            std::cerr << "Error al escribir el nuevo registro en el archivo." << std::endl;
            return;
        }

        // Insertar la nueva persona en el B-Tree
        tree.Insert(person.dni, person.pageID);
    }

    // TODO: Change logic
    void DeleteRecordFromDisk(BTree& tree, const char* dni, const char* btreeFilename) {
        // Paso 1: Buscar el registro por DNI en el archivo 'people.bin'
        long pageID = tree.SearchDNIAndGetPageID(dni);

        if (pageID < 0) {
            std::cerr << "Registro con DNI " << dni << " no encontrado." << std::endl;
            return;
        }
        else {
            std::cout << "Registro con DNI " << dni << " encontrado en " << pageID << std::endl;
        }

        // Paso 2: Marcar el registro como eliminado en 'people.bin'
        MarkRecordAsDeleted(pageID);

        if (!this->file.good()) {
            std::cerr << "Error al marcar el registro como eliminado en el archivo." << std::endl;
            return;
        }

        // Paso 3: Eliminar la clave del B-Tree
        tree.Remove(dni);
    }

    void PrintOneHundredRecords(const char* filename, int inicio, int final) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error abriendo el archivo " << filename << std::endl;
            return;
        }

        Personita persona;
        int count = 0;
        int records_to_skip = inicio;

        std::cout << "--------------------------------------------------------------------------------------\n";
        std::cout << "| DNI       | Nombre                | Apellido             | Edad | Correo                     |\n";
        std::cout << "--------------------------------------------------------------------------------------\n";

        // Saltar los registros hasta el índice de inicio
        while (file.read(reinterpret_cast<char*>(&persona), sizeof(Personita)) && count < records_to_skip) {
            count++;
        }

        // Imprimir los registros desde el índice de inicio hasta el índice final
        count = 0;
        while (file.read(reinterpret_cast<char*>(&persona), sizeof(Personita)) && count <= (final - inicio)) {
            std::cout << "| " << std::setw(10) << std::left << persona.dni
                << " | " << std::setw(20) << std::left << persona.name
                << " | " << std::setw(20) << std::left << persona.lastname
                << " | " << std::setw(4) << std::left << persona.edad
                << " | " << std::setw(25) << std::left << persona.email
                << " |\n";
            count++;
        }
        std::cout << "--------------------------------------------------------------------------------------\n";
        file.close();
    }

    void RangeSearchAndPrint(BTree& tree, const std::string& start, const std::string& end) {
        std::chrono::time_point<std::chrono::system_clock> inicio;
        std::chrono::time_point<std::chrono::system_clock> fin;

        inicio = std::chrono::system_clock::now();
        std::vector<std::string> dnisInRange = tree.RangeSearch(start, end);
        fin = std::chrono::system_clock::now();

        std::chrono::duration<double> tiempo = fin - inicio;
        double tiempo_segundos = tiempo.count();
        std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";

        std::cout << "DNIs encontrados en el rango [" << start << ", " << end << "]:" << std::endl;

        size_t totalRecords = dnisInRange.size();
        size_t recordsPerPage = 100;
        size_t currentPage = 0;
        bool morePages = true;

        while (morePages && currentPage * recordsPerPage < totalRecords) {
            std::cout << "--------------------------------------------------------------------------------------\n";
            std::cout << "| DNI       | Nombre                | Apellido             | Edad | Correo                     |\n";
            std::cout << "--------------------------------------------------------------------------------------\n";

            size_t startIdx = currentPage * recordsPerPage;
            size_t endIdx = std::min(startIdx + recordsPerPage, totalRecords);

            for (size_t i = startIdx; i < endIdx; ++i) {
                const auto& dni = dnisInRange[i];
                int pageID = tree.SearchDNIAndGetPageID(dni.c_str());
                if (pageID != -1) {
                    Personita person = ReadFileGetPersonByPageID(pageID);
                    std::cout << "| " << std::setw(10) << std::left << person.dni
                        << " | " << std::setw(20) << std::left << person.name
                        << " | " << std::setw(20) << std::left << person.lastname
                        << " | " << std::setw(4) << std::left << person.edad
                        << " | " << std::setw(25) << std::left << person.email
                        << " |\n";
                }
            }

            std::cout << "--------------------------------------------------------------------------------------\n";
            std::cout << "Mostrando registros " << startIdx + 1 << " a " << endIdx << " de " << totalRecords << std::endl;

            if (endIdx < totalRecords) {
                char choice;
                std::cout << "¿Desea ver los siguientes 100 registros? (s/n): ";
                std::cin >> choice;
                if (choice == 's' || choice == 'S') {
                    currentPage++;
                }
                else {
                    morePages = false;
                }
            }
            else {
                morePages = false;
            }
        }

        std::cout << "Total DNIs mostrados: " << std::min(totalRecords, (currentPage + 1) * recordsPerPage) << std::endl;
    }

private:
    // Private member variable to hold the value
    std::fstream file;
    const char* filename;

    void MarkRecordAsDeleted(long pageID) {
        // Crear una cadena de "DNI eliminado"
        const char deletedDNI[9] = "DELETED";

        // Mover el puntero a la posición del registro a eliminar
        this->file.clear();
        this->file.seekp(pageID * sizeof(Personita), std::ios::beg);

        // Leer el registro actual
        Personita person;
        this->file.read(reinterpret_cast<char*>(&person), sizeof(person));

        // Marcar el registro como eliminado
        std::memcpy(person.dni, deletedDNI, sizeof(deletedDNI) - 1);
        person.dni[sizeof(deletedDNI) - 1] = '\0'; // Agregar el carácter nulo al final

        // Escribir el registro modificado de nuevo en el archivo
        this->file.seekp(pageID * sizeof(Personita), std::ios::beg);
        this->file.write(reinterpret_cast<char*>(&person), sizeof(person));

        if (!this->file.good()) {
            std::cerr << "Error al marcar el registro como eliminado en el archivo." << std::endl;
        }
    }

    void SerializeBTreeNode(BTreeNode* node, std::ofstream& outFile) {
        if (node == nullptr) return;

        // Escribir si el nodo es una hoja
        outFile.write(reinterpret_cast<char*>(&node->isLeaf), sizeof(node->isLeaf));
        // Escribir el numero de claves en el nodo
        outFile.write(reinterpret_cast<char*>(&node->actualNumberKeys), sizeof(node->actualNumberKeys));

        // Escribir las claves y los identificadores de pagina
        for (int i = 0; i < node->actualNumberKeys; ++i) {
            outFile.write(node->dnis[i].data(), 9); // 9 bytes para el DNI
            outFile.write(reinterpret_cast<char*>(&node->pagesID[i]), sizeof(node->pagesID[i]));
        }

        // Si el nodo no es una hoja, hay que escribir también sus hijos
        if (!node->isLeaf) {
            for (int i = 0; i <= node->actualNumberKeys; ++i) {
                SerializeBTreeNode(node->children[i], outFile); // Serializar los nodos hijos recursivamente
            }
        }
    }


    BTreeNode* DeserializeBTreeNode(std::ifstream& inFile, int minimunDegree) {
        bool isLeaf;
        int actualNumberKeys;
        inFile.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
        inFile.read(reinterpret_cast<char*>(&actualNumberKeys), sizeof(actualNumberKeys));

        BTreeNode* node = new BTreeNode(minimunDegree, isLeaf);
        node->actualNumberKeys = actualNumberKeys;

        for (int i = 0; i < actualNumberKeys; ++i) {
            inFile.read(node->dnis[i].data(), 9);
            inFile.read(reinterpret_cast<char*>(&node->pagesID[i]), sizeof(node->pagesID[i]));
        }

        if (!isLeaf) {
            for (int i = 0; i <= actualNumberKeys; ++i) {
                node->children[i] = DeserializeBTreeNode(inFile, minimunDegree);
            }
        }

        return node;
    }
};