#pragma once
#include "Utils.h"
#include "MockDatabase.h"

void mainMenu();
void helpMenu();
void auditorMenu(std::string command, std::vector<std::regex> regexs, MockDatabase& m,User u);
void bidderMenu(std::string command, std::vector<std::regex> regexs, MockDatabase& m,User u);

