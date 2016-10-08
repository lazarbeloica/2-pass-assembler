#include "symtab.h"

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
	for (int i = 0; i < table.size(); i++)
	{
		delete symTable[i];
		symTable.empty();
		table.empty();
	}
}

int SymbolTable::exists(string name)
{
	map<string, int>::iterator it;
	it = table.find(name);
	
	return it == table.end() ? -1 : table[name];
 }

void SymbolTable::add(Symbol *sym)
{
	symTable.push_back(sym);	//ubacuje na kraj
	sym->num = symTable.size() - 1;	//racuna redni broj
	table.insert(pair<string, int> (sym->name, sym->num));	//mapira redni broj u hes

}

void SymbolTable::add(string name, int offset, string secNam, Section *sp, bool local)
{
	add(new Symbol(name, offset, secNam, sp, local));
}

Symbol * SymbolTable::get(string name)
{
	int pos = exists(name);
	return pos == -1 ? 0 : symTable[pos];
}

Symbol * SymbolTable::get(int num)
{
	return num >= symTable.size() ? 0 : symTable[num];
}

void SymbolTable::addWithChck(string name, int offset, string secNam, Section *sp, bool local)
{
	if (exists(name) == -1)
		add(name, offset, secNam, sp,local);
}

string SymbolTable::toString()
{
	Symbol *sym;
	stringstream ss;
	ss << left << setw(10) << "r.br." << left << setw(20) << "ime" << left << setw(20) << "sekcija" << left << setw(10) << "offset" << left << setw(8) << "doseg" << endl;
	
	for (int i = 0; i < table.size(); i++)
	{
		sym = symTable[i];
		ss << left << setw(10)<< dec << sym->num << left << setw(20) << sym->name << left << setw(20) << sym->sectionName<< left << setw(10)
			<< hex << sym->offset << left << setw(8) << (sym->local ? " local " : " global ") << endl;

	}
	return ss.str();
}
