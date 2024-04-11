#include "../RaceRecordRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>
#include<iostream>

std::string AllIncludedQuery()
{
	return "SELECT \
				rr.Id, \
				rr.Result, \
				rr.RaceId, \
				rr.JockeyId, \
				rr.HorseId, \
				r.Date, \
				j.Name, \
				j.Experience, \
				j.YearOfBirth, \
				j.Address, \
				j.IdentityId, \
				j.Id, \
				h.Nickname, \
				h.Age, \
				h.Experience, \
				h.Price, \
				o.Id, \
				o.Name, \
				o.YearOfBirth, \
				o.Address, \
				o.IdentityId\
			FROM \
				RaceRecord AS rr \
					JOIN Race AS r ON rr.RaceId = r.Id \
					JOIN Horse AS h ON rr.HorseId = h.Id \
					JOIN Jockey AS j ON rr.JockeyId = j.Id \
					JOIN Owner AS o ON h.OwnerId = o.Id ";

}

std::vector<RaceRecord> GetJockeyRecords(int jockeyId)
{
	std::vector<RaceRecord> records;
	std::string query = AllIncludedQuery() + " WHERE rr.JockeyId = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for RaceRecords: " << sqlite3_errmsg(db) << std::endl;
		return records;
	}

	sqlite3_bind_int(stmt, 1, jockeyId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		RaceRecord rr;
		rr.Id = sqlite3_column_int(stmt, 0);
		rr.Result = sqlite3_column_int(stmt, 1);
		rr.RaceId = sqlite3_column_int(stmt, 2);
		rr.JockeyId = sqlite3_column_int(stmt, 3);
		rr.horse = new Horse;
		rr.HorseId = sqlite3_column_int(stmt, 4);
		rr.horse->Id = rr.HorseId;
		rr.race = new Race;
		rr.race->Id = rr.RaceId;
		rr.race->Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		rr.jockey = new Jockey;
		rr.jockey->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		rr.jockey->Experience = sqlite3_column_double(stmt, 7);
		rr.jockey->YearOfBirth = sqlite3_column_int(stmt, 8);
		rr.jockey->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		rr.jockey->IdentityId = sqlite3_column_int(stmt, 10);
		rr.jockey->Identity = nullptr;
		rr.jockey->Id = sqlite3_column_int(stmt, 11);
		rr.JockeyId = rr.jockey->Id;
		rr.horse->Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
		rr.horse->Age = sqlite3_column_int(stmt, 13);
		rr.horse->Experience = sqlite3_column_double(stmt, 14);
		rr.horse->Price = sqlite3_column_double(stmt, 15);
		rr.horse->owner = new Owner;
		rr.horse->owner->Id = sqlite3_column_int(stmt, 16);
		rr.horse->OwnerId = rr.horse->owner->Id;
		rr.horse->owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
		rr.horse->owner->YearOfBirth = sqlite3_column_int(stmt, 18);
		rr.horse->owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
		rr.horse->owner->IdentityId = sqlite3_column_int(stmt, 20);

		records.push_back(rr);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return records;
}

std::vector<RaceRecord> GetByHorseId(int horseId)
{
	std::vector<RaceRecord> records;
	std::string query = AllIncludedQuery() + " WHERE h.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for RaceRecords: " << sqlite3_errmsg(db) << std::endl;
		return records;
	}

	sqlite3_bind_int(stmt, 1, horseId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		RaceRecord rr;
		rr.Id = sqlite3_column_int(stmt, 0);
		rr.Result = sqlite3_column_int(stmt, 1);
		rr.RaceId = sqlite3_column_int(stmt, 2);
		rr.JockeyId = sqlite3_column_int(stmt, 3);
		rr.horse = new Horse;
		rr.HorseId = sqlite3_column_int(stmt, 4);
		rr.horse->Id = rr.HorseId;
		rr.race = new Race;
		rr.race->Id = rr.RaceId;
		rr.race->Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		rr.jockey = new Jockey;
		rr.jockey->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		rr.jockey->Experience = sqlite3_column_double(stmt, 7);
		rr.jockey->YearOfBirth = sqlite3_column_int(stmt, 8);
		rr.jockey->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		rr.jockey->IdentityId = sqlite3_column_int(stmt, 10);
		rr.jockey->Identity = nullptr;
		rr.jockey->Id = sqlite3_column_int(stmt, 11);
		rr.JockeyId = rr.jockey->Id;
		rr.horse->Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
		rr.horse->Age = sqlite3_column_int(stmt, 13);
		rr.horse->Experience = sqlite3_column_double(stmt, 14);
		rr.horse->Price = sqlite3_column_double(stmt, 15);
		rr.horse->owner = new Owner;
		rr.horse->owner->Id = sqlite3_column_int(stmt, 16);
		rr.horse->OwnerId = rr.horse->owner->Id;
		rr.horse->owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
		rr.horse->owner->YearOfBirth = sqlite3_column_int(stmt, 18);
		rr.horse->owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
		rr.horse->owner->IdentityId = sqlite3_column_int(stmt, 20);

		records.push_back(rr);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return records;
}

