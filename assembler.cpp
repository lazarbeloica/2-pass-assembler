#include "assembler.h"

inline string Assembler::trim(string & str)
{
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
	str.erase(0, str.find_first_not_of('\t'));       //prefixing spaces
	str.erase(str.find_last_not_of('\t') + 1);         //surfixing spaces
	return str;
}

inline int Assembler::strToNum(string &line)
{
	int n = 0;
	trim(line);
	
	if (line.empty())
		throw new Error("Nema argumenata za konverziju u broj!");

	int i = 0;
	while ( i < line.size() && line[i] >= '0' && line[i] <= '9')
		n = n * 10 + (line[i++] - '0');
	
	if (i < line.size() && line[i] != ' '  && line[i] != ',')
		throw new Error("Argument nije broj!");

	line.erase(0, i + 1);

	return n;
}

inline string Assembler::getArgument(string & line)
{
	trim(line);
	if (line.empty())
		return "";
	string tmp;
	int pos;

	if (line.find_last_of(',') == line.size() - 1)
		throw new Error("Losa sintaksa!");

	pos = line.find_first_of(',');
	tmp = line.substr(0, pos);
	line.erase(0, pos == string::npos ? pos : pos + 1);
	trim(tmp);

	return tmp;
}

inline void Assembler::toHex(string &line, int width)
{
	trim(line);

	stringstream ss;
	ss << hex << line;
	signed n;
	ss >> n;

	bitset<4> b1(n); 
	bitset<8> b2(n);
	bitset<16> b3(n);

	switch (width)
	{
	case 1: line = b1.to_string(); break;
	case 4: line = b2.to_string(); width = 2; break;
	case 8: line = b3.to_string(); width = 4; break;
	}

	line = "0b" + line;
	ss << hex << line;
	string tmp = ss.str();
	tmp.erase(0, tmp.find_first_of('x') + 1);
	for (int i = tmp.size(); i < width * 2; i++)
		tmp = "0" + tmp;
	line = "";
	while (!tmp.empty())
	{
		line = line + tmp.substr(tmp.size() - 2, 2);//proveri ovo
		tmp.erase(tmp.size() - 2, 2);
	}
}

inline int Assembler::stringToInt(string arg)
{
	int base;
	if (arg.find_first_of('x') != string::npos)
	{
		base = 16;
		arg.erase(0, arg.find_first_of('x') + 1);
	}
	else if (arg.find_first_of('b') != string::npos)
	{
		base = 2;
		arg.erase(0, arg.find_first_of('b') + 1);
	}
	else if (arg.find_first_of('o') != string::npos)
	{
		base = 8;
		arg.erase(0, arg.find_first_of('0') + 1);
	}
	else base = 10;

	return stoi(arg, 0, base);
}

inline string Assembler::binToHec(string num)
{
	num.erase(0, num.find_first_of('b') + 1);
	for (int i = 4 - num.size() % 4; i < 4; i++)
		num = num[0] + num;
	string result = "";
	for (int j = num.size(); j > 0; j -= 4)
	{
		string x = "";
		for (int i = 0; i < 4; i++)
			x = num[j - i - 1] + x;

		if (x == "0000")
			x = "0";
		else if (x == "0001")
			x = "1";
		else if (x == "0010")
			x = "2";
		else if (x == "0011")
			x = "3";
		else if (x == "0100")
			x = "4";
		else if (x == "0101")
			x = "5";
		else if (x == "0110")
			x = "6";
		else if (x == "0111")
			x = "7";
		else if (x == "1000")
			x = "8";
		else if (x == "1001")
			x = "9";
		else if (x == "1010")
			x = "A";
		else if (x == "1011")
			x = "B";
		else if (x == "1100")
			x = "C";
		else if (x == "1101")
			x = "D";
		else if (x == "1110")
			x = "E";
		if (x == "1111")
			x = "F";

		result = x + result;
	}


	return result;
}




