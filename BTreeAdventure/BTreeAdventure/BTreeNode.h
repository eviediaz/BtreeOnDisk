#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm> // para std::lower_bound y std::upper_bound

class BTreeNode 
{
public:
    int minimunDegree;
    std::vector<int> pagesID;
    std::vector<std::string> dnis; // this is the "key"
    std::vector<BTreeNode*> children;
    int actualNumberKeys;
    bool isLeaf;

    BTreeNode(int minimunDegree, bool isLeaf) 
    {
        this->minimunDegree = minimunDegree;
        this->isLeaf = isLeaf;
        actualNumberKeys = 0;
        pagesID.resize(2 * minimunDegree - 1);
        dnis.resize(2 * minimunDegree - 1);
        children.resize(2 * minimunDegree);
    }    
    
    void InsertNonFull(const char* dni, int pageID) {
        // Inicializa i con el índice del elemento más a la derecha
        int i = actualNumberKeys - 1;

        // Si este es un nodo hoja
        if (isLeaf) {
            // El siguiente bucle hace dos cosas:
            // a) Encuentra la ubicación de la nueva clave a insertar
            // b) Mueve todas las claves mayores un lugar hacia adelante

            while (i >= 0 && (std::memcmp(dnis[i].c_str(), dni, 8) > 0)) {
                dnis[i + 1] = dnis[i];
                pagesID[i + 1] = pagesID[i];
                i--;
            }

            // Inserta la nueva clave en la ubicación encontrada
            dnis[i + 1] = std::string(dni);
            pagesID[i + 1] = pageID;
            actualNumberKeys = actualNumberKeys + 1;
        }
        else { // Si este nodo no es hoja
            // Encuentra el hijo que va a tener la nueva clave
            while (i >= 0 && (std::memcmp(dnis[i].c_str(), dni, 8) > 0)) i--;

            // Ver si el hijo encontrado está lleno
            if (children[i + 1]->actualNumberKeys == 2 * minimunDegree - 1) {
                // Si el hijo está lleno, entonces divídelo
                SplitChild(i + 1, children[i + 1]);

                // Después de dividir, la clave del medio de children[i] sube y children[i]
                // se divide en dos. Ver cuál de los dos va a tener la nueva clave
                if (std::memcmp(dnis[i + 1].c_str(), dni, 8) < 0) i++;
            }
            children[i + 1]->InsertNonFull(dni, pageID);
        }
        
    }
    

    void SplitChild(int i, BTreeNode* y)
    {
        // Crea un nuevo nodo que va a almacenar (t-1) claves de y
        BTreeNode* z = new BTreeNode(y->minimunDegree, y->isLeaf);
        z->actualNumberKeys = minimunDegree - 1;

        // Copia las últimas (t-1) claves de y a z
        for (int j = 0; j < minimunDegree - 1; j++)
        {
            z->dnis[j] = y->dnis[j + minimunDegree];
            z->pagesID[j] = y->pagesID[j + minimunDegree];
        }

        // Copia los últimos t hijos de y a z
        if (!y->isLeaf) {
            for (int j = 0; j < minimunDegree; j++)
                z->children[j] = y->children[j + minimunDegree];
        }

        // Reduce el número de claves en y
        y->actualNumberKeys = minimunDegree - 1;

        // Dado que este nodo va a tener un nuevo hijo,
        // crea espacio para el nuevo hijo
        for (int j = actualNumberKeys; j >= i + 1; j--)
            children[j + 1] = children[j];

        // Enlaza el nuevo hijo a este nodo
        children[i + 1] = z;

        // Una clave de y se moverá a este nodo. Mueve todas las claves mayores
        // un lugar hacia adelante
        for (int j = actualNumberKeys - 1; j >= i; j--)
        {
            dnis[j + 1] = dnis[j];
            pagesID[j + 1] = pagesID[j];
        }

        // Copia la clave del medio de y a este nodo
        dnis[i] = y->dnis[minimunDegree - 1];
        pagesID[i] = y->pagesID[minimunDegree - 1];

        // Incrementa el conteo de claves en este nodo
        actualNumberKeys = actualNumberKeys + 1;
    }

    int search(const char* dni) {
        // Encuentra la primera clave mayor o igual a dni
        int i = 0;
        while (i < actualNumberKeys && (std::memcmp(dnis[i].c_str(), dni, 8) < 0)) {
            i++;
        }

        // Si la clave se encuentra en este nodo, devuelve su page ID
        if (i < actualNumberKeys && std::memcmp(dnis[i].c_str(), dni, 8) == 0) {
            return pagesID[i];
        }

        // Si la clave no se encuentra aquí y este es un nodo hoja
        if (isLeaf) {
            std::cout << "Clave " << dni << " no encontrada en el B-Tree." << std::endl;
            return -1; // Indicador de que la clave no se encontró
        }

        // Va al subárbol adecuado
        return children[i]->search(dni);
    }

    void Traverse() {
        int i;
        for (i = 0; i < actualNumberKeys; i++) {
            // Si este nodo no es hoja, entonces antes de imprimir la clave[i],
            // recorre el subárbol enraizado con el hijo children[i].
            if (!isLeaf) children[i]->Traverse();
            std::cout << " " << dnis[i] << " -page id: " << pagesID[i] << std::endl;
        }

        // Imprime el subárbol enraizado con el último hijo
        if (!isLeaf) children[i]->Traverse();
    }

    void printBTree(int level = 0, int index = 0) {
        // Imprime el nivel y el índice del nodo
        std::cout << std::string(level * 2, ' ') << "Nivel " << level << " - Nodo " << index << ":\n";

        // Imprime las claves y los ID de página del nodo
        std::cout << std::string(level * 2, ' ') << "Claves: ";
        for (int i = 0; i < actualNumberKeys; ++i) {
            std::cout << dnis[i] << " (Pagina ID: " << pagesID[i] << ") ";
        }
        std::cout << "\n";

        // Recorre los hijos del nodo
        for (int i = 0; i <= actualNumberKeys; ++i) {
            if (children[i] != nullptr) {
                children[i]->printBTree(level + 1, i);
            }
        }
    }
};