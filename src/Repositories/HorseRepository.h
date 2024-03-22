#ifndef _HORSE_REPO_QUILBOARS_GUARD
#define _HORSE_REPO_QUILBOARS_GUARD

#include "../Models/Horse.h"
#include <vector>

struct HorseResults {
	int HorseId;
	int TimesWon;
};

std::vector<HorseResults> GetHorsesWon();

int Add(Horse horse);

int Update(Horse horse);

int DeleteHorse(int horseId);

Horse GetHorse(int hoeseId);

#endif