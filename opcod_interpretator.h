#pragma once
#include <map>
#include <string>
#include <sstream>
#include <bitset>
#include "error.h"

using namespace std;

#define INTERUPT 1
#define ARITMETICAL 2
#define LOGICAL 3
#define MEMORY 4
#define CALL 5
#define PERIFERY 6
#define SHIFT 7
#define BITWRITE 8


class OpCodeInterpretator
{
private:
	map<string, string> coder;

	inline string trim(string& str);
	inline string toHex(string &num, int bytes);
	inline string binToHec(string num);
	inline int stringToInt(string arg);


	int operandGroupType(string cmd);

	string parseArgsInt(string &arg);
	string parseArgsArit(string &arg, string cmd);
	string parseArgsLogic(string &arg);
	string parseArgsMem(string &arg);
	string parseArgsCall(string &arg);
	string parseArgsPerif(string &arg);
	string parseArgsShift(string &arg);
	string parseArgsBitWr(string &arg);
public:
	OpCodeInterpretator();
	
	string getOpCode(string cmd, int &type);

	string parseArgs(int &type, string &args, string cmd);
};