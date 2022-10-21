// Name: Ryan DeSellems
// File Name: dfa.cpp
// Date: 1 October, 2022
// Description: A program that will continuously input a password string from a
//              user and either reject or accept that password given that:
//                  - The password is 3-4 in length
//                  - The password contains at least one letter
//              This will be done using a finite automata defined in a file.
//              The automata is defined in the file "transitionfunction.txt"

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

string  getPassword();

bool  validatePassword(string passToValidate);

void    processPassword(string wToProcess,
						unordered_map<string,string> dfaToTraverse);

string  processSymbol(string currentState,
					  char symbolToProcess,
					  unordered_map<string,string> dfaToTraverse);

string  checkEndState(string symbol,
					  unordered_map<string,string> dfaToTraverse);

void    buildAutomata(unordered_map<string,string> &dfaToBuild);

//=============================================================================
int main()
{
	unordered_map<string, string> passAutomata;

	buildAutomata(passAutomata);            // build to dfa from transition func.
	while(true)
	{
		string w = getPassword();           // w is user string to process
		processPassword(w,passAutomata);    // process w with the dfa
	}
}
//=============================================================================
string getPassword()
{
	string input;                           // get password
	cout << "Enter password string [Ctrl + C to Exit]: ";
	getline(cin, input);
	while(!validatePassword(input))
	{
		cout << "Invalid input. String contains characters outside of alphabet.\n";
		cout << "Enter password string [Ctrl + C to Exit]: ";
		getline(cin, input);
	}
	return input;
}
//=============================================================================
bool validatePassword(string passToValidate)
{
	for(int k=0;k<passToValidate.length();k++)
	{
		if (passToValidate[k] != 'a' &&
			passToValidate[k] != 'b' &&
			passToValidate[k] != '1' &&
			passToValidate[k] != '0')
		{
			return false;
		}
	}
	return true;
}
//=============================================================================
void processPassword(string wToProcess,
					 unordered_map<string,string> dfaToTraverse)
{
	int     pos = 0;                        //location in dfa
	char    symbol = wToProcess[pos];       //first symbol of string
	string  currentState = "qV";            //V is place holder for state #
	string  posString = to_string(pos);     //make pos a string for key in map

	currentState.replace(currentState.find("V"),1,posString);
																		 // replace V in qV with val of pos
	while(pos < wToProcess.length() && currentState != "")               //while symbols to process
	{                                                                    //and symbol is valid

		cout << "[" << currentState << "]" << "-" << symbol << "->";     //layout output
		currentState = processSymbol(currentState,symbol,dfaToTraverse); //get state by processing symbol
		pos++;                                                           //next pos in string
		symbol = wToProcess[pos];                                        //next symbol in string
	}
	cout << "[" << currentState << "]: ";                                //loop broke, so output final state
	string finalResult = checkEndState(currentState,dfaToTraverse);      //look up end state to get accepted or rejected
	cout << finalResult << endl << endl;                                 //display status of end result

}
//=============================================================================
string processSymbol(string currentState,
					 char symbolToProcess,
					 unordered_map<string,string> dfaToTraverse)
{
	string stateSymbolKey = currentState + ":" + symbolToProcess;        //build map key from state + symbol

	auto result = dfaToTraverse.find(stateSymbolKey);                    //get map iterator to new state
	if(result == dfaToTraverse.end())                                    //if its not in map then invalid
	{
		return "";
	}
	string newState = result->second;                                    //if in map return value of pair

	return newState;
}
//=============================================================================
string checkEndState(string currentState,
					 unordered_map<string,string> dfaToTraverse)
{
	auto result = dfaToTraverse.find(currentState);            //iterator points to result of current state
	if(result == dfaToTraverse.end())                          //if state isnt in table, reject
	{
		return "Rejected";
	}
	return result->second;                                     //return value of state : accept or reject
}
//=============================================================================
void buildAutomata(unordered_map<string,string> &dfaToBuild)
{
	string   transitionRule;                                //string that will be used to hold transition function
	ifstream inFile;
	string firstKey;                                        //state and input symbol to process
	string secondKey;                                       //state to map to firstKey
	int    equalPos;                                        //location of equal sign in transition function

	inFile.open("transitionfunction.txt");                  //open file containing transition function
	if(!inFile.is_open())                                   //handle file opening
	{
		cout << "Error loading transition function file.\nProcess terminated.\n";
		exit(0);
	}

	while(getline(inFile, transitionRule))                  //while not eof
	{
		if(transitionRule.find("q") == 0)                   //make sure line starts with transition rule
		{
			equalPos  = transitionRule.find('=');           //get position of equal sign, which divides the rule
			firstKey  = transitionRule.substr(0,equalPos);  //firstKey is on left side of equal sign
			secondKey = transitionRule.substr(equalPos+1);  //secondKey is on right side
			secondKey.pop_back();                           //trim newline from line
			dfaToBuild[firstKey] = secondKey;               //insert key into dfa map
		}
	}
}
//=============================================================================
