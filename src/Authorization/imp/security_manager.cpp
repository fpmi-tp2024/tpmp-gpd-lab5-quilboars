#include "../security_manager.h"
#include "../../Repositories/UserRepository.h"
#include "../sha256.h"
#include <iostream>

std::string GetHash(std::string str);

bool CheckLoginCollision(const std::string& login) {
	auto users = TryGetUserByLogin(login);
	if (users.size() > 0) {
		return true;
	}
	return false;
}


Result VerifyCredentials(const std::string& login, const std::string& password, OUT UserSession* session) {
	auto users = TryGetUserByLogin(login);
	if (users.size() != 1) {
		return Result::LOGIN_COLLISION;
	}

	auto hash = GetHash(password);
	if (hash != users[0].PasswordSigned) {
		return Result::WRONG_PASSWORD;
	}

	session->Id = users[0].Id;
	session->role = users[0].role;
	return Result::NO_ERROR;
}

Result RegisterNewUser(const std::string& login, const std::string& password, Role role, OUT UserSession* session) {
	if (CheckLoginCollision(login)) {
		return Result::LOGIN_COLLISION;
	}
	auto hash = GetHash(password);

	User user = {0, login, hash, role};
	if (0 != AddUser(&user)) {
		return Result::DB_ERROR;
	}
	// Need to get new user's id
	auto users = TryGetUserByLogin(login);
	if (users.size() != 1) {
		return Result::LOGIN_COLLISION;
	}

	session->Id = users[0].Id;
	session->role = users[0].role;
	return Result::NO_ERROR;
}

std::string GetHash(std::string str) {
	return sha256(str);
}