#include "../PrizeRepository.h"
#include "../RaceRecordRepository.h"
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include "../../Models/RaceRecord.h"
#include <algorithm>
#include <vector>
#include <string.h>

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

	char* zErrMsg = 0;

	std::string str1 = "INSERT INTO Prize (Prize, RaceRecordId) VALUES (";

	std::string str1_appended = str1
		.append(std::to_string(money * 0.5))
		.append(", ")
		.append(std::to_string(records[0].Id))
		.append("); ");

	std::string str2 = "INSERT INTO Prize (Prize, RaceRecordId) VALUES (";

	std::string str2_appended = str2
		.append(std::to_string(money * 0.3))
		.append(", ")
		.append(std::to_string(records[1].Id))
		.append("); ");

	std::string str3 = "INSERT INTO Prize (Prize, RaceRecordId) VALUES (";


	std::string str3_appended = str3
		.append(std::to_string(money * 0.2))
		.append(", ")
		.append(std::to_string(records[2].Id))
		.append("); ");

	std::string command = str1_appended.append(str2_appended).append(str3_appended);

	int rc = sqlite3_exec(db, command.c_str(), nullptr, 0, &zErrMsg);

	return rc;
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