#ifndef _OWNER_REPO_QUILBOARS_GUARD
#define _OWNER_REPO_QUILBOARS_GUARD

#include <vector>
#include "../Models/Horse.h"
#include "../Models/Owner.h"
#include <iostream>

std::vector<Horse> GetHorsesByOwnerId(int OwnerId);

Horse GetBestHorse(int OwnerId);

int Update(Owner owner);

Owner GetOwnerInfo(int ownerId);

Owner GetOwnerByIdentityId(int identityId);

int AddOwner(Owner owner);

#endif