// Daniel Cheng - dyc8av
// February 5, 2015
// implementations of parse.h are located here

#include <iostream>
#include "parse.h"
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
using namespace std;

// constructor
Parse::Parse() {
}

// will check of legal characters by finding if any invalid are present
// if none are present will return true
bool Parse::legalChar(string s) {
	size_t found = s.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-/_|<> ");
	if(found == string::npos) {
		return true;
	}
	else {
		return false;
	}
}

// will check if the line ends with a pipe
bool Parse::checkPipe(string s) {
	size_t found1 = s.find("|",s.length()-1);
	if(found1 == string::npos) {
		return true;
	}
	else {
		return false;
	}
}

// check if line ends with <
bool Parse::checkIn(string s) {
	size_t found3 = s.find("<",s.length()-1);
	if(found3 == string::npos) {
		return true;
	}
	else {
		return false;
	}
}

// check if line ends with >
bool Parse::checkOut(string s) { 
	size_t found3 = s.find(">",s.length()-1);
	if(found3 == string::npos) {
		return true;
	}
	else {
		return false;
	}
}

//convert vector<string> to const char* for execv()
const char** Parse::changev(vector<string> runvec){
	const char **argv = new const char*[runvec.size()+1];
	for(int i = 0; i < runvec.size(); i++) {
		argv[i] = runvec[i].c_str();
	}
	argv[runvec.size()+1] = NULL;

	return argv;
}

//this method will take in a vector of strings, which are the input seperated
//by spaces. This will put the vector into an vector of token groups.
vector< vector<string> > Parse::tokenGroup(vector<string> sTokens, int n) {
	vector< vector<string> > runvec;
		vector<string> temp;
		//combine into token groups with operators
		for(int i = 0; i < n; i++) {
			//if a pipe, put into own token group
			if(sTokens[i] == "|"){
				runvec.push_back(temp);
				temp.clear();
				temp.push_back("|");
				runvec.push_back(temp);
				temp.clear();
			}
			//if a <, put into own token group
			else if(sTokens[i] == "<"){
				runvec.push_back(temp);
				temp.clear();
				temp.push_back("<");
				runvec.push_back(temp);
				temp.clear();
			}
			//if a >, put into own token group
			else if(sTokens[i] == ">"){
				runvec.push_back(temp);
				temp.clear();
				temp.push_back(">");
				runvec.push_back(temp);
				temp.clear();
			}
			//if last word, put into token group
			else if(i == n-1) {
				temp.push_back(sTokens[i]);
				runvec.push_back(temp);
				temp.clear();
			}
			else {
				temp.push_back(sTokens[i]);
			}
		}
		return runvec;
}

// will pipe output of p1 into input of p2
void Parse::pipeThese(char** p1, char** p2){
    int pds[2];
    pipe(pds);
    pid_t pid1;
	pid_t pid2;
    // child process #1
    if (pid1 = fork() == 0) {
        dup2(pds[1], 1);
		close(pds[0]);
		close(pds[1]);
        execv(p1[0], p1);
    } 
    // child process #2
    else if ((pid2 = fork()) == 0) {
        dup2(pds[0], 0);
		close(pds[1]);
        execv(p2[0], p2);
	} 
	else {
    	close(pds[1]);
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);	
	}
}

// will redirect command p into file
void Parse::redirectOUT(char** p, const char* file) {
	pid_t pid = fork();	
	int fd;
	if(pid == 0) {
		if((fd = open(file, O_CREAT | O_WRONLY)) < 0) {
			cout << "Error in creating file" << endl;
		}
		dup2(fd,1);
		close(fd);
		execv(p[0],p);
	}
}

// will redirect file into command p
void Parse::redirectIN(char** p, const char* file) {
	pid_t pid = fork();	
	if(pid == 0) {
		int fd = open(file,O_RDONLY);
		dup2(fd,0);
		close(fd);
		execv(p[0],p);
	}

}