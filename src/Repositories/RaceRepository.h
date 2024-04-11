#ifndef _RACE_REPO_QUILBOARS_GUARD
#define _RACE_REPO_QUILBOARS_GUARD

#include "../Models/Race.h"

int AddRace(Race race);

int UpdateRace(Race race);

int DeleteRace(int raceId);

Race GetRaceById(int raceId);

#endif