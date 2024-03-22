#ifndef _RACE_RECORD_REPOSITORY_QUILBOARS_GUARD
#define _RACE_RECORD_REPOSITORY_QUILBOARS_GUARD

#include "../Models/RaceRecord.h"
#include <vector>

std::vector<RaceRecord> GetJockeyRecords(int jockeyId);

std::vector<RaceRecord> GetByHorseId(int horseId);

std::vector<RaceRecord> GetByPeriod(std::string from, std::string to);

RaceRecord GetRaceRecordById(int raceRecordId);

int AddRaceRecord(RaceRecord raceRecord);

std::vector<RaceRecord> GetByRaceId(int raceId);

#endif