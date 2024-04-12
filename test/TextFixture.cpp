#include <stdio.h>
#include <fstream>
#include <gtest/gtest.h>
#include "Repositories/HorseRepository.h"
#include "Repositories/JockeyRepository.h"
#include "Repositories/OwnerRepository.h"
#include "Repositories/PrizeRepository.h"
#include "Repositories/RaceRecordRepository.h"
#include "Repositories/RaceRepository.h"
#include "Repositories/UserRepository.h"
#include "DBManagment/ConnectionKeeper.h"
#include "sqlite3.h"
#include <filesystem>

static const std::string db_path = "./test.db";
static const std::string temp_db_path = "./temp_test.db";

class RepositoriesTests :public ::testing::Test {
protected:
	void SetUp() override {
		ASSERT_TRUE(std::filesystem::exists(db_path)) << "Can't find test db: " << db_path;

		std::filesystem::remove(temp_db_path);
		std::filesystem::copy(db_path, temp_db_path);
		ASSERT_EQ(0, OpenConnection(temp_db_path));
	}

	void TearDown() override {
		CloseConnection();
		std::filesystem::remove(temp_db_path);
	}

	static int callback_exec(void* data, int columns_num, char** text, char** column_name) {
		std::string* output = (std::string*)data;

		for (int i = 0; i < columns_num; i++) {
			output->append(text[i] ? text[i] : "NULL");
			output->append(" ");
		}
		output->append("\n");
		return 0;
	}

	std::string ExecuteSQL(const std::string& query) {
		std::string output;
		int rc = sqlite3_exec(GetConnection(), query.c_str(), callback_exec, &output, NULL);

		if (rc != SQLITE_OK) {
			printf("An error occured: %s.\n", sqlite3_errmsg(GetConnection()));
			return "";
		}
		return output;
	}

};

TEST_F(RepositoriesTests, RaceRecordRepositoryGetTest) { //TODO need to fix this
	auto info1 = GetJockeyRecords(1);
	EXPECT_EQ(info1.size(), 0);

	auto info2 = GetByHorseId(1);
	EXPECT_EQ(info2.size(), 0);

	auto info3 = GetByPeriod("19700101", "20220404");
	EXPECT_EQ(info3.size(), 0);

	auto info4 = GetByRaceId(1);
	EXPECT_EQ(info4.size(), 0);
}

TEST_F(RepositoriesTests, HorseRepositoryAddTest) {
	Horse horse;
	horse.Age = 1;
	horse.Experience = 1;
	horse.Nickname = "A";
	horse.OwnerId = 1;
	horse.Price = 1;
	std::string sql = "SELECT COUNT(*) FROM Horse";

	auto countOfHorsesBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfHorsesBeforeAdd);
	Add(horse);
	auto countOfHorsesAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfHorsesAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, HorseRepositoryDeleteTest) {
	std::string sql = "SELECT COUNT(*) FROM Horse";

	auto countOfHorsesBeforeDelete = ExecuteSQL(sql);
	int num1 = stoi(countOfHorsesBeforeDelete);
	DeleteHorse(1);
	auto countOfHorsesAfterDelete = ExecuteSQL(sql);
	int num2 = stoi(countOfHorsesAfterDelete);

	EXPECT_EQ(num1 - 1, num2);
}

TEST_F(RepositoriesTests, HorseRepositoryUpdateTest) {
	auto horse = GetHorse(2);
	EXPECT_EQ(horse.Id, 2);

	std::string sql = "SELECT * FROM Horse WHERE Id = 2";
	auto info1 = ExecuteSQL(sql);
	horse.Nickname = "a";
	Update(horse);
	auto info2 = ExecuteSQL(sql);

	EXPECT_STRNE(info1.c_str(), info2.c_str());
}

TEST_F(RepositoriesTests, JockeyRepositoryAddTest) {
	Jockey jockey;
	jockey.Address = "a";
	jockey.Experience = 1;
	jockey.Name = "a";
	jockey.YearOfBirth = 1;
	std::string sql = "SELECT COUNT(*) FROM Jockey";

	auto countOfJockeysBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfJockeysBeforeAdd);
	AddJockey(jockey);
	auto countOfJockeysAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfJockeysAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, JockeyRepositoryUpdateTest) {
	auto jockey = GetJockeyInfo(1);
	EXPECT_EQ(jockey.Id, 1);

	std::string sql = "SELECT * FROM Jockey WHERE Id = 1";
	auto info1 = ExecuteSQL(sql);
	jockey.Name = "a";
	Update(jockey);
	auto info2 = ExecuteSQL(sql);

	EXPECT_STRNE(info1.c_str(), info2.c_str());
}

