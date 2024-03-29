#ifndef _SECURITY_MANAGER_QUILBOARS_GUARD
#define _SECURITY_MANAGER_QUILBOARS_GUARD
#include <string>
#include "../Models/Authorization/Role.h"
#include "../Types/types.h"
#include "user_session.h"

bool CheckLoginCollision(const std::string& login);

Result VerifyCredentials(const std::string& login, const std::string& password, OUT UserSession* session);

Result RegisterNewUser(const std::string& login, const std::string& password, Role role, OUT UserSession* session);

#endif //_SECURITY_MANAGER_QUILBOARS_GUARD