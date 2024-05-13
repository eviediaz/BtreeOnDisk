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

bool compare(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b)
{
    return std::get<2>(a) < std::get<2>(b);
}

std::vector<std::tuple<int, int, int>> kruskal(Grafo& G)
{
    std::vector<std::tuple<int, int, int>> mst; // Minimum Spanning Tree
    std::vector<std::tuple<int, int, int>> aristas = G.obtener_aristas();

    // Ordenar las aristas en orden creciente de peso
    std::sort(aristas.begin(), aristas.end(), compare);

    // Inicializar UnionFind
    UnionFind uf(G.get_nn());

    // Recorrer todas las aristas en orden creciente de peso
    for (const auto& arista : aristas)
    {
        int u, v, peso;
        std::tie(u, v, peso) = arista;

        // Si unir estos dos nodos no crea un ciclo en el MST, añadir esta arista al MST
        if (uf.encontrar(u) != uf.encontrar(v))
        {
            mst.push_back(arista);
            uf.unir(u, v);
        }
    }

    return mst;
}

int main()
{
    int NUMBER_NODES = 5;
    int MAX_NUMBER_GEN_SEED = 500;
    Grafo G(NUMBER_NODES); // Crear un grafo con 500 nodos

    srand(time(0)); // Initialize random number generator.

    // Agregar las aristas con los pesos especificados
    G.agregar_arista(0, 1, 2);
    G.agregar_arista(0, 2, 9);
    G.agregar_arista(0, 3, 9);
    G.agregar_arista(0, 4, 9);
    G.agregar_arista(1, 2, 9);
    G.agregar_arista(1, 2, 9);
    G.agregar_arista(1, 3, 2);
    G.agregar_arista(1, 4, 8);
    G.agregar_arista(2, 3, 1);
    G.agregar_arista(2, 4, 9);
    G.agregar_arista(3, 4, 1);
    /*
    for (int i = 0; i < NUMBER_NODES; i++)
    {
        for (int j = i + 1; j < NUMBER_NODES; j++)
        {
            int peso = (rand() % MAX_NUMBER_GEN_SEED) + 1; // Genera un peso aleatorio entre 1 y 50
            G.agregar_arista(i, j, peso);
        }
    }
    */

    G.printGraph();

    // Correr el algoritmo de Kruskal para encontrar el MST
    //std::vector<std::tuple<int, int, int>> MST = Kruskal(G);


    std::cout << " \nKRUSKAL RESULT : " << std::endl;
    // Imprimir las aristas con el MST
    /*
    for (auto &arista : MST)
    {
        int u, v, peso;
        std::tie(u, v, peso) = arista;
        std::cout << u << " - " << v << " : " << peso << std::endl;
    }
    */
    std::vector<std::tuple<int, int, int>> mst = kruskal(G);
    for (const auto& arista : mst)
    {
        int u, v, peso;
        std::tie(u, v, peso) = arista;
        std::cout << u << " - " << v << " : " << peso << std::endl;
    }


    return 0;
}
