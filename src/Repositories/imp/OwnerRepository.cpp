#include "../OwnerRepository.h"
#include "sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string>
#include <string.h>

static int callback(void* out_param, int argc, char** argv, char** azColName)
{
	std::vector<Horse>* out_horses = (std::vector<Horse>*)out_param;

	for (int i = 0; i < argc; i += 10)
	{
		Horse horse;
		if (strcmp(azColName[i], "Id") == 0)
		{
			horse.Id = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "Nickname") == 0)
		{
			horse.Nickname = std::string(argv[i + 1]);
		}
		if (strcmp(azColName[i + 2], "Age") == 0)
		{
			horse.Age = strtol(argv[i + 2], nullptr, 10);
		}
		if (strcmp(azColName[i + 3], "Experience") == 0)
		{
			horse.Experience = strtod(argv[i + 3], nullptr);
		}

		if (strcmp(azColName[i + 4], "Price") == 0)
		{
			horse.Price = strtod(argv[i + 4], nullptr);
		}
		if (strcmp(azColName[i + 5], "OwnerId") == 0)
		{
			horse.OwnerId = strtol(argv[i + 5], nullptr, 10);
			horse.owner = new Owner;
			horse.owner->Id = horse.OwnerId;
		}
		if (strcmp(azColName[i + 6], "Name") == 0)
		{
			horse.owner->Name= std::string(argv[i + 6]);
		}
		if (strcmp(azColName[i + 7], "YearOfBirth") == 0)
		{
			horse.owner->YearOfBirth = strtod(argv[i + 7], nullptr);
		}

		if (strcmp(azColName[i + 8], "Address") == 0)
		{
			horse.owner->Address = std::string(argv[i + 8]);
		}

		if (strcmp(azColName[i + 9], "IdentityId") == 0)
		{
			horse.owner->IdentityId = strtod(argv[i + 9], nullptr);
		}
		out_horses->push_back(horse);
	}

	return 0;
}

static int callback_Owner(void* out_param, int argc, char** argv, char** azColName)
{
	Owner* out_owner = (Owner*)out_param;

	if (argc == 0) 
	{
		out_owner = nullptr;
		return 0;
	}

	for (int i = 0; i < argc; i += 6)
	{
		if (strcmp(azColName[i], "Id") == 0)
		{
			out_owner->Id = strtol(argv[i], nullptr, 10);
		}
		if (strcmp(azColName[i + 1], "Name") == 0)
		{
			out_owner->Name = std::string(argv[i + 1]);
		}
		if (strcmp(azColName[i + 2], "YearOfBirth") == 0)
		{
			out_owner->YearOfBirth = strtol(argv[i + 2], nullptr, 10);
		}
		if (strcmp(azColName[i + 3], "Address") == 0)
		{
			out_owner->Address = std::string(argv[i + 3]);
		}
		if (strcmp(azColName[i + 4], "IdentityId") == 0)
		{
			out_owner->IdentityId = strtol(argv[i + 4], nullptr, 10);
		}
	}

	return 0;
}


std::vector<Horse> GetHorsesByOwnerId(int OwnerId) 
{
	std::vector<Horse> horses;
	std::string query = "SELECT \
						 h.Id, \
						 h.Nickname, \
						 h.Age, \
						 h.Experience, \
						 h.Price, \
						 h.OwnerId, \
						 o.Name, \
						 o.YearOfBirth, \
						 o.Address, \
						 o.IdentityId \
						 FROM Horse AS h \
							JOIN Owner AS o ON h.OwnerId = o.Id \
							WHERE o.Id = ";

	std::string query_appended = query.append(std::to_string(OwnerId));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), callback, &horses, &zErrMsg);

	return horses;
}

Horse GetBestHorse(int OwnerId) {
	auto horces = GetHorsesByOwnerId(OwnerId);
	Horse answer = horces[0];

	for(auto h : horces)
	{
		if (h.Experience > answer.Experience) {
			answer = h;
		}
	}

	return answer;
}

int Update(Owner owner) 
{
	std::string query = "UPDATE Owner SET Name = '";

	std::string query_appended = query
		.append(owner.Name)
		.append("', YearOfBirth = ")
		.append(std::to_string(owner.YearOfBirth))
		.append(", Address = '")
		.append(owner.Address)
		.append("' WHERE Id = ")
		.append(std::to_string(owner.Id));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), nullptr, 0, &zErrMsg);

	return rc;
}

Owner GetOwnerInfo(int ownerId) 
{
	Owner owner;
	std::string query = "SELECT * FROM Owner AS o WHERE o.Id = ";

	std::string query_appended = query.append(std::to_string(ownerId));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), callback_Owner, &owner, &zErrMsg);

	return owner;
}

Owner GetOwnerByIdentityId(int identityId) 
{
	Owner owner;
	std::string query = "SELECT * FROM Owner AS o WHERE o.IdentityId = ";

	std::string query_appended = query.append(std::to_string(identityId));

	sqlite3* db = GetConnection();

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, query_appended.c_str(), callback_Owner, &owner, &zErrMsg);

	return owner;
}

int AddOwner(Owner owner) 
{
	sqlite3* db = GetConnection();

	std::string command = "INSERT INTO Owner (Name, YearOfBirth, Address, IdentityId) VALUES ('";
	std::string appended_command = command
		.append(owner.Name)
		.append("', ")
		.append(std::to_string(owner.YearOfBirth))
		.append(", '")
		.append(owner.Address)
		.append("', ")
		.append(std::to_string(owner.IdentityId))
		.append(")");

	char* zErrMsg = 0;

	int rc = sqlite3_exec(db, appended_command.c_str(), nullptr, 0, &zErrMsg);
	return rc;
}