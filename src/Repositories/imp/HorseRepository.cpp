#include "../HorseRepository.h"
#include <vector>
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>

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

static int callback_horse(void* out_param, int argc, char** argv, char** azColName) {
	Horse* horse  = (Horse*)out_param;

	for (int i = 0; i < argc; i += 6) {
		if (strcmp(azColName[i], "Id") == 0) {
			horse->Id = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "Nickname") == 0) {
			horse->Nickname = argv[i + 1];
		}
		if (strcmp(azColName[i + 2], "Age") == 0) {
			horse->Age = strtol(argv[i + 2], nullptr, 10);
		}
		if (strcmp(azColName[i + 3], "Experience") == 0) {
			horse->Experience = strtof(argv[i + 3], nullptr);
		}
		if (strcmp(azColName[i + 4], "Price") == 0) {
			horse->Price = strtof(argv[i + 4], nullptr);
		}
		if (strcmp(azColName[i + 5], "OwnerId") == 0) {
			horse->OwnerId = strtol(argv[i + 5], nullptr, 10);
		}
	}

	return 0;
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
	char* zErrMsg = 0;

	int isPresent = 0;

	std::string query = "SELECT Count(Owner.Id) AS Count FROM Owner WHERE Owner.Id = ";

	sqlite3* db = GetConnection();

	int rc1 = sqlite3_exec(db, query.append(std::to_string(horse.OwnerId)).c_str(), callback_count, &isPresent, &zErrMsg);

	if (isPresent == 0) 
	{
		return -1;
	}

	std::string command = "INSERT INTO Horse (Nickname, Age, Experience, Price, OwnerId) VALUES ('";

	std::string appended_query = command
		.append(horse.Nickname)
		.append("', ")
		.append(std::to_string(horse.Age))
		.append(", ")
		.append(std::to_string(horse.Experience))
		.append(", ")
		.append(std::to_string(horse.Price))
		.append(", ")
		.append(std::to_string(horse.OwnerId))
		.append(")");

	int rc = sqlite3_exec(db, appended_query.c_str(), nullptr, 0, &zErrMsg);
	return rc;
}

int Update(Horse horse)
{
	char* zErrMsg = 0;

	int isPresent = 0;

	std::string query = "SELECT Count(Owner.Id) AS Count FROM Owner WHERE Owner.Id = ";

	sqlite3* db = GetConnection();

	int rc1 = sqlite3_exec(db, query.append(std::to_string(horse.OwnerId)).c_str(), callback_count, &isPresent, &zErrMsg);

	if (isPresent != 0) //TODO 0 is correct, check it
	{
		return -1;
	}

	std::string query2 = "UPDATE Horse SET Nickname = '";

	std::string appended_query = query2
		.append(horse.Nickname)
		.append("', Age = ")
		.append(std::to_string(horse.Age))
		.append(", Price =")
		.append(std::to_string(horse.Price))
		.append(", OwnerId = ")
		.append(std::to_string(horse.OwnerId))
		.append(" WHERE Id = ")
		.append(std::to_string(horse.Id));

	int rc = sqlite3_exec(db, appended_query.c_str(), nullptr, 0, &zErrMsg);
	return rc;
}

int DeleteHorse(int horseId) 
{
	sqlite3* db = GetConnection();

	std::string query = "DELETE FROM RaceRecord WHERE RaceRecord.HorseId = ";

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.append(std::to_string(horseId)).c_str(), nullptr, 0, &zErrMsg);

	if (rc != 0) 
	{
		return rc;
	}

	std::string query2 = "DELETE FROM Horse WHERE Horse.Id = ";

	int rc2 = sqlite3_exec(db, query2.append(std::to_string(horseId)).c_str(), nullptr, 0, &zErrMsg);

	return rc2;
}

Horse GetHorse(int hoeseId)
{
	Horse horse;
	std::string query = "SELECT * FROM Horse WHERE Horse.Id = ";

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.append(std::to_string(hoeseId)).c_str(), callback_horse, &horse, &zErrMsg);

	return horse;
}