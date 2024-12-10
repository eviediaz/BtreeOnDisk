void LoadBTreeToRAM(PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName);

void InsertOperation(DataGenerator& dataGenerator, BTree& t, PageManager& pageManager, std::string& btreeSerializedFileName, bool& btreeUpdated);

void SearchOperation(BTree& t, PageManager& pageManager);

void DeleteOperation(PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName, bool& btreeUpdated, int& retflag);

void PrintRecordsFromXtoY(PageManager& pageManager, std::string& citizenDataFileName);

void SaveChangesInBTree(bool btreeUpdated, PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName);

void GenerateRandomDataAndCreateBTree(std::string& citizenDataFileName, DataGenerator& dataGenerator, long numberOfRecordsToGenerate, PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName);

void BenchMarkGenerarArchivoCincoMillones();

void BenchmarkGenerarArchivoDiezMillones();

void BenchmarkGenerarArchivoQuinceMillones();

void BenchmarkGenerarArchivoVeinteMillones();

void BenchmarkGenerarArchivoTreintaMillones();

void BenchmarkCargaYSerializadoBTreeCincoMillones();

void BenchmarkCargaYSerializadoBTreeDiezMillones();

void BenchmarkCargaYSerializadoBTreeQuinceMillones();

void BenchmarkCargaYSerializadoBTreeVeinteMillones();

void BenchmarkCargaYSerizliadoBTreeTreintaMillones();

void BenchmarkLecturaSerializadoCincoMillones();

void BenchMarkLecturaSerializadoDiezMillones();

void BenchmarkLecturaSerializadoQuinceMillones();

void BenchmarkLecturaSerializadoVeinteMillones();

void BenchmarkLecturaSerializadoTreintaMillones();

void RangeSearchOperation(PageManager& pageManager, BTree& t);

void DeleteOperation(PageManager& pageManager, BTree& t, std::string& btreeSerializedFileName, bool& btreeUpdated);
