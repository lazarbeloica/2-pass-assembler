#include "opcod_interpretator.h"

OpCodeInterpretator::OpCodeInterpretator()
{
	coder.insert(pair<string, string>("int","0"));
	coder.insert(pair<string, string>("add", "1"));
	coder.insert(pair<string, string>("sub", "2"));
	coder.insert(pair<string, string>("mul", "3"));
	coder.insert(pair<string, string>("div", "4"));
	coder.insert(pair<string, string>("cmp", "5"));
	coder.insert(pair<string, string>("and", "6"));
	coder.insert(pair<string, string>("or", "7"));
	coder.insert(pair<string, string>("not", "8"));
	coder.insert(pair<string, string>("test", "9"));
	coder.insert(pair<string, string>("ldr", "A"));
	coder.insert(pair<string, string>("str", "A"));
	coder.insert(pair<string, string>("call", "C"));
	coder.insert(pair<string, string>("in", "D"));
	coder.insert(pair<string, string>("out", "D"));
	coder.insert(pair<string, string>("mov", "E"));
	coder.insert(pair<string, string>("shr", "E"));
	coder.insert(pair<string, string>("shl", "E"));
	coder.insert(pair<string, string>("ldch", "F"));
	coder.insert(pair<string, string>("ldcl", "F"));

	coder.insert(pair<string, string>("eq", "0"));
	coder.insert(pair<string, string>("ne", "2"));
	coder.insert(pair<string, string>("gt", "4"));
	coder.insert(pair<string, string>("ge", "6"));
	coder.insert(pair<string, string>("lt", "8"));
	coder.insert(pair<string, string>("le", "A"));
	coder.insert(pair<string, string>("_", "E"));

	coder.insert(pair<string, string>("eqs", "1"));
	coder.insert(pair<string, string>("nes", "3"));
	coder.insert(pair<string, string>("gts", "5"));
	coder.insert(pair<string, string>("ges", "7"));
	coder.insert(pair<string, string>("lts", "9"));
	coder.insert(pair<string, string>("les", "B"));
	coder.insert(pair<string, string>("_s", "F"));
}

string OpCodeInterpretator::getOpCode(string cmd, int &type)
{
	string ext;
	if (cmd[cmd.size() - 1 ] == 's')
	{
		if (cmd.find_last_of("_") != string::npos)
		{
			ext = "_s";
			cmd.erase(cmd.size() - 2, 2);
		}
		else
		{
			ext = cmd.substr(cmd.size() - 3, 3);
			cmd.erase(cmd.size() - 3, 3);
		}
	}
	else if (cmd.find_last_of('_') != string::npos)
	{
		ext = "_";
		cmd.erase(cmd.size() - 1, 1);
	}
	else {
		ext = cmd.substr(cmd.size() - 2, 2);
		cmd.erase(cmd.size() - 2, 2);
	}

	map<string, string>::iterator it1, it2;
	it1 = coder.find(ext);
	it2 = coder.find(ext);
	if (it1 == coder.end() || it2 == coder.end())
		throw new Error("Nepodrzana instrukcija!");

	ext = coder[ext];
	cmd = coder[cmd];
	
	type = operandGroupType(cmd);

	return ext+cmd;
}

inline string OpCodeInterpretator::trim(string & str)
{
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
	str.erase(0, str.find_first_not_of('\t'));       //prefixing spaces
	str.erase(str.find_last_not_of('\t') + 1);         //surfixing spaces
	return str;
}

inline string OpCodeInterpretator::toHex(string & par, int bytes)
{
	string num = par.substr(0, par.find_first_of(' '));
	par.erase(0, par.find_first_of(' '));
	trim(par);
	trim(num);

	stringstream src; 
	src << hex << num;
	num = src.str();
	num.erase(0, num.find_first_of('x') + 1);
	string result = "";
	int t = num.size();
	for (int i = 0; i < bytes && i < num.size(); i++)
		result = num[num.size() - i - 1] + result;
		
		
	return result;
}

