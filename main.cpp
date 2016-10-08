#include <iostream>
#include <sstream>
#include <fstream>

#include "assembler.h"
#include "section_store.h"
#include "line_tokinizer.h"
#include "symtab.h"

using namespace std;

#define I_FILE_MSG "Nije pronadjen ulazni fajl "
#define ARG_MSG "Nema dovoljno argumenata za pokretanje!"
void main(int argc, char* argv[])
{
	
	if (argc < 2)
	{
		cout << ARG_MSG << endl;
		system("PAUSE");
		exit(1);
	}

	for (int i = 1; i < argc; i++)
	{
		stringstream ss;
		ss << argv[i];
		string in = ss.str();
		cout << endl;
		ifstream input(in);
		if (!input.is_open())
		{
			cout << I_FILE_MSG + in << endl;
			continue;
		}

		string out = "";
		out += in.substr(0, in.find_first_of('.') + 1);
		out.erase(out.find('.'), string::npos);	//brise ekstenziju
		out += ".s";
		ofstream output(out);

		Assembler assm(input);

		try {
			assm.translate();
		}
		catch (Error *e) { cout << "Greska u " + in + "\n" << e->getMessage() << endl; }

		assm.writeToFile(output);
		cout << "Uspesno kreiran fajl " + out << endl;
	}

	

	system("PAUSE");
}