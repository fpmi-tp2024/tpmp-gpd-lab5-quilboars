#include "../UIManager.h"
#include "../../Models/Jockey.h"
#include "../../Models/Owner.h"
#include "../../Repositories/JockeyRepository.h"
#include "../../Repositories/OwnerRepository.h"
#include "../../Repositories/HorseRepository.h"
#include "../../Repositories/RaceRecordRepository.h"
#include "../../Repositories/RaceRepository.h"
#include "../../Repositories/PrizeRepository.h"
#include "../../Models/Authorization/Role.h"
#include "../../Authorization/security_manager.h"
#include "../../Authorization/auth_controller.h"
#include <iostream>
#include <vector>
#include <string>

UserSession user;

bool ShowMenu()
{
	if (user.role == _HorseOwner) {
		return ShowMenuForOwner();
	}
	else if (user.role == _Jockey) {
		return ShowMenuForJockey();
	}
	else {
		return ShowMenuForAdmin();
	}
}

bool ShowMenuForJockey()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - SELECT OPERATIONS\n";
		std::cout << "2 - UPDATE YOUR INFO\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			SelectForJockey();
			break;
		case 2:
			GetInfoAndUpdateJockey(user.Id);
			break;
		default:
			break;
		}
	}

	return true;
}

bool ShowMenuForOwner()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - SELECT OPERATIONS\n";
		std::cout << "2 - INSERT OPERATIONS\n";
		std::cout << "3 - DELETE HORSE\n";
		std::cout << "4 - UPDATE\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			SelectForOwner();
			break;
		case 2:
			InsertForOwner();
			break;
		case 3: 
		{
			int id;

			std::cout << "Enter horse id:\n";
			std::cin >> id;
			DeleteHorse(id);
			break;
		}
		case 4:
			UpdateForOwner();
			break;
		default:
			break;
		}
	}

	return true;
}

bool ShowMenuForAdmin()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - SELECT OPERATIONS\n";
		std::cout << "2 - INSERT OPERATIONS\n";
		std::cout << "3 - DELETE RACE\n";
		std::cout << "4 - UPDATE RACE\n";
		std::cout << "5 - GIVE MONEY TO WINNERS\n";
		std::cout << "6 - CREATE ADMIN\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			SelectForAdmin();
			break;
		case 2:
			InsertForAdmin();
			break;
		case 3:
		{
			int id;

			std::cout << "Enter race id:\n";
			std::cin >> id;
			DeleteRace(id);
			break;
		}
		case 4:
		{
			int id;

			std::cout << "Enter race id:\n";
			std::cin >> id;
			GetInfoAndUpdateRace(id);
			break;
		}
		case 5:
		{
			double money;
			int id;

			std::cout << "Enter race id:\n";
			std::cin >> id;
			std::cout << "Enter amount of money:\n";
			std::cin >> money;
			GivePrize(money, id);
			break;
		}
		case 6:
			GetInfoAndAddAdmin();
			break;
		default:
			break;
		}
	}

	return true;
}

void InsertForAdmin()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - Add race\n";
		std::cout << "2 - Add new owner\n";
		std::cout << "3 - Add new jockey\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			GetInfoAndAddRace();
			break;
		case 2:
			GetInfoAndAddOwner();
			break;
		case 3:
			GetInfoAndAddJockey();
			break;
		default:
			break;
		}
	}
}

void InsertForOwner()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - Add horse\n";
		std::cout << "2 - Add race record\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			GetInfoAndAddHorse();
			break;
		case 2:
			GetInfoAndAddRaceRecord();
			break;
		default:
			break;
		}
	}
}

void UpdateForOwner()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - Update horse\n";
		std::cout << "2 - Update your info\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
		{
			int id;

			std::cout << "Enter horse id:\n";
			std::cin >> id;
			GetInfoAndUpdateHorse(id);
			break;
		}
		case 2: {
			auto owner = GetOwnerByIdentityId(user.Id);
			GetInfoAndUpdateOwner(owner.Id);
			break;
		}
		default:
			break;
		}
	}
}