inline string OpCodeInterpretator::binToHec(string num)
{
	num.erase(0, num.find_first_of('b') + 1);
	for (int i = 4 - num.size() % 4; i < 4; i++)
		num = num[0] + num;
	string result = "";
	for (int j = num.size(); j > 0 ; j -= 4)
	{
		string x = "";
		for (int i = 0; i < 4; i++)
			x = num[ j - i - 1] + x;

		if (x == "0000")
			x = "0";
		else if (x == "0001")
			x = "1";
		else if(x == "0010")
			x = "2";
		else if(x == "0011")
			x = "3";
		else if(x == "0100")
			x = "4";
		else if(x == "0101")
			x = "5";
		else if(x == "0110")
			x = "6";
		else if(x == "0111")
			x = "7";
		else if(x == "1000")
			x = "8";
		else if(x == "1001")
			x = "9";
		else if(x == "1010")
			x = "A";
		else if(x == "1011")
			x = "B";
		else if(x == "1100")
			x = "C";
		else if(x == "1101")
			x = "D";
		else if(x == "1110")
			x = "E";
		if (x == "1111")
			x = "F";

		result = x + result;
	}


	return result;
}

inline int OpCodeInterpretator::stringToInt(string arg)
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

	int res;
	try
	{
		res = stoi(arg, 0, base);
	}
	catch (...)
	{
		arg.erase(2, 1);
		res = stoi(arg, 0, base);
	}
	
	return res;
}

int OpCodeInterpretator::operandGroupType(string cmd)
{
	if(cmd == "0")
		return INTERUPT;
	if(cmd == "1" || cmd == "2" || cmd == "3" || cmd == "4" || cmd == "5")
		return ARITMETICAL;
	if (cmd == "7" || cmd == "8" || cmd == "9")
		return LOGICAL;
	if (cmd == "A")
		return MEMORY;
	if (cmd == "C")
		return CALL;
	if (cmd == "D")
		return PERIFERY;
	if (cmd == "E")
		return SHIFT;
	if (cmd == "F")
		return BITWRITE;
}

string OpCodeInterpretator::parseArgs(int & type, string & args, string cmd)
{
	switch (type) 
	{
	case INTERUPT: return parseArgsInt(args);
case ARITMETICAL: return parseArgsArit(args, cmd);
case LOGICAL: return parseArgsLogic(args);
case MEMORY: return parseArgsMem(args);
case CALL: return parseArgsCall(args);
case PERIFERY: return parseArgsPerif(args);
case SHIFT: return parseArgsShift(args);
case BITWRITE: return parseArgsBitWr(args);
	}
}

string OpCodeInterpretator::parseArgsInt(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') != string::npos && arg.find_first_of('\t') != string::npos)
		throw new Error("Interapt instrukcije primaju samo jedan argument!");
	if(arg.size()>3)
		throw new Error("Interapt instrukcije primaju samo jedan argument manji od 16!");
	string adr = "0" + toHex(arg, 1) + "0000";
	if(!arg.empty() || adr == "0" || adr == "1" || adr == "2" || adr == "3")
		throw new Error("Interapt instrukcije primaju samo jedan argument izmedju 4 i 15!");
	return adr;
}

string OpCodeInterpretator::parseArgsArit(string & arg, string cmd)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Aritmeticke instrukcije primaju dva argumenta!");
	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));  // "0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2;
