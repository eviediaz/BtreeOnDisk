#pragma once
#include <vector>
#include <windows.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include "BTree.h"
#include "Person.h"
#include <iomanip>

// Función para crear un archivo binario
bool createBinaryFile(const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error al crear el archivo: " << filename << std::endl;
        return false;
    }
    file.close();
    std::cout << "Archivo binario creado exitosamente: " << filename << std::endl;
    return true;
}

class PageManager : protected std::fstream // FStream methods and attributes are going to be protected
{

public:
    // Constructor
    PageManager(const char* filename)
        : filename(filename), hFile(INVALID_HANDLE_VALUE), hMapFile(NULL), pBuf(NULL), fileSize(0) {

        // Verificar el nombre del archivo y la ruta
        if (filename == NULL || strlen(filename) == 0) {
            std::cerr << "Error: Nombre de archivo invalido" << std::endl;
            return;
        }

        // Intentar abrir el archivo con la ruta absoluta proporcionada
        hFile = CreateFileA(
            filename,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            return;
        }

        // Verificar el tamaño del archivo
        LARGE_INTEGER fileSizeLarge;
        if (!GetFileSizeEx(hFile, &fileSizeLarge)) {
            std::cerr << "Error al obtener el tamanio del archivo: " << filename << std::endl;
            CloseHandle(hFile);
            return;
        }
        fileSize = static_cast<size_t>(fileSizeLarge.QuadPart);

        // Si el tamaño del archivo es cero, establecer un tamaño inicial
        if (fileSize == 0) {
            const int initialSize = 1024; // Tamaño inicial en bytes
            if (!SetFilePointerEx(hFile, { initialSize }, NULL, FILE_BEGIN) || !SetEndOfFile(hFile)) {
                std::cerr << "Error al establecer el tamanio inicial del archivo: " << filename << std::endl;
                CloseHandle(hFile);
                return;
            }
            fileSize = initialSize;
        }

        hMapFile = CreateFileMappingA(
            hFile,
            NULL,
            PAGE_READWRITE,
            0,
            0,
            NULL);

        if (hMapFile == NULL) {
            std::cerr << "Error al crear el mapeo de archivo: " << filename << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            CloseHandle(hFile);
            return;
        }

        pBuf = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            0);

        if (pBuf == NULL) {
            std::cerr << "Error al mapear la vista del archivo: " << filename << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            CloseHandle(hMapFile);
            CloseHandle(hFile);
            return;
        }

