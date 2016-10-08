#pragma once

#include <string>
#include "symtab.h"
#include "line_tokinizer.h"
#include <fstream>
#include "section.h"
#include <iostream>
#include "section_store.h"
#include <sstream>
#include <bitset>
#include "opcod_interpretator.h"
#include <iostream>
#include <iomanip>

using namespace std;

#define EXTRN_ERR "Spoljna promenjiva definisana na pogresnom mestu!"
#define PUBLIC_ERR "Globalna promenjiva definisana na pogresnom mestu!"
#define END_ERR "Linija u kojoj je .end nije ima jos instrukcija!"
//realokacija u okviru istog fajla
#define LOCAL_REALOC_ABS "R_32_RELADD"
//realokacija u okviru eksternog fajla
#define EXTERN_REALOC_ABS "R_EXT32_PCREL"
#define LOCAL_REALOC_SUB "R_32_RELSUB"

class Assembler {
private:
	ifstream &input;
	SymbolTable *symTab;
	SectionStore *sectionStore;
	OpCodeInterpretator coder;

	Section *curentSec = 0;
	bool IEallowed = true;		//dozvoljene su import i eksport operacije
	bool end = false;

	//f-je za obradu stringa
	inline string trim(string& str);
	inline int strToNum(string &line);
	inline string getArgument(string &line);
	inline void toHex(string &line, int width);//vraca litlendian reprezentaaciju u hex formatu sa prosirenjem
	inline int stringToInt(string arg);
	inline string binToHec(string num);

	//pomocne f-je za prvi prolaz asemblera
	void analizeExtern(string &line);
	void analizePublic(string &line);
	void analizeEnd(string &line);
	void analizeDefinition(string &line);
	void analizeTxtSec(string &line);
	void analizeBssSec(string &line);
	void analizeDataSec(string &line);
	void analizeUndefined();

	void inline analizeAlign(string &line);
	void inline analizeLong(string &line);
	void inline analizeWord(string &line);
	void inline analizeChar(string &line);
	void inline analizeSkip(string &line);
	void inline analizeNoDir(string &line);

	//f-je za drugi proaz
	void redoArgList(string &line);
	int reinterpretLine(string &line);
	void redoDirectiveArg(string &arg);

	void inline executeAlign(string &line);
	void inline executeLong(string &line);
	void inline executeWord(string &line);
	void inline executeChar(string &line);
	void inline executeSkip(string &line);
	void inline executeNoDir(string &line);

	void executePublic(string&);
	void executeExtern(string&);
	void switchSec(string&);
	void executeDefinition(string&);

	void localRelocation(string&);
	void externalRelocation(string&);
	void localRelocationSub(string &arg);

	void firstPass();
	void secondPass();
public:
	Assembler(ifstream &in):input(in)
	{
		symTab = new SymbolTable();
		sectionStore = new SectionStore();
	}
	~Assembler()
	{
		delete symTab;
		delete sectionStore;
	}
	void translate() throw();
	void writeToFile(ofstream &out);
};