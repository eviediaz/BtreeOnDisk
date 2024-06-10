#include "PageManager.h"

/// @brief You have to give the name of the file anduse sizeof(Peruvian/Struct) and pass it to recordSize paraemeter
/// @param filename 
/// @param recordSize 
/// @return the num of pages of a file
long calculateNumPages(const char *filename, long recordSize) {
    PageManager pageManager(filename);
    long fileSize = pageManager.getFileSize();

    if (fileSize < 0) {
        std::cerr << "Error: File size is negative. Something went wrong." << std::endl;
        return 0;
    }

    // Calculate the number of pages
    long numPages = fileSize / recordSize;

    // If there is a remainder, we need an additional page to store the remaining data
    if (fileSize % recordSize != 0) {
        numPages += 1;
    }

    return numPages;
}