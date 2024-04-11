#include "../UserRepository.h"
#include "../../sqlite/sqlite3.h"
#include "../../DBManagment/ConnectionKeeper.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include<iostream>

std::vector<User> TryGetUserByLogin(std::string login)
{
    std::vector<User> vectorOfUsers;
    sqlite3* db = GetConnection();

    std::string query = "SELECT * FROM User WHERE User.Email = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        return vectorOfUsers;
    }

    sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW)
    {
        User user;

        user.Id = sqlite3_column_int(stmt, 0);
        user.Email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        user.PasswordSigned = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        user.role = static_cast<Role>(sqlite3_column_int(stmt, 3));

        vectorOfUsers.push_back(user);

        rc = sqlite3_step(stmt);
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error executing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    return vectorOfUsers;
}

int AddUser(User* user)
{
    sqlite3* db = GetConnection();

    std::string query = "INSERT INTO User (Email, PasswordSigned, Role) VALUES (?, ?, ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error preparing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_text(stmt, 1, user->Email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user->PasswordSigned.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, static_cast<int>(user->role));

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