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
		std::regex("open[ ]+--auction[ ]+.+"),
		std::regex("close[ ]+--auction[ ]+.+"),
		std::regex("delete[ ]+--auction[ ]+.+"),
		std::regex("update[ ]+--auction[ ]+.+(( )+--from[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9])?(( )+--to[ ]+[0-9][0-9]/[0-9][0-9]/[0-9][0-9])?(( )+--s_price[ ]+[0-9]+)?")
	};

	bidderMenuCommands = {
		std::regex("create bid[ ]+--auction[ ]+.*[ ]+--amount[ ]+[0-9]+"),
		std::regex("show[ ]+--all[ ]*"),
		std::regex("show[ ]+--self[ ]*"),
		std::regex("show[ ]+--auction[ ]+.*"),
		std::regex("withdraw[ ]+--bid[ ]+.*"),
		std::regex("increase[ ]+--bid[ ]+.*[ ]+--amount[ ]+[0-9]+")
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

void deleteAuction(MockDatabase& m, User u, unsigned int id)
{
	for (auto a = m.accessibleAuctions.begin(); a != m.accessibleAuctions.end();++a) {
		if (a->getId() == id) {
			if (a->getOwner().getUsername().compare(u.getUsername()) == 0) {
				m.accessibleAuctions.erase(a);
				std::cout << "Auction deleted successfully!" << std::endl;
				return;
			}else{
				std::cout << "The given auction does not belong to user " << u.getUsername() << " ans cannot be deleted" << std::endl;
				return;
			}
		}
	}
	std::cout << "Auction does not exist!" << std::endl;
}



void updateAuction(MockDatabase& m, User u, unsigned int id, std::time_t from, std::time_t to, float s_price)
{
	for (int i = 0; i < m.accessibleAuctions.size(); i++) {
		Auction a = m.accessibleAuctions[i];
		if (a.getId() == id) {
			if (a.getOwner().getUsername() == u.getUsername()) {
				if (from != NULL) {
					a.setFrom(from);
				}
				if (to != NULL) {
					a.setTo(to);
				}
				if (s_price != NULL) {
					a.setStartingPrice(s_price);
				}
				std::cout << "Auction updated successfully" << std::endl;
				m.accessibleAuctions[i] = a;
			}
			else {
				std::cout << "The user " << u.getUsername() << " have no permission to update this auction" << std::endl;
			}
		}
	}
}

void displayBids(const MockDatabase m)
{
	std::cout << "ID\t|" << "User\t|" << "Auction\t|" << "Amount\t|" << "Commited At\t|" << "Is winning\t" << std::endl;
	for (Bid b : m.commitedBids) {
		std::cout << b.getBidId() << "\t|" << b.getBidder().getUsername() << "\t|" << b.getAuctionId() << "\t\t|" << b.getAmount() << "\t" << timeToString(b.getCommissionTime()) << "\t|" << b.isWining() << std::endl;
	}
}

void createBid(MockDatabase& m, const User u, float amount, int auction_index)
{
	Auction a = m.accessibleAuctions[auction_index];

	//Handle former highest bidder
	Bid b_old = a.getWinner();
	b_old.setWins(false);
	int b_old_index = findBidIndex(m,b_old.getBidId());
	if (b_old_index != -1) {
		m.commitedBids[b_old_index] = b_old;
		return;
	}

	//Handle new highest bidder
	Bid b_new = Bid(m.bidIncrementNumber + 1, u, amount, a.getId());
	b_new.setWins(true);
	m.bidIncrementNumber = m.bidIncrementNumber + 1;


	a.setWinner(b_new);
	a.setCurrentPrice(amount);

	//update the auction and the new bids
	m.accessibleAuctions[auction_index] = a;
	m.insertBid(b_new);
}

void displayBidsOfUser(const MockDatabase m, User u)
{
	std::cout << "ID\t|" << "Auction\t|" << "Amount\t|" << "Commited At\t|" << "Is winning\t" << std::endl;
	for (Bid b : m.commitedBids) {
		if(b.getBidder().getUsername().compare(u.getUsername()) == 0)
		std::cout << b.getBidId() << "\t|" << b.getAuctionId() << "\t\t|" << b.getAmount() << "\t" << timeToString(b.getCommissionTime()) << "\t|" << b.isWining() << std::endl;
	}
}

void displayBidsOfAuction(const MockDatabase m, unsigned int auctionid)
{
	std::cout << "ID\t|" << "User\t|" << "Amount\t|" << "Commited At\t|" << "Is winning\t" << std::endl;
	for (Bid b : m.commitedBids) {
		if (b.getAuctionId() == auctionid)
			std::cout << b.getBidId() << "\t|" << b.getBidder().getUsername() << "\t|" << b.getAmount() << "\t" << timeToString(b.getCommissionTime()) << "\t|" << b.isWining() << std::endl;
	}
}

int findAuction(MockDatabase& m, unsigned int auctionid)
{
	int a_index = -1;
	for (unsigned int i = 0; i < m.accessibleAuctions.size();i++) {
		if (m.accessibleAuctions[i].getId() == auctionid) {
			a_index = i;
			break;
		}
	}
	if (a_index == -1) {
		std::cout << "Auction not found!" << std::endl;
	}
	return a_index;
}

void openAuction(MockDatabase& m, User u, unsigned int id) {
	for (int i = 0; i < m.accessibleAuctions.size(); i++) {
		Auction a = m.accessibleAuctions[i];
		if (a.getId() == id) {
			if (a.getOwner().getUsername() == u.getUsername()) {
				if (!a.isOpen()) {
					a.reopen();
					std::cout << "Auction opened successfully!" << std::endl;
					m.accessibleAuctions[i] = a;
				}
				else {
					std::cout << "Auction already open!" << std::endl;
				}
			}
			else {
				std::cout << "The user " << u.getUsername() << " have no permission to close this auction" << std::endl;
			}
		}
	}
}

void closeAuction(MockDatabase& m, User u,unsigned int id) {
	for (int i = 0; i < m.accessibleAuctions.size();i++) {
		Auction a = m.accessibleAuctions[i];
		if (a.getId() == id) {
			if (a.getOwner().getUsername() == u.getUsername()) {
				if (a.isOpen()) {
					a.close();
					std::cout << "Auction closed successfully!" << std::endl;
					m.accessibleAuctions[i] = a;
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

void withdrawBid(MockDatabase& m, unsigned int bidId) {
	for (unsigned int i = 0; i < m.commitedBids.size(); i++) {
		if (m.commitedBids[i].getBidId() == bidId) {
			m.commitedBids[i].remove();
			return;
		}
	}
}

void increaseBid(MockDatabase& m, unsigned int bidId, float accAmount) {
	for (unsigned int i = 0; i < m.commitedBids.size(); i++) {
		if (m.commitedBids[i].getBidId() == bidId) {
			m.commitedBids[i].increaseAmount(accAmount);
			return;
		}
	}
}

int findBidIndex(MockDatabase m, unsigned int id) {
	int b_index = -1;
	for (int i = 0; i < m.commitedBids.size(); i++) {
		if (m.commitedBids[i].getBidId() == id) {
			b_index = i;
			break;
		}
	}
	return b_index;
}