#ifndef _AUTH_CONTROLLER_QUILBOARS_GUARD
#define _AUTH_CONTROLLER_QUILBOARS_GUARD
#include "../Types/types.h"
#include "user_session.h"

#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

bool IsValidEmail(const string& email);
string PromptPasswordRegistration();
string PromptEmail(bool isCollisionAccepted, string fail_msg);
Result EnterAdditionalInfo(UserSession* session);

UserSession Authorization();

Result Registration(OUT UserSession* session);

Result SignIn(OUT UserSession* session);


#endif //_AUTH_CONTROLLER_QUILBOARS_GUARD