#ifndef _AUTH_CONTROLLER_QUILBOARS_GUARD
#define _AUTH_CONTROLLER_QUILBOARS_GUARD
#include "../Types/types.h"
#include "user_session.h"

UserSession Authorization();

Result Registration(OUT UserSession* session);

Result SignIn(OUT UserSession* session);

#endif //_AUTH_CONTROLLER_QUILBOARS_GUARD