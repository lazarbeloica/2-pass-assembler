#include "line_tokinizer.h"




int LineTokinizer::getLineType(string &line) throw()
{
	trim(line);
	if (line.empty())
		return 0;

	int result = 0;
	
	if (line.find(':') != string::npos)
	{
		result = DEFINITION_LINE;
		if (line.find(':') == string::npos)
			throw new Error(ERR_BAD_SINTX);
		return result;
	}

	int pos = line.find('.');
	if (pos != string::npos) {	//radi se o sekciji/uvozu/izvozu
		switch (line.at(pos+1)) 
		{
		case 'p': if(line.find(".public") == string::npos) throw new Error(ERR_BAD_SINTX);
			line.erase(pos, pos + 7);
			if(line.empty()) throw new Error(ERR_BAD_SINTX);
			if (line.at(0) > ' ') throw new Error(ERR_BAD_SINTX);
			result = EXPORT_LINE;
			break;
		
		case 'e': if(line.find(".extern") == string::npos)
			if (line.find(".end") != string::npos) 
			{
				line.erase(pos, pos + 4);
				if (!line.empty()) 
					if (line.at(0) > ' ') throw new Error(ERR_BAD_SINTX);	//iskljucuju se svi stampajuci znaci
				result = END_LINE;
				break;
			}else throw new Error(ERR_BAD_SINTX);
			
			line.erase(pos, pos + 7);
			if (line .empty()) throw new Error(ERR_BAD_SINTX);
			if (line.at(0) > ' ') throw new Error(ERR_BAD_SINTX);
			result = IMPORT_LINE;
			break;
		
		case 't': if(line.find(".text") == string::npos) throw new Error(ERR_BAD_SINTX);
			//line.erase(pos, pos + 5);
			if (!line.empty())
				if (line.at(0) != '.') throw new Error(ERR_BAD_SINTX);
			result = SECTION_TXT_LINE;
			break;
		
		case 'd': if(line.find(".data") == string::npos) throw new Error(ERR_BAD_SINTX);
			//line.erase(pos, pos + 5);
			if (!line.empty())
				if (line.at(0) != '.') throw new Error(ERR_BAD_SINTX);
			result = SECTION_DATA_LINE;
			break;
		
		case 'b': if(line.find(".bss") == string::npos) throw new Error(ERR_BAD_SINTX);
			//line.erase(pos, pos + 4);
			if (!line.empty())
				if (line.at(0) !='.') throw new Error(ERR_BAD_SINTX);	
			result = SECTION_BSS_LINE;
			break; 
		
		default: result = OTHR_DIR;		//neka od direktiva char/long ...
		}
	}
	return result;
}

int LineTokinizer::analizeDirective(string & line) throw()
{
	trim(line);
	int pos;
	if ((pos = line.find('.')) == string::npos)
		return NO_DIR;
	int res = 0;
	switch (line[pos+1])
	{
	case 's': if ((pos = line.find(".skip")) == string::npos)
		throw new Error(ERR_BAD_SINTX);
		pos += 4;
		res =  SKIP_DIR; break;
	case 'w': if ((pos = line.find(".word")) == string::npos)
		throw new Error(ERR_BAD_SINTX);	
		pos += 4;
		res =  WRD_DIR; break;
	case 'l': if ((pos = line.find(".long")) == string::npos)
		throw new Error(ERR_BAD_SINTX);
		pos += 4;
		res =  LONG_DIR; break;
	case 'a': if ((pos = line.find(".align")) == string::npos)
		throw new Error(ERR_BAD_SINTX);	
		pos += 5;
		res =  ALIGN_DIR; break;
	case 'c': if ((pos = line.find(".char")) == string::npos)
		throw new Error(ERR_BAD_SINTX);
		pos += 4;
		res = CHR_DIR; break;
	default: throw new Error("Nepodrzana direktiva!");
	}
	line.erase(0, pos + 1);
	return res;

}
