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
	struct tocken_table_field
	{
		size_t column, line, id;
		string value;
	};
	vector<tocken_table_field> tocken_table;
	vector<string> errors;
};