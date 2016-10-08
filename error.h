#pragma once

#include <string>

using namespace std;

class Error {
private:
	string message;
public:
	Error(string msg) :message(msg) {}

	string getMessage() { return message; }

};