#ifndef _HORSE_QUILBOARS_GUARD
#define _HORSE_QUILBOARS_GUARD

#include <cstddef>

#include "Owner.h"

struct Horse 
{
	int Id;
	std::string Nickname;
	int Age;
	double Experience;
	double Price;

	//fk Owner
	int OwnerId;
	Owner* owner = NULL;
};

#endif