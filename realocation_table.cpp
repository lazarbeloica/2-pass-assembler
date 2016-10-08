#include "realocation_table.h"



void RealocationTable::add(string sec, int off, string type, int n)
{
	table.push_back(new RealocationEntry(sec, off, type, n));
}

string RealocationTable::toString()
{
	RealocationEntry *sym;
	stringstream ss;

	
	ss << left <<setw(8) << "sim." << setw(10) << "ofset" << setw(10) << "tip" << endl;

	for (int i = 0; i < table.size(); i++)
	{
		sym = table[i];
		ss << left << setw(8)<< dec << sym->num << left << setw(10) << hex << sym->offset << left << setw(10) << sym->realType << endl;
	}
	return ss.str();
}
