#include "../HorseRepository.h"
#include <vector>
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>
#include <iostream>

static int callback(void* out_param, int argc, char** argv, char** azColName)
{
	std::vector<HorseResults>* out_horses_results = (std::vector<HorseResults>*)out_param;

	for (int i = 0; i < argc; i += 2)
	{
		HorseResults horses_res;
		if (strcmp(azColName[i], "HorseId") == 0)
		{
			horses_res.HorseId = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "TimesWon") == 0)
		{
			horses_res.TimesWon = strtol(argv[i + 1], nullptr, 10);
		}
		out_horses_results->push_back(horses_res);
	}

	return 0;
}

std::vector<HorseResults> GetHorsesWon() 
{
	std::vector<HorseResults> results;

	std::string query = "SELECT rr.HorseId, Count(rr.Result) AS TimesWon FROM RaceRecord AS rr WHERE rr.Result = 1 GROUP BY rr.HorseId";

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.c_str(), callback, &results, &zErrMsg);

	return results;
}

int Add(Horse horse)
{
	char* zErrMsg = nullptr;
	int isPresent = 0;

	std::string query = "SELECT Count(Owner.Id) AS Count FROM Owner WHERE Owner.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_bind_int(stmt, 1, horse.OwnerId);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		isPresent = sqlite3_column_int(stmt, 0);
	}
	else
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		isPresent = 0;
	}

	sqlite3_finalize(stmt);

	if (isPresent == 0)
	{
		std::cerr << "Owner not found: " << horse.OwnerId << std::endl;
		return -1;
	}

	query = "INSERT INTO Horse (Nickname, Age, Experience, Price, OwnerId) VALUES (?, ?, ?, ?, ?)";

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_bind_text(stmt, 1, horse.Nickname.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, horse.Age);
	sqlite3_bind_double(stmt, 3, horse.Experience);
	sqlite3_bind_double(stmt, 4, horse.Price);
	sqlite3_bind_int(stmt, 5, horse.OwnerId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);

	return 0;
}

int Update(Horse horse)
{
	char* zErrMsg = nullptr;
	int isPresent = 0;

	std::string query = "SELECT COUNT(Owner.Id) AS Count FROM Owner WHERE Owner.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_bind_int(stmt, 1, horse.OwnerId);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		isPresent = sqlite3_column_int(stmt, 0);
	}
	else
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		isPresent = 0;
	}

	sqlite3_finalize(stmt);

	if (isPresent == 0)
	{
		std::cerr << "Owner not found: " << horse.OwnerId << std::endl;
		return -1;
	}

	query = "UPDATE Horse SET Nickname = ?, Age = ?, Experience = ?, Price = ?, OwnerId = ? WHERE Id = ?";

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_bind_text(stmt, 1, horse.Nickname.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, horse.Age);
	sqlite3_bind_double(stmt, 3, horse.Experience);
	sqlite3_bind_double(stmt, 4, horse.Price);
	sqlite3_bind_int(stmt, 5, horse.OwnerId);
	sqlite3_bind_int(stmt, 6, horse.Id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);

	return 0;
}

int DeleteHorse(int horseId)
{
	sqlite3* db = GetConnection();

	std::string query = "DELETE FROM RaceRecord WHERE RaceRecord.HorseId = ?";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing DELETE statement (RaceRecord): " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_int(stmt, 1, horseId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing DELETE statement (RaceRecord): " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return rc;
	}

	sqlite3_finalize(stmt);

	query = "DELETE FROM Horse WHERE Horse.Id = ?";

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing DELETE statement (Horse): " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_int(stmt, 1, horseId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing DELETE statement (Horse): " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return rc;
	}

	sqlite3_finalize(stmt);

	return SQLITE_OK;
}

Horse GetHorse(int horseId)
{
	Horse horse;
	std::string query = "SELECT * FROM Horse WHERE Horse.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		return horse;
	}

	sqlite3_bind_int(stmt, 1, horseId);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW)
	{
		horse.Id = sqlite3_column_int(stmt, 0);
		horse.Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		horse.Age = sqlite3_column_int(stmt, 2);
		horse.Experience = sqlite3_column_double(stmt, 3);
		horse.Price = sqlite3_column_double(stmt, 4);
		horse.OwnerId = sqlite3_column_int(stmt, 5);
	}
	else
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return horse;
}