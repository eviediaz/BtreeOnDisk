#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
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
    Personita ReadGetObjectByPageID(const long& pageID)
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

    void SerializeTree(BTreeNode* node, std::ofstream& outFile) {
        if (node == nullptr) return;
        outFile.write(reinterpret_cast<char*>(&node->isLeaf), sizeof(node->isLeaf));
        outFile.write(reinterpret_cast<char*>(&node->actualNumberKeys), sizeof(node->actualNumberKeys));

        for (int i = 0; i < node->actualNumberKeys; ++i) {
            outFile.write(node->dnis[i].data(), 9); // 9 bytes para el DNI
            outFile.write(reinterpret_cast<char*>(&node->pagesID[i]), sizeof(node->pagesID[i]));
        }

        if (!node->isLeaf) {
            for (int i = 0; i <= node->actualNumberKeys; ++i) {
                SerializeTree(node->children[i], outFile); // Serializar los nodos hijos recursivamente
            }
        }
    }

    BTreeNode* DeserializeTree(std::ifstream& inFile, int minimunDegree) {
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
                node->children[i] = DeserializeTree(inFile, minimunDegree);
            }
        }

        return node;
    }

    void SerializeBTree(BTree& tree, const char* outputFilename) {
        std::ofstream outFile(outputFilename, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Error al abrir el archivo de salida: " << outputFilename << " - " << strerror(errno) << std::endl;
            return;
        }
        SerializeTree(tree.GetRoot(), outFile);
        outFile.close();
        std::cout << "B-Tree serializado en: " << outputFilename << std::endl;
    }

    void DeserializeBTree(BTree& tree, const char* inputFilename) {
        std::ifstream inFile(inputFilename, std::ios::binary);
        if (!inFile.is_open()) {
            std::cerr << "Error al abrir el archivo de entrada: " << inputFilename << " - " << strerror(errno) << std::endl;
            return;
        }
        BTreeNode* root = DeserializeTree(inFile, tree.GetMinimunDegree());
        tree.SetRoot(root);
        inFile.close();
        std::cout << "B-Tree deserializado desde: " << inputFilename << std::endl;
    }

    // TODO: Change logic
    void AddNewPerson(BTree& tree, Personita& person) {
        // Verificar el tamaño del archivo para determinar el nuevo pageID
        long newPageID = GetFileSize() / sizeof(Personita);

        // Asignar el nuevo pageID a la persona
        person.pageID = newPageID;

        WriteObjectInDisk(newPageID, person);

        if (!this->file.good()) {
            std::cerr << "Error al escribir el nuevo registro en el archivo." << std::endl;
            return;
        }

        // Insertar la nueva persona en el B-Tree
        tree.Insert(person.dni, person.pageID);
    }

    /// <summary>
    /// Write a 'Personita' object (478 bytes) in the file
    /// </summary>
    /// <param name="newPageID"></param>
    /// <param name="person"></param>
    void WriteObjectInDisk(long newPageID, Personita& person)
    {
        // Escribir el nuevo registro en el archivo
        this->file.clear();
        this->file.seekp(newPageID * sizeof(Personita), std::ios::beg);
        this->file.write(reinterpret_cast<char*>(&person), sizeof(person));
    }

    // TODO: Change logic
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
        this->file.clear();
        this->file.seekg(0, std::ios::beg); // Mover el puntero al inicio del archivo
        Personita person;
        long currentID = 0;

        while (this->file.read(reinterpret_cast<char*>(&person), sizeof(Personita))) {
            if (currentID != pageID) {
                tempFile.write(reinterpret_cast<char*>(&person), sizeof(person));
            }
            currentID++;
        }

        if (this->file.fail() && !this->file.eof()) {
            std::cerr << "Error al leer el archivo original." << std::endl;
            tempFile.close();
            std::remove(tempFilename); // Eliminar el archivo temporal en caso de error
            return;
        }

        // Cerrar archivos
        tempFile.close();
        this->file.close();

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
        this->file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!this->file.is_open()) {
            std::cerr << "Error al reabrir el archivo: " << filename << std::endl;
        }

        // Eliminar el registro del B-Tree utilizando el DNI obtenido
        tree.Remove(dniToRemove.c_str());
    }

private:
    // Private member variable to hold the value
    std::fstream file;
    const char* filename;
};