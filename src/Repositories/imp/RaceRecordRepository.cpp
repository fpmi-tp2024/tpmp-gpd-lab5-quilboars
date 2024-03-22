#include "../RaceRecordRepository.h"
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>

static int callback_RaceRecords(void* out_param, int argc, char** argv, char** azColName)
{
	std::vector<RaceRecord>* out_vector = (std::vector<RaceRecord>*)out_param;

	for (int i = 0; i < argc; i += 21)
	{
		RaceRecord rr;
		if (strcmp(azColName[i], "Id") == 0)
		{
			rr.Id = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "Result") == 0)
		{
			rr.Result = strtol(argv[i + 1], nullptr, 10);
		}
		if (strcmp(azColName[i + 2], "RaceId") == 0)
		{
			rr.RaceId = strtol(argv[i + 2], nullptr, 10);
		}
		if (strcmp(azColName[i + 3], "JockeyId") == 0)
		{
			rr.JockeyId = strtol(argv[i + 3], nullptr, 10);
		}
		if (strcmp(azColName[i + 4], "HorseId") == 0)
		{
			rr.horse = new Horse;
			rr.HorseId = strtol(argv[i + 4], nullptr, 10);
			rr.horse->Id = rr.HorseId;
		}
		if (strcmp(azColName[i + 5], "Date") == 0)
		{
			rr.race = new Race;
			rr.race->Id = rr.RaceId;
			rr.race->Date = std::string(argv[i + 5]);
		}
		if (strcmp(azColName[i + 6], "Name") == 0)
		{
			rr.jockey = new Jockey;
			rr.jockey->Name = std::string(argv[i + 6]);
		}
		if (strcmp(azColName[i + 7], "Experience") == 0)
		{
			rr.jockey->Experience = strtod(argv[i + 7], nullptr);
		}
		if (strcmp(azColName[i + 8], "YearOfBirth") == 0)
		{
			rr.jockey->YearOfBirth = strtol(argv[i + 8], nullptr, 10);
		}
		if (strcmp(azColName[i + 9], "Address") == 0)
		{
			rr.jockey->Address = std::string(argv[i + 9]);
		}
		if (strcmp(azColName[i + 10], "IdentityId") == 0)
		{
			rr.jockey->IdentityId = strtol(argv[i + 10], nullptr, 10);
			rr.jockey->Identity = nullptr;
		}
		if (strcmp(azColName[i + 11], "Id") == 0)
		{
			rr.jockey->Id = strtol(argv[i + 11], nullptr, 10);
			rr.JockeyId = rr.jockey->Id;
		}
		if (strcmp(azColName[i + 12], "Nickname") == 0)
		{
			rr.horse->Nickname = std::string(argv[i + 12]);
		}
		if (strcmp(azColName[i + 13], "Age") == 0)
		{
			rr.horse->Age = strtol(argv[i + 13], nullptr, 10);
		}
		if (strcmp(azColName[i + 14], "Experience") == 0)
		{
			rr.horse->Experience = strtod(argv[i + 14], nullptr);
		}
		if (strcmp(azColName[i + 15], "Price") == 0)
		{
			rr.horse->Price = strtod(argv[i + 15], nullptr);
		}

		if (strcmp(azColName[i + 16], "Id") == 0)
		{
			rr.horse->owner = new Owner;
			rr.horse->owner->Id = strtod(argv[i + 16], nullptr);
			rr.horse->OwnerId = rr.horse->owner->Id;
		}

		if (strcmp(azColName[i + 17], "Name") == 0)
		{
			rr.horse->owner->Name = std::string(argv[i + 17]);
		}

		if (strcmp(azColName[i + 18], "YearOfBirth") == 0)
		{
			rr.horse->owner->YearOfBirth = strtol(argv[i + 18], nullptr, 10);
		}

		if (strcmp(azColName[i + 19], "Address") == 0)
		{
			rr.horse->owner->Address = std::string(argv[i + 19]);
		}

		if (strcmp(azColName[i + 20], "IdentityId") == 0)
		{
			rr.horse->owner->IdentityId = strtol(argv[i + 20], nullptr, 10);
		}

		out_vector->push_back(rr);
	}

	return 0;	
}

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

