#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

class Personita
{
public:
    char name[100];
    int edad;
    int dni;
    int id;
    Personita() : edad(0), dni(0), id(0) { std::memset(name, 0, sizeof(name)); };
    Personita(const char *name, int edad, int dni, int id) : edad(edad), dni(dni), id(id)
    {
        std::strncpy(this->name, name, sizeof(this->name) - 1); // Hacer una copia del valor ingresado y copiarlo hasta lo que alcance según el tamaño del arreglo
        this->name[sizeof(this->name) - 1] = '\0';              // Asegura que la cadena esté terminada en nulo
    }
};

void write(std::vector<Personita> &p, std::fstream &archivo)
{
    for (size_t i = 0; i < p.size(); i++)
    {
        archivo.seekp(i * sizeof(p[i]), std::ios::beg);
        p[i].id = i;
        archivo.write(reinterpret_cast<char *>(&p[i]), sizeof(p[i]));
    }
};

void read(int id, std::fstream &archivo)
{
    Personita p;
    archivo.seekg(id * sizeof(p), std::ios::beg);
    archivo.read(reinterpret_cast<char *>(&p), sizeof(p));
    if (archivo.gcount() == sizeof(Personita))
    {
        std::cout << "Nombre: " << p.name << std::endl;
        std::cout << "Edad: " << p.edad << std::endl;
        std::cout << "DNI: " << p.dni << std::endl;
    }
    else
    {
        std::cerr << "Error leyendo en el índice " << id << std::endl;
    }
};

int main()
{
    std::fstream archivo("lol.bin", std::ios::binary | std::ios::in | std::ios::out);

    // Verificamos si el archivo se abrió correctamente
    if (!archivo)
    {
        std::cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    Personita persona1("Juan", 30, 12345678, 0);
    Personita persona2("Maroa", 25, 87654321, 1);
    Personita persona3("Karin", 45, 10192922, 2);

    std::vector<Personita> personas;
    personas.push_back(persona1);
    personas.push_back(persona2);
    personas.push_back(persona3);

    // write(personas, archivo);
    read(personas[2].id, archivo);

    archivo.close();

    return 0;
}