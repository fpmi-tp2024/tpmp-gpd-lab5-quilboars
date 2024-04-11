#include "../RaceRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string>
#include <string.h>
#include<iostream>

int AddRace(Race race)
{
	sqlite3* db = GetConnection();

	std::string query = "INSERT INTO Race (Date) VALUES (?)";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, race.Date.c_str(), -1, SQLITE_TRANSIENT);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}

int UpdateRace(Race race)
{
	sqlite3* db = GetConnection();

	std::string query = "UPDATE Race SET Date = ? WHERE Race.Id = ?";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, race.Date.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, race.Id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}

int DeleteRace(int raceId)
{
	sqlite3* db = GetConnection();

	std::string query = "DELETE FROM RaceRecord WHERE RaceRecord.RaceId = ?";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing DELETE statement for RaceRecord: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_int(stmt, 1, raceId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing DELETE statement for RaceRecord: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	sqlite3_reset(stmt);
	query = "DELETE FROM Race WHERE Race.Id = ?";

	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing DELETE statement for Race: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_int(stmt, 1, raceId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing DELETE statement for Race: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}