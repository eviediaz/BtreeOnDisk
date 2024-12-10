#pragma once
#include "BTreeNode.h"
class BTree 
{
    BTreeNode* root;
    int minimunDegree;  // Grado mínimo
public:
    BTree(int minimunDegree) {
        root = nullptr;
        this->minimunDegree = minimunDegree;
    }

    BTreeNode* GetRoot() {
        return root;
    }

    int GetMinimunDegree()
    {
        return minimunDegree;
    }

    void SetRoot(BTreeNode* newRoot) {
        root = newRoot;
    }

    void Insert(const char* dniToInsert, int pageID)
    {
        // Si el árbol está vacío
        if (root == nullptr) {
            // Asigna memoria para la raíz
            root = new BTreeNode(minimunDegree, true);
            std::memcpy(root->dnis[0].data(), dniToInsert, 8);
            root->dnis[0][8] = '\0';  // Inserta la clave
            root->pagesID[0] = pageID;
            root->actualNumberKeys = 1;  // Actualiza el número de claves en la raíz
        }
        else { // Si el árbol no está vacío
            // Si la raíz está llena, entonces el árbol crece en altura
            if (root->actualNumberKeys == 2 * minimunDegree - 1) {
                // Asigna memoria para la nueva raíz
                BTreeNode* s = new BTreeNode(minimunDegree, false);

                // Hace la antigua raíz como hijo de la nueva raíz
                s->children[0] = root;

                // Divide la antigua raíz y mueve 1 clave a la nueva raíz
                s->SplitChild(0, root);

                // La nueva raíz tiene dos hijos ahora. Decide cuál de los
                // dos hijos va a tener la nueva clave
                int i = 0;
                
                if ((std::memcmp(s->dnis[0].data(), dniToInsert, 8) < 0)) i++;
                s->children[i]->InsertNonFull(dniToInsert, pageID);

                // Cambia la raíz
                root = s;
            }
            else { // Si la raíz no está llena, llama a insertNonFull para la raíz
                root->InsertNonFull(dniToInsert, pageID);
            }
        }
    }

    int SearchDNIAndGetPageID(const char* dni) {
        // Si la raíz es nula, el árbol está vacío, retorna -1
        if (root == nullptr) {
            std::cout << "\nEl arbol B esta vacio. Clave " << dni << " no encontrada." << std::endl;
            return -1;
        }
        // Llama al método search de la raíz
        return root->search(dni);
    }

    void Remove(const char* dni) {
        if (!root) {
            std::cout << "\nEl arbol esta vacío. No se puede eliminar la clave: " << dni << std::endl;
            return;
        }

        root->Remove(dni);

        if (root->actualNumberKeys == 0) {
            BTreeNode* tmp = root;
            if (root->isLeaf) {
                root = nullptr;
            }
            else {
                root = root->children[0];
            }
            delete tmp;
        }
    }

    void TraverseTree() 
    {
        if (root != nullptr) root->Traverse();
    }

    void PrintBTree() {
        if (root != nullptr) root->printBTree();
    }

    std::vector<std::string> RangeSearch(const std::string& start, const std::string& end) {
        std::vector<std::string> result;
        if (root != nullptr) {
            root->RangeSearch(start, end, result);
        }
        return result;
    }

};