        std::cout << "Archivo abierto y mapeado exitosamente: " << filename << std::endl;
    }

    ~PageManager() {
        if (pBuf != NULL) {
            UnmapViewOfFile(pBuf);
            pBuf = NULL;
        }

        if (hMapFile != NULL) {
            CloseHandle(hMapFile);
            hMapFile = NULL;
        }

        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
        }

        std::cout << "Recursos de archivo liberados correctamente." << std::endl;
    }

    /// <summary>
    /// open the file and get the syze of the file in bytes 
    /// </summary>
    /// <returns> returns the size of the file </returns>
    std::uintmax_t GetFileSize() {
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(hFile, &fileSize)) {
            return static_cast<std::uintmax_t>(fileSize.QuadPart);
        }
        else {
            std::cerr << "Error al obtener el tamaño del archivo." << std::endl;
            return -1;
        }
    }

    /// <summary>
    /// read the file and search it's location in disk by it's pageID
    /// </summary>
    /// <param name="pageID"></param>
    /// <returns> returns an 'Personita' object </returns>
    Personita ReadGetObjectByPageID(const long& pageID)
    {

    };

    /// <summary>
    /// Read the file by a given chunk, reads the chunk and insert the data in the BTree
    ///  and load it to RAM
    /// </summary>
    /// <param name="tree"></param>
    void ReadFileAndLoadToBtree(BTree& tree) {
        // Verificar que los manejadores y el buffer están correctamente inicializados
        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error: archivo no abierto." << std::endl;
            return;
        }

        if (hMapFile == NULL) {
            std::cerr << "Error: mapeo de archivo no creado." << std::endl;
            return;
        }

        if (pBuf == NULL) {
            std::cerr << "Error: vista del archivo no mapeada." << std::endl;
            return;
        }

        // Obtener el tamaño del archivo (de nuevo, por si ha cambiado desde la apertura)
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(hFile, &fileSize)) {
            std::cerr << "Error al obtener el tamaño del archivo." << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            return;
        }

        // Leer los datos y cargarlos en el B-Tree
        size_t recordSize = sizeof(char) * 8 + sizeof(long);  // Tamaño del registro: dni (8 chars) + pageID (long)
        size_t numberOfRecords = fileSize.QuadPart / recordSize;

        for (size_t i = 0; i < numberOfRecords; ++i) {
            char dni[8];
            long pageID;

            // Leer dni
            memcpy(dni, pBuf + i * recordSize, sizeof(dni));
            // Leer pageID
            memcpy(&pageID, pBuf + i * recordSize + sizeof(dni), sizeof(pageID));

            // Insertar en el B-Tree
            tree.Insert(dni, pageID);
        }
    }

    void SerializeTree(BTreeNode* node, const char* filename) {
        if (node == nullptr) return;

        // Crear o abrir el archivo
        HANDLE hFile = CreateFileA(
            filename,
            GENERIC_WRITE | GENERIC_READ,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error al abrir o crear el archivo." << std::endl;
            return;
        }

        // Crear un mapeo de archivo
        HANDLE hMapFile = CreateFileMappingA(
            hFile,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(BTreeNode) * 1024, // Tamaño del archivo que se va a mapear
            NULL);

        if (hMapFile == NULL) {
            std::cerr << "Error al crear el mapeo de archivo." << std::endl;
            CloseHandle(hFile);
            return;
        }

        // Mapear una vista del archivo en la memoria
        char* pBuf = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            0);

        if (pBuf == NULL) {
            std::cerr << "Error al mapear la vista del archivo en la memoria." << std::endl;
            CloseHandle(hMapFile);
            CloseHandle(hFile);
            return;
        }

        // Serializar el árbol
        SerializeTreeRecursive(node, pBuf);

        // Desmapear la vista del archivo
        UnmapViewOfFile(pBuf);

        // Cerrar los handles
        CloseHandle(hMapFile);
        CloseHandle(hFile);
    }

    void SerializeTreeRecursive(BTreeNode* node, char*& pBuf) {
        if (node == nullptr) return;

        // Escribir si el nodo es una hoja
        memcpy(pBuf, &node->isLeaf, sizeof(node->isLeaf));
        pBuf += sizeof(node->isLeaf);

        // Escribir el número de claves en el nodo
        memcpy(pBuf, &node->actualNumberKeys, sizeof(node->actualNumberKeys));
        pBuf += sizeof(node->actualNumberKeys);

        // Escribir las claves y los identificadores de página
        for (int i = 0; i < node->actualNumberKeys; ++i) {
            memcpy(pBuf, node->dnis[i].data(), 9); // 9 bytes para el DNI
            pBuf += 9;
            memcpy(pBuf, &node->pagesID[i], sizeof(node->pagesID[i]));
            pBuf += sizeof(node->pagesID[i]);
        }

        // Si el nodo no es una hoja, escribir también sus hijos
        if (!node->isLeaf) {
            for (int i = 0; i <= node->actualNumberKeys; ++i) {
                SerializeTreeRecursive(node->children[i], pBuf); // Serializar los nodos hijos recursivamente
            }
        }
    }

    /// <summary>
    /// Write a 'Personita' object (478 bytes) in the file
    /// </summary>
    /// <param name="newPageID"></param>
    /// <param name="person"></param>
    void WritePersonitaInDisk(long newPageID, Personita& person)
    {
        // Verificar el tamaño del archivo
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(hFile, &fileSize)) {
            std::cerr << "Error al obtener el tamanio del archivo: " << this->filename << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            return;
        }

        // Si el archivo es más pequeño que la posición donde queremos escribir, aumentar el tamaño del archivo
        long requiredSize = (newPageID + 1) * sizeof(Personita);
        if (fileSize.QuadPart < requiredSize) {
            LARGE_INTEGER newSize;
            newSize.QuadPart = requiredSize;
            if (!SetFilePointerEx(hFile, newSize, NULL, FILE_BEGIN) || !SetEndOfFile(hFile)) {
                std::cerr << "Error al aumentar el tamanio del archivo." << std::endl;
                std::cerr << "Codigo de error: " << GetLastError() << std::endl;
                return;
            }
        }

        // Obtener el tamaño de página del sistema
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        DWORD pageSize = systemInfo.dwAllocationGranularity;

        // Calcular el offset alineado
        DWORD offsetLow = (newPageID * sizeof(Personita)) & ~(pageSize - 1);
        DWORD offsetHigh = ((newPageID * sizeof(Personita)) >> 32) & ~(pageSize - 1);
        DWORD alignmentOffset = (newPageID * sizeof(Personita)) & (pageSize - 1);

        // Mapear la vista del archivo en la memoria con el offset alineado
        char* pBufLocal = (char*)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            offsetHigh,
            offsetLow,
            alignmentOffset + sizeof(Personita));

        if (pBufLocal == NULL) {
            std::cerr << "Error al mapear la vista del archivo en la memoria." << std::endl;
            std::cerr << "Codigo de error: " << GetLastError() << std::endl;
            return;
        }

        // Ajustar el puntero para la alineación correcta
        pBufLocal += alignmentOffset;

        // Escribir el nuevo registro en la memoria mapeada
        memcpy(pBufLocal, &person, sizeof(Personita));

        // Desmapear la vista del archivo
        UnmapViewOfFile(pBufLocal - alignmentOffset);
        
    }


    BTreeNode* DeserializeTree(char*& pBuf, size_t bufferSize, size_t& currentOffset, int minimunDegree) {
        if (currentOffset + sizeof(bool) + sizeof(int) > bufferSize) {
            std::cerr << "Error: buffer overflow al deserializar el nodo." << std::endl;
            return nullptr;
        }

        bool isLeaf;
        int actualNumberKeys;

        // Leer isLeaf y actualNumberKeys desde el buffer mapeado
        memcpy(&isLeaf, pBuf + currentOffset, sizeof(isLeaf));
        currentOffset += sizeof(isLeaf);

        memcpy(&actualNumberKeys, pBuf + currentOffset, sizeof(actualNumberKeys));
        currentOffset += sizeof(actualNumberKeys);

        // Crear el nodo BTreeNode
        BTreeNode* node = new BTreeNode(minimunDegree, isLeaf);
        node->actualNumberKeys = actualNumberKeys;

        // Leer dnis y pagesID desde el buffer mapeado
        for (int i = 0; i < actualNumberKeys; ++i) {
            if (currentOffset + 9 + sizeof(long) > bufferSize) {
                std::cerr << "Error: buffer overflow al deserializar los datos del nodo." << std::endl;
                delete node;
                return nullptr;
            }
            memcpy(node->dnis[i].data(), pBuf + currentOffset, 9);
            currentOffset += 9;

            memcpy(&node->pagesID[i], pBuf + currentOffset, sizeof(node->pagesID[i]));
            currentOffset += sizeof(node->pagesID[i]);
        }

        // Si no es una hoja, deserializar también sus hijos
        if (!isLeaf) {
            for (int i = 0; i <= actualNumberKeys; ++i) {
                node->children[i] = DeserializeTree(pBuf, bufferSize, currentOffset, minimunDegree);
                if (node->children[i] == nullptr) {
                    delete node;
                    return nullptr;
                }
            }
        }

        return node;
    }

    BTreeNode* DeserializeBTree(int minimunDegree) {
        if (pBuf == NULL) {
            std::cerr << "Error: vista del archivo no mapeada." << std::endl;
            return nullptr;
        }

        // Deserializar el árbol desde el buffer mapeado
        size_t currentOffset = 0;
        return DeserializeTree(pBuf, fileSize, currentOffset, minimunDegree);
    }


    

private:
    // Private member variable to hold the value
    HANDLE hFile;
    HANDLE hMapFile;
    char* pBuf;
    size_t fileSize;
    const char* filename;
};