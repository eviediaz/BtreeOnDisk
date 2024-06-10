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
    };

    // Destructor
    ~PageManager()
    {
        close();
    }

    template <typename T>
    T readPage(const long &n, const T &object)
    {
        clear();
        seekg(n * sizeof(T), std::ios::beg);
        read(reinterpret_cast<char *>(&T), sizeof(T));
        return T;
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
        }
    }

private:
    // Private member variable to hold the value
    const char *filename;
};
