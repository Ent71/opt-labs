#pragma once
#include "tables.h"
#include "symbol.h"

#include <string>

using namespace std;



class Lexer
{
public:
	void Parse(string input_file_name);
	void Print(ofstream& os);
	void ShowErrors(ofstream& os);
private:
	vector<string> errors;
};