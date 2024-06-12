#pragma once
#include <vector>
#include <fstream>
#include <iostream>
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
    long getFileSize() {
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

    template <typename T>
    void WriteObjectsPageID(std::vector<T>& objectsVector)
    {
        clear();
        for (size_t i = 0; i < objectsVector.size(); i++)
        {
            seekp(i * sizeof(objectsVector[i]), std::ios::beg);
            objectsVector[i].pageID = i;
            write(reinterpret_cast<char*>(&objectsVector[i]), sizeof(objectsVector[i]));
            if (!good()) {
                std::cerr << "Error al escribir en page: " << i << std::endl;
            }
        }
    };

    std::vector<Personita> LoadDataToVector() {
        std::vector<Personita> persons;
        if (!is_open()) {
            std::cerr << "Error: el archivo no esta abierto." << std::endl;
            return persons;
        }

        clear(); // Limpiar cualquier bandera de error anterior
        seekg(0, std::ios::beg); // Mover el puntero al inicio del archivo

        Personita person;
        while (read(reinterpret_cast<char*>(&person), sizeof(Personita))) {
            if (good()) {
                persons.push_back(person);
            }
            else {
                std::cerr << "Error al leer una entrada en el archivo." << std::endl;
                break;
            }
        }

        if (fail() && !eof()) {
            std::cerr << "Error al leer el archivo." << std::endl;
        }

        return persons;
    }

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

private:
    // Private member variable to hold the value
    const char* filename;
};