static int callback_count(void* out_param, int argc, char** argv, char** azColName)
{
	int* out_count = (int*)out_param;

	for (int i = 0; i < argc; i += 1)
	{
		if (strcmp(azColName[i], "Count") == 0)
		{
			*out_count = (int)strtol(argv[i], nullptr, 10);
		}
	}

	return 0;
}

std::vector<RaceRecord> GetJockeyRecords(int JockeyId)
{
	std::vector<RaceRecord> records;

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query_string_appended = AllIncludedQuery().append("WHERE rr.JockeyId = ").append(std::to_string(JockeyId));

	int rc = sqlite3_exec(db, query_string_appended.c_str(), callback_RaceRecords, &records, &zErrMsg);

	return records;
}

std::vector<RaceRecord> GetByHorseId(int horseId)
{
	std::vector<RaceRecord> records;

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query_string_appended = AllIncludedQuery().append("WHERE h.Id = ").append(std::to_string(horseId));

	int rc = sqlite3_exec(db, query_string_appended.c_str(), callback_RaceRecords, &records, &zErrMsg);

	return records;
}

std::vector<RaceRecord> GetByPeriod(std::string from, std::string to)
{
	std::vector<RaceRecord> records;

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query_string_appended = AllIncludedQuery().append("WHERE r.Date > '").append(from).append("' AND r.Date < '").append(to).append("'");

	int rc = sqlite3_exec(db, query_string_appended.c_str(), callback_RaceRecords, &records, &zErrMsg);

	return records;
}

int AddRaceRecord(RaceRecord raceRecord)
{
	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int isPresent = 0;

	std::string query = "SELECT Count(Horse.Id) AS Count FROM Horse WHERE Horse.Id = ";

	int rc1 = sqlite3_exec(db, query.append(std::to_string(raceRecord.HorseId)).c_str(), callback_count, &isPresent, &zErrMsg);

	if (isPresent == 0)
	{
		return -1;
	}

	query = "SELECT Count(Jockey.Id) AS Count FROM Jockey WHERE Jockey.Id = ";

	int rc2 = sqlite3_exec(db, query.append(std::to_string(raceRecord.JockeyId)).c_str(), callback_count, &isPresent, &zErrMsg);

	if (isPresent == 0)
	{
		return -1;
	}

	query = "SELECT Count(Race.Id) AS Count FROM Race WHERE Race.Id = ";

	int rc3 = sqlite3_exec(db, query.append(std::to_string(raceRecord.RaceId)).c_str(), callback_count, &isPresent, &zErrMsg);

	if (isPresent == 0)
	{
		return -1;
	}

	query = "INSERT INTO RaceRecord (Result, RaceId, HorseId, JockeyId) VALUES (";

	std::string query_appended = query
		.append(std::to_string(raceRecord.Result))
		.append(", '")
		.append(std::to_string(raceRecord.RaceId))
		.append("', '")
		.append(std::to_string(raceRecord.HorseId))
		.append("', '")
		.append(std::to_string(raceRecord.JockeyId))
		.append("')");

	int rc4 = sqlite3_exec(db, query_appended.c_str(), nullptr, 0, &zErrMsg);
	return rc4;
}

RaceRecord GetRaceRecordById(int raceRecordId) 
{
	std::vector<RaceRecord> records;

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query_string_appended = AllIncludedQuery().append("WHERE rr.Id = ").append(std::to_string(raceRecordId));

	int rc = sqlite3_exec(db, query_string_appended.c_str(), callback_RaceRecords, &records, &zErrMsg);

	return records[0];
}

std::vector<RaceRecord> GetByRaceId(int raceId) 
{
	std::vector<RaceRecord> records;

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query_string_appended = AllIncludedQuery().append("WHERE rr.RaceId = ").append(std::to_string(raceId));

	int rc = sqlite3_exec(db, query_string_appended.c_str(), callback_RaceRecords, &records, &zErrMsg);

	return records;
}