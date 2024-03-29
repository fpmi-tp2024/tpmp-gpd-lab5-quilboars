#include <stdio.h>
#include <iostream>
#include "DBManagment/ConnectionKeeper.h"
#include "Repositories/UserRepository.h"
#include "Models/Authorization/User.h"
#include "Repositories/RaceRecordRepository.h"
#include "Models/RaceRecord.h"
#include "Repositories/OwnerRepository.h"
#include "Repositories/JockeyRepository.h"
#include "Repositories/HorseRepository.h"
#include "Repositories/RaceRepository.h"
#include "Repositories/PrizeRepository.h"
#include "UserInteraction/UIManager.h"
#include "Authorization/auth_controller.h"

int main() {
	OpenConnection("../../hippodrome.db");
	int choice = -1;

	while (choice != 0)
	{
		std::cout << "Menu:\n";
		std::cout << "1 - START\n";
		std::cout << "0 - EXIT\n";

		std::cin >> choice;
		std::cin.ignore();
		switch (choice)
		{
		case 1:
		{
			auto authorizedUser = Authorization();
			user = authorizedUser;
			ShowMenu();

			break;
		}
		default:
			break;
		}
	}
	CloseConnection();

	return 0;
}