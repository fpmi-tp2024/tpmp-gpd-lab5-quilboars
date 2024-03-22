#include "../JockeyRepository.h"
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string.h>

static int callback_Jockey(void* out_param, int argc, char** argv, char** azColName)
{
	Jockey* out_jockey = (Jockey*)out_param;
	if (argc == 0) 
	{
		out_jockey = nullptr;
		return 0;
	}

	for (int i = 0; i < argc; i += 6)
	{
		if (strcmp(azColName[i], "Id") == 0)
		{
			out_jockey->Id = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "Name") == 0)
		{
			out_jockey->Name = std::string(argv[i + 1]);
		}
		if (strcmp(azColName[i + 2], "Experience") == 0)
		{
			out_jockey->Experience = strtod(argv[i + 2], nullptr);
		}
		if (strcmp(azColName[i + 3], "YearOfBirth") == 0)
		{
			out_jockey->YearOfBirth = strtol(argv[i + 3], nullptr, 10);
		}
		if (strcmp(azColName[i + 4], "Address") == 0)
		{
			out_jockey->Address = std::string(argv[i + 4]);
		}
		if (strcmp(azColName[i + 5], "IdentityId") == 0)
		{
			out_jockey->IdentityId = strtol(argv[i + 5], nullptr, 10);
		}
	}

	return 0;
}

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
	std::string query = "SELECT * FROM Jockey AS j WHERE j.Id = ";

	std::string query_appended = query.append(std::to_string(jockeyId));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), callback_Jockey, &jockey, &zErrMsg);

	return jockey;
}

int Update(Jockey jockey) 
{
	std::string query = "UPDATE Jockey SET Name = '";

	std::string query_appended = query
		.append(jockey.Name)
		.append("', ")
		.append("Experience = ")
		.append(std::to_string(jockey.Experience))
		.append(", YearOfBirth = ")
		.append(std::to_string(jockey.YearOfBirth))
		.append(", Address = '")
		.append(jockey.Address)
		.append("' WHERE Id = ")
		.append(std::to_string(jockey.Id));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), nullptr, 0, &zErrMsg);

	return rc;
}

Jockey GetJockeyByIdentityId(int identityId) 
{
	Jockey j;
	std::string query = "SELECT * FROM Jockey WHERE Jockey.IdentityId =";

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query.append(std::to_string(identityId)).c_str(), callback_Jockey, &j, &zErrMsg);

	return j;
}

int AddJockey(Jockey jockey) 
{
	sqlite3* db = GetConnection();

	std::string command = "INSERT INTO Jockey (Name, YearOfBirth, Address, IdentityId, Experience) VALUES ('";
	std::string appended_command = command
		.append(jockey.Name)
		.append("', ")
		.append(std::to_string(jockey.YearOfBirth))
		.append(", '")
		.append(jockey.Address)
		.append("', ")
		.append(std::to_string(jockey.IdentityId))
		.append(", ")
		.append(std::to_string(jockey.Experience))
		.append(")");

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, appended_command.c_str(), nullptr, 0, &zErrMsg);
	return rc;
}