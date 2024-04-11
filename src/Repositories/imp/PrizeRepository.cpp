#include "../PrizeRepository.h"
#include "../RaceRecordRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include "../../Models/RaceRecord.h"
#include <algorithm>
#include <vector>
#include <string.h>
#include<iostream>

bool ResultComparer(RaceRecord a, RaceRecord b) 
{
	return a.Result < b.Result;
}

int GivePrize(double money, int raceId)
{
    std::vector<RaceRecord> records = GetByRaceId(raceId);

    if (records.size() < 3)
    {
        return -1;
    }

    std::sort(records.begin(), records.end(), ResultComparer);

    sqlite3* db = GetConnection();

    std::string query = "INSERT INTO Prize (Prize, RaceRecordId) VALUES (?, ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing INSERT statement for Prize: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_double(stmt, 1, money * 0.5);
    sqlite3_bind_int(stmt, 2, records[0].Id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing INSERT statement for Prize (1st place): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_reset(stmt);

    sqlite3_bind_double(stmt, 1, money * 0.3);
    sqlite3_bind_int(stmt, 2, records[1].Id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing INSERT statement for Prize (2nd place): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }
    sqlite3_reset(stmt);

    sqlite3_bind_double(stmt, 1, money * 0.2);
    sqlite3_bind_int(stmt, 2, records[2].Id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing INSERT statement for Prize (3rd place): " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rc;
    }

    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

static int callback(void* out_param, int argc, char** argv, char** azColName)
{
	std::vector<Prize>* out_vector = (std::vector<Prize>*)out_param;

	for (int i = 0; i < argc; i += 3)
	{
		Prize prize;
		if (strcmp(azColName[i], "Id") == 0)
		{
			prize.Id = std::strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "RaceRecordId") == 0)
		{
			prize.RaceRecordId = std::strtol(argv[i + 1], nullptr, 10);
		}
		if (strcmp(azColName[i + 2], "Prize") == 0)
		{
			prize.Prize = std::strtod(argv[i + 2], nullptr);
		}
		out_vector->push_back(prize);
	}

	return 0;
}

std::vector<Prize> GetAll()
{
	std::vector<Prize> prizes;
	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string str1 = "SELECT * FROM Prize";

	int rc = sqlite3_exec(db, str1.c_str(), callback, &prizes, &zErrMsg);

	return prizes;
}