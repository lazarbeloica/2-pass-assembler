#include "section.h"

//lista naziva sekcija
const string Section::UND_SEC_NAME = "<UND>";
const string Section::TXT_SEC_NAME = "text";
const string Section::BSS_SEC_NAME = "bss";
const string Section::DATA_SEC_NAME = "data";

string Section::getName()
{
	return sectionName;
}

string Section::getType()
{
	return sectionName;
}

int Section::getLocationCounter()
{
	return lc;
}

void Section::addByte(string s)
{
	stringstream ss;
	ss << hex << s;
	byteString += ss.str();
}

void Section::addRealoc(int offset, string type, int num)
{
	realocations.add(getType(), offset, type, num);
}

void Section::clCounter()
{
	if (restartFlag)
		return;
	lc = 0;
	restartFlag = true;
}

void Section::operator++()
{
	lc += 4;	//receno je da su sve instrukcije 4B
}

Section & Section::operator+=(int n)
{
	lc += n;
	return *this;
}

void Section::inc(int n)
{
	lc += n;
}

string Section::toString()
{

	stringstream ss;
	ss << getName() << endl << endl;
	ss << setw(2);
	for (int i = 0; i < byteString.size(); i+=2)
	{
		if (i % 8 == 0)
			ss << setw(2) << hex << i / 2 << " :  ";
		string tmp = byteString.substr(i, 2) + " ";
		ss<< tmp;
		if (i % 8 == 6)
			ss << "\n";
	}

	ss << endl << endl;

	ss << ".rel" << getName() << '\n' <<endl;
	ss << realocations.toString() << endl;

	return ss.str();
}
