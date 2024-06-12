// BTreeAdventure.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <cstring>
#include <chrono>
#include "PageManager.h"
#include "Person.h"
#include "BTree.h"

// Declaración previa de funciones de generación aleatoria
std::string generar_dni_aleatorio();
std::string generar_nombre_aleatorio();
std::string generar_apellido_aleatorio();
std::string generar_nacionalidad_aleatorio();
std::string generar_lugar_nacimiento_aleatorio();
std::string generar_direccion_aleatorio();
std::string generar_telefono_aleatorio();
std::string generar_correo_aleatorio(std::string& nombre, std::string& apellido);
std::string generar_estado_civil_aleatorio();

Personita generar_persona_aleatoria(int pageID) {
    std::string dni = generar_dni_aleatorio();
    std::string nombre = generar_nombre_aleatorio();
    std::string apellido = generar_apellido_aleatorio();
    int edad = 18 + (std::rand() % 60); // Generar una edad aleatoria entre 18 y 77
    std::string nacionalidad = generar_nacionalidad_aleatorio();
    std::string lugarNacimiento = generar_lugar_nacimiento_aleatorio();
    std::string direccion = generar_direccion_aleatorio();
    std::string telefono = generar_telefono_aleatorio();
    std::string correo = generar_correo_aleatorio(nombre, apellido);
    std::string estadoCivil = generar_estado_civil_aleatorio();

    // Crear el objeto Personita
    Personita persona(
        nombre.c_str(),
        edad,
        dni.c_str(),
        pageID,
        apellido.c_str(),
        nacionalidad.c_str(),
        lugarNacimiento.c_str(),
        direccion.c_str(),
        telefono.c_str(),
        correo.c_str(),
        estadoCivil.c_str()
    );

    return persona;
}

// Implementación de las funciones de generación aleatoria
std::string generar_dni_aleatorio() {
    static const std::string digitos = "0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, digitos.size() - 1);

    std::string resultado;
    for (size_t i = 0; i < 8; ++i) {
        resultado += digitos[dis(gen)];
    }
    return resultado;
}

std::string generar_nombre_aleatorio() {
    static const std::vector<std::string> nombres = {
        "Pepito", "Juan", "Maria", "Andre", "Pedro", "Ana", "Evie", "Dennis",
        "Jesus", "Jorge", "Rosa", "Luis", "Carlos", "Sofia", "Lucia", "Miguel",
        "Jose", "Manuel", "Ricardo", "Raul", "Luisa", "Fernando", "Javier",
        "Joaquin", "Julieta", "Julia", "Diego", "Valentina", "Felipe", "Camila",
        "Mateo", "Isabella", "Nicolas", "Sara", "Samuel", "Laura", "Daniel",
        "Mariana", "Alejandro", "Carolina"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nombres.size() - 1);

    return nombres[dis(gen)];
}

std::string generar_apellido_aleatorio() {
    static const std::vector<std::string> apellidos = {
        "Gonzales", "Lopez", "Perez", "Rodriguez", "Garcia", "Diaz", "Ceballos",
        "Vargas", "Torres", "Mendoza", "Carrasco", "Gutierrez", "Villanueva",
        "Cordova", "Cruz", "Vasquez", "Villegas", "Vega", "Vera", "Velasquez",
        "Vargas", "Valencia", "Urbina", "Ugarte", "Toro", "Tello", "Tafur",
        "Soto", "Silva", "Sifuentes", "Serrano", "Serna", "Segura", "Santos",
        "Santiago", "Sanchez", "Salazar", "Ruiz", "Rojas", "Rivera", "Rios",
        "Rivas", "Rivas", "Reyes", "Reyna", "Requena", "Ramirez", "Ramos",
        "Ramirez", "Quispe", "Quintana", "Puma", "Pizarro", "Pinto", "Pineda",
        "Pinedo", "Pinedo", "Perez"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, apellidos.size() - 1);

    return apellidos[dis(gen)];
}

std::string generar_nacionalidad_aleatorio() {
    static const std::vector<std::string> estados = { "Peruano(a)" };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, estados.size() - 1);

    return estados[dis(gen)];
}

