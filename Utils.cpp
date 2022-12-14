#pragma once
#include "Utils.h"

char* currentTime() {
	using std::chrono::system_clock;
	std::chrono::time_point <std::chrono::system_clock> now = system_clock::now();
	std::time_t current = system_clock::to_time_t(now);
	return std::ctime(&current);
}

std::string toString(Mode m) {
	if (m == AUCTION_AUDITOR) {
		return "Auction Audit";
	}
	if (m == BIDDER) {
		return "Bid";
	}
	return "";
}

void initializeCommandsRegex(std::vector<std::regex>& mainMenuCommands,
	std::vector<std::regex>& auditMenuCommands,
	std::vector<std::regex>& bidderMenuCommands) {

	mainMenuCommands = {
		std::regex("mode[ ]+--audit"),
		std::regex("mode[ ]+--bid"),
		std::regex("login[ ]+--username .*[ ]+--password .*"),
		std::regex("signup[ ]+--username .*[ ]+--password .*"),
		std::regex("quit*"),
		std::regex("help*") };

	auditMenuCommands = {
		std::regex("create[ ]+--from[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9] --to[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9] --s_price[ ]+[0-9]+"),
		std::regex("show[ ]+--all[ ]*"),
		std::regex("show[ ]+--self[ ]*"),
		std::regex("close[ ]+--auction[ ]+.+"),
		std::regex("delete[ ]+--auction[ ]+.+"),
		std::regex("update[ ]+--auction[ ]+.+(( )+--from[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9])?(( )+--to[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9])?(( )+--s_price[ ]+[0-9]+)?")
	};

	bidderMenuCommands = {
		std::regex("bid[ ]*--bid .*")
	};
}

void initializeMockDatabase(MockDatabase& database) {

}


bool successfulLogin(std::string username, std::string password, MockDatabase database) {
	for (User u : database.registeredUsers) {
		if (u.getUsername().compare(username) == 0 && u.getPassword().compare(password) == 0) {
			return true;
		}
	}
	return false;
}

bool userExists(std::string username,MockDatabase database) {
	for (User u : database.registeredUsers) {
		if (u.getUsername().compare(username) == 0) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> splitCommand(std::string command) {
	std::vector<std::string> retVector;
	std::stringstream ss(command);
	std::string word;
	while (ss >> word) {
		retVector.push_back(word);
	}
	return retVector;
}

std::time_t stringToTime(std::string time) {

	int year = std::stoi(time.substr(6, 7)) + 2000;
	int month = std::stoi(time.substr(3, 4));
	int day = std::stoi(time.substr(0, 2));

	struct tm breakdown = { 0 };
	breakdown.tm_year = year - 1900;
	breakdown.tm_mon = month - 1;
	breakdown.tm_mday = day;

	return mktime(&breakdown);
}

std::string timeToString(std::time_t t) {
	std::tm* ptm = std::localtime(&t);
	char buffer[32];
	// Format: Mo, 15.06.2009 20:20:00
	std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
	std::string r(buffer);
	return r;
}


void displayAuctions(const MockDatabase m) {
	std::cout << "ID	|OWNER		|FROM				|TO				|STARTING PRICE	|CURRENT PRICE" << std::endl;
	for (Auction a : m.accessibleAuctions) {
		std::cout << a.getId() << "	|" << a.getOwner().getUsername() << "		|" << timeToString(a.getFrom()) << "	|" << timeToString(a.getTo()) << "	|" << a.getStartingPrice() << "		|" << a.getCurrentPrice() << std::endl;
	}
}

void displayAuctionsOfUser(const MockDatabase m, User u) {
	std::cout << "ID	|OWNER		|FROM				|TO				|STARTING PRICE	|CURRENT PRICE" << std::endl;
	for (Auction a : m.accessibleAuctions) {
		if (a.getOwner().getUsername().compare(u.getUsername()) == 0) {
			std::cout << a.getId() << "	|" << a.getOwner().getUsername() << "		|" << timeToString(a.getFrom()) << "	|" << timeToString(a.getTo()) << "	|" << a.getStartingPrice() << "		|" << a.getCurrentPrice() << std::endl;
		}
	}
}

void closeAuction(MockDatabase& m, User u,unsigned int id) {
	for (Auction a : m.accessibleAuctions) {
		if (a.getId() == id) {
			if (a.getOwner().getUsername() == u.getUsername()) {
				if (a.isOpen()) {
					a.close();
					std::cout << "Auction closed successfully!" << std::endl;
				}
				else {
					std::cout << "Auction already closed!" << std::endl;
				}
			}
			else {
				std::cout << "The user " << u.getUsername() << " have no permission to close this auction" << std::endl;
			}
		}
	}
}