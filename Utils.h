#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string>
#include <ctime>
#include <vector>

#include "Menus.h"
#include "MockDatabase.h"

const enum Mode{
	AUCTION_AUDITOR,
	BIDDER
};

char* currentTime();
std::string toString(Mode m);
void initializeCommandsRegex(std::vector<std::regex>& mainMenuCommands,
	std::vector<std::regex>& auditMenuCommands,
	std::vector<std::regex>& bidderMenuCommands);
void initializeMockDatabase(MockDatabase& database);


//LOGIN\SIGNUP FUNCTIONS
bool successfulLogin(std::string username, std::string password, MockDatabase database);
bool userExists(std::string username,MockDatabase database);

std::vector<std::string> splitCommand(std::string command);
std::time_t stringToTime(std::string time);
std::string timeToString(std::time_t t);

//AUCTION FUNCTIONS
void displayAuctions(const MockDatabase m);
void displayAuctionsOfUser(const MockDatabase m, User u);
void closeAuction(MockDatabase& m, User u, unsigned int id);
void deleteAuction(MockDatabase& m, User u, unsigned int id);
void updateAuction(MockDatabase& m, User u, unsigned int id, std::time_t from, std::time_t to, float s_price);