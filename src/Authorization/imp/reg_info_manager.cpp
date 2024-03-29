#include "../reg_info_manager.h"
#include "../../Repositories/JockeyRepository.h"
#include "../../Repositories/OwnerRepository.h"

Result RegisterJockey(std::string name, int yearOfBirth, std::string address, int user_id) {
	Jockey jockey = { 0 };
	jockey.Name = name;
	jockey.YearOfBirth = yearOfBirth;
	jockey.Address = address;
	jockey.IdentityId = user_id;
	if (0 != AddJockey(jockey)) {
		return Result::DB_ERROR;
	}
	return Result::NO_ERROR;
}

Result RegisterOwner(std::string name, int yearOfBirth, std::string address, int user_id) {
	Owner owner = { 0 };
	owner.Name = name;
	owner.YearOfBirth = yearOfBirth;
	owner.Address = address;
	owner.IdentityId = user_id;
	if (0 != AddOwner(owner)) {
		return Result::DB_ERROR;
	}
	return Result::NO_ERROR;
}