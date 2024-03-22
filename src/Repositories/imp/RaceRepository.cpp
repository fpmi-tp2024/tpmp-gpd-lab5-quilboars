#include "../RaceRepository.h"
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string>
#include <string.h>

int AddRace(Race race)
{
	sqlite3* db = GetConnection();

	std::string query = "INSERT INTO Race (Date) VALUES ('";
	std::string query_appended = query.append(race.Date).append("')");

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), nullptr, 0, &zErrMsg);

	return rc;
}

int UpdateRace(Race race)
{
	sqlite3* db = GetConnection();

	std::string query = "UPDATE Race SET Date = '";
	std::string query_appended = query
		.append(race.Date)
		.append("' WHERE Race.Id = ")
		.append(std::to_string(race.Id));

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), nullptr, 0, &zErrMsg);

	return rc;
}

int DeleteRace(int raceId)
{
	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	std::string query = "DELETE FROM RaceRecord WHERE RaceRecord.RaceId = ";

	std::string query_append = query.append(std::to_string(raceId));

	int rc = sqlite3_exec(db, query_append.c_str(), nullptr, 0, &zErrMsg);

	if (rc != 0) 
	{
		return rc;
	}

	std::string query_command = "DELETE FROM Race WHERE Race.Id = ";

	int rc2 = sqlite3_exec(db, query_command.append(std::to_string(raceId)).c_str(), nullptr, 0, &zErrMsg);

	return rc2;
}