void SelectForJockey()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "SELECT operations menu:\n";
		std::cout << "1 - The jockey who participates the most times in the races\n";
		std::cout << "2 - All races\n";
		std::cout << "3 - Your info\n";
		std::cout << "4 - Your races\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			PrintInfoAboutBestJockey();
			break;
		case 2:
			PrintInfoAboutAllRaces();
			break;
		case 3: {
			auto jockey = GetJockeyByIdentityId(user.Id);
			auto info1 = GetJockeyInfo(jockey.Id);
			PrintJockeyInfo(info1, true);
			break;
		}
			
		case 4:
		{
			auto jockey = GetJockeyByIdentityId(user.Id);
			auto info2 = GetJockeyRecords(jockey.Id);
			for (auto race : info2)
			{
				PrintRaceInfo(race);
			}
			break;
		}
			
		default:
			break;
		}
	}
}

void SelectForOwner()
{
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "SELECT operations menu:\n";
		std::cout << "1 - The jockey who participates the most times in the races\n";
		std::cout << "2 - All races\n";
		std::cout << "3 - The horse that wins the maximum number of times\n";
		std::cout << "4 - Info about your horses\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			PrintInfoAboutBestJockey();
			break;
		case 2:
			PrintInfoAboutAllRaces();
			break;
		case 3:
		{
			auto owner = GetOwnerByIdentityId(user.Id);
			auto info1 = GetBestHorse(owner.Id);
			PrintHorseInfo(info1);
			break;
		}
		case 4:
		{
			auto owner = GetOwnerByIdentityId(user.Id);
			auto info2 = GetHorsesByOwnerId(owner.Id);
			for (auto h : info2)
			{
				PrintHorseInfo(h);
			}
			break;
		}
		default:
			break;
		}
	}
}

void SelectForAdmin() {
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "SELECT operations menu:\n";
		std::cout << "1 - The jockey who participates the most times in the races\n";
		std::cout << "2 - All races\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		switch (choice)
		{
		case 1:
			PrintInfoAboutBestJockey();
			break;
		case 2:
			PrintInfoAboutAllRaces();
			break;
		default:
			break;
		}
	}
}

void PrintInfoAboutBestJockey()
{
	auto info = GetBestJockey();
	auto jockeyInfo = GetJockeyInfo(info.JockeyId);

	PrintJockeyInfo(jockeyInfo, false);
	std::cout << " ,race amount: " << info.RaceAmount << ";\n";
}

void PrintInfoAboutAllRaces()
{
	std::string begin, end;
	std::cout << "Enter the beginning of the period:\n";
	std::cin >> begin;
	std::cout << "Enter the end of the period:\n";
	std::cin >> end;

	auto info = GetByPeriod(begin, end);
	for(auto race : info)
	{
		PrintRaceInfo(race);
	}
}

void PrintHorseInfo(Horse horse)
{
	std::cout << "Horce id: " << horse.Id << ", nickname: " << horse.Nickname << ", owner id: " << horse.OwnerId
		<< ", age: " << horse.Age << ", experience: " << horse.Experience << ", price: " << horse.Price << ";\n";
}

void PrintJockeyInfo(Jockey jockey, bool nextline)
{
	std::cout << "Jockey id: " << jockey.Id << ", Name: " << jockey.Name << ", year of birth: " << jockey.YearOfBirth << ", address: " << jockey.Address << ", jockey experience: " << jockey.Experience;
	if (nextline) {
		std::cout << ";\n";
	}
}

void PrintRaceInfo(RaceRecord race)
{
	std::cout << "Race id: " << race.RaceId << ", jockey: " << race.jockey->Name << ", jockey id: " << race.JockeyId
		<< ", horse: " << race.horse->Nickname << ", horse id: " << race.HorseId << ", result: " << race.Result << ";\n";
}

void GetInfoAndUpdateJockey(int jockeyId)
{
	Jockey jockey = GetJockeyByIdentityId(jockeyId);

	std::cout << "Enter name:\n";
	std::cin >> jockey.Name;
	std::cout << "Enter year of birth:\n";
	std::cin >> jockey.YearOfBirth;
	std::cout << "Enter address:\n";
	std::cin >> jockey.Address;
	std::cout << "Enter experience:\n";
	std::cin >> jockey.Experience;

	Update(jockey);
}

