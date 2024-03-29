#include "../auth_controller.h"
#include "../security_manager.h"
#include "../reg_info_manager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

bool IsValidEmail(const string& email);
string PromptPasswordRegistration();
string PromptEmail(bool isCollisionAccepted, string fail_msg);
Result EnterAdditionalInfo(UserSession* session);

UserSession Authorization() { //TODO: return result from SignIn/Reg
	UserSession session = { 0, (Role)0 };
	string answer;
	while (true) { // Until user enters Yes or No
		cout << "Do you have an account? (Yes/No): ";
		getline(cin, answer);
		if (answer == "Yes") {
			if (FAILED(SignIn(&session))) {
				cerr << "Sign in failed. Try again." << endl;
			} else {
				cout << "You have successfully signed in." << endl;
				break;
			}
		} else if (answer == "No") {
			if (FAILED(Registration(&session))) {
				cerr << "Registration failed. Try again." << endl;
			} else { 
				EnterAdditionalInfo(&session);
				cout << "You have successfully registered." << endl;
				break; 
			}
		} else {
			cout << "Please, enter Yes or No." << endl;
		}
	}

	return session;
}

Result Registration(OUT UserSession* session) {
	bool done = false;
	int role_input = 0;
	char c = 0;
	for (string line; cout << "Enter your role (Horse owner - 1, Jockey - 2): " && getline(cin, line);) {
		istringstream iss(line);
		if (iss >> role_input && (c = iss.get() && (c == '\0' || c == '\n'))) {
			if (role_input == 1 || role_input == 2) {
				done = true;
				break;
			}
		}
		cout << "Please, enter 1 or 2." << endl;
	}
	if (!done) { cerr << "Premature end of input.\n"; };

	std::string email = PromptEmail(false, "Email you entered is already in use.");

	std::string password = PromptPasswordRegistration();

	return RegisterNewUser(email, password, (Role)role_input, session);
}

Result SignIn(OUT UserSession* session) {
	std::string email = PromptEmail(true, "There is no user with entered email.");

	std::string password;
	while (true) { // Until user enters correct password
		cout << "Enter your password: ";
		getline(cin, password);
		Result result = VerifyCredentials(email, password, session);
		if (Result::NO_ERROR == result) {
			break;
		} else if (Result::WRONG_PASSWORD == result) {
			cout << "Failed to log in, check your password." << endl;
		} else {
			return result;
		}
	}

	return Result::NO_ERROR;
}

string PromptEmail(bool isCollisionAccepted, string fail_msg) {
	string email;
	while (true) { // Until user enters valid email which obeys isCollisionAcceptes arg
		cout << "Enter your email: ";
		getline(cin, email);
		if (IsValidEmail(email)) {
			if (isCollisionAccepted == CheckLoginCollision(email)) {
				break;
			} else {
				cout << fail_msg << endl;
			}
		} else {
			cout << "Please, enter a valid email." << endl;
		}
	}
	return email;
}

string PromptPasswordRegistration() {
	string password;
	while (true) { // Until user enters long enough password
		cout << "Enter your password: ";
		getline(cin, password);
		if (password.size() >= 8) {
			break;
		} else {
			cout << "Please, enter a password at least 8 characters long." << endl;
		}
	}
	return password;
}

bool IsValidEmail(const string& email) {
	auto index_at = email.find('@');
	if (string::npos == index_at) {
		return false;
	}
	auto index_dot = email.find('.', index_at);
	if (string::npos == index_dot) {
		return false;
	}
	return true;
}

Result EnterAdditionalInfo(UserSession* session) {
	if (session == NULL) {
		return Result::NULL_POINTER;
	}
	if (session->Id <= 0) {
		return Result::INVALID_ID;
	}
	if (session->role != _HorseOwner && session->role != _Jockey) {
		return Result::INVALID_ARGUMENT;
	}

	string name;
	cout << "Enter your name: ";
	getline(cin, name);

	int yearOfBirth = 0;
	auto t = time(0);
	auto tm = *localtime(&t);

	bool done = false;
	char c = 0;
	for (string line; cout << "Enter your year of birth: " && getline(cin, line);) {
		istringstream iss(line);
		if (iss >> yearOfBirth && (c = iss.get() && (c == '\0' || c == '\n'))) {
			if (yearOfBirth > 1900 && yearOfBirth <= tm.tm_year + 1900) {
				done = true;
				break;
			}
		}
		cout << "Please, enter a valid year of birth." << endl;
	}
	if (!done) { cerr << "Premature end of input.\n"; };

	string address;
	cout << "Enter your address: ";
	getline(cin, address);

	switch (session->role) {
	case _Jockey: {
		return RegisterJockey(name, yearOfBirth, address, session->Id);
		break;
	}
	case _HorseOwner: {
		return RegisterOwner(name, yearOfBirth, address, session->Id);
		break;
	}
	default:
		return Result::INVALID_ARGUMENT;
	}
}