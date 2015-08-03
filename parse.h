// Daniel Cheng - dyc8av
// February 5, 2015
// This file declares the header files to be implemented and used

#ifndef PARSE_H
#define PARSE_H
#include <string>
#include <vector>
using namespace std;

class Parse {
	
public: 
	Parse(); // default constructor
	bool legalChar(string s); // check if input has legal characters
	bool checkPipe(string s); // check if input begins/ends with a pipe
	bool checkIn(string s); // check if input begins/ends with <
	bool checkOut(string s); // check if input begins/ends with >
	// will create the token groups seperated by operators
	vector< vector<string> > tokenGroup(vector<string> sTokens, int n); 
	// convert a vector<string> into appropiate format for execv()
	const char** changev(vector<string> runvec); 
	void pipeThese(char** p1, char** p2); // pipe output if p1 into input of p2
	void redirectOUT(char** p, const char* file); // redirect OUT from p into file
	void redirectIN(char** p, const char* file); // redirect IN to p from file
};
#endif