void GetInfoAndAddJockey()
{
	Jockey jockey;
	UserSession session;
	std::string email, password;

	std::cout << "Enter email:\n";
	std::cin >> email;
	password = PromptPasswordRegistration();

	if (RegisterNewUser(email, password, (Role)2, &session) != Result::NO_ERROR) {
		std::cerr << "Registration error!\n";
		return;
	}

	std::cout << "Enter name:\n";
	std::cin >> jockey.Name;
	std::cout << "Enter year of birth:\n";
	std::cin >> jockey.YearOfBirth;
	std::cout << "Enter address:\n";
	std::cin >> jockey.Address;
	std::cout << "Enter experience:\n";
	std::cin >> jockey.Experience;
	jockey.IdentityId = session.Id;

	AddJockey(jockey);
}

void GetInfoAndUpdateOwner(int ownerId)
{
	Owner owner = GetOwnerInfo(ownerId);

	std::cout << "Enter name:\n";
	std::cin >> owner.Name;
	std::cout << "Enter year of birth:\n";
	std::cin >> owner.YearOfBirth;
	std::cout << "Enter address:\n";
	std::cin >> owner.Address;
	
	Update(owner);
}

void GetInfoAndAddOwner()
{
	Owner owner;
	UserSession session;
	std::string email, password;

	std::cout << "Enter email:\n";
	std::cin >> email;
	password = PromptPasswordRegistration();

	if (RegisterNewUser(email, password, (Role)1, &session) != Result::NO_ERROR) {
		std::cerr << "Registration error!\n";
		return;
	}

	std::cout << "Enter name:\n";
	std::cin >> owner.Name;
	std::cout << "Enter year of birth:\n";
	std::cin >> owner.YearOfBirth;
	std::cout << "Enter address:\n";
	std::cin >> owner.Address;
	owner.IdentityId = session.Id;

	AddOwner(owner);
}

void GetInfoAndUpdateRace(int raceId)
{
	auto race = GetRaceById(raceId);

	std::cout << "Enter date:\n";
	std::cin >> race.Date;

	UpdateRace(race);
}

void GetInfoAndAddRace()
{
	Race race;

	std::cout << "Enter date:\n";
	std::cin >> race.Date;

	AddRace(race);
}

void GetInfoAndAddRaceRecord()
{
	RaceRecord raceRecord;
	
	std::cout << "Enter horse id:\n";
	std::cin >> raceRecord.HorseId;
	std::cout << "Enter jockey id:\n";
	std::cin >> raceRecord.JockeyId;
	std::cout << "Enter race id:\n";
	std::cin >> raceRecord.RaceId;
	std::cout << "Enter result:\n";
	std::cin >> raceRecord.Result;

	AddRaceRecord(raceRecord);
}

void GetInfoAndUpdateHorse(int horseId)
{
	Horse horse = GetHorse(horseId);

	std::cout << "Enter nickname:\n";
	std::cin >> horse.Nickname;
	auto owner = GetOwnerByIdentityId(user.Id);
	horse.OwnerId = owner.Id;
	std::cout << "Enter age:\n";
	std::cin >> horse.Age;
	std::cout << "Enter experience:\n";
	std::cin >> horse.Experience;
	std::cout << "Enter price:\n";
	std::cin >> horse.Price;

	Update(horse);
}

void GetInfoAndAddHorse()
{
	Horse horse;

	std::cout << "Enter nickname:\n";
	std::cin >> horse.Nickname;
	auto owner = GetOwnerByIdentityId(user.Id);
	horse.OwnerId = owner.Id;
	std::cout << "Enter age:\n";
	std::cin >> horse.Age;
	std::cout << "Enter experience:\n";
	std::cin >> horse.Experience;
	std::cout << "Enter price:\n";
	std::cin >> horse.Price;

	Add(horse);
}

void GetInfoAndAddAdmin()
{
	UserSession session;
	std::string email, password;

	std::cout << "Enter email:\n";
	std::cin >> email;
	password = PromptPasswordRegistration();

	if (RegisterNewUser(email, password, (Role)3, &session) != Result::NO_ERROR) {
		std::cerr << "Registration error!\n";
		return;
	}
}