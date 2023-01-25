# Rails Bid System
### Abstract
A simple command line tool for biding in auctions. A user can create an account and login. Than, she can switch to "audit" or "bid" mode. In the "audit" mode, there are commands
for creating, deleting,closing etc. auctions. In the "bid" mode there are commands for bidding in an open auction, withdraw bid, update bid etc.
### Install
This is a simple c++ program. In order to install it , you have to clone this repo and follow the standard c++ compile and run procedure [Link](https://www.tutorialspoint.com/How-to-compile-and-run-the-Cplusplus-program)
\
Using visual studio for this project is recommended [Link](https://learn.microsoft.com/en-us/visualstudio/get-started/tutorial-open-project-from-repo?view=vs-2022)
### Implementation details
The program uses the regex standard library for evaluating teh commands in the command line. This is the only 'non-standard" library that you have to install manually to 
run this program. The program also makes heavy use of the C++ standard templete library(STL). In order to understand the program, a good understanding of STL is recommended.
