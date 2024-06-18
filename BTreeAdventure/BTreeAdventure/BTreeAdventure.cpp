#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <cstring>

class Personita {
public:
    char name[100];
    int edad;
    int dni;
    int id;

    Personita() : edad(0), dni(0), id(0) {
        std::memset(name, 0, sizeof(name));
    }

    Personita(const char* name, int edad, int dni, int id) : edad(edad), dni(dni), id(id) {
        std::strncpy(this->name, name, sizeof(this->name) - 1); // Copiar el nombre
        this->name[sizeof(this->name) - 1] = '\0';              // Asegura que la cadena esté terminada en nulo
    }
};

class PageManager {
private:
    void PrintLastError() {
        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0) return; // No hay error

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);

        std::cerr << "Error: " << message << std::endl;
    }

    HANDLE CreateAndOpenFile(const char* filePath, bool createNew) {
        HANDLE hFile = CreateFileA(
            filePath,              // Nombre del archivo
            GENERIC_READ | GENERIC_WRITE, // Acceso de lectura y escritura
            0,                     // No compartir el archivo
            NULL,                  // Seguridad predeterminada
            createNew ? CREATE_ALWAYS : OPEN_ALWAYS, // Crear nuevo archivo o abrir si existe
            FILE_ATTRIBUTE_NORMAL, // Atributos normales
            NULL                   // No usar plantillas
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error al crear o abrir el archivo: ";
            PrintLastError();
        }
        return hFile;
    }

public:
    void AppendPersonitasToFile(const std::vector<Personita>& newPersonitas, const char* filePath) {
        // Crear o abrir el archivo
        HANDLE hFile = CreateAndOpenFile(filePath, false);
        if (hFile == INVALID_HANDLE_VALUE) {
            return;
        }

        // Obtener el tamaño del archivo existente
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE) {
            std::cerr << "Error al obtener el tamaño del archivo: ";
            PrintLastError();
            CloseHandle(hFile);
            return;
        }

        // Calcular el tamaño total a escribir (tamaño existente + tamaño nuevo)
        size_t newSize = newPersonitas.size() * sizeof(Personita);
        size_t totalSize = fileSize + newSize;

        // Crear un objeto de mapeo de archivo con el nuevo tamaño
        HANDLE hMapping = CreateFileMappingA(
            hFile,                  // Manejador del archivo
            NULL,                   // Seguridad predeterminada
            PAGE_READWRITE,         // Permisos de lectura y escritura
            0,                      // Tamaño máximo del archivo (parte alta)
            static_cast<DWORD>(totalSize), // Tamaño máximo del archivo (parte baja)
            NULL                    // Sin nombre
        );

        if (hMapping == NULL) {
            std::cerr << "Error al crear el mapeo de archivo: ";
            PrintLastError();
            CloseHandle(hFile);
            return;
        }

        // Mapear el archivo en memoria
        char* fileMemory = (char*)MapViewOfFile(
            hMapping,              // Manejador del mapeo de archivo
            FILE_MAP_WRITE,        // Permisos de escritura
            0,                     // Offset de alta palabra
            0,                     // Offset de baja palabra
            totalSize              // Tamaño del mapeo
        );

        if (fileMemory == NULL) {
            std::cerr << "Error al mapear el archivo en memoria: ";
            PrintLastError();
            CloseHandle(hMapping);
            CloseHandle(hFile);
            return;
        }

        // Copiar los datos nuevos en la memoria mapeada después de los datos existentes
        std::memcpy(fileMemory + fileSize, newPersonitas.data(), newSize);

        // Desmapear el archivo de la memoria
        if (!UnmapViewOfFile(fileMemory)) {
            std::cerr << "Error al desmapear el archivo: ";
            PrintLastError();
        }

        // Cerrar los manejadores
        CloseHandle(hMapping);
        CloseHandle(hFile);

        std::cout << "Se han agregado los nuevos registros al archivo binario exitosamente." << std::endl;
    }

    void ReadPersonitasFromFile(const char* filePath, int searchID) {
        // Abrir el archivo en modo de solo lectura
        HANDLE hFile = CreateFileA(
            filePath,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error al abrir el archivo para lectura: ";
            PrintLastError();
            return;
        }

        // Obtener el tamaño del archivo
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE) {
            std::cerr << "Error al obtener el tamaño del archivo: ";
            PrintLastError();
            CloseHandle(hFile);
            return;
        }

        // Crear un objeto de mapeo de archivo
        HANDLE hMapping = CreateFileMappingA(
            hFile,
            NULL,
            PAGE_READONLY,
            0,
            0,
            NULL
        );

        if (hMapping == NULL) {
            std::cerr << "Error al crear el mapeo de archivo: ";
            PrintLastError();
            CloseHandle(hFile);
            return;
        }

        // Mapear el archivo en memoria
        const char* fileMemory = (const char*)MapViewOfFile(
            hMapping,
            FILE_MAP_READ,
            0,
            0,
            fileSize
        );

        if (fileMemory == NULL) {
            std::cerr << "Error al mapear el archivo en memoria: ";
            PrintLastError();
            CloseHandle(hMapping);
            CloseHandle(hFile);
            return;
        }

        // Calcular el número de registros de Personita
        size_t numRecords = fileSize / sizeof(Personita);

        // Variable para indicar si se encontró el registro
        bool found = false;

        // Leer y buscar el registro por ID
        for (size_t i = 0; i < numRecords; ++i) {
            const Personita* p = reinterpret_cast<const Personita*>(fileMemory + i * sizeof(Personita));
            if (p->id == searchID) {
                std::cout << "Datos encontrados para ID " << searchID << ":" << std::endl;
                std::cout << "Nombre: " << p->name << std::endl;
                std::cout << "Edad: " << p->edad << std::endl;
                std::cout << "DNI: " << p->dni << std::endl;
                std::cout << "ID: " << p->id << std::endl;
                found = true;
                break; // Salir del bucle una vez encontrado
            }
        }

        if (!found) {
            std::cout << "No se encontró ninguna persona con ID " << searchID << "." << std::endl;
        }

        // Desmapear el archivo de la memoria
        if (!UnmapViewOfFile(fileMemory)) {
            std::cerr << "Error al desmapear el archivo: ";
            PrintLastError();
        }

        // Cerrar los manejadores
        CloseHandle(hMapping);
        CloseHandle(hFile);
    }
};

int main() {
    // Crear tres instancias de Personita
    Personita p1("Juan Perez", 25, 12345678, 1);
    Personita p2("Ana Gomez", 30, 87654321, 2);
    Personita p3("Luis Martinez", 22, 11223344, 3);

    // Agregar las personas a un vector
    std::vector<Personita> personitas = { p1, p2, p3 };

    // Ruta completa al archivo
    const char* filePath = "personitas.bin"; // Cambia a la ruta completa de tu archivo

    // Crear instancia de PageManager
    PageManager pageManager;

    // Escribir las personas en el archivo usando file mapping
    //pageManager.AppendPersonitasToFile(personitas, filePath);

    // Crear una nueva instancia de Personita
    Personita p4("Issael Rocha", 12, 71887766, 5);

    // Agregar la nueva persona al archivo
    std::vector<Personita> newPersonitas = { p4 };
    //pageManager.AppendPersonitasToFile(newPersonitas, filePath);

    // Leer las personas del archivo usando file mapping y buscar por ID
    int searchID = 5; // Cambia este valor para buscar un ID específico
    pageManager.ReadPersonitasFromFile(filePath, searchID);

    return 0;
}
