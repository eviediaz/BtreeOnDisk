#include <iostream>
#include <string>
#include <chrono>
#include "BTreeMemory.h"
#include "Persona.h"

using APP_CORE::MEMORY::btree;

int main()
{
    btree<int> bt;
    //const int meme = 1000000;
    long long xd = 10000000;

    std::chrono::time_point<std::chrono::system_clock> inicio;
    std::chrono::time_point<std::chrono::system_clock> fin;
    //int input[15] = {6,2,3,7,1,5,12,9,8,0,15,21,19,27,13};

    inicio = std::chrono::system_clock::now();
    
    /*
    for (int i = 0; i < meme; i++)
    {
        bt.insert(9);
    }
    */

    for (long long i = 0; i < xd; i++)
    {
        bt.insert(9);
    }
    fin = std::chrono::system_clock::now();

    std::chrono::duration<double> tiempo = fin - inicio;
    double tiempo_segundos = tiempo.count();
    std::cout << "\nEl tiempo de ejecucion es: " << tiempo_segundos << " segundos";
    std::cout << "\nfinished xd\n";
    //bt.print();
    //std::cout << " ###### IN ORDER #########\n";
    //bt.inorder();

    return 0;
}