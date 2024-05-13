#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class Grafo
{
private:
    int nn;                                         // numero de nodos
    std::vector<std::tuple<int, int, int>> aristas; // lista de tuplas contodas las aristas (nodo (int), nodo (int), peso (int))

public:
    Grafo(int n) : nn(n) {}

    void agregar_arista(int u, int v, int peso) // agrega una arista al grafo con sus nodos y peso correspondiente
    {
        aristas.push_back(std::make_tuple(u, v, peso)); // funcion de la biblioteca estandar para crear una tupla
    }

    std::vector<std::tuple<int, int, int>> obtener_aristas()
    {
        return aristas;
    }

    int get_nn() const { return nn; } // método de acceso (getter) para nn, no se porque no dejaba con la friend class
};

class UnionFind
{
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int tamaño)
    {
        parent.resize(tamaño);  // haz parent de longitud tamaño y llena con valores de 0 a n-1 (for)
        rank.resize(tamaño, 1); // haz size de longitud tamaño y llena con 1's
        for (int i = 0; i < tamaño; i++)
        {
            parent[i] = i; // padre de cada elemento es el mismo elemento
        }
    }

    // encuentra el representante del conjunto que contiene a u
    int encontrar(int u)
    {
        if (parent[u] != u) // si el padre de 'u' no es 'u', entonces el padre de 'u' es el padre de su padre
        {
            parent[u] = encontrar(parent[u]);
        }
        return parent[u];
    }

    void unir(int u, int v) // une los conjuntos que contienen a u y v
    {
        int raiz_u = encontrar(u);
        int raiz_v = encontrar(v);
        if (raiz_u != raiz_v) // los padres pertenecen a conjuntos distintos, entonces los unimos
        {
            if (rank[raiz_u] > rank[raiz_v])
            {
                parent[raiz_v] = raiz_u;
            }
            else if (rank[raiz_u] < rank[raiz_v])
            {
                parent[raiz_u] = raiz_v;
            }
            else
            {
                parent[raiz_v] = raiz_u;
                rank[raiz_u] += 1;
            }
        }
    }
};

std::vector<std::tuple<int, int, int>> Kruskal(Grafo &G)
{
    std::vector<std::tuple<int, int, int>> MST;                                     // lista  de tuplas para guardar el MST (Minimum Spanning Tree)
    UnionFind uf(G.get_nn());                                                       // crear una instancia de UnionFind, le pasamos el numero de nodos
    std::vector<std::tuple<int, int, int>> aristas_ordenadas = G.obtener_aristas(); // obtenemos las aristas
    std::sort(aristas_ordenadas.begin(), aristas_ordenadas.end());                  // Paso 1: las ordenamos en orden no decreciente

    for (auto &arista : aristas_ordenadas) // Paso 2: Agregar aristas al MST, asegurando que no formen ciclos para peso, u, v en aristas ordenadas:
    {
        int peso, u, v;
        std::tie(u, v, peso) = arista;          // desempaqueta la tupla arista en tres variables
        if (uf.encontrar(u) != uf.encontrar(v)) // si u y v no pertenecen al mismo conjunto, lo que significa
                                                // que agregar la arista no formaría un ciclo en el MST, se une u y v
        {
            uf.unir(u, v);
            MST.push_back(std::make_tuple(u, v, peso)); // agregamos como tupla a nuestro MST
        }
    }

    return MST;
}

int main()
{
    Grafo G(500); // Crear un grafo con 500 nodos

    srand(time(0)); // Initialize random number generator.

    for (int i = 0; i < 500; i++)
    {
        for (int j = i + 1; j < 500; j++)
        {
            int peso = (rand() % 50) + 1; // Genera un peso aleatorio entre 1 y 50
            G.agregar_arista(i, j, peso);
        }
    }

    // Correr el algoritmo de Kruskal para encontrar el MST
    std::vector<std::tuple<int, int, int>> MST = Kruskal(G);

    // Imprimir las aristas con el MST
    for (auto &arista : MST)
    {
        int u, v, peso;
        std::tie(u, v, peso) = arista;
        std::cout << u << " - " << v << " : " << peso << std::endl;
    }

    return 0;
}
