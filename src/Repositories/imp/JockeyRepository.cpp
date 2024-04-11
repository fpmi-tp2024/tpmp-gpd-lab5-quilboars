#include "../JockeyRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>
#include<iostream>

static int callback_JockeyResult(void* out_param, int argc, char** argv, char** azColName)
{
	std::vector<JockeyExperince>* out_jockey = (std::vector<JockeyExperince>*)out_param;

	for (int i = 0; i < argc; i += 2)
	{
		JockeyExperince jockeyExperience;

		if (strcmp(azColName[i], "JockeyId") == 0)
		{
			jockeyExperience.JockeyId = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "RaceAmount") == 0)
		{
			jockeyExperience.RaceAmount = strtol(argv[i + 1], nullptr, 10);
		}

		out_jockey->push_back(jockeyExperience);
	}

	return 0;
}

std::vector<JockeyExperince> GetJockeyExperience() 
{
	std::vector<JockeyExperince> results;

	std::string query = "SELECT rr.JockeyId, Count(rr.JockeyId) AS RaceAmount FROM RaceRecord AS rr Group By rr.JockeyId";

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.c_str(), callback_JockeyResult, &results, &zErrMsg);

	return results;
}

JockeyExperince GetBestJockey() 
{
	auto jockeys = GetJockeyExperience();
	JockeyExperince answer = jockeys[0];

	for(auto j : jockeys)
	{
		if (j.RaceAmount > answer.RaceAmount) {
			answer = j;
		}
	}

	return answer;
}

Jockey GetJockeyInfo(int jockeyId) 
{
    Jockey jockey;
    std::string query = "SELECT * FROM Jockey AS j WHERE j.Id = ?";

    sqlite3* db = GetConnection();

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SELECT statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
        return jockey;
    }

    sqlite3_bind_int(stmt, 1, jockeyId);

	rc = sqlite3_step(stmt);
	while (rc == SQLITE_ROW)
	{

		jockey.Id = sqlite3_column_int(stmt, 0);
		jockey.Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		jockey.Experience = sqlite3_column_double(stmt, 2);
		jockey.YearOfBirth = sqlite3_column_int(stmt, 3);
		jockey.Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		jockey.IdentityId = sqlite3_column_int(stmt, 5);

		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

    sqlite3_finalize(stmt);

    return jockey;
}

int Update( Jockey jockey)
{
	std::string query = "UPDATE Jockey SET Name = ?, Experience = ?, YearOfBirth = ?, Address = ? WHERE Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing UPDATE statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, jockey.Name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_double(stmt, 2, jockey.Experience);
	sqlite3_bind_int(stmt, 3, jockey.YearOfBirth);
	sqlite3_bind_text(stmt, 4, jockey.Address.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 5, jockey.Id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing UPDATE statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}

Jockey GetJockeyByIdentityId(int identityId)
{
	Jockey j;
	std::string query = "SELECT * FROM Jockey WHERE Jockey.IdentityId = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
		return j;
	}

	sqlite3_bind_int(stmt, 1, identityId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		j.Id = sqlite3_column_int(stmt, 0);
		j.Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		j.Experience = sqlite3_column_double(stmt, 2);
		j.YearOfBirth = sqlite3_column_int(stmt, 3);
		j.Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		j.IdentityId = sqlite3_column_int(stmt, 5);

		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return j;
}

int AddJockey(Jockey jockey)
{
	sqlite3* db = GetConnection();

	std::string query = "INSERT INTO Jockey (Name, YearOfBirth, Address, IdentityId, Experience) VALUES (?, ?, ?, ?, ?)";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing INSERT statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, jockey.Name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, jockey.YearOfBirth);
	sqlite3_bind_text(stmt, 3, jockey.Address.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, jockey.IdentityId);
	sqlite3_bind_double(stmt, 5, jockey.Experience);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing INSERT statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;

	return rc;
}