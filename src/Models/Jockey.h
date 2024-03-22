#ifndef _JOCKEY_QUILBOARS_GUARD
#define _JOCKEY_QUILBOARS_GUARD

#include <cstddef>

#include "./Authorization/User.h"

struct Jockey 
{
	int Id;
	std::string Name;
	double Experience;
	int YearOfBirth;
	std::string Address;

	//fk User
	int IdentityId;
	User* Identity = NULL;
};

#endif