#pragma once


class User {
private:
	std::string username;
	std::string password;
public:
	User(std::string uname, std::string pword) :username(uname), password(pword) {
	}

	std::string getUsername() {
		return username;
	}

	std::string getPassword() {
		return password;
	}
};


class Bid {
private:
	unsigned int bidId;
	User bidder;
	float amount;
	unsigned int  auctionId;
	std::time_t commitedAt;
	bool wins,deleted;

public:
	Bid() = default;
	Bid(unsigned int bidId,User bidder, float amount, unsigned int auctionId):bidId(bidId),bidder(bidder),amount(amount),
	auctionId(auctionId),commitedAt(std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now())),wins(false),deleted(false){
	}

	unsigned int getBidId() {
		return bidId;
	}
	User getBidder() {
		return bidder;
	}
	void setBidder(User u) {
		this->bidder = u;
	}
	float getAmount() {
		return amount;
	}
	void setAmount(float amount) {
		this->amount = amount;
	}

	void increaseAmount(float amount) {
		this->amount = this->amount + amount;
		std::cout << "Successfully increased amount. New bid value: " << amount << std::endl;
	}

	unsigned int getAuctionId() {
		return auctionId;
	}

	std::time_t getCommissionTime() {
		return commitedAt;
	}

	void setCommissionTime(std::time_t commitedAt) {
		this->commitedAt = commitedAt;
	}

	bool getWins() {
		return wins;
	}

	std::string isWining() {
		return wins ? "Yes" : "No";
	}

	void setWins(bool wins) {
		this->wins = wins;
	}

	bool getDeleted() {
		return deleted;
	}

	void remove() {
		deleted = true;
		std::cout << "Bid successfully deleted!" << std::endl;
	}
};


class Auction {
private:
	unsigned int id;
	std::time_t from, to;
	User owner;
	float startingPrice, currentPrice;
	std::vector<Bid*> currBids;
	Bid winner;
	bool open;
public:
	Auction() = default;

	Auction(unsigned int id,std::time_t from, std::time_t to, User owner, float startingPrice,Bid winner):id(id),from(from),to(to),
		owner(owner),startingPrice(startingPrice),currentPrice(startingPrice),winner(winner){
		open = false;
	}

	unsigned int getId() {
		return id;
	}

	User getOwner() {
		return owner;
	}

	std::time_t getFrom() {
		return from;
	}

	void setFrom(std::time_t from) {
		this->from = from;
	}

	std::time_t getTo() {
		return to;
	}

	void setTo(std::time_t to) {
		this->to = to;
	}

	float getStartingPrice() {
		return startingPrice;
	}

	void setStartingPrice(float sprice) {
		this->startingPrice = sprice;
	}

	float getCurrentPrice() {
		return currentPrice;
	}

	void setCurrentPrice(float price) {
		this->currentPrice = price;
	}

	void addBid(Bid* b) {
		currBids.push_back(b);
	}

	bool isOpen() {
		return open;
	}

	void close() {
		open = false;
	}

	void reopen() {
		open = true;
	}

	void setWinner(Bid b) {
		this->winner = b;
	}

	Bid getWinner() {
		return winner;
	}
};


struct MockDatabase {
	std::vector<User> registeredUsers;
	std::vector<Auction> accessibleAuctions;
	std::vector<Bid> commitedBids;
	unsigned int auctionIncrementNumber;
	unsigned int bidIncrementNumber;

	MockDatabase() {
		auctionIncrementNumber = 0;
		bidIncrementNumber = 0;
	}

	void insertUser(User u) {
		registeredUsers.push_back(u);
	}

	void insertAuction(Auction a) {
		accessibleAuctions.push_back(a);
	}

	void insertBid(Bid b) {
		commitedBids.push_back(b);
	}

	User findByUsername(std::string username) {
		for (User u : registeredUsers) {
			if (u.getUsername().compare(username) == 0) {
				return u;
			}
		}
		return User("Unknown","");
	}

	unsigned int increaseAuctionNumber() {
		auctionIncrementNumber = auctionIncrementNumber + 1;
		return auctionIncrementNumber;
	}

	unsigned int increaseBidNumber() {
		bidIncrementNumber = bidIncrementNumber + 1;
		return bidIncrementNumber;
	}
};