void Assembler::firstPass()
{
	IEallowed = true;		//dozvoljene su import i eksport operacije
	end = false;
	string line;
	analizeUndefined();
	
	while (getline(input, line) && !end)
	{
		trim(line);
		if (line.empty())
			continue;
		try {
			switch (LineTokinizer::getLineType(line))
			{
			case SECTION_TXT_LINE: analizeTxtSec(line); break;
			case SECTION_DATA_LINE: analizeDataSec(line); break;
			case SECTION_BSS_LINE: analizeBssSec(line); break;
			case EXPORT_LINE: analizePublic(line); break;
			case IMPORT_LINE: analizeExtern(line); break;
			case END_LINE: analizeEnd(line); break;
			case DEFINITION_LINE: analizeDefinition(line); break;
			case OTHR_DIR: switch (LineTokinizer::analizeDirective(line))
				{
					case ALIGN_DIR: analizeAlign(line); break;
					case CHR_DIR: analizeChar(line); break;
					case LONG_DIR: analizeLong(line); break;
					case SKIP_DIR: analizeSkip(line); break;
					case WRD_DIR: analizeWord(line); break;
					case NO_DIR: analizeNoDir(line); break;
				}break;
			case NO_DIR: analizeNoDir(line); break;
			default:
				break;
			}
		}
		catch (Error e) {		//hvata greske ako je losa sintaksa
			cout << e.getMessage() << endl;
			exit(1);
		}
	}

}

void Assembler::secondPass()
{
	string line;
	string helper = Section::UND_SEC_NAME;
	switchSec(helper);
	IEallowed = true;
	end = false;
	input.clear();
	input.seekg(0, ios::beg);
	while (getline(input, line) && !end)
	{
		trim(line);
		if (line.empty())
			continue;
		try {
			switch (LineTokinizer::getLineType(line))
			{
			case SECTION_TXT_LINE: switchSec(line); break;
			case SECTION_DATA_LINE: switchSec(line); break;
			case SECTION_BSS_LINE: switchSec(line); break;
			case EXPORT_LINE: executePublic(line); break;
			case IMPORT_LINE: executeExtern(line); break;
			case END_LINE: analizeEnd(line); break;
			case DEFINITION_LINE: executeDefinition(line); break;
			case OTHR_DIR: switch (LineTokinizer::analizeDirective(line))
			{
				case ALIGN_DIR: executeAlign(line); break;
				case CHR_DIR: executeChar(line); break;
				case LONG_DIR: executeLong(line); break;
				case SKIP_DIR: executeSkip(line); break;
				case WRD_DIR: executeWord(line); break;
				case NO_DIR: executeNoDir(line); break;
			}break;
			case NO_DIR: executeNoDir(line); break;
			default:
				break;
			}
		}
		catch (Error e) {		//hvata greske ako je losa sintaksa
			cout << e.getMessage() << endl;
			exit(1);
		}
	}
}

void Assembler::translate() throw()
{
	firstPass();
	secondPass();
}

void Assembler::writeToFile(ofstream & out)
{
	stringstream ss;
	ss << setfill('-') << setw(40) << "TABELA SIMBOLA" << setfill('-') << setw(39)<< '-' << '\n' <<endl;
	ss << setfill(' ');
	ss << symTab->toString();

	ss<< '\n'<< '\n' << setfill('-') << setw(40) << "TABELA SEKCIJA"<< setfill('-') << setw(39) << '-' << '\n'<< endl;
	ss << setfill(' ');
	ss << sectionStore->toString() << endl;
	out << ss.str();
}

void Assembler::executePublic(string &line)
{
	trim(line);
	string tmp;
	int pos;

	if (line.find_last_of(',') == line.size()-1)
		throw new Error("Losa sintaksa!");

	do
	{
		pos = line.find_first_of(',');
		tmp = line.substr(0, pos);
		line.erase(0, pos == string::npos ? pos : pos+1);
		trim(tmp);

		if (tmp.empty())
			throw new Error("Losa sintaksa!");

		Symbol *sym = symTab->get(tmp);
		if (sym == 0)
			throw new Error("Simbol " + tmp + " je definisan kao globalni u dajlu, a nema definiciju!");
		sym->local = false;

	} while (!line.empty());
}

