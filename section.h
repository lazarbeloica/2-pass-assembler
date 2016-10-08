#pragma once

#include <string>
#include <list>
#include "realocation_table.h"
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

class Section 
{
private:
	RealocationTable realocations;
	string sectionName;
	string byteString = "";	//niz bajtova koji se dobija prevodjenjem
	int lc = 0;					//location counter
	bool restartFlag = false;

public:
	Section(string name) : sectionName(name) {}

	string getName();
	string getType();

	int getLocationCounter();

	void addByte(string);
	void addRealoc(int offset, string type, int num);

	void clCounter();
	void operator ++();
	Section& operator += (int);
	void inc(int);

	string toString();

	const static string UND_SEC_NAME;
	const static string TXT_SEC_NAME;
	const static string BSS_SEC_NAME;
	const static string DATA_SEC_NAME;
};
