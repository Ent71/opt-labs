#include "symbol.h"

#include <cctype>

using namespace std;

vector<SymbolCategories> symbol_categories(256, SymbolCategories::InvalidCharacters);

void fill_symbol_categories()
{
	symbol_categories[32] = SymbolCategories::WhiteSpace;
	for (int i = 8; i < 14; i++)
	{
		symbol_categories[i] = SymbolCategories::WhiteSpace;
	}
	for (int i = 48; i < 58; i++)
	{
		symbol_categories[i] = SymbolCategories::Number;
	}
	for (int i = 97; i < 123; i++)
	{
		symbol_categories[i] = SymbolCategories::Leter;
	}
	for (int i = 65; i < 91; i++)
	{
		symbol_categories[i] = SymbolCategories::Leter;
	}
	symbol_categories[58] = SymbolCategories::Delimiter;
	symbol_categories[59] = SymbolCategories::Delimiter;
	symbol_categories[61] = SymbolCategories::Delimiter;
	symbol_categories[40] = SymbolCategories::CommentDelimiter;
	symbol_categories['%'] = SymbolCategories::ControlTaskSymbol;
}

char Symbol::GetValue() const
{
	return value;
}

SymbolCategories Symbol::GetType() const
{
	return type;
}

bool Symbol::get(ifstream& is)
{
	
	if (is.get(value))
	{
		type = symbol_categories[value];
		return true;
	}
	type = SymbolCategories::Eof;
	return false;
}