void Assembler::executeExtern(string &line)
{
	trim(line);
	string tmp;
	int pos;

	if (line.find_last_of(',') == line.size() - 1)
		throw new Error("Losa sintaksa!");

	do
	{
		pos = line.find_first_of(',');
		tmp = line.substr(0, pos);
		line.erase(0, pos == string::npos ? pos : pos + 1);
		trim(tmp);

		if (tmp.empty())
			throw new Error("Losa sintaksa!");

		if (symTab->exists(tmp) != -1)
			throw new Error("Simbol " + tmp + " je visetruko definisan!");
		
		symTab->add(tmp, 0, curentSec->getName(), curentSec, false);

	} while (!line.empty());
}

void Assembler::switchSec(string &line)
{
	trim(line);
	
	if (curentSec != 0)
		sectionStore->put(curentSec);

	curentSec = sectionStore->get(line);
	curentSec->clCounter();
	IEallowed = false;
}

void Assembler::executeDefinition(string &line)
{
	trim(line);

	int pos = line.find(':');
	string newSymbol = line.substr(0, pos);

	line.erase(0, pos + 1);
	trim(line);

	while (line.empty())		//definicija je data u sledecem redu	
	{
		getline(input, line);
		trim(line);
	}

	int lcInc = 0;

	switch (LineTokinizer::analizeDirective(line))
	{
	case ALIGN_DIR: executeAlign(line); break;
	case CHR_DIR: executeChar(line); break;
	case LONG_DIR: executeLong(line); break;
	case SKIP_DIR: executeSkip(line); break;
	case WRD_DIR: executeWord(line); break;
	case NO_DIR: executeNoDir(line); break;
	}
}

void Assembler::redoArgList(string &line)
{
	trim(line);
//	string cmd = line.substr(0, line.find_first_of(' ') < line.find_first_of('\t') ? line.find_first_of(' ') : line.find_first_of('\t'));
//	line.erase(0, line.find_first_of(' ') < line.find_first_of('\t') ? line.find_first_of(' ') : line.find_first_of('\t'));
	//trim(line);
	string cmd ="";
	string arg;
	while (!line.empty())
	{
		Symbol *sym = 0;
		arg = line.substr(0, line.find_first_of(','));
		line.erase(0, line.find_first_of(',') == string::npos ? string::npos : line.find_first_of(',') + 1);
		trim(line);
		trim(arg);

		if (arg == "r0")
			cmd += "0x00";
		else if (arg == "r1")
			cmd += "0x01";
		else if (arg == "r2")
			cmd += "0x02";
		else if (arg == "r3")
			cmd += "0x03";
		else if (arg == "r4")
			cmd += "0x04";
		else if (arg == "r5")
			cmd += "0x05";
		else if (arg == "r6")
			cmd += "0x06";
		else if (arg == "r7")
			cmd += "0x07";
		else if (arg == "r8")
			cmd += "0x08";
		else if (arg == "r9")
			cmd += "0x09";
		else if (arg == "r10")
			cmd += "0x0A";
		else if (arg == "r11")
			cmd += "0x0B";
		else if (arg == "r12")
			cmd += "0x0C";
		else if (arg == "r13")
			cmd += "0x0D";
		else if (arg == "r14")
			cmd += "0x0E";
		else if (arg == "r15")
			cmd += "0x0F";
		else if (arg == "pc")
			cmd += "0x10";
		else if (arg == "lr")
			cmd += "0x11";
		else if (arg == "rsp")
			cmd += "0x12";
		else if (arg == "psw")
			cmd += "0x13";
		//dorada prebacivanje u hex broj i provera da nije to!
		else {

			
			bool flag = true;
			int address = 0;

			
			if(!(arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'b' || arg[1] == 'o')))
			for (int i = 0; i < arg.size(); i++) 
			{	//provera da nije dekadni sistem
				if (arg[i] < '0' || arg[i] > '9')		//radi se o simbolu
					if (!(i==0 && arg[i] == '-') && !(i == 0 && arg[i] == '-'))
					{
						if ((sym = symTab->get(arg)) != 0)
						{
							if (sym->section == curentSec)	//simbol definisan u istom seg
							{
								address = sym->offset - curentSec->getLocationCounter();
							}
							else {		//u nekom drugom segmentu u istom fajlu
								address = sym->offset - curentSec->getLocationCounter();
								//cmd += "0x00000000";
								localRelocation(arg);
							}
						}
						else
						{		//van fjla
							address = 0;
							//cmd += "0x00000000";
							externalRelocation(arg);
						}
						flag = false;
						break;
					}
				
			}
			if ((arg[0] == '0' && (arg[1]=='x' || arg[1] == 'b' || arg[1] == 'o')) || flag)
			{	//u pitanju je direktna vrednost
				address = stringToInt(arg);
			//	stringstream ss;
			//	ss << hex << stringToInt(arg);
			//	arg = ss.str();
			//	if (arg.find_first_of('x') != string::npos)
			//		arg.erase(0, arg.find_first_of('x') + 1);
			//	if (arg.size() > 8)
			//		arg.erase(0, arg.size() - 8);
			//	for (int i = arg.size(); i < 8; i++)
			//		arg = "0" + arg;
			//	cmd += "0x" + arg;
			}
			stringstream ss;
			ss << hex << address;
			arg = ss.str();
			if (arg.find_first_of('x') != string::npos)
				arg.erase(0, arg.find_first_of('x') + 1);
			if (arg.size() > 8)
				arg.erase(0, arg.size() - 8);
			for (int i = arg.size(); i < 8; i++)
				arg = "0" + arg;
			cmd += "0x" + arg;

		}
		cmd += " ";
	}
	
	line = cmd;
}

