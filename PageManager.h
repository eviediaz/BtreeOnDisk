#pragma once
#include <vector>
#include <fstream>
#include <iostream>

class PageManager : protected std::fstream // FStream methods and attributes are going to be protected
{

public:
    // Constructor
    PageManager(const char *filename)
    {
        filename = filename;
        open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!is_open()) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
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

    template <typename T>
    T readPage(const long &n, const T &object)
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
    void writePage(const long &n, std::vector<T> &p)
    {
        clear();
        for (size_t i = 0; i < p.size(); i++)
        {
            seekp(i * sizeof(p[i]), std::ios::beg);
            p[i].id = i;
            write(reinterpret_cast<char *>(&p[i]), sizeof(p[i]));
            if (!good()) {
                std::cerr << "Error al escribir en page: " << n << std::endl;
            }
        }
    }

private:
    // Private member variable to hold the value
    const char *filename;
};
