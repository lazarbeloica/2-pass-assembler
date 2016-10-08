#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

class Section;

struct Symbol
{
	string name;	//naziv simbola
	int num;		//redni broj  tabeli
	int offset;		//pomeraj
	bool local;		//da li je doseg lokalan ili  nije
	string sectionName;
	Section *section;
	
	//konstruktor simbola, argumetni: ime, redni broj, pomeraj i doseg
	Symbol(string n, int rbr, int offs, string sec, Section *sp, bool l = true) :name(n), num(rbr), offset(offs), sectionName(sec), section(sp), local(l) {}
	Symbol(string n, int offs, string sec, Section *sp, bool l = true) :name(n), offset(offs), sectionName(sec), section(sp), local(l) {}
};


class SymbolTable 
{
private:
	int counter = 0;		//brojac rednog broja
	map<string, int> table;	//mapira ime simbola u ulaz u vektoru simbola, radi brze pretrage
	vector<Symbol*> symTable;
public:
	SymbolTable();
	~SymbolTable();

	int exists(string name);			//proverava da li simbol vec postoji u tabeli i vraca njegovu lokaciju ili -1 ako ne
	
	void add(Symbol *sym);
	void add(string name, int offset, string secNam, Section *sp = 0, bool local = true);				//dodaje nov simbol u tabelu
	
	Symbol* get(string name);			//dohvata simbol, bez brisanja iz tabele, po imenu
	Symbol* get(int num);				//dohvata simbol sa zadatim rednim brojem

	void addWithChck(string name, int offset, string secNam, Section *sp = 0, bool local = true);		//ubacuje simbol u tabelu ako vec ne postoji,ako postoji, ne radi nista
	string toString();
};