std::string generar_lugar_nacimiento_aleatorio() {
    static const std::vector<std::string> lugar_nacimiento = {
        "Lima", "Cajamarca", "Cusco", "Arequipa", "Puno", "Ica", "Tacna",
        "Piura", "Lambayeque", "La Libertad", "Ancash", "Junin", "Huanuco",
        "Pasco", "Ucayali", "Madre de Dios", "Loreto", "San Martin", "Amazonas",
        "Tumbes", "Moquegua", "Apurimac", "Ayacucho", "Huancavelica"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, lugar_nacimiento.size() - 1);

    return lugar_nacimiento[dis(gen)];
}

std::string generar_direccion_aleatorio() {
    static const std::vector<std::string> direcciones = {
        "Calle Rosas 123", "Av. Javier Prado 456", "Jirón Rickenbaker 789",
        "Jr. Ancash 143", "Av. Abancay 561", "Callejón Secreto", "Calle Zurbaran 201",
        "Av. Primavera 232", "Jr. Puno 154", "Jr. Ayacucho 371", "Calle Luiggi Pirandello 314",
        "Av. Guardia Civil 1070", "Jr. Lampa 101", "Psj. 106", "Calle 175",
        "Av. Tacna 971", "Jr. Huallaga 138", "Psj. Velasquez 211", "Calle Paita 347",
        "Av. Alfonso Ugarte 893", "Jr. Carabaya 87", "Psj. Cotahuasi 55",
        "Calle Las Herramientas 166", "Av. Elmer Faucett 1124", "Jr. Galileo 19", "Psj. Venus 20"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, direcciones.size() - 1);

    return direcciones[dis(gen)];
}

std::string generar_telefono_aleatorio() {
    static const std::string digitos = "0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, digitos.size() - 1);

    std::string resultado;
    for (size_t i = 0; i < 7; ++i) {
        resultado += digitos[dis(gen)];
    }
    return "9" + resultado;
}

std::string generar_correo_aleatorio(std::string& nombre, std::string& apellido) {
    static const std::vector<std::string> dominios = {
        "@gmail.com", "@hotmail.com", "@yahoo.com", "@outlook.com", "@icloud.com",
        "@yahoo.es", "@gmail.es", "@hotmail.es"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dominios.size() - 1);

    std::string nombreLower = nombre;
    std::transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);

    std::string apellidoLower = apellido;
    std::transform(apellidoLower.begin(), apellidoLower.end(), apellidoLower.begin(), ::tolower);

    return nombreLower + apellidoLower + dominios[dis(gen)];
}

std::string generar_estado_civil_aleatorio() {
    static const std::vector<std::string> estados = {
        "So", "Ca", "Di", "Vi", "Co"
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, estados.size() - 1);

    return estados[dis(gen)];
}

int main()
{
    BTree t(7); // Un árbol B con grado mínimo 4
    PageManager pageManager("people.bin");

    /*
    for (long i = 0; i < 33000000; ++i) {
        Personita persona = generar_persona_aleatoria(i);
        pageManager.WriteObjectInDisk(i, persona);

        if (i % 100000 == 0) { // Mostrar progreso cada 100,000 registros
            std::cout << "Progreso: " << i << " registros generados." << std::endl;
        }
    }
    

    std::cout << "Generacion completada." << std::endl;
    long fileSize = pageManager.GetFileSize();

    // Calcular el número total de registros en el archivo
    long totalRegistros = fileSize / sizeof(Personita);

    // Calcular el grado mínimo
    int gradoMinimo = static_cast<int>(std::ceil(static_cast<double>(totalRegistros) / 2));
    std::cout << "El grado minimo del arbol B es: " << gradoMinimo << std::endl;
    */
    pageManager.ReadFileAndLoadToBtree(t);
    std::cout << "BTree cargado a RAM\n";

    /*
    // Agregar un nuevo registro
    Personita newPerson("JESUS XD", 24, "71454823", -1); // -1 para indicar que aún no tiene pageID
    pageManager.AddNewPerson(t, newPerson);
    */
    
    
    //std::cout << "El recorrido del arbol construido es:" << std::endl;
    //t.PrintBTree();
    
    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;
    
    inicio = std::chrono::system_clock::now();
    //obtener el page ID de un DNI
    int pageid = t.GetPageIDByDNI("00114579");
    //int pageid = 756;

    if (pageid >= 0) {
        Personita p1 = pageManager.ReadGetObjectByPageID(pageid);
        std::cout << "Nombre: " << p1.name << std::endl;
        std::cout << "Edad: " << p1.edad << std::endl;
        std::cout << "DNI: " << p1.dni << std::endl;
        //std::cout << "SE VA A ELIMINAR !!!! " << "\n";
        //pageManager.DeleteRecordFromDisk(pageid, t);
    }
    else 
    {
        std::cout << "No existe p " << "\n";
    }
    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    
        
    return 0;
}

