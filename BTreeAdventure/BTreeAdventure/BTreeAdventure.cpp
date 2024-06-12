// BTreeAdventure.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include "PageManager.h"
#include "Person.h"
#include "BTree.h"

void write(std::vector<Personita>& persona, std::fstream& archivo)
{
    for (size_t i = 0; i < persona.size(); i++)
    {
        archivo.seekp(i * sizeof(persona[i]), std::ios::beg);
        persona[i].pageID = i;
        archivo.write(reinterpret_cast<char*>(&persona[i]), sizeof(persona[i]));
    }
};

void read(int id, std::fstream& archivo)
{
    Personita p;
    archivo.seekg(id * sizeof(p), std::ios::beg);
    archivo.read(reinterpret_cast<char*>(&p), sizeof(p));
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
    BTree t(4); // Un árbol B con grado mínimo 4
    PageManager pageManager("lol.bin");

    pageManager.ReadFileAndLoadToBtree(t);

    /*
    // Agregar un nuevo registro
    Personita newPerson("JULIAN", 24, NewDNI, -1); // -1 para indicar que aún no tiene pageID
    pageManager.AddNewPerson(t, newPerson);
    */

    std::cout << "El recorrido del arbol construido es:" << std::endl;
    t.PrintBTree();

    // yo - eliminar
    int pageid = t.GetPageIDByDNI("71454824");

    if (pageid >= 0) {
        Personita p1 = pageManager.ReadGetObjectByPageID(pageid);
        std::cout << "Nombre: " << p1.name << std::endl;
        std::cout << "Edad: " << p1.edad << std::endl;
        std::cout << "DNI: " << p1.dni << std::endl;
    }

    t.Remove("71454824");
    std::cout << "El recorrido del árbol después de eliminar un registro es:" << std::endl;
    t.PrintBTree();
        
    /*
    // new person
    int pageid2 = t.GetPageIDByDNI("21354829");
    if (pageid2 >= 0) {
        Personita p1 = pageManager.ReadGetObjectByPageID(pageid2);
        std::cout << "Nombre: " << p1.name << std::endl;
        std::cout << "Edad: " << p1.edad << std::endl;
        std::cout << "DNI: " << p1.dni << std::endl;
    }
    */
        
    return 0;
    
    /*
    std::fstream archivo("lol.bin", std::ios::binary | std::ios::in | std::ios::out);

    // Verificamos si el archivo se abrió correctamente
    if (!archivo)
    {
        std::cerr << "Error al abrir el archivo.\n";
        return 1;
    }

    
    Personita persona1("Juan", 30, "12345678", 0);
    Personita persona2("Maroa", 25, "87654321", 1);
    Personita persona3("Karin", 45, "10192922", 2);

    std::vector<Personita> personas;
    personas.push_back(persona1);
    personas.push_back(persona2);
    personas.push_back(persona3);
    

    //write(personas, archivo);
    read(personas[2].id, archivo);

    archivo.close();

    return 0;
    */
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