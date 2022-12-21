#pragma once
#include "Utils.h"

void mainMenu() {
	
	Mode mode = BIDDER;
	User usr = User("Unknown"," ");

	/// <summary>
	/// Command palletes for main menu, audit and bidder modes.
	/// Initialize them once at the beginning of the program
	/// </summary>
	std::vector<std::regex> mainMenuCommandsRegexs, auditMenuCommandsRegexs, bidderMenuCommandsRegexs;
	initializeCommandsRegex(mainMenuCommandsRegexs, auditMenuCommandsRegexs, bidderMenuCommandsRegexs);

	MockDatabase m;
	initializeMockDatabase(m);

	while (1) {
		std::string command = "";
		std::cout << "Somethingby's Auctions: ["<< usr.getUsername()<<" @ " << toString(mode) << " MODE] >> ";

		char c = std::cin.get();
		while (c != '\n') {
			command = command + c;
			c = std::cin.get();
		}
		
		if (std::regex_match(command, mainMenuCommandsRegexs[0])) {
			mode = AUCTION_AUDITOR;
			continue;
		}

		if (std::regex_match(command, mainMenuCommandsRegexs[1])) {
			mode = BIDDER;
			continue;
		}

		if (std::regex_match(command, mainMenuCommandsRegexs[2])) {
			std::vector<std::string> splits = splitCommand(command);
			if (successfulLogin(splits[2], splits[4],m)) {
				usr = m.findByUsername(splits[2]);
			}
			else {
				std::cout << "Username or password are incorrect" << std::endl;
			}
			continue;
		}

		if (std::regex_match(command, mainMenuCommandsRegexs[3])) {
			std::vector<std::string> splits = splitCommand(command);
			if (userExists(splits[2], m)) {
				std::cout << "Username already exists!" << std::endl;
				continue;
			}
			User u = User(splits[2], splits[4]);
			m.insertUser(u);
			std::cout << "Successfully registered user with username:" << splits[2] << std::endl;
			continue;
		}

		if (std::regex_match(command, mainMenuCommandsRegexs[4])) {
			break;
		}
		
		if (std::regex_match(command, mainMenuCommandsRegexs[5])) {
			helpMenu();
			continue;
		}

		if (command.size() > 0) {
			if (usr.getUsername().compare("Unknown") == 0) {
				std::cout << "Wrong command or Unknown user!" << std::endl;
				continue;
			}
			if (mode == AUCTION_AUDITOR) {
				auditorMenu(command, auditMenuCommandsRegexs,m,usr);
			}
			if (mode == BIDDER) {
				bidderMenu(command, bidderMenuCommandsRegexs,m,usr);
			}
		}
	}
}

void helpMenu() {
	std::cout << "mode --audit\t\t\t\t\t\t=> For switching to the Auction audit mode \n" << std::endl;
	std::cout << "mode --bid\t\t\t\t\t\t=> For swithcing to the bid mode \n" << std::endl;
	std::cout << "login --username <username> --password <password>  \t=> For login to an existing account\n" << std::endl;
	std::cout << "signup --username <username> --password <password> \t=> For creating a new  account\n" << std::endl;
	std::cout << "quit\t\t\t\t\t\t\t=> For quiting \n" << std::endl;
	std::cout << "help\t\t\t\t\t\t\t=> For help \n" << std::endl;
}

void auditorMenu(std::string command, std::vector<std::regex> regexs, MockDatabase& m, User u)
{
	if(std::regex_match(command, regexs[0])) {
		std::vector<std::string> splits = splitCommand(command);
		Bid mock_bid = Bid(-1, u, -1, -1);
		m.insertAuction(Auction(m.increaseAuctionNumber(), stringToTime(splits[2]), stringToTime(splits[4]), u, std::stof(splits[6]), mock_bid));
		std::cout << "Created auction!.From:" << splits[2] << ", to:" << splits[4] << ", staring price:" << splits[6] << "$"<<std::endl;
		return;
	}

	if (std::regex_match(command, regexs[1])) {
		displayAuctions(m);
		return;
	}

	if (std::regex_match(command, regexs[2])) {
		displayAuctionsOfUser(m, u);
		return;
	}

	if (std::regex_match(command, regexs[3])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int id = std::stoull(splits[2]);
		closeAuction(m, u,id);
		return;
	}

	if (std::regex_match(command, regexs[4])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int id = std::stoull(splits[2]);
		deleteAuction(m, u, id);
		return;
	}

	if (std::regex_match(command, regexs[5])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int id = std::stoull(splits[2]);
		if (splits.size() <= 3) {
			std::cout << "Unrecognized command"<<std::endl;
			return;
		}
		std::time_t f=NULL, t=NULL;
		float p=NULL;

		if (splits[3].compare("--from") == 0) {
			f = stringToTime(splits[4]);
		}

		updateAuction(m, u, id, f, t, p);
		return;
	}


	std::cout << "Unknown audit mode command!" << std::endl;
}







void bidderMenu(std::string command, std::vector<std::regex> regexs, MockDatabase& m,User u) {
	
	if (std::regex_match(command, regexs[0])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int auctionId = std::stoull(splits[3]);
		float amount = std::stof(splits[5]);

		Auction a = findAuction(m, u);
		Bid n_bid = Bid(m.bidIncrementNumber + 1, u, amount, a.getId());
		m.insertBid(n_bid);
	}

}