int Assembler::reinterpretLine(string &line)
{
	if (line.find("in") != string::npos)
		line += ", 1";		//dodaje se arg da je u pitanju in posto je op code isti

	else if(line.find("out") != string::npos)
		line += ", 0";		

	else if (line.find("shr") != string::npos)
		line += ", 0";		
	else if (line.find("shl") != string::npos)
		line += ", 1";		

	else if (line.find("ldch") != string::npos)
		line += ", 1";		

	else if (line.find("ldcl") != string::npos)
		line += ", 0";		
	else if (line.find("ldr") != string::npos)
	{
		if(line[0] == 'i')
			line += ", 4, 1";		//f=4 i\s=1	
		else line += ", 5, 1";
		line.erase(0, 1);	//brise karakter i ili d
	}
	else if (line.find("str") != string::npos)
	{
		if (line[0] == 'i')
			line += ", 4, 0";		//f=4 i\s=1	
		else line += ", 5, 0";
		line.erase(0, 1);	//brise karakter i ili d
	}
	else if (line.find("iret") != string::npos)
	{
		line.erase(0, 4);	//brise iret
		if (line.empty())
			line = ", lr ,0 ,0";
		line = "mov_s pc" + line;	//menja naziv instrukcije i dodaje odrediste
	}
	else if (line.find("mov") != string::npos)
	{
		string tmp = line;
		tmp.erase(0, tmp.find_first_of(' ') + 1 );
		trim(tmp);
		int n = 0;
		while (tmp!="")
		{
			if (tmp.find_first_of(',') == string::npos)
				tmp.erase(0, tmp.find_first_of(','));
			else
				tmp.erase(0, tmp.find_first_of(',') + 1);
			trim(tmp);
			n++;
		}
		if (n  == 2 )
			line += ", 0, 0";
		if (n == 3)
			line += ", 0";
	}
	else if (line.find("call") != string::npos)
	{
		if (line.find_first_of(',') == string::npos) 
		{
			string cmd = line.substr(0, line.find_first_of(' ') + 1);
			line.erase(0, line.find_first_of(' ') + 1);
			line = cmd + " pc, " + line;
		}
	}
	if (line.find("ldc ") != string::npos || line.find("ldc\t") != string::npos)	//pravi dve linije
	{

		string c = line.substr( line.find_last_of(',') + 1, line.size() - line.find_last_of(','));
		trim(c);
		//redoArgList(c);
		line.erase(0, 4);	//brise i komandu
		line.erase(line.find_last_of(','), string::npos);
		trim(line);
	//	stringstream ss;
	//	unsigned a;
	//	ss << hex << c;
	//	ss >> a;
//		bitset<32> b(a);
//		c = b.to_string();
//		string c1 = "", c2 = "";
	//	for (int i = 0; i < 16; i++)
	//	{
	//		c1 += c[i];
	//		c2 += c[i + 16];
	//	}
	//	c1 = ", 0x" + binToHec(c1);
	//	c1 = ", 0x" + binToHec(c2);
		string cmd1 = "ldch_ " + line+", " + c + ", 1";
		string cmd2 = "ldcl_ " + line + ", " + c + ", 0";
		line = cmd1 + "\n" + cmd2;
		return 2;
	}
	return 1;
}

