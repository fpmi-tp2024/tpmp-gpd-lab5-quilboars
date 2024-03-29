#ifndef _UIMANAGER_QUILBOARS_GUARD
#define _UIMANAGER_QUILBOARS_GUARD

#include "../Models/Authorization/User.h"
#include "../Models/RaceRecord.h"
#include "../Models/Jockey.h"
#include "../Models/Horse.h"
#include "../Authorization/user_session.h"

extern UserSession user;

bool ShowMenu();
bool ShowMenuForJockey();
bool ShowMenuForOwner();
bool ShowMenuForAdmin();
void InsertForAdmin();
void InsertForOwner();
void UpdateForOwner();
void SelectForJockey();
void SelectForOwner();
void SelectForAdmin();
void PrintInfoAboutBestJockey();
void PrintInfoAboutAllRaces();
void PrintHorseInfo(Horse horce);
void PrintJockeyInfo(Jockey jockey, bool nextline);
void PrintRaceInfo(RaceRecord race);
void GetInfoAndUpdateJockey(int jockeyId);
void GetInfoAndAddJockey();
void GetInfoAndUpdateOwner(int ownerId);
void GetInfoAndAddOwner();
void GetInfoAndUpdateRace(int raceId);
void GetInfoAndAddRace();
void GetInfoAndAddRaceRecord();
void GetInfoAndUpdateHorse(int horseId);
void GetInfoAndAddHorse();

#endif