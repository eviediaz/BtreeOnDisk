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

void mostrar_menu() {
    std::cout << "Menu de opciones:\n";
    std::cout << "1. Insertar un nuevo registro\n";
    std::cout << "2. Buscar un registro por DNI\n";
    std::cout << "3. Eliminar un registro por DNI\n";
    std::cout << "4. Imprimir los primeros 100 registros\n";
    std::cout << "5. Limpiar consola\n";
    std::cout << "6. Busqueda por rangos\n";
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
    // BENCHMARKING - GENERATING DATA
    // descomentar los respectivos benchmark a realizar
    //BenchMarkGenerarArchivoCincoMillones();

    //BenchmarkGenerarArchivoDiezMillones();

    //BenchmarkGenerarArchivoQuinceMillones();

    //BenchmarkGenerarArchivoVeinteMillones();

    //BenchmarkGenerarArchivoTreintaMillones();

    // luego de generar la data para los archivos probar el tiempo de carga 
    // de lectura de datos del arhcivo a la construccion del btree
    // nota pueden modificar el orden del BTree dentro de las funciones
    //BenchmarkCargaYSerializadoBTreeCincoMillones();

    //BenchmarkCargaYSerializadoBTreeDiezMillones();

    //BenchmarkCargaYSerializadoBTreeQuinceMillones();
    
    //BenchmarkCargaYSerializadoBTreeVeinteMillones();
    
    //BenchmarkCargaYSerizliadoBTreeTreintaMillones();

    // con el btree ya serializado probar los benchmark de tiempo de carga
    // del btree serializado a ram
    //BenchmarkLecturaSerializadoCincoMillones();

    //BenchMarkLecturaSerializadoDiezMillones();

    //BenchmarkLecturaSerializadoQuinceMillones();

    //BenchmarkLecturaSerializadoVeinteMillones();

    //BenchmarkLecturaSerializadoTreintaMillones();

    
    // FILE NAMES
    std::string btreeSerializedFileName = "btreeSerializadoFinal.bin";
    std::string citizenDataFileName = "people.bin";

    // NUMBER OF RANDOM RECORDS TO GENERATE
    long numberOfRecordsToGenerate = 100;    

    /*
    * calculus of T in BTree
    * we want a height of 4
    * h = log N in base T
    * h = log N / log T
    * we know
    * N = 33 000 000
    * h = 7.52 / log T
    * 4 = 7.52 / log T
    * aproximation 7.52 ->8
    * log T = 2
    * T = 100 , start "guessing" with values
    * logT=128 -> 2.11  logT = 256-> 3
    * 1.3GB             1.4GB
    * logT=195 
    */
    BTree t(195); // create btree with a given order (t) 

    // the data generator to generate randomized citizen data
    DataGenerator dataGenerator;

    // the page manager to do write and read disk operations in a given file
    PageManager pageManager(citizenDataFileName.c_str());

    if (pageManager.GetFileSize() == 0)
    {
        GenerateRandomDataAndCreateBTree(citizenDataFileName, dataGenerator, numberOfRecordsToGenerate, pageManager, t, btreeSerializedFileName);
    }
    else 
    {
        std::cout << "File " << citizenDataFileName << " has data -> loading BTree\n";
        LoadBTreeToRAM(pageManager, t, btreeSerializedFileName);
        ClearConsole();
    }
    
    bool btreeUpdated = false;
    int option;
    do {
        mostrar_menu();
        std::cin >> option;
        std::cin.ignore(); // Limpiar el buffer de entrada

        switch (option) {
        case 1: {
            InsertOperation(dataGenerator, t, pageManager, btreeSerializedFileName, btreeUpdated);
            break;
        }
        case 2: {
            SearchOperation(t, pageManager);
            break;
        }
        case 3: {
            DeleteOperation(pageManager, t, btreeSerializedFileName, btreeUpdated);
            break;
        }
        case 4: {
            PrintRecordsFromXtoY(pageManager, citizenDataFileName);
            break;
        }
        case 5:
            ClearConsole();
            break;
        case 6:
            RangeSearchOperation(pageManager, t);
            break;
        case 7:
            // Serializar el B-Tree en el archivo 'btree_serialized.bin'
            SaveChangesInBTree(btreeUpdated, pageManager, t, btreeSerializedFileName);
            break;
        default:
            std::cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (option != 7);
    
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
        
    return 0;
    
}

void RangeSearchOperation(PageManager& pageManager, BTree& t)
{
    std::string start, end;
    std::cout << "Ingrese el DNI inicial del rango: ";
    std::cin >> start;
    std::cout << "Ingrese el DNI final del rango: ";
    std::cin >> end;

    pageManager.RangeSearchAndPrint(t, start, end);
}

void BenchmarkLecturaSerializadoTreintaMillones()
{
    std::string thirtyMillionCitizenDataFileName = "thirtyMPeople.bin";
    std::string thirtyMBTreeFilename = "ThirtyMBTree.bin";
    PageManager pageManager5(thirtyMillionCitizenDataFileName.c_str());
    BTree thirtyMillionBtree(195);
    std::chrono::time_point<std::chrono::system_clock> inicio5;
    std::chrono::time_point<std::chrono::system_clock> fin5;
    inicio5 = std::chrono::system_clock::now();
    std::cout << " Testint generating 30 million data";
    pageManager5.DeserializeBTree(thirtyMillionBtree, thirtyMBTreeFilename.c_str());
    fin5 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo5 = fin5 - inicio5;
    double tiempo_segundos5 = tiempo5.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos5 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkLecturaSerializadoVeinteMillones()
{
    std::string twentyMillionCitizenDataFileName = "twentyMPeople.bin";
    std::string twentyMBTreeFilename = "TwentyMBTree.bin";
    PageManager pageManager4(twentyMillionCitizenDataFileName.c_str());
    BTree twentyMillionBtree(105);
    std::chrono::time_point<std::chrono::system_clock> inicio4;
    std::chrono::time_point<std::chrono::system_clock> fin4;
    inicio4 = std::chrono::system_clock::now();
    std::cout << " Testint loading 20 million data and desearializing it";
    pageManager4.DeserializeBTree(twentyMillionBtree, twentyMBTreeFilename.c_str());
    fin4 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo4 = fin4 - inicio4;
    double tiempo_segundos4 = tiempo4.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos4 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkLecturaSerializadoQuinceMillones()
{
    std::string fifteenMillionCitizenDataFileName = "fifteenMPeople.bin";
    std::string fifteenMBTreeFilename = "FifteenMBTree.bin";
    PageManager pageManager3(fifteenMillionCitizenDataFileName.c_str());
    BTree fifteenMillionBtree(75);
    std::chrono::time_point<std::chrono::system_clock> inicio3;
    std::chrono::time_point<std::chrono::system_clock> fin3;
    inicio3 = std::chrono::system_clock::now();
    std::cout << " Testint loading 15 million data to btree and deserialzing";
    pageManager3.DeserializeBTree(fifteenMillionBtree, fifteenMBTreeFilename.c_str());
    fin3 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo3 = fin3 - inicio3;
    double tiempo_segundos3 = tiempo3.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos3 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchMarkLecturaSerializadoDiezMillones()
{
    std::string tenMillionCitizenDataFileName = "tenMPeople.bin";
    std::string tenMBTreeFilename = "TenMBTree.bin";
    PageManager pageManager2(tenMillionCitizenDataFileName.c_str());
    BTree tenMillionBtree(45);
    std::chrono::time_point<std::chrono::system_clock> inicio2;
    std::chrono::time_point<std::chrono::system_clock> fin2;
    inicio2 = std::chrono::system_clock::now();
    std::cout << " Testint loading 10 million data to btree and deserialazing";
    pageManager2.DeserializeBTree(tenMillionBtree, tenMBTreeFilename.c_str());
    fin2 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo2 = fin2 - inicio2;
    double tiempo_segundos2 = tiempo2.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos2 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkLecturaSerializadoCincoMillones()
{
    std::string fiveMillionCitizenDataFileName = "fiveMPeople.bin";
    std::string fiveMBTreeFilename = "FiveMBTree.bin";
    PageManager pageManager1(fiveMillionCitizenDataFileName.c_str());
    BTree fiveMillionBtree(25);
    std::chrono::time_point<std::chrono::system_clock> inicio1;
    std::chrono::time_point<std::chrono::system_clock> fin1;
    inicio1 = std::chrono::system_clock::now();
    std::cout << " Testing load 5 million from BTree serialized";
    pageManager1.DeserializeBTree(fiveMillionBtree, fiveMBTreeFilename.c_str());
    fin1 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo1 = fin1 - inicio1;
    double tiempo_segundos1 = tiempo1.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos1 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkCargaYSerizliadoBTreeTreintaMillones()
{
    std::string thirtyMillionCitizenDataFileName = "thirtyMPeople.bin";
    std::string thirtyMBTreeFilename = "ThirtyMBTree.bin";
    PageManager pageManager5(thirtyMillionCitizenDataFileName.c_str());
    BTree thirtyMillionBtree(195);
    std::chrono::time_point<std::chrono::system_clock> inicio5;
    std::chrono::time_point<std::chrono::system_clock> fin5;
    inicio5 = std::chrono::system_clock::now();
    std::cout << " Testint generating 30 million data";
    pageManager5.ReadFileAndLoadToBtree(thirtyMillionBtree);
    pageManager5.SerializeBTree(thirtyMillionBtree, thirtyMBTreeFilename.c_str());
    fin5 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo5 = fin5 - inicio5;
    double tiempo_segundos5 = tiempo5.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos5 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkCargaYSerializadoBTreeVeinteMillones()
{
    std::string twentyMillionCitizenDataFileName = "twentyMPeople.bin";
    std::string twentyMBTreeFilename = "TwentyMBTree.bin";
    PageManager pageManager4(twentyMillionCitizenDataFileName.c_str());
    BTree twentyMillionBtree(105);
    std::chrono::time_point<std::chrono::system_clock> inicio4;
    std::chrono::time_point<std::chrono::system_clock> fin4;
    inicio4 = std::chrono::system_clock::now();
    std::cout << " Testint generating 20 million data";
    pageManager4.ReadFileAndLoadToBtree(twentyMillionBtree);
    pageManager4.SerializeBTree(twentyMillionBtree, twentyMBTreeFilename.c_str());
    fin4 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo4 = fin4 - inicio4;
    double tiempo_segundos4 = tiempo4.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos4 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkCargaYSerializadoBTreeQuinceMillones()
{
    std::string fifteenMillionCitizenDataFileName = "fifteenMPeople.bin";
    std::string fifteenMBTreeFilename = "FifteenMBTree.bin";
    PageManager pageManager3(fifteenMillionCitizenDataFileName.c_str());
    BTree fifteenMillionBtree(75);
    std::chrono::time_point<std::chrono::system_clock> inicio3;
    std::chrono::time_point<std::chrono::system_clock> fin3;
    inicio3 = std::chrono::system_clock::now();
    std::cout << " Testint loading 15 million data to btree and serialzing";
    pageManager3.ReadFileAndLoadToBtree(fifteenMillionBtree);
    pageManager3.SerializeBTree(fifteenMillionBtree, fifteenMBTreeFilename.c_str());
    fin3 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo3 = fin3 - inicio3;
    double tiempo_segundos3 = tiempo3.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos3 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkCargaYSerializadoBTreeDiezMillones()
{
    std::string tenMillionCitizenDataFileName = "tenMPeople.bin";
    std::string tenMBTreeFilename = "TenMBTree.bin";
    PageManager pageManager2(tenMillionCitizenDataFileName.c_str());
    BTree tenMillionBtree(45);
    std::chrono::time_point<std::chrono::system_clock> inicio2;
    std::chrono::time_point<std::chrono::system_clock> fin2;
    inicio2 = std::chrono::system_clock::now();
    std::cout << " Testint loading 10 million data to btree and serialazing";
    pageManager2.ReadFileAndLoadToBtree(tenMillionBtree);
    pageManager2.SerializeBTree(tenMillionBtree, tenMBTreeFilename.c_str());
    fin2 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo2 = fin2 - inicio2;
    double tiempo_segundos2 = tiempo2.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos2 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkCargaYSerializadoBTreeCincoMillones()
{
    std::string fiveMillionCitizenDataFileName = "fiveMPeople.bin";
    std::string fiveMBTreeFilename = "FiveMBTree.bin";
    PageManager pageManager1(fiveMillionCitizenDataFileName.c_str());
    BTree fiveMillionBtree(25);
    std::chrono::time_point<std::chrono::system_clock> inicio1;
    std::chrono::time_point<std::chrono::system_clock> fin1;
    inicio1 = std::chrono::system_clock::now();
    std::cout << " Testint loading 5 million data to BTree and serializing it";
    pageManager1.ReadFileAndLoadToBtree(fiveMillionBtree);
    pageManager1.SerializeBTree(fiveMillionBtree, fiveMBTreeFilename.c_str());
    fin1 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo1 = fin1 - inicio1;
    double tiempo_segundos1 = tiempo1.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos1 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkGenerarArchivoTreintaMillones()
{
    std::string thirtyMillionCitizenDataFileName = "thirtyMPeople.bin";
    std::string thirtyMBTreeFilename = "ThirtyMBTree.bin";
    long thirtyMillionRecordsToGenerate = 30000000;
    DataGenerator dataGenerator5;
    PageManager pageManager5(thirtyMillionCitizenDataFileName.c_str());
    std::chrono::time_point<std::chrono::system_clock> inicio5;
    std::chrono::time_point<std::chrono::system_clock> fin5;
    inicio5 = std::chrono::system_clock::now();
    std::cout << " Testint generating 30 million data";
    std::cout << "File " << thirtyMillionCitizenDataFileName << " has no data -> randomizing and writing data\n";
    dataGenerator5.GenerateNRecordsData(thirtyMillionRecordsToGenerate, pageManager5);
    fin5 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo5 = fin5 - inicio5;
    double tiempo_segundos5 = tiempo5.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos5 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkGenerarArchivoVeinteMillones()
{
    std::string twentyMillionCitizenDataFileName = "twentyMPeople.bin";
    std::string twentyMBTreeFilename = "TwentyMBTree.bin";
    long twentyMillionRecordsToGenerate = 20000000;
    DataGenerator dataGenerator4;
    PageManager pageManager4(twentyMillionCitizenDataFileName.c_str());
    std::chrono::time_point<std::chrono::system_clock> inicio4;
    std::chrono::time_point<std::chrono::system_clock> fin4;
    inicio4 = std::chrono::system_clock::now();
    std::cout << " Testint generating 20 million data";
    std::cout << "File " << twentyMillionCitizenDataFileName << " has no data -> randomizing and writing data\n";
    dataGenerator4.GenerateNRecordsData(twentyMillionRecordsToGenerate, pageManager4);
    fin4 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo4 = fin4 - inicio4;
    double tiempo_segundos4 = tiempo4.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos4 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkGenerarArchivoQuinceMillones()
{
    std::string fifteenMillionCitizenDataFileName = "fifteenMPeople.bin";
    std::string fifteenMBTreeFilename = "FifteenMBTree.bin";
    long fifteenMillionRecordsToGenerate = 15000000;
    DataGenerator dataGenerator3;
    PageManager pageManager3(fifteenMillionCitizenDataFileName.c_str());
    std::chrono::time_point<std::chrono::system_clock> inicio3;
    std::chrono::time_point<std::chrono::system_clock> fin3;
    inicio3 = std::chrono::system_clock::now();
    std::cout << " Testint generating 15 million data";
    std::cout << "File " << fifteenMillionCitizenDataFileName << " has no data -> randomizing and writing data\n";
    dataGenerator3.GenerateNRecordsData(fifteenMillionRecordsToGenerate, pageManager3);
    fin3 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo3 = fin3 - inicio3;
    double tiempo_segundos3 = tiempo3.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos3 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchmarkGenerarArchivoDiezMillones()
{
    std::string tenMillionCitizenDataFileName = "tenMPeople.bin";
    std::string tenMBTreeFilename = "TenMBTree.bin";
    long tenMillionRecordsToGenerate = 10000000;
    DataGenerator dataGenerator2;
    PageManager pageManager2(tenMillionCitizenDataFileName.c_str());
    std::chrono::time_point<std::chrono::system_clock> inicio2;
    std::chrono::time_point<std::chrono::system_clock> fin2;
    inicio2 = std::chrono::system_clock::now();
    std::cout << " Testint generating 10 million data";
    std::cout << "File " << tenMillionCitizenDataFileName << " has no data -> randomizing and writing data\n";
    dataGenerator2.GenerateNRecordsData(tenMillionRecordsToGenerate, pageManager2);
    fin2 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo2 = fin2 - inicio2;
    double tiempo_segundos2 = tiempo2.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos2 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void BenchMarkGenerarArchivoCincoMillones()
{
    std::string fiveMillionCitizenDataFileName = "fiveMPeople.bin";
    std::string fiveMBTreeFilename = "FiveMBTree.bin";
    long fiveMillionRecordsToGenerate = 5000000;
    DataGenerator dataGenerator1;
    PageManager pageManager1(fiveMillionCitizenDataFileName.c_str());
    std::chrono::time_point<std::chrono::system_clock> inicio1;
    std::chrono::time_point<std::chrono::system_clock> fin1;
    inicio1 = std::chrono::system_clock::now();
    std::cout << " Testint generating 5 million data";
    std::cout << "File " << fiveMillionCitizenDataFileName << " has no data -> randomizing and writing data\n";
    dataGenerator1.GenerateNRecordsData(fiveMillionRecordsToGenerate, pageManager1);
    fin1 = std::chrono::system_clock::now();
    std::chrono::duration<double> tiempo1 = fin1 - inicio1;
    double tiempo_segundos1 = tiempo1.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos1 << " segundos";
    std::cout << "\n== PROGRAMA TERMINADO ==\n";
}

void GenerateRandomDataAndCreateBTree(std::string& citizenDataFileName, DataGenerator& dataGenerator, long numberOfRecordsToGenerate, PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName)
{
    std::cout << "File " << citizenDataFileName << " has no data -> randomizing and writing data\n";

    dataGenerator.GenerateNRecordsData(numberOfRecordsToGenerate, pageManager);
    pageManager.ReadFileAndLoadToBtree(t);
    pageManager.SerializeBTree(t, btreeSerializedFileName.c_str());
}

void SaveChangesInBTree(bool btreeUpdated, PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName)
{
    if (btreeUpdated) {
        std::cout << "Guardando datos del BTree...\n";
        pageManager.SerializeBTree(t, btreeSerializedFileName.c_str());
    }
    std::cout << "Saliendo...\n";
}

void PrintRecordsFromXtoY(PageManager& pageManager, std::string& citizenDataFileName)
{
    int inicio, final;
    std::cout << "Ingrese el indice de inicio: ";
    std::cin >> inicio;
    std::cout << "Ingrese el indice final: ";
    std::cin >> final;

    pageManager.PrintOneHundredRecords(citizenDataFileName.c_str(), inicio, final);
}

void DeleteOperation(PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName, bool& btreeUpdated)
{
    // Eliminar un registro por DNI
    std::string dni;
    std::cout << "Ingrese el DNI a eliminar: ";
    std::getline(std::cin, dni);

    pageManager.DeleteRecordFromDisk(t, dni.c_str(), btreeSerializedFileName.c_str());
    btreeUpdated = true;
}

void SearchOperation(BTree& t, PageManager& pageManager)
{
    // Buscar un registro por DNI
    std::string dni;
    std::cout << "Ingrese el DNI a buscar: ";
    std::getline(std::cin, dni);

    // search given DNI in BTree
    int pageID = t.SearchDNIAndGetPageID(dni.c_str());
    if (pageID != -1) {
        Personita persona = pageManager.ReadFileGetPersonByPageID(pageID);
        std::cout << "\n";
        persona.ImprimirDatos();
    }
    else {
        std::cout << "No existe una persona con el DNI " << dni << "\n";
    }
    std::cout << "\n";
}

void InsertOperation(DataGenerator& dataGenerator, BTree& t, PageManager& pageManager, std::string& btreeSerializedFileName, bool& btreeUpdated)
{
    // Insertar un nuevo registro
    Personita newRandomPerson = dataGenerator.GenerateRandomPersonita();
    std::cout << "\n";
    newRandomPerson.ImprimirDatos();

    // check if DNI exists
    int pageID = t.SearchDNIAndGetPageID(newRandomPerson.dni);
    std::cout << "\n";
    // if DNI does not exist then
    if (pageID == -1)
    {
        std::cout << "GOOD !! La data se insertara al BTree\n";
        pageManager.InsertNewRecordInDiskAndBTree(t, newRandomPerson, btreeSerializedFileName.c_str());
        btreeUpdated = true;
    }
    // if DNI exists then
    else
    {
        std::cout << "Ya existe ese DNI...genera otro\n";
    }
    std::cout << "\n";
}

void LoadBTreeToRAM(PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName)
{
    std::atomic<bool> loading{ true };
    std::thread loadingThread(LoadingMessage, std::ref(loading));
    //std::cout << "data generada -> se carga el serializado";
    pageManager.DeserializeBTree(t, btreeSerializedFileName.c_str());
    // Detener el mensaje de carga y esperar que termine el thread
    loading = false;
    loadingThread.join();
    std::cout << "\nB-Tree cargado a RAM.\n";
}
