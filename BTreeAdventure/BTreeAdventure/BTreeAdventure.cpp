#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <cstring>
#include "PageManager.h"
#include "Person.h"
#include "DataGenerator.h"

int main() {
    // Crear instancia de DataGenerator
    DataGenerator dataGenerator;

    // Crear instancia de PageManager
    PageManager pageManager;

    // Ruta completa al archivo
    const char* filePath = "citizens.bin"; // Cambia a la ruta completa de tu archivo

    // Generar y agregar 30 millones de personas aleatorias al archivo
    long numPersons = 33000000;
    dataGenerator.GenerateNRecordsData(numPersons, pageManager, filePath);

    return 0;
}
