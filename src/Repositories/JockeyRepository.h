#ifndef _JOCKEY_REPO_QUILBOARS_GUARD
#define _JOCKEY_REPO_QUILBOARS_GUARD

#include "../Models/Jockey.h"
#include <vector>

struct JockeyExperince 
{
	int JockeyId;
	int RaceAmount;
};

std::vector<JockeyExperince> GetJockeyExperience();

JockeyExperince GetBestJockey();

Jockey GetJockeyInfo(int jockeyId);

int Update(Jockey jockey);

Jockey GetJockeyByIdentityId(int identityId);

int AddJockey(Jockey jockey);

#endif