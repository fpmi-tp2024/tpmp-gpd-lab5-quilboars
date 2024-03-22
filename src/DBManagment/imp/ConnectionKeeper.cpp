#include "../ConnectionKeeper.h"
#include "sqlite3.h"
#include <stdio.h>
sqlite3* db;

int OpenConnection(const std::string& path) 
{
	int rc = sqlite3_open(path.c_str(), &db);
	return rc;
}

sqlite3* GetConnection() 
{
	return db;
}

int CloseConnection() 
{
	return sqlite3_close(db);;
}