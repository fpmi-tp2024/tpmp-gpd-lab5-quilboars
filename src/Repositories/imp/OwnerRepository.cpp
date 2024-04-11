#include "../OwnerRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <string>
#include <string.h>
#include<iostream>

std::vector<Horse> GetHorsesByOwnerId(int ownerId)
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
                         WHERE o.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for Horses: " << sqlite3_errmsg(db) << std::endl;
		return horses;
	}

	sqlite3_bind_int(stmt, 1, ownerId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		Horse horse;
		horse.Id = sqlite3_column_int(stmt, 0);
		horse.Nickname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		horse.Age = sqlite3_column_int(stmt, 2);
		horse.Experience = sqlite3_column_double(stmt, 3);
		horse.Price = sqlite3_column_double(stmt, 4);
		horse.OwnerId = sqlite3_column_int(stmt, 5);
		horse.owner = new Owner;
		horse.owner->Id = horse.OwnerId;
		horse.owner->Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		horse.owner->YearOfBirth = sqlite3_column_int(stmt, 7);
		horse.owner->Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
		horse.owner->IdentityId = sqlite3_column_int(stmt, 9);

		horses.push_back(horse);
		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return horses;
}

Horse GetBestHorse(int OwnerId) {
	auto horses = GetHorsesByOwnerId(OwnerId);
	Horse answer = horses[0];

	for(auto h : horses)
	{
		if (h.Experience > answer.Experience) {
			answer = h;
		}
	}

	return answer;
}

int Update(Owner owner)
{
	std::string query = "UPDATE Owner SET Name = ?, YearOfBirth = ?, Address = ? WHERE Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing UPDATE statement for Owner: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, owner.Name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, owner.YearOfBirth);
	sqlite3_bind_text(stmt, 3, owner.Address.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, owner.Id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing UPDATE statement for Owner: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}

Owner GetOwnerInfo(int ownerId)
{
	Owner owner;
	std::string query = "SELECT * FROM Owner AS o WHERE o.Id = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for Owner: " << sqlite3_errmsg(db) << std::endl;
		return owner;
	}

	sqlite3_bind_int(stmt, 1, ownerId);

	rc = sqlite3_step(stmt);

	while (rc == SQLITE_ROW)
	{
		owner.Id = sqlite3_column_int(stmt, 0);
		owner.Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		owner.YearOfBirth = sqlite3_column_int(stmt, 2);
		owner.Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		owner.IdentityId = sqlite3_column_int(stmt, 4);

		rc = sqlite3_step(stmt);
	}

	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);

	return owner;
}

Owner GetOwnerByIdentityId(int identityId)
{
	Owner owner;
	std::string query = "SELECT * FROM Owner AS o WHERE o.IdentityId = ?";

	sqlite3* db = GetConnection();

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing SELECT statement for Owner: " << sqlite3_errmsg(db) << std::endl;
		return owner;
	}

	sqlite3_bind_int(stmt, 1, identityId);

	rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW)
	{
		owner.Id = sqlite3_column_int(stmt, 0);
		owner.Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		owner.YearOfBirth = sqlite3_column_int(stmt, 2);
		owner.Address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		owner.IdentityId = sqlite3_column_int(stmt, 4);
	}
	else 
	{
		std::cerr << "Error executing SELECT statement for Owner: " << sqlite3_errmsg(db) << std::endl;
	}
	
	sqlite3_finalize(stmt);

	return owner;
}

int AddOwner(Owner owner)
{
	sqlite3* db = GetConnection();

	std::string query = "INSERT INTO Owner (Name, YearOfBirth, Address, IdentityId) VALUES (?, ?, ?, ?)";

	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK)
	{
		std::cerr << "Error preparing INSERT statement for Owner: " << sqlite3_errmsg(db) << std::endl;
		return rc;
	}

	sqlite3_bind_text(stmt, 1, owner.Name.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 2, owner.YearOfBirth);
	sqlite3_bind_text(stmt, 3, owner.Address.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt, 4, owner.IdentityId);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		std::cerr << "Error executing INSERT statement for Owner: " << sqlite3_errmsg(db) << std::endl;
	}

	sqlite3_finalize(stmt);
	if (rc == SQLITE_DONE)
		rc = 0;
	return rc;
}