TEST_F(RepositoriesTests, JockeyRepositoryGetBestJockeyTest) {
	auto jockey = GetBestJockey();
	EXPECT_EQ(jockey.JockeyId, 10);
}

TEST_F(RepositoriesTests, OwnerRepositoryAddTest) {
	Owner owner;
	owner.Address = "a";
	owner.Name = "a";
	owner.YearOfBirth = 1;
	std::string sql = "SELECT COUNT(*) FROM Owner";

	auto countOfOwnersBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfOwnersBeforeAdd);
	AddOwner(owner);
	auto countOfOwnersAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfOwnersAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, OwnerRepositoryUpdateTest) {
	auto owner = GetOwnerInfo(1);
	EXPECT_EQ(owner.Id, 1);

	std::string sql = "SELECT * FROM Owner WHERE Id = 1";
	auto info1 = ExecuteSQL(sql);
	owner.Name = "a";
	Update(owner);
	auto info2 = ExecuteSQL(sql);

	EXPECT_STRNE(info1.c_str(), info2.c_str());
}

TEST_F(RepositoriesTests, OwnerRepositoryGetBestHorseTest) {
	auto horse = GetBestHorse(1);
	EXPECT_EQ(horse.Id, 2);
}

TEST_F(RepositoriesTests, OwnerRepositoryGetHorsesByOwnerIdTest) {
	auto horses = GetHorsesByOwnerId(1);
	EXPECT_NE(horses.size(), 0);
}

TEST_F(RepositoriesTests, PrizeRepositoryGetAllTest) {
	auto info = GetAll();
	EXPECT_NE(info.size(), 0);
}

TEST_F(RepositoriesTests, RaceRecordRepositoryAddTest) {
	RaceRecord raceRecord;
	raceRecord.HorseId = 1;
	raceRecord.JockeyId = 1;
	raceRecord.RaceId = 1;
	std::string sql = "SELECT COUNT(*) FROM RaceRecord";

	auto countOfRaceRecordsBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfRaceRecordsBeforeAdd);
	AddRaceRecord(raceRecord);
	auto countOfRaceRecordsAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfRaceRecordsAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, RaceRepositoryAddTest) {
	Race race;
	race.Date = "20220404";
	std::string sql = "SELECT COUNT(*) FROM Race";

	auto countOfRacesBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfRacesBeforeAdd);
	AddRace(race);
	auto countOfRacesAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfRacesAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, RaceRepositoryUpdateTest) {
	Race race;
	race.Date = "04.04.2022";
	race.Id = 1;
	std::string sql = "SELECT * FROM Race WHERE Id = 1";

	auto info1 = ExecuteSQL(sql);
	UpdateRace(race);
	auto info2 = ExecuteSQL(sql);

	EXPECT_STRNE(info1.c_str(), info2.c_str());
}

TEST_F(RepositoriesTests, RaceRepositoryDeleteTest) {

	std::string sql = "SELECT COUNT(*) FROM Race";

	auto countOfRacesBeforeDelete = ExecuteSQL(sql);
	int num1 = stoi(countOfRacesBeforeDelete);
	DeleteRace(1);
	auto countOfRacesAfterDelete = ExecuteSQL(sql);
	int num2 = stoi(countOfRacesAfterDelete);

	EXPECT_EQ(num1 - 1, num2);
}

TEST_F(RepositoriesTests, UserRepositoryAddTest) {
	User user;
	user.Email = "a";
	user.PasswordSigned = "a";
	user.role = _Jockey;
	std::string sql = "SELECT COUNT(*) FROM User";

	auto countOfUsersBeforeAdd = ExecuteSQL(sql);
	int num1 = stoi(countOfUsersBeforeAdd);
	AddUser(&user);
	auto countOfUsersAfterAdd = ExecuteSQL(sql);
	int num2 = stoi(countOfUsersAfterAdd);

	EXPECT_EQ(num1 + 1, num2);
}

TEST_F(RepositoriesTests, UserRepositoryGetTest) {
	auto user = TryGetUserByLogin("Elza");
	EXPECT_NE(user.size(), 0);
}

TEST_F(RepositoriesTests, HorseRepository) {
	auto horse = GetHorse(2);
	auto nickname = ExecuteSQL("SELECT Nickname from Horse WHERE Id=2");
	nickname = nickname.substr(0, nickname.find(" "));
	ASSERT_STREQ(nickname.c_str(), horse.Nickname.c_str());
}
