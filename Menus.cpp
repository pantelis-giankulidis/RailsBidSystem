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
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
	std::cout << "\t\t\t\tGENERAL COMMANDS" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "mode --audit\t\t\t\t\t\t=> For switching to the Auction audit mode \n" << std::endl;
	std::cout << "mode --bid\t\t\t\t\t\t=> For swithcing to the bid mode \n" << std::endl;
	std::cout << "login --username <username> --password <password>  \t=> For login to an existing account\n" << std::endl;
	std::cout << "signup --username <username> --password <password> \t=> For creating a new  account\n" << std::endl;
	std::cout << "quit\t\t\t\t\t\t\t=> For quiting \n" << std::endl;
	std::cout << "help\t\t\t\t\t\t\t=> For help \n" << std::endl;
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%$$$$$$$$" << std::endl;
	std::cout << "\t\t\t\tAUDIT MODE COMMANDS" << std::endl;
	std::cout << "------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "create --from DD/MM/YY --to DD/MM/YY --s_price <amount>\t=> For creating new auction\n" << std::endl;
	std::cout << "show --all\t\t\t\t\t\t=>For displaying all auctions\n" << std::endl;
	std::cout << "show --self\t\t\t\t\t\t=>For displaying all auctions belonging to logged user\n" << std::endl;
	std::cout << "open --auction <auction id>\t\t\t\t=>For opening a closed auction \n" << std::endl;
	std::cout << "close --auction <auction id>\t\t\t\t=>For closing an open auction \n" << std::endl;
	std::cout << "delete --auction <auction id>\t\t\t\t=>For deleting an auction \n" << std::endl;
	std::cout << "update --auction <auction id> (optional)--from DD/MM/YY (optional)--to DD/MM/YY (optional)--s_price <amount>\t=>For updating an existing auction\n" << std::endl;
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
	std::cout << "\t\t\t\tBID MODE COMMANDS" << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;
	std::cout << "create bid --auction <auction id> --amount <amount>\t=>For bidding in an auction\n" << std::endl;
	std::cout << "show --all\t\t\t\t\t\t=>For displaying all bids for all auctions\n" << std::endl;
	std::cout << "show --self\t\t\t\t\t\t=?For displaying all bids of the logged in user\n" << std::endl;
	std::cout << "show --auction <auction id>\t\t\t\t=>For displaying all bids of an auction\n" << std::endl;
	std::cout << "withdraw --bid <bid id>\t\t\t\t\t=>For withdrwaing a bid\n" << std::endl;
	std::cout << "increase --bid <bid id> --amount <amount>\t\t=>For increasing an existing bid\n" << std::endl;
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
}



/*
* Available commands for the Audit mode
* 1.create an auction
* 2.update an auction
* 3.open auctiom
* 4.close auction
* 5.show auctions
*/
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
		openAuction(m, u, id);
		return;
	}

	if (std::regex_match(command, regexs[4])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int id = std::stoull(splits[2]);
		closeAuction(m, u,id);
		return;
	}

	if (std::regex_match(command, regexs[5])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int id = std::stoull(splits[2]);
		deleteAuction(m, u, id);
		return;
	}

	if (std::regex_match(command, regexs[6])) {
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






/*
Available commands for Bid mode
1.Make bid to ana auction
2.update existing bid
3.withdraw bid
4.show bids
*/
void bidderMenu(std::string command, std::vector<std::regex> regexs, MockDatabase& m, User u) {

	if (std::regex_match(command, regexs[0])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int auctionId = std::stoull(splits[3]);
		float amount = std::stof(splits[5]);
		int a_index = findAuction(m, auctionId);
		if (a_index == -1) {
			return;
		}
		Auction a = m.accessibleAuctions[a_index];

		if (a.getOwner().getUsername().compare(u.getUsername()) == 0) {
			std::cout << "You can't bid to your own auction. This is an illegal action." << std::endl;
			return;
		}
		if (a.getStartingPrice() > amount) {
			std::cout << "The starting price for bidding in this auction is " << a.getStartingPrice() << std::endl;
			return;
		}
		if (a.getCurrentPrice() > amount) {
			std::cout << "The current highert bid is " << a.getCurrentPrice() << ", your bid is not eligible." << std::endl;
			return;
		}

		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		if (!a.isOpen() || std::difftime(now,a.getFrom())<0 || std::difftime(now,a.getTo())>0) {
			std::cout << "The auction id you have provided, does not belong to an auction available for bidding." << std::endl;
			return;
		}

		createBid(m, u, amount, a_index);
		std::cout << "Bid of " << amount << " for auction " << auctionId << " successfully created" << std::endl;
		return;
	}

	if (std::regex_match(command, regexs[1])) {
		displayBids(m);
		return;
	}

	if (std::regex_match(command, regexs[2])) {
		displayBidsOfUser(m, u);
		return;
	}

	if (std::regex_match(command, regexs[3])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int auctionId = std::stoull(splits[2]);
		displayBidsOfAuction(m, auctionId);
		return;
	}

	if (std::regex_match(command, regexs[4])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int bidid = std::stoull(splits[2]);
		withdrawBid(m, bidid);
		return;
	}

	if (std::regex_match(command, regexs[5])) {
		std::vector<std::string> splits = splitCommand(command);
		unsigned int bidid = std::stoull(splits[2]);
		float amount = std::stof(splits[4]);
		increaseBid(m, bidid, amount);
		return;
	}

	std::cout << "Unknown bidder mode command!" << std::endl;
}

