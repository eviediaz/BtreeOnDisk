#pragma once
#include <iostream>
#include <cstring>

class Personita
{
public:
    int pageID;
    char dni[9]; // Cambiado a 9 para incluir el carácter nulo '\0'
    char name[100];
    int edad;
    // TODO: agregar mas campos

    // Constructor por defecto
    Personita() : edad(0), pageID(0)
    {
        std::memset(name, 0, sizeof(name));
        std::memset(dni, 0, sizeof(dni)); // Inicializar dni con ceros
    }

    // Constructor parametrizado
    Personita(const char* name, int edad, const char* dni, int pageId) : edad(edad), pageID(pageId)
    {
        std::strncpy(this->name, name, sizeof(this->name) - 1); // Copiar el nombre asegurando el terminador nulo
        this->name[sizeof(this->name) - 1] = '\0';              // Asegurar el terminador nulo

        std::strncpy(this->dni, dni, sizeof(this->dni) - 1);    // Copiar el DNI asegurando el terminador nulo
        this->dni[sizeof(this->dni) - 1] = '\0';                // Asegurar el terminador nulo
    }
};