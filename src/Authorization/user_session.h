#ifndef _USER_SESSION_QUILBOARS_GUARD
#define _USER_SESSION_QUILBOARS_GUARD
#include "../Models/Authorization/Role.h"

struct UserSession {
	int Id;
	Role role;
};

#endif 