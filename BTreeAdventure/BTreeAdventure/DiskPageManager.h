#pragma once

/*
* manage data pages in a disk file
*/

// main librarires
#include <cstdlib> // for general utilites
#include <fstream> // for file handling
#include <iostream> // for input/output
#include <string> // for handling sequence of characters

// define a macro
#ifndef BTREE_DISKPAGEMANAGER_H
#define	BTREE_DISKPAGEMANAGER_H

namespace APP_CORE
{
	namespace DISK
	{
		class DiskPageManager : protected std::fstream
		{
		public:
			// main and public functions
			DiskPageManager(std::string fileName, bool truncate);
			~DiskPageManager();
			inline bool IsEmpty();

			template<class Register>
			void save(const long& n, Register& reg);

		private:
			// private attributes
			std::string fileName; // store file name
			int pageSize; // store size of data page
			bool isEmpty; // flag if file it's empty or not
			long pageIDCount; // store total number of pages in the file

		};

		/// <summary>
		/// opening specified file in binary mode 
		/// can truncate file if it's specified
		/// if file does not exist or it's truncate then create an empty file
		/// </summary>
		DiskPageManager::DiskPageManager(std::string fileName, bool truncate = false) : std::fstream(fileName.data(), std::ios::in | std::ios::out | std::ios::binary)
		{
			// open mode:
			// std::ios::in file open for reading
			// std::ios::out file open for writing
			// std::ios::binary file operations are made in binary mode

			isEmpty = false;
			this->fileName = fileName;
			// if file does not exist or it's truncate then create an empty file
			if (!good() || truncate)
			{
				isEmpty = true;
				// add open mode:
				// std::ios::trunc any contents that existed in the file before it's open are discarded
				open(fileName.data(), std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
			}
		}

		/// <summary>
		/// Close file
		/// </summary>
		DiskPageManager::~DiskPageManager()
		{
			close();
		}

		/// <summary>
		/// return true if file it's empty, else return false
		/// </summary>
		/// <returns></returns>
		inline bool DiskPageManager::IsEmpty()
		{
			return isEmpty;
		}
	}
}

#endif // !BTREE_DISKPAGEMANAGER_H