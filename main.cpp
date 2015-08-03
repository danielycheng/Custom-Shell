// Daniel Cheng - dyc8av
// February 5, 2015
// this is the main file, where it all runs!

#include <iostream>
#include "parse.h"
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
using namespace std;

int main()
{
	cout << "\nRunning Daniel's Shell\n" << "Created by dyc8av\n" << endl;
	//while there is still input from the user
	while(true) {

		Parse p; // create parse in order to call methods from
		string input = ""; // where raw input is stored
		char* cinput = NULL;
		vector<pid_t> spid;
		int status;
		vector<string> sTokens; //where tokens are stored unorganized
		int nTokens = 0;
		getline(cin,input);

// -------------------- parsing input --------------------------

		if(cin.eof()) { // break if end-of-file
			exit(0);
		}
		else if(input == "exit") { // break if "exit"
			exit(0);
	}
		else if(input.length() > 100) { // reload while loop if over 100 characters
			cout << "Your input is over 100 characters." << endl;
			continue;
		}
		else if(!p.legalChar(input)) { // reload while loop of invalid characters
			cout << "Your input contains invalid characters." << endl;
			continue;
		}
		else if (!p.checkPipe(input)) { // check pipe location is valid
			cout << "Your input ends in a pipe!" << endl;
			continue;
		}
		else if (!p.checkIn(input)) { // check if < location is valid
			cout << "Your input ends in an operator!" << endl;
			continue;
		}
		else if (!p.checkOut(input)) { // check if > location is valid
			cout << "Your input ends in an operator!" << endl;
			continue;
		}
		else {

		//parse the input line into appropiate tokens
			cinput = new char[input.size() +1];
			strcpy(cinput,input.c_str());
			char* tokens = strtok(cinput," ");
		while(tokens != NULL) { //read and split into tokens 
			sTokens.push_back(strdup(tokens));
			nTokens++;
			tokens = strtok(NULL," ");
		}

// ---------------------- end parsing -------------------------------

		//create vector of list of inputs to be execv()
		//combine into token groups with operators
		vector< vector<string> > tokenVec = p.tokenGroup(sTokens,nTokens);

		//execv the commands
		for(int i = 0; i < tokenVec.size(); i++) {
			// if the command is a pipe
			if(tokenVec[i][0] == "|") {
				const char **lastVec1 = p.changev(tokenVec[i-1]);
				const char **lastVec2 = p.changev(tokenVec[i+1]);
				p.pipeThese((char **) lastVec1,(char **) lastVec2);
			}
			// if direction in
			else if(tokenVec[i][0] == "<") {
				const char **execVec = p.changev(tokenVec[i-1]);
				const char **fileVec = p.changev(tokenVec[i+1]);
				p.redirectIN((char **) execVec, fileVec[0]);
			}
			// if direction out
			else if(tokenVec[i][0] == ">") {
				const char **execVec = p.changev(tokenVec[i-1]);
				const char **fileVec = p.changev(tokenVec[i+1]);
				p.redirectOUT((char **) execVec, fileVec[0]);

			}
			else {
				if(i < tokenVec.size()-1) {
					// check if next token is a command, then wait to execv() that command
					if(tokenVec[i+1][0] == "|" || tokenVec[i+1][0] == ">" || tokenVec[i+1][0] == "<" ) {
						continue;
					}
					else {
						pid_t pid;
						pid = fork(); // fork the process
						if(pid == 0) {// child
						//convert vector<string> to appropiate format
							const char **lastVec = p.changev(tokenVec[i]);
							execv(lastVec[0], (char **) lastVec);
						}
						else {
							spid.push_back(pid);
						}
					}
				}
				if(tokenVec.size() == 1) { // for first case
					pid_t pid;
						pid = fork(); // fork the process
						if(pid == 0) { // child
							const char **lastVec = p.changev(tokenVec[i]);
							execv(lastVec[0], (char **) lastVec);
						}
						else {
							spid.push_back(pid);

						}
					}
				}
			}

			// used to wait for children to finish and can check status
			for(int i = 0; i < spid.size(); i++) {
				waitpid(spid[i],&status,0);
			}
		}

	}
	return 0;
}