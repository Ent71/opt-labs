#pragma once

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
//%\d+-\l*
enum class SymbolCategories
{
	WhiteSpace,
	Number,
	Leter,
	Delimiter,
	CommentDelimiter,
	InvalidCharacters,
	ControlTaskSymbol,
	Eof
};

extern vector<SymbolCategories> symbol_categories;


void fill_symbol_categories();

class Symbol
{
public:
	char GetValue() const;
	SymbolCategories GetType() const;
	bool get(ifstream& is);
private:
	char value;
	SymbolCategories type;
};