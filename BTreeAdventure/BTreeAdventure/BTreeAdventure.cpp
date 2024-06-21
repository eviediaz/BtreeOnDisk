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
#include "BTreeAdventure.h"
#include "DiskHashMap.h"
#include "DiskBTree.h"

void mostrar_menu() {
    std::cout << "Menu de opciones:\n";
    std::cout << "1. Insertar un nuevo registro\n";
    std::cout << "2. Buscar un registro por DNI\n";
    std::cout << "3. Eliminar un registro por DNI\n";
    std::cout << "4. Restaurar un ciudadano\n";
    std::cout << "5. Imprimir los primeros 100 registros\n";
    std::cout << "6. Limpiar consola\n";
    std::cout << "7. Salir\n";
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
    BTree t(8); // create btree with a given order (t) 
    std::string btreeSerializedFileName = "BTREEtestingData.bin";
    std::string citizenDataFileName = "testingData.bin";

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

    const char* filename = "btree_data.dat";
    int minDegree = 2;

    // Initialize DiskBTree
    DiskBTree diskBTree(filename, minDegree);

    // Insert some records
    diskBTree.Insert("12345678", 1);
    diskBTree.Insert("87654321", 2);
    diskBTree.Insert("11112222", 3);
    diskBTree.Insert("33334444", 4);
    diskBTree.Insert("55556666", 5);

    // Search for specific records
    std::cout << "Searching for key '12345678': Page ID = " << diskBTree.Search("12345678") << std::endl;
    std::cout << "Searching for key '87654321': Page ID = " << diskBTree.Search("87654321") << std::endl;
    std::cout << "Searching for key '00000000': Page ID = " << diskBTree.Search("00000000") << std::endl;

    // Traverse and print the B-Tree
    std::cout << "Traversing B-Tree:" << std::endl;
    diskBTree.Traverse();

    // Remove some records
    std::cout << "Removing key '11112222'" << std::endl;
    diskBTree.Remove("11112222");

    // Traverse and print the B-Tree after removal
    std::cout << "Traversing B-Tree after removal:" << std::endl;
    diskBTree.Traverse();

    return 0;

    /*
    //const char* filename = "recycle.bin";
    //DiskHashMap hashmap(filename);

    // Inserta una entrada
    hashmap.Insert("12345678", 1);
    hashmap.Insert("87654321", 2);
    hashmap.Insert("57754221", 3);
    hashmap.Insert("98775401", 4);
    hashmap.Insert("61254221", 5);
    hashmap.Insert("32054221", 6);
    

    // Buscar entradas
    long value1 = hashmap.Search("12345678");
    long value2 = hashmap.Search("87654321");
    long value3 = hashmap.Search("57754221");
    long value4 = hashmap.Search("98775401");
    long value5 = hashmap.Search("61254221");
    long value6 = hashmap.Search("32054221");

    std::cout << "Valor para 12345678: " << value1 << std::endl;
    std::cout << "Valor para 87654321: " << value2 << std::endl;
    std::cout << "Valor para 57754221: " << value3 << std::endl;
    std::cout << "Valor para 98775401: " << value4 << std::endl;
    std::cout << "Valor para 61254221: " << value5 << std::endl;
    std::cout << "Valor para 32054221: " << value6 << std::endl;

    // Elimina una entrada
    //hashmap.Remove("57754221");
    //value3 = hashmap.Search("57754221");
    //std::cout << "Valor después de eliminar 57754221: " << value3 << std::endl;
    //xd(dataGenerated, dataGenerator, pageManager, t, btreeSerializedFileName, citizenDataFileName);
    */

    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
        
    return 0;
}

void xd(bool dataGenerated, DataGenerator& dataGenerator, PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName, std::string& citizenDataFileName)
{
    if (!dataGenerated)
    {
        //std::cout << "data NO GENERADA -> SE CREA DATOS RANDOM";

        long numberOfRecordsToGenerate = 50;
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

    bool btreeUpdated = false;
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
                std::cout << "GOOD !! La data se insertara al BTree\n";
                // Verificar el tamaño del archivo para determinar el nuevo pageID
                //long newPageID = pageManager.GetFileSize() / sizeof(Personita);
                //t.Insert(nuevaPersona.dni, newPageID);
                pageManager.AddNewPerson(t, nuevaPersona, btreeSerializedFileName.c_str());
                btreeUpdated = true;
            }
            else
            {
                std::cout << "Ya existe ese DNI...genera otro\n";
            }
            std::cout << "\n";


            break;
        }
        case 2: {
            // Buscar un registro por DNI
            std::string dni;
            std::cout << "Ingrese el DNI a buscar: ";
            std::getline(std::cin, dni);

            int pageID = t.GetPageIDByDNI(dni.c_str());
            if (pageID >= 0) {
                Personita persona = pageManager.ReadFileGetPersonByPageID(pageID);
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
            btreeUpdated = true;
            break;
        }
        case 4: {
            // Buscar el DNI de la persona a restarurar en el archivo dumped
            std::string dni;
            std::cout << "Ingrese el DNI a restaurar: ";
            std::getline(std::cin, dni);

            int pageID = t.GetPageIDByDNI(dni.c_str());
            if (pageID >= 0) {
                // restaurar
                pageManager.RestorePerson(pageID);
            }
            else {
                std::cout << "No existe una persona con el DNI " << dni << "\n";
            }
            std::cout << "\n";

            break;
        }
        case 5:
            int inicio, final;
            std::cout << "Ingrese el indice de inicio: ";
            std::cin >> inicio;
            std::cout << "Ingrese el indice final: ";
            std::cin >> final;

            pageManager.PrintOneHundredRecords(citizenDataFileName.c_str(), inicio, final);
            break;
        case 6:
            ClearConsole();
            break;
        case 7:
            // Serializar el B-Tree en el archivo 'btree_serialized.bin'
            if (btreeUpdated) {
                std::cout << "Guardando datos del BTree...\n";
                pageManager.SerializeBTree(t, btreeSerializedFileName.c_str());

            }
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 7);
}