void Assembler::redoDirectiveArg(string &line)
{
	string arg = line.substr(0, line.find_first_of('-'));
	trim(arg);
	line.erase(0, line.find_first_of('-'));

	Symbol *sym;
	bool flag = true;
	int address = 0;
	int factor = 1;
	do{
		flag = true;
		for (int i = 0; i < arg.size(); i++)
		{	//provera da nije dekadni sistem
			if (arg[i] < '0' || arg[i] > '9')		//radi se o simbolu
				if (!(i == 0 && arg[i] == '-') && !(i == 0 && arg[i] == '-'))
				{
					if ((sym = symTab->get(arg)) != 0)
					{
						if (sym->section == curentSec)	//simbol definisan u istom seg
						{
							address += factor * (sym->offset - curentSec->getLocationCounter());
						}
						else {		//u nekom drugom segmentu u istom fajlu
							address += factor * ( sym->offset - curentSec->getLocationCounter());
							localRelocation(arg);
						}
					}
					else
					{		//van fjla
						//address += factor * 0;
						//externalRelocation(arg);
						throw new Error(".long ne podrzava referenciranje eksternih promenljivih !");
					}
					flag = false;
					break;
				}
		}
		if ((arg[1] == '0' && (arg[1] == 'x' || arg[1] == 'b' || arg[1] == 'o')) || flag)
		{	//u pitanju je direktna vrednost
			address += stringToInt(arg);
		}

		if (line.find_first_of('-') != string::npos) 
		{
			line.erase(0, line.find_first_of('-') + 1);
			arg = line.substr(0, line.find_first_of('-'));
			factor = -1;
			if (line.find_first_of('-') != string::npos)
				throw new Error(".long podrzava samo jedan operator - !");
			arg = line;
			trim(arg);
			line = "";
		}
		else arg = line;
		
		if (line.find_first_of('+') != string::npos)
			throw new Error(".long podrzava samo jedan operator - !");

	} while (arg != "");

	
	stringstream ss;
	ss << hex << address;
	arg = ss.str();
	if (arg.find_first_of('x') != string::npos)
		arg.erase(0, arg.find_first_of('x') + 1);
	if (arg.size() > 8)
		arg.erase(0, arg.size() - 8);
	for (int i = arg.size(); i < 8; i++)
		arg = "0" + arg;
	line = "";

	for (int i = arg.size() - 1; i > 0; i -= 2)	//litle-endian
		line = line + arg.substr(i - 1, 1) + arg.substr(i, 1);
}

inline void Assembler::executeAlign(string & line)
{
	int padding = 4 - curentSec->getLocationCounter() % 4;
	
	for (int i = 0; i < padding; i++)
		curentSec->addByte("00");
	curentSec->inc(padding);
}