/*
#define _CRT_SECURE_NO_WARNINGS
#include "BTreeDisk.h"
#include "DiskPageManager.h"
#include <iostream>
#include <chrono>
#include <vector>

struct Pair {
    long id;
    long page_id;

    bool operator<(const Pair& p) const {
        if (this->id < p.id) return true;
        else return false;
    }

    bool operator<=(const Pair& p) const {
        if (this->id <= p.id) return true;
        else return false;
    }

    bool operator==(const Pair& p) const {
        if (this->id == p.id) return true;
        else return false;
    }
};

struct Peruvian {
    int id;
    char name[32];
    char surname[32];
};

struct Computer {
    int id;
    int price;
    int speed;
    int hd;
    int ram;
    int screen;
    char cd[4];
    char multi[4];
    char premium[4];
    int ads;
    int trend;
};

std::ostream& operator<<(std::ostream& out, Pair& p) {
    out << p.id;
    return out;
}

#define PAGE_SIZE  1024

// Other examples:
// PAGE_SIZE 1024 bytes => 1Kb
// PAGE_SIZE 1024*1024 bytes => 1Mb
// 3 * sizeof(long)  =  count+right+page_id
// PAGE_SIZE = 3 * sizeof(long) +  (BTREE_ORDER + 1) * sizeof(T) + (BTREE_ORDER + 2) * sizeof(long)


#define BTREE_ORDER   ((PAGE_SIZE - (5 * sizeof(long) + sizeof(Pair)) ) /  (sizeof(Pair) + sizeof(long)))

using namespace APP_CORE::DISK;

int main()
{
    /*
    std::shared_ptr <DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", false);
    btree<Pair, BTREE_ORDER> bt(pm);

    DiskPageManager record_manager("computers.bin", false);

    auto iter = bt.find(Pair{ 1, -1 });
    auto end = bt.find(Pair{ 5120, -1 });

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;
    for (; iter != end; ++iter) {
        auto pair = *iter;
        Computer record;
        record_manager.recover(pair.page_id, record);
        std::cout << "ID: " << record.id << " - ";
        std::cout << "PRICE: " << record.price << " - ";
        std::cout << "SPEED: " << record.speed << " - ";
        std::cout << "HD: " << record.hd << " - ";
        std::cout << "RAM: " << record.ram << std::endl;
    }

    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    */

    /*
    int xd;
    std::shared_ptr <DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", false);
    btree<Pair, BTREE_ORDER> bt(pm);

    DiskPageManager record_manager("computers.bin", false);

    auto iter = bt.find(Pair{ 1420, -1 });
    auto pair = *iter;
    Computer record;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;

    record_manager.recover(pair.page_id, record);
    std::cout << "ID: " << record.id << std::endl;
    std::cout << "PRICE: " << record.price << std::endl;
    std::cout << "SPEED: " << record.speed << std::endl;
    std::cout << "HD: " << record.hd << std::endl;
    std::cout << "RAM: " << record.ram << std::endl;

    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    */


    /*
    std::cout << "TEST 1 : INSERT" << '\n';
    std::fstream in;
    in.open("Computers.csv", std::ios::in);
    if (!in.is_open()) {
        std::cout << "is not opened" << '\n';
        return 1;
    }

    std::shared_ptr <DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", true);
    btree<Pair, BTREE_ORDER> bt(pm);

    DiskPageManager record_manager("computers.bin", true);
    std::string line;
    getline(in, line);
    Computer record;
    int i = 1;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;

    while (getline(in, line)) {
        int pos_in_line = 0;
        std::vector<std::string> tokens;
        std::string token;
        while ((pos_in_line = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos_in_line);
            line.erase(0, pos_in_line + 1);
            tokens.push_back(token);
        }
        tokens[0].erase(remove(tokens[0].begin(), tokens[0].end(), '\"'), tokens[0].end());
        record.id = stoi(tokens[0]);
        record.price = stoi(tokens[1]);
        record.speed = stoi(tokens[2]);
        record.hd = stoi(tokens[3]);
        record.ram = stoi(tokens[4]);
        record.screen = stoi(tokens[5]);
        tokens[6].erase(remove(tokens[6].begin(), tokens[6].end(), '\"'), tokens[6].end());
        strcpy(record.cd, tokens[6].c_str());
        tokens[7].erase(remove(tokens[7].begin(), tokens[7].end(), '\"'), tokens[7].end());
        strcpy(record.multi, tokens[7].c_str());
        tokens[8].erase(remove(tokens[8].begin(), tokens[8].end(), '\"'), tokens[8].end());
        strcpy(record.premium, tokens[8].c_str());
        record.ads = stoi(tokens[9]);
        record.trend = stoi(token);

        record_manager.save(i, record);
        bt.insert(Pair{ record.id, i });
        i++;
    }

    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    */

    /* PERUAAAANOS MEMEEES
    std::cout << "TEST 1 : INSERT" << '\n';
    std::fstream in;
    in.open("PeruvianData.csv", std::ios::in);
    if (!in.is_open()) {
        std::cout << "File is not opened" << '\n';
        // Handle failure appropriately, e.g., exit or return an error code.
        return 1;
    }

    // Assuming you have defined pagemanager and btree somewhere in your code.
    // You can replace them with appropriate types.

    std::shared_ptr<DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", true);
    btree<Pair, BTREE_ORDER> bt(pm);

    // Assuming you have defined pagemanager somewhere in your code.
    DiskPageManager record_manager("peruvians.bin", true);

    std::string line;
    getline(in, line); // Skip header line if any
    Peruvian record;
    int i = 1;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;
    while (getline(in, line)) 
    {
        int pos_in_line = 0;
        std::vector<std::string> tokens;
        std::string token;

        while ( (pos_in_line = line.find(','))
            != std::string::npos) 
        {
            token = line.substr(0, pos_in_line);
            line.erase(0, pos_in_line + 1);
            tokens.push_back(token);
        }
        tokens[0].erase(remove(tokens[0].begin(), tokens[0].end(), '\"'), tokens[0].end());
        record.id = std::stod(tokens[0]);
        tokens[1].erase(remove(tokens[1].begin(), tokens[1].end(), '\"'), tokens[1].end());
        std::strcpy(record.name, tokens[1].c_str());
        tokens[2].erase(remove(tokens[2].begin(), tokens[2].end(), '\"'), tokens[2].end());
        std::strcpy(record.surname, "xd");

        record_manager.save(i, record);
        bt.insert(Pair{ record.id, i });
        i++;
    }
    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    


    
    std::shared_ptr<DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", true);
    btree< Pair, BTREE_ORDER> bt(pm);
    DiskPageManager record_manager("peruanos.bin", true);
    long page_id;
    int index = 1;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;
    
    inicio = std::chrono::system_clock::now();
    for (long long i = 1; i <= 3000000; i++)
    {
        Peruvian p{ index, "ALDHAIR", "PINADO"};
        page_id = i;
        record_manager.save(page_id, p); // save record to disk
        bt.insert(Pair{ p.id, page_id });  // insert into b+tree index
        index++;
    }
    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    
    
    /*
    std::shared_ptr <DiskPageManager> pm = std::make_shared<DiskPageManager>("b+tree.index", false);
    btree<Pair, BTREE_ORDER> bt(pm);
    DiskPageManager record_manager("peruanos.bin", false);
    long page_id;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;

    auto iter = bt.find(Pair{ 2, -1});
    auto end = bt.find(Pair{3, -1});

    inicio = std::chrono::system_clock::now();
    for (; iter != end; ++iter)
    {
        auto pair = *iter;
        Peruvian s;
        record_manager.recover(pair.page_id, s);
        std::cout << s.id << " " << s.name << " " << s.surname << '\n';
    }
    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    
    //std::cout << "Hello World!\n";
    
    
    return 0;

}
*/