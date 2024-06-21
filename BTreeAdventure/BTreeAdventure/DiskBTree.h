#pragma once
#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <queue>
#include "BTree.h"

class DiskBTree {
private:
    const char* filename;
    HANDLE hFile;
    BTree* btree;

public:
    DiskBTree(const char* filename, int minDegree) : filename(filename) {
        hFile = CreateFileA(
            filename,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        btree = new BTree(minDegree);
        LoadFromFile();  // Load data from file during initialization
    }

    ~DiskBTree() {
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
        }
        delete btree;
    }

    void WriteNodeToDisk(BTreeNode* node, int pageID) {
        DWORD written;
        SetFilePointer(hFile, pageID * sizeof(BTreeNode), NULL, FILE_BEGIN);
        WriteFile(hFile, reinterpret_cast<char*>(&node->minimunDegree), sizeof(node->minimunDegree), &written, NULL);
        WriteFile(hFile, reinterpret_cast<char*>(&node->actualNumberKeys), sizeof(node->actualNumberKeys), &written, NULL);
        WriteFile(hFile, node->dnis.data(), node->dnis.size() * sizeof(node->dnis[0]), &written, NULL);
        WriteFile(hFile, node->pagesID.data(), node->pagesID.size() * sizeof(node->pagesID[0]), &written, NULL);
        WriteFile(hFile, &node->isLeaf, sizeof(node->isLeaf), &written, NULL);

        for (int i = 0; i <= node->actualNumberKeys; ++i) {
            if (node->children[i]) {
                int childPageID = i; // Assumed mapping, should be managed properly
                WriteFile(hFile, &childPageID, sizeof(childPageID), &written, NULL);
            }
        }
    }

    BTreeNode* ReadNodeFromDisk(int pageID) {
        DWORD read;
        SetFilePointer(hFile, pageID * sizeof(BTreeNode), NULL, FILE_BEGIN);

        BTreeNode* node = new BTreeNode(btree->GetMinimunDegree(), true);
        ReadFile(hFile, reinterpret_cast<char*>(&node->minimunDegree), sizeof(node->minimunDegree), &read, NULL);
        ReadFile(hFile, reinterpret_cast<char*>(&node->actualNumberKeys), sizeof(node->actualNumberKeys), &read, NULL);
        ReadFile(hFile, node->dnis.data(), node->dnis.size() * sizeof(node->dnis[0]), &read, NULL);
        ReadFile(hFile, node->pagesID.data(), node->pagesID.size() * sizeof(node->pagesID[0]), &read, NULL);
        ReadFile(hFile, &node->isLeaf, sizeof(node->isLeaf), &read, NULL);

        std::queue<std::pair<BTreeNode*, int>> nodesToProcess;
        nodesToProcess.push(std::make_pair(node, pageID));

        while (!nodesToProcess.empty()) {
            std::pair<BTreeNode*, int> current = nodesToProcess.front();
            BTreeNode* currentNode = current.first;
            int currentPageID = current.second;
            nodesToProcess.pop();

            SetFilePointer(hFile, currentPageID * sizeof(BTreeNode), NULL, FILE_BEGIN);

            for (int i = 0; i <= currentNode->actualNumberKeys; ++i) {
                int childPageID;
                ReadFile(hFile, &childPageID, sizeof(childPageID), &read, NULL);
                if (childPageID != -1) {
                    BTreeNode* childNode = new BTreeNode(btree->GetMinimunDegree(), true);
                    nodesToProcess.push(std::make_pair(childNode, childPageID));
                    currentNode->children[i] = childNode;
                }
                else {
                    currentNode->children[i] = nullptr;
                }
            }
        }

        return node;
    }

    void LoadFromFile() {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
            return;  // File is empty or error in getting file size
        }

        btree->SetRoot(ReadNodeFromDisk(0));  // Read the root node from the file
    }

    void Insert(const char* dniToInsert, int pageID) {
        btree->Insert(dniToInsert, pageID);
        WriteNodeToDisk(btree->GetRoot(), 0); // Root node
    }

    int Search(const char* dni) {
        return btree->GetPageIDByDNI(dni);
    }

    void Remove(const char* dni) {
        btree->Remove(dni);
        WriteNodeToDisk(btree->GetRoot(), 0); // Root node
    }

    void Traverse() {
        btree->TraverseTree();
    }

    void PrintBTree() {
        btree->PrintBTree();
    }
};