#ifndef _USER_QUILBOARS_GUARD
#define _USER_QUILBOARS_GUARD

#include "Role.h"
#include <string>

struct User 
{
	int Id;
	std::string Email;
	std::string PasswordSigned;
	Role role;
};

#endif