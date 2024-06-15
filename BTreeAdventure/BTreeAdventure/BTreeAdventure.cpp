// BTreeAdventure.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <cstring>
#include <thread>
#include <chrono>
#include "DataGenerator.h"
#include "PageManager.h"
#include "Person.h"
#include "BTree.h"

void mostrar_menu() {
    std::cout << "Menu de opciones:\n";
    std::cout << "1. Insertar un nuevo registro\n";
    std::cout << "2. Buscar un registro por DNI\n";
    std::cout << "3. Eliminar un registro por DNI\n";
    std::cout << "4. Imprimir los primeros 100 registros\n";
    std::cout << "5. Limpiar consola\n";
    std::cout << "6. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

void ClearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void LoadingMessage(std::atomic<bool>& loading) {
    const char* message = "Cargando BTree a RAM";
    const char* dots[] = { "", ".", "..", "..." };
    int index = 0;

    while (loading) {
        // Escribir el mensaje con puntos
        std::cout << "\r" << message << dots[index++] << "   " << std::flush;
        if (index == 4) index = 0;

        // Esperar un breve momento antes de actualizar
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Borrar el mensaje una vez que termine de cargar
    std::cout << "\r" << message << "               \n";
}

int main()
{
    /*
    * TODO: Improve BTree order calculus logic
    int minimunDegree                       4 bytes                            
    std::vector<int> pagesID;               (2 * t - 1 ) * 4 bytes
    std::vector<std::array<char, 9>> dnis;  (2 * t - 1 ) * 9 bytes
    std::vector<BTreeNode*> children;       (2 * t ) * 8 bytes
    int actualNumberKeys;                   4 bytes
    bool isLeaf;                            1 bytes

    */


    // for data 33 M
    // estimate size of BTreeNode 4096 bytes 
    // t = 195 -> size of each node it's 8kb 8096
    // btre t = 195;
    BTree t(195); // create btree with a given order (t) 
    std::string btreeSerializedFileName = "btree_serialized.bin";
    std::string citizenDataFileName = "people.bin";

    /*
    // For data < 1M
    // the file where the btree is goign to serialize/deserialize
    btree t = 8
    std::string btreeSerializedFileName = "btreeTestUno.bin";
    std::string citizenDataFileName = "testUno.bin";
    */

    // the page manager to do write and read disk operations in a given file
    PageManager pageManager(citizenDataFileName.c_str());
    DataGenerator dataGenerator;

    std::ifstream infile(citizenDataFileName);
    bool dataGenerated = infile.good();
    infile.close();

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;

    inicio = std::chrono::system_clock::now();
    if (!dataGenerated) 
    {
        //std::cout << "data NO GENERADA -> SE CREA DATOS RANDOM";
        
        long numberOfRecordsToGenerate = 500000;
        dataGenerator.GenerateNRecordsData(numberOfRecordsToGenerate, pageManager);
        pageManager.ReadFileAndLoadToBtree(t);
        pageManager.SerializeBTree(t, btreeSerializedFileName.c_str());
        
    }
    else 
    {
        std::atomic<bool> loading{ true };
        std::thread loadingThread(LoadingMessage, std::ref(loading));
        //std::cout << "data generada -> se carga el serializado";
        pageManager.DeserializeBTree(t, btreeSerializedFileName.c_str());
        // Detener el mensaje de carga y esperar que termine el thread
        loading = false;
        loadingThread.join();
        std::cout << "\nB-Tree cargado a RAM.\n";
        ClearConsole();
    }
    

    int opcion;
    do {
        mostrar_menu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar el buffer de entrada

        switch (opcion) {
        case 1: {
            // TODO: Check if generated DNI person exits in BTree
            // Insertar un nuevo registro

            Personita nuevaPersona = dataGenerator.GenerateRandomPersonita();
            std::cout << "\n";
            nuevaPersona.ImprimirDatos();

            int pageID = t.GetPageIDByDNI(nuevaPersona.dni);
            std::cout << "\n";
            if (pageID == -1) 
            {
                std::cout << "GOOD !! insertando....\n";
            }
            else 
            {
                std::cout << "ya existe ese DNI...genera otro\n";
            }
            std::cout << "\n";

            /*
            std::cout << "Ingrese el nombre: ";
            std::getline(std::cin, nombre);
            std::cout << "Ingrese el apellido: ";
            std::getline(std::cin, apellido);
            std::cout << "Ingrese el DNI: ";
            std::getline(std::cin, dni);
            std::cout << "Ingrese la edad: ";
            std::cin >> edad;
            std::cin.ignore();
            std::cout << "Ingrese la nacionalidad: ";
            std::getline(std::cin, nacionalidad);
            std::cout << "Ingrese el lugar de nacimiento: ";
            std::getline(std::cin, lugarNacimiento);
            std::cout << "Ingrese la direccion: ";
            std::getline(std::cin, direccion);
            std::cout << "Ingrese el telefono: ";
            std::getline(std::cin, telefono);
            std::cout << "Ingrese el correo: ";
            std::getline(std::cin, correo);
            std::cout << "Ingrese el estado civil: ";
            std::getline(std::cin, estadoCivil);

            Personita nuevaPersona(
                nombre.c_str(), edad, dni.c_str(), -1, apellido.c_str(),
                nacionalidad.c_str(), lugarNacimiento.c_str(), direccion.c_str(),
                telefono.c_str(), correo.c_str(), estadoCivil.c_str()
            );

            pageManager.AddNewPerson(t, nuevaPersona, btreeSerializedFileName.c_str());
            */
            break;
        }
        case 2: {
            // Buscar un registro por DNI
            std::string dni;
            std::cout << "Ingrese el DNI a buscar: ";
            std::getline(std::cin, dni);

            int pageID = t.GetPageIDByDNI(dni.c_str());
            if (pageID >= 0) {
                Personita persona = pageManager.ReadGetObjectByPageID(pageID);
                std::cout << "\n";
                persona.ImprimirDatos();
            }
            else {
                std::cout << "No existe una persona con el DNI " << dni << "\n";
            }
            std::cout << "\n";
            break;
        }
        case 3: {
            // Eliminar un registro por DNI
            std::string dni;
            std::cout << "Ingrese el DNI a eliminar: ";
            std::getline(std::cin, dni);

            pageManager.DeleteRecordFromDisk(t, dni.c_str(), btreeSerializedFileName.c_str());
            break;
        }
        case 4: {
            int inicio, final;
            std::cout << "Ingrese el indice de inicio: ";
            std::cin >> inicio;
            std::cout << "Ingrese el indice final: ";
            std::cin >> final;

            pageManager.PrintOneHundredRecords(citizenDataFileName.c_str(), inicio, final);
            break;
        }
        case 5:
            ClearConsole();
            break;
        case 6:
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 6);

    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
        
    return 0;
}
