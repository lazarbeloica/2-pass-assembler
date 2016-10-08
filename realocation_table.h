#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

struct RealocationEntry
{
	string sectionName;
	int offset;
	string realType;
	int num;
	RealocationEntry(string sec, int off, string type, int n) :sectionName(sec), offset(off), realType(type), num(n) {}
};

class RealocationTable
{
private:
	vector<RealocationEntry*> table;


public:
	void add(string sec, int off, string type, int n);
	string toString();
	
};