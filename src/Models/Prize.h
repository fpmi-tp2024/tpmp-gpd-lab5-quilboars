#ifndef _PRIZE_QUILBOARS_GUARD
#define _PRIZE_QUILBOARS_GUARD

#include "RaceRecord.h"

struct Prize
{
	int Id;
	double Prize;

	//fk RaceRecord
	int RaceRecordId;
};

#endif