//	if (arg.find('#') != string::npos)
//	{
//		arg.erase(0, arg.find('#') + 1);
//		adr2 = trim(arg);//"0x" + toHex(arg, 5);
//	}
//	else
		adr2 = trim(arg);//"0x" + toHex(arg, 2);
		
	if(adr2.find_first_of(' ')!=string::npos)
		throw new Error("Aritmeticke instrukcije primaju samo dva argumenta!");
	if (adr2 == "0x19" || adr1 == "0x19")
		throw new Error("Aritmeticke instrukcije ne mogu da adresiraju psw!");
	if (!(cmd == "add" || cmd == "sub") && (adr2 == "0x18" || adr2 == "0x17" || adr2 == "0x16" || adr1 == "0x18" || adr1 == "0x17" || adr1 == "0x16"))
		throw new Error("Adresiran nedozvoljeni registar!");
	stringstream ss;
	unsigned a1, a2, a3;	

	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;
	ss.clear();
	ss << hex << adr2;
	ss >> a3;

	bitset<5> b1(a1);
	bitset<5> b2(a2);
	bitset<18> b3(a3);
	bitset<1> br(0);
	bitset<1> bi(1);

	string x;
	if (adr2.size() > 4)
		x = "0b" + b1.to_string()+ bi.to_string() + b3.to_string();
	else
		x = "0b" + b1.to_string()+ br.to_string() + b2.to_string();

	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i-= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsLogic(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Aritmeticke instrukcije primaju dva argumenta!");
	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));  //"0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2 = trim(arg);//"0x" + toHex(arg, 2);

	if (adr2.find_first_of(' ')!=string::npos)
		throw new Error("Aritmeticke instrukcije primaju samo dva argumenta!");
	if (adr2 == "0x19" || adr1 == "0x19")
		throw new Error("Aritmeticke instrukcije ne mogu da adresiraju psw!");
	if (adr2 == "0x17" || adr2 == "0x16" || adr1 == "0x17" || adr1 == "0x16")
		throw new Error("Adresiran nedozvoljeni registar!");
	stringstream ss;
	unsigned a1, a2;

	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;

	bitset<5> b1(a1);
	bitset<5> b2(a2);

	string x = "0b" + b1.to_string();
	x += b2.to_string();
	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsMem(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Aritmeticke instrukcije primaju pet argumenta!");
	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));// "0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr5 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr3 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr4;
//	if (arg.find('#') != string::npos)
//	{
//		arg.erase(0, arg.find('#') + 1);
	adr4 = trim(arg);//"0x" + toHex(arg, 5);
