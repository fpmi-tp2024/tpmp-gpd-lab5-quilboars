#ifndef _CONNECTION_KEEPER_QUILBOARS_GUARD
#define _CONNECTION_KEEPER_QUILBOARS_GUARD

#include "sqlite3.h"
#include <string>

int OpenConnection(const std::string& path);
sqlite3* GetConnection();
int CloseConnection();

#endif