inline void Assembler::executeLong(string & line)
{
	trim(line);
	int pos = -1;
	do {
		line.erase(0, pos + 1);
		
		string arg = getArgument(line);
		while (!arg.empty()) 
		{
			redoDirectiveArg(arg);
			curentSec->addByte(arg);
			curentSec->inc(4);
			arg = getArgument(line);
		}
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::executeWord(string & line)
{
	trim(line);
	int pos = -1;
	do {
		line.erase(0, pos + 1);
		
		string arg = getArgument(line);
		if (arg.find_first_of('-') != string::npos || arg.find_first_of('+') != string::npos)
			throw new Error(".word ne podrzava operator + i -");
		while (!arg.empty())
		{
			toHex(arg, 4);
			curentSec->addByte(arg);
			curentSec->inc(2);
			arg = getArgument(line);
		}
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::executeChar(string & line)
{
	trim(line);
	int pos = -1;
	do {
		line.erase(0, pos + 1);

		string arg = getArgument(line);
		if (arg.find_first_of('-') != string::npos || arg.find_first_of('+') != string::npos)
			throw new Error(".word ne podrzava operator + i -");
		while (!arg.empty())
		{
			toHex(arg, 1);
			curentSec->addByte(arg);
			curentSec->inc(1);
			arg = getArgument(line);
		}
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::executeSkip(string & line)
{
	int n;
	trim(line);
	n = strToNum(line);

	for (int i = 0; i < n; i++)
		curentSec->addByte("00");

	curentSec->inc(n);
}

inline void Assembler::executeNoDir(string & line)
{
	trim(line);
	while (line.find_first_of('\t')!=string::npos)
		line[line.find_first_of('\t')] = ' ';
	string tmpLine = "";
	if(reinterpretLine(line) != 1)		//deluje cudno, ali to je zbog pseudo instrukcije koja se prevodi u sve
	{
		tmpLine = line;
		line = tmpLine.substr(0, tmpLine.find_first_of('\n'));
		tmpLine.erase(0, tmpLine.find_first_of('\n') + 1);
	}
	while (line != "")
	{
		int pos = line.find_first_of(' ');
		string inst = line.substr(0, pos);
		string x = inst;
		line.erase(0, pos + 1);
		trim(line);
		redoArgList(line);
		int instType;
		inst = coder.getOpCode(inst, instType);
		line = coder.parseArgs(instType, line, x);
		line = inst + line;
		curentSec->addByte(line);
		curentSec->inc(4);
		line = tmpLine;
		tmpLine = "";
	}
}

void Assembler::localRelocation(string &arg)
{
	Symbol *sym = symTab->get(arg);
	Symbol *symSec = symTab->get(sym->section->getName());
	curentSec->addRealoc(curentSec->getLocationCounter(), LOCAL_REALOC_ABS, symSec->num);
}

void Assembler::externalRelocation(string &line)
{
	Symbol *sym = new Symbol(line, 0, "-1", 0, false);
	symTab->add(sym);
	curentSec->addRealoc(curentSec->getLocationCounter(), EXTERN_REALOC_ABS, sym->num);
}


void Assembler::localRelocationSub(string &arg)
{
	Symbol *sym = symTab->get(arg);
	Symbol *symSec = symTab->get(sym->section->getName());
	curentSec->addRealoc(curentSec->getLocationCounter(), LOCAL_REALOC_SUB, symSec->num);
}

void Assembler::analizeExtern(string &line)		//u prvom prolazu samo proverava sintaksu
{
	if (!IEallowed)	//definise se ectern ili public u nekoj sekciji
		throw new Error(EXTRN_ERR);
	trim(line);
	if (line.empty())
		throw new Error("Nisu definisane promenljive u .extern");
}

void Assembler::analizePublic(string &line)	//u prvomprolazu ce samo da proveri sintaksu
{
	if (!IEallowed)	//definise se ectern ili public u nekoj sekciji
		throw new Error(PUBLIC_ERR);
	trim(line);
	if (line.empty())
		throw new Error("Nisu definisane promenljive u .public");
}

void Assembler::analizeEnd(string &line)	//oznacava kraj prvog proaza
{
	trim(line);
	if (!line.empty())
		throw new Error( "Nije samo .end u liniji!");
	end = true;
}

void Assembler::analizeDefinition(string &line)
{
	if (IEallowed)
		throw new Error("Lokalna promenljiva se definise van sekcije!");

	trim(line);

	int pos = line.find(':');
	string newSymbol = line.substr(0, pos);
	if (symTab->exists(newSymbol) != -1)
		throw new Error("Simbol " + newSymbol + " je visestruko definisan!");
	symTab->add(newSymbol, curentSec->getLocationCounter(), curentSec->getName(), curentSec);	//ovo bi ja prebacio za kasnije

	line.erase(0, pos + 1);
	trim(line);

	if (line.empty())		//definicija je data u sledecem redu	
	{
		do
		{
			if (!getline(input, line))
				throw new Error("Dosegnut kraj fajla, a nije pronadjena definicija simbola " + newSymbol);
			trim(line);
		} while (line.empty());
	}

	int lcInc = 0;

	switch (LineTokinizer::analizeDirective(line))
	{
	case ALIGN_DIR: analizeAlign(line); break;
	case CHR_DIR: analizeChar(line); break;
	case LONG_DIR: analizeLong(line); break;
	case SKIP_DIR: analizeSkip(line); break;
	case WRD_DIR: analizeWord(line); break;
	case NO_DIR: analizeNoDir(line); break;
	}
	
}

void Assembler::analizeTxtSec(string &line)
{//da li ce \n da pravi problem i da li treba da se menja sa \0
	trim(line);
	if (line.find_first_of(' ') != string::npos)
		throw new Error("U jednoj liniji sme da postoji samo ime sekcije!");
	int pos;
	if ((pos = line.find('\n')) != string::npos)
		line[pos] = '\0';
	
	if(curentSec != 0)
		sectionStore->put(curentSec);

	curentSec = sectionStore->get(line);
	if (curentSec == 0)
	{
		curentSec = new Section(line);
		symTab->add(curentSec->getName(), curentSec->getLocationCounter(), curentSec->getType(), curentSec);
	}
	IEallowed = false;
}

void Assembler::analizeBssSec(string &line)
{
	trim(line);
	if (line.find_first_of(' ') != string::npos)
		throw new Error("U jednoj liniji sme da postoji samo ime sekcije!");
	int pos;
	if ((pos = line.find('\n')) != string::npos)
		line[pos] = '\0';

	if (curentSec != 0)
		sectionStore->put(curentSec);

	curentSec = sectionStore->get(line);
	if (curentSec == 0)
	{
		curentSec = new Section(line);
		symTab->add(curentSec->getName(), curentSec->getLocationCounter(), curentSec->getType(), curentSec);
	}
	IEallowed = false;
}

void Assembler::analizeDataSec(string &line)
{
	trim(line);
	if (line.find_first_of(' ') != string::npos)
		throw new Error("U jednoj liniji sme da postoji samo ime sekcije!");
	int pos;
	if ((pos = line.find('\n')) != string::npos)
		line[pos] = '\0';

	if (curentSec != 0)
		sectionStore->put(curentSec);

	curentSec = sectionStore->get(line);
	if (curentSec == 0)
	{
		curentSec = new Section(line);
		symTab->add(curentSec->getName(), curentSec->getLocationCounter(), curentSec->getType(), curentSec);
	}
	IEallowed = false;
}

void Assembler::analizeUndefined()
{
	if (curentSec != 0)
		sectionStore->put(curentSec);

	curentSec = sectionStore->get(Section::UND_SEC_NAME);
	if (curentSec == 0)
	{
		curentSec = new Section(Section::UND_SEC_NAME);
		symTab->add(curentSec->getName(), curentSec->getLocationCounter(), curentSec->getType(), curentSec);
	}
}

void Assembler::analizeAlign(string &line)
{
	trim(line);
	if (!line.empty())
		throw new Error(".align direktiva ne sme da ima argument!");
	curentSec->inc(4 - curentSec->getLocationCounter() % 4);
}

inline void Assembler::analizeLong(string &line)
{
	trim(line);
	if (line.empty())
		throw new Error(".long direktiva mora da ima argument!");
	int pos = -1;
	do {
		line.erase(0, pos + 1);
		curentSec->inc(4);
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::analizeWord(string &line)
{
	trim(line);
	if (line.empty())
		throw new Error(".word direktiva mora da ima argument!");
	int pos = -1;
	do {
		line.erase(0, pos + 1);
		curentSec->inc(2);
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::analizeChar(string &line)
{
	trim(line);
	if (line.empty())
		throw new Error(".char direktiva mora da ima argument!");
	int pos = -1;
	do {
		line.erase(0, pos + 1);
		curentSec->inc(1);
	} while ((pos = line.find(',')) != string::npos);
}

inline void Assembler::analizeSkip(string & line)
{
	int n;
	trim(line);
	try {
		n = strToNum(line);
	}
	catch (Error e) {
		throw new Error("Greska u .skip: " + e.getMessage());
	}

	trim(line);
	if (!line.empty())
		throw new Error("Skip sme da ima tacno jedan argument!");

	curentSec->inc(n);
}

inline void Assembler::analizeNoDir(string & line)
{
	//pretpostavka je da se radi o instrukciji
	if (!line.empty())
		curentSec->inc(4);
	if(line.find("ldc ")!=string::npos || line.find("ldc\t") != string::npos)	//ovo se prevoodi u 2 instrukcije
		curentSec->inc(4);
}

