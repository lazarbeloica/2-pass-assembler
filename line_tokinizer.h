#pragma once

#include <string>
#include "error.h"

using namespace std;


#define SECTION_TXT_LINE 1
#define SECTION_BSS_LINE 2
#define SECTION_DATA_LINE 4
#define EXPORT_LINE 8
#define IMPORT_LINE 16
#define END_LINE 32
#define DEFINITION_LINE 64
#define OTHR_DIR 128

#define ERR_BAD_SINTX "Losa sintaksa!"

#define ALIGN_DIR 1
#define CHR_DIR 2
#define LONG_DIR 3
#define SKIP_DIR 4
#define WRD_DIR 5
#define NO_DIR 0

class LineTokinizer {
private:
	static inline string trim(string& str)
	{
		str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
		str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
		return str;
	}

	
public:
	static int getLineType(string &line) throw();
	static int analizeDirective(string &line) throw();



};



