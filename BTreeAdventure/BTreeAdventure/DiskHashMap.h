#pragma once
#include <iostream>
#include <windows.h>
#include <cstring>

const size_t KEY_SIZE = 9; // 8 caracteres + 1 para '\0'
const size_t VALUE_SIZE = sizeof(long);
const size_t RECORD_SIZE = KEY_SIZE + VALUE_SIZE;

class DiskHashMap {
public:
    DiskHashMap(const char* filename) : filename(filename), hFile(INVALID_HANDLE_VALUE) {
        // Abrir el archivo o crear uno nuevo si no existe
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
            return;
        }
    }

    ~DiskHashMap() {
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
        }
    }

    void Insert(const char* key, long value) {
        // Verificar si la clave ya existe
        if (Search(key) != -1) {
            std::cerr << "Clave " << key << " ya existe." << std::endl;
            return;
        }

        // Mover el puntero del archivo al final para agregar la nueva entrada
        SetFilePointer(hFile, 0, NULL, FILE_END);

        // Escribir la clave
        DWORD written;
        WriteFile(hFile, key, KEY_SIZE, &written, NULL);

        // Escribir el valor
        WriteFile(hFile, &value, VALUE_SIZE, &written, NULL);
    }

    long Search(const char* key) {
        // Obtener el tamaño del archivo
        DWORD fileSize = GetFileSize(hFile, NULL);

        if (fileSize == INVALID_FILE_SIZE) {
            std::cerr << "Error al obtener el tamaño del archivo." << std::endl;
            return -1;
        }

        // Calcular el número de registros
        size_t numberOfRecords = fileSize / RECORD_SIZE;

        // Búsqueda lineal en el archivo
        for (size_t i = 0; i < numberOfRecords; ++i) {
            char currentKey[KEY_SIZE];
            long currentValue;

            // Mover el puntero del archivo a la posición del registro
            SetFilePointer(hFile, i * RECORD_SIZE, NULL, FILE_BEGIN);

            // Leer la clave y el valor del registro
            DWORD read;
            ReadFile(hFile, currentKey, KEY_SIZE, &read, NULL);
            ReadFile(hFile, &currentValue, VALUE_SIZE, &read, NULL);

            if (std::strcmp(currentKey, key) == 0) {
                return currentValue;
            }
        }

        return -1; // Indicador de que la clave no se encontró
    }

    void Remove(const char* key) {
        // Obtener el tamaño del archivo
        DWORD fileSize = GetFileSize(hFile, NULL);

        if (fileSize == INVALID_FILE_SIZE) {
            std::cerr << "Error al obtener el tamaño del archivo." << std::endl;
            return;
        }

        // Calcular el número de registros
        size_t numberOfRecords = fileSize / RECORD_SIZE;

        // Búsqueda binaria en el archivo
        long left = 0;
        long right = numberOfRecords - 1;

        while (left <= right) {
            long mid = left + (right - left) / 2;
            char currentKey[KEY_SIZE];
            long currentValue;

            // Mover el puntero del archivo a la posición del registro medio
            SetFilePointer(hFile, mid * RECORD_SIZE, NULL, FILE_BEGIN);

            // Leer la clave y el valor del registro
            DWORD read;
            ReadFile(hFile, currentKey, KEY_SIZE, &read, NULL);
            ReadFile(hFile, &currentValue, VALUE_SIZE, &read, NULL);

            if (std::strcmp(currentKey, key) == 0) {
                // Marcar la clave como eliminada
                strncpy(currentKey, "Deleted", KEY_SIZE - 1);
                currentKey[KEY_SIZE - 1] = '\0';

                // Obtener la posición actual del puntero
                DWORD currentPosition = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
                if (currentPosition == INVALID_SET_FILE_POINTER) {
                    std::cerr << "Error al obtener la posición actual del puntero del archivo." << std::endl;
                    return;
                }

                // Calcular la nueva posición
                DWORD newPosition = currentPosition - RECORD_SIZE;

                // Establecer la nueva posición para sobrescribir el registro
                SetFilePointer(hFile, newPosition, NULL, FILE_BEGIN);

                // Escribir la clave marcada como eliminada
                DWORD written;
                WriteFile(hFile, currentKey, KEY_SIZE, &written, NULL);

                // Escribir un valor inválido
                long invalidValue = -1;
                WriteFile(hFile, &invalidValue, VALUE_SIZE, &written, NULL);
                return;
            }

            if (std::strcmp(currentKey, key) < 0) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
    }


private:
    const char* filename;
    HANDLE hFile;
};
