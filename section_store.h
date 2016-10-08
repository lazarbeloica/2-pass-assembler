#pragma once

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "section.h"

using namespace std;

class SectionStore
{
private:
	map<string, Section*> store;
public:

	bool exists(string name);

	void put(Section *);
	Section* get(string name);
	string toString();
};