std::vector<RaceRecord> GetByPeriod(std::string from, std::string to)
{
	std::vector<RaceRecord> records;
	std::string query = AllIncludedQuery() + " WHERE r.Date >= ? AND r.Date <= ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for RaceRecords: " << sqlite3_errmsg(db) << std::endl;
		return records;
	}

	sqlite3_bind_text(stmt, 1, from.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, to.c_str(), -1, SQLITE_STATIC);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		RaceRecord rr;
		rr.Id = sqlite3_column_int(stmt, 0);
		rr.Result = sqlite3_column_int(stmt, 1);
		rr.RaceId = sqlite3_column_int(stmt, 2);
		rr.JockeyId = sqlite3_column_int(stmt, 3);
		rr.horse = new Horse;
		rr.HorseId = sqlite3_column_int(stmt, 4);
		rr.horse->Id = rr.HorseId;
		rr.race = new Race;
		rr.race->Id = rr.RaceId;
		rr.race->Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		rr.jockey = new Jockey;
		rr.jockey->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		rr.jockey->Experience = sqlite3_column_double(stmt, 7);
		rr.jockey->YearOfBirth = sqlite3_column_int(stmt, 8);
		rr.jockey->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		rr.jockey->IdentityId = sqlite3_column_int(stmt, 10);
		rr.jockey->Identity = nullptr;
		rr.jockey->Id = sqlite3_column_int(stmt, 11);
		rr.JockeyId = rr.jockey->Id;
		rr.horse->Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
		rr.horse->Age = sqlite3_column_int(stmt, 13);
		rr.horse->Experience = sqlite3_column_double(stmt, 14);
		rr.horse->Price = sqlite3_column_double(stmt, 15);
		rr.horse->owner = new Owner;
		rr.horse->owner->Id = sqlite3_column_int(stmt, 16);
		rr.horse->OwnerId = rr.horse->owner->Id;
		rr.horse->owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
		rr.horse->owner->YearOfBirth = sqlite3_column_int(stmt, 18);
		rr.horse->owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
		rr.horse->owner->IdentityId = sqlite3_column_int(stmt, 20);

		records.push_back(rr);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return records;
}

int AddRaceRecord(RaceRecord raceRecord)
{
	sqlite3* db = GetConnection();

	int isPresent = 0;
	{
		std::string query = "SELECT Count(Horse.Id) AS Count FROM Horse WHERE Horse.Id = ?";
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::cerr << "Error preparing SELECT statement for Horse: " << sqlite3_errmsg(db) << std::endl;
			return -1;
		}

		sqlite3_bind_int(stmt, 1, raceRecord.HorseId);

		rc = sqlite3_step(stmt);

		while (rc == SQLITE_ROW)
		{
			isPresent = sqlite3_column_int(stmt, 0);

			rc = sqlite3_step(stmt);
		}

		if (rc != SQLITE_DONE)
		{
			std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		}


		sqlite3_finalize(stmt);
	}

	if (isPresent == 0)
	{
		return -1;
	}

	{
		std::string query = "SELECT Count(Jockey.Id) AS Count FROM Jockey WHERE Jockey.Id = ?";
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::cerr << "Error preparing SELECT statement for Jockey: " << sqlite3_errmsg(db) << std::endl;
			return -1;
		}

		sqlite3_bind_int(stmt, 1, raceRecord.JockeyId);
		rc = sqlite3_step(stmt);

		while (rc == SQLITE_ROW)
		{
			isPresent = sqlite3_column_int(stmt, 0);

			rc = sqlite3_step(stmt);
		}

		if (rc != SQLITE_DONE)
		{
			std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		}

		sqlite3_finalize(stmt);
	}

	if (isPresent == 0)
	{
		return -1;
	}

	{
		std::string query = "SELECT Count(Race.Id) AS Count FROM Race WHERE Race.Id = ?";
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::cerr << "Error preparing SELECT statement for Race: " << sqlite3_errmsg(db) << std::endl;
			return -1;
		}

		sqlite3_bind_int(stmt, 1, raceRecord.RaceId);
		rc = sqlite3_step(stmt);

		while (rc == SQLITE_ROW)
		{
			isPresent = sqlite3_column_int(stmt, 0);

			rc = sqlite3_step(stmt);
		}

		if (rc != SQLITE_DONE)
		{
			std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
		}

		sqlite3_finalize(stmt);
	}

	if (isPresent == 0)
	{
		return -1;
	}

	std::string query = "INSERT INTO RaceRecord (Result, RaceId, HorseId, JockeyId) VALUES (?, ?, ?, ?)";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing INSERT statement for RaceRecord: " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_bind_int(stmt, 1, raceRecord.Result);
	sqlite3_bind_int(stmt, 2, raceRecord.RaceId);
	sqlite3_bind_int(stmt, 3, raceRecord.HorseId);
	sqlite3_bind_int(stmt, 4, raceRecord.JockeyId);

	rc = sqlite3_step(stmt);

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing INSERT statement for RaceRecord: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}