//	}
//	else
	//	throw new Error("Neposredna vrednost se navodi sa prefiksom #");

	if (adr1.empty() || adr2.empty() || adr3.empty() || adr4.empty() || adr5.empty() || adr5.find_first_of(' ') != string::npos)
		throw new Error("Aritmeticke instrukcije primaju samo dva argumenta!");

	stringstream ss;
	unsigned a1, a2, a3, a4, a5;

	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;
	ss.clear();
	ss << hex << adr3;
	ss >> a3;
	ss.clear();
	ss << hex << adr4;
	ss >> a4;
	ss.clear();
	ss << hex << adr5;
	ss >> a5;


	bitset<5> b1(a1);
	bitset<5> b2(a2);
	bitset<3> b3(a3);
	bitset<1> b4(a4);
	bitset<10> b5(a5);

	string x = "0b" + b1.to_string() + b2.to_string() + b3.to_string() + b4.to_string() + b5.to_string();
	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsCall(string & arg)
{
	trim(arg);
	string adr1;
	string adr2;
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
	{
		adr1 = "0x10";
		adr2 = trim(arg);
	}else
	{
		adr1 = trim(arg.substr(0, arg.find_first_of(' ')));
		adr2 = trim(arg.erase(0, arg.find_first_of(' ')));
	}
	
	if (adr1.empty() || adr2.empty() || adr2.find_first_of(' ') != string::npos)
		throw new Error("Call instrukcije primaju najvise dva argumenta!");


	stringstream ss;
	unsigned a1, a2;


	ss << hex << adr1;
	ss >> a1;
	stringstream ss1;
	ss1 << hex << adr2;
	ss1 >> a2;
//	a1 = stringToInt(adr1);
//	a2 = stringToInt(adr2);

	bitset<5> b1(a1);
	bitset<19> b2(a2);

	string x = "0b" + b1.to_string() + b2.to_string();
	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsPerif(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Instrukcije kom sa periferijama primaju tri argumenta!");
//	string adr1 = "0x" + toHex(arg, 1);
//	string adr2;
	
//	adr2 = "0x" + toHex(arg, 1);

//	if (!arg.empty())
//		throw new Error("Instrukcije kom sa periferijama primaju tri argumenta!");
//	string adr3 = toHex(arg, 1);


	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));// "0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr3 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);

	if (adr1.empty() || adr2.empty() || adr3.empty() || adr3.find_first_of(' ') != string::npos)
		throw new Error("Inst za rad sa periferijama instrukcije primaju samo tri argumenta!");


	stringstream ss;
	unsigned a1, a2, a3;

	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;
	ss.clear();
	ss << hex << adr3;
	ss >> a3;

	bitset<4> b1(a1);
	bitset<4> b2(a2);
	bitset<1> b3(a3);

	string x = "0b" + b1.to_string() + b2.to_string() + b3.to_string();

	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsShift(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Pomeracke instrukcije primaju cetiri argumenta!");
//	string adr1 = "0x" + toHex(arg, 2);
//	string adr2 = "0x" + toHex(arg, 2);

//	trim(arg);
//	if(arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
//		throw new Error("Pomeracke instrukcije primaju cetiri argumenta!");
	
//	string adr3 = "0x" + toHex(arg, 2);
//	string adr4 = "0x" + toHex(arg, 1);

	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));// "0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr3 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr4 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);

	if (adr1.empty() || adr2.empty() || adr3.empty() || adr4.empty() || adr4.find_first_of(' ') != string::npos)
		throw new Error("Pomeracke instrukcije primaju samo cetiri argumenta!");

	stringstream ss;
	unsigned a1, a2, a3, a4;

	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;
	ss.clear();
	ss << hex << adr3;
	ss >> a3;
	ss.clear();
	ss << hex << adr4;
	ss >> a4;


	bitset<5> b1(a1);
	bitset<5> b2(a2);
	bitset<5> b3(a3);
	bitset<1> b4(a4);

	string x = "0b" + b1.to_string() + b2.to_string() + b3.to_string() + b4.to_string();
	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}

string OpCodeInterpretator::parseArgsBitWr(string & arg)
{
	trim(arg);
	if (arg.find_first_of(' ') == string::npos && arg.find_first_of('\t') == string::npos)
		throw new Error("Bitske instrukcije primaju tri argumenta!");
//	string adr1 = "0x" + toHex(arg, 1);
//	string 	adr2 = "0x" + toHex(arg, 1);
//	string 	adr3 = "0x" + toHex(arg, 4);

//	if (!arg.empty())
//		throw new Error("Aritmeticke instrukcije primaju samo tri argumenta!");
	
	string adr1 = trim(arg.substr(0, arg.find_first_of(' ')));// "0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr3 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 2);
	arg.erase(0, arg.find_first_of(' ') + 1);
	string adr2 = trim(arg.substr(0, arg.find_first_of(' ')));//"0x" + toHex(arg, 1);

	if (adr1.empty() || adr2.empty() || adr3.empty() || adr3.find_first_of(' ') != string::npos)
		throw new Error("Bitske instrukcije primaju samo tri argumenta!");

	stringstream ss;
	unsigned a1, a2, a3;
	unsigned ax = 0;
	ss << hex << adr1;
	ss >> a1;
	ss.clear();
	ss << hex << adr2;
	ss >> a2;
	ss.clear();
	ss << hex << adr3;
	ss >> a3;

	bitset<4> b1(a1);
	bitset<1> b2(a2);
	bitset<16> b4(a3);

	string x = "0b" + b1.to_string() + b2.to_string() + "000" + b4.to_string();
	for (int i = x.size(); i < 24 + 2; i++)
		x += "0";

	x = binToHec(x);
	// invertovanje u litle-endian
	string res = "";
	for (int i = x.size() - 1; i > 0; i -= 2)
		res = res + x.substr(i - 1, 1) + x.substr(i, 1);
	return res;
}