RaceRecord GetRaceRecordById(int raceRecordId)
{
	std::vector<RaceRecord> records;
	std::string query = AllIncludedQuery() + " WHERE rr.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for RaceRecords: " << sqlite3_errmsg(db) << std::endl;
		return RaceRecord();
	}

	sqlite3_bind_int(stmt, 1, raceRecordId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		RaceRecord rr;
		rr.Id = sqlite3_column_int(stmt, 0);
		rr.Result = sqlite3_column_int(stmt, 1);
		rr.RaceId = sqlite3_column_int(stmt, 2);
		rr.JockeyId = sqlite3_column_int(stmt, 3);
		rr.horse = new Horse;
		rr.HorseId = sqlite3_column_int(stmt, 4);
		rr.horse->Id = rr.HorseId;
		rr.race = new Race;
		rr.race->Id = rr.RaceId;
		rr.race->Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		rr.jockey = new Jockey;
		rr.jockey->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		rr.jockey->Experience = sqlite3_column_double(stmt, 7);
		rr.jockey->YearOfBirth = sqlite3_column_int(stmt, 8);
		rr.jockey->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		rr.jockey->IdentityId = sqlite3_column_int(stmt, 10);
		rr.jockey->Identity = nullptr;
		rr.jockey->Id = sqlite3_column_int(stmt, 11);
		rr.JockeyId = rr.jockey->Id;
		rr.horse->Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
		rr.horse->Age = sqlite3_column_int(stmt, 13);
		rr.horse->Experience = sqlite3_column_double(stmt, 14);
		rr.horse->Price = sqlite3_column_double(stmt, 15);
		rr.horse->owner = new Owner;
		rr.horse->owner->Id = sqlite3_column_int(stmt, 16);
		rr.horse->OwnerId = rr.horse->owner->Id;
		rr.horse->owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
		rr.horse->owner->YearOfBirth = sqlite3_column_int(stmt, 18);
		rr.horse->owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
		rr.horse->owner->IdentityId = sqlite3_column_int(stmt, 20);

		records.push_back(rr);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	if (records.empty())
	{
		std::cerr << "Error in the Repositories/RaceRecordRepository.cpp in GetRaceRecordById function" << std::endl;
		return RaceRecord();
	}
	else
	{
		return records[0];
	}
}

std::vector<RaceRecord> GetByRaceId(int raceId)
{
	std::vector<RaceRecord> records;
	std::string query = AllIncludedQuery() + " WHERE rr.RaceId = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for RaceRecords: " << sqlite3_errmsg(db) << std::endl;
		return records;
	}

	sqlite3_bind_int(stmt, 1, raceId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		RaceRecord rr;
		rr.Id = sqlite3_column_int(stmt, 0);
		rr.Result = sqlite3_column_int(stmt, 1);
		rr.RaceId = sqlite3_column_int(stmt, 2);
		rr.JockeyId = sqlite3_column_int(stmt, 3);
		rr.horse = new Horse;
		rr.HorseId = sqlite3_column_int(stmt, 4);
		rr.horse->Id = rr.HorseId;
		rr.race = new Race;
		rr.race->Id = rr.RaceId;
		rr.race->Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		rr.jockey = new Jockey;
		rr.jockey->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		rr.jockey->Experience = sqlite3_column_double(stmt, 7);
		rr.jockey->YearOfBirth = sqlite3_column_int(stmt, 8);
		rr.jockey->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		rr.jockey->IdentityId = sqlite3_column_int(stmt, 10);
		rr.jockey->Identity = nullptr;
		rr.jockey->Id = sqlite3_column_int(stmt, 11);
		rr.JockeyId = rr.jockey->Id;
		rr.horse->Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
		rr.horse->Age = sqlite3_column_int(stmt, 13);
		rr.horse->Experience = sqlite3_column_double(stmt, 14);
		rr.horse->Price = sqlite3_column_double(stmt, 15);
		rr.horse->owner = new Owner;
		rr.horse->owner->Id = sqlite3_column_int(stmt, 16);
		rr.horse->OwnerId = rr.horse->owner->Id;
		rr.horse->owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
		rr.horse->owner->YearOfBirth = sqlite3_column_int(stmt, 18);
		rr.horse->owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
		rr.horse->owner->IdentityId = sqlite3_column_int(stmt, 20);

		records.push_back(rr);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return records;
}