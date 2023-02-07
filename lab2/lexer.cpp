#include "lexer.h"


#include <fstream>

using namespace std;

map<string, int> key_word_table = { {"PROGRAM", 401}, {"BEGIN", 402}, {"END", 403},
										{"VAR", 404}, {"INTEGER", 405}, {"FLOAT", 406},
										{"ENDIF", 407}, {"IF", 408}, {"THEN", 409}, {"ELSE", 410}, {"LABEL", 411} };
map<string, int> identifier_table;
map<string, int> constants_table;
map<string, int> control_task_table;
vector<TockenTableField> tocken_table;

bool Lexer::Parse(const string& input_file_name)
{
	tocken_table.clear();
	identifier_table.clear();
	constants_table.clear();
	errors.clear();
	ifstream input_file(input_file_name);
	Symbol current_symbol;
	if (current_symbol.get(input_file))
	{
		int constants_id = 501, identifier_id = 1001, control_task_id = 5001;
		int column = 1, line = 1, bcolumn = 1, bline = 1;
		string current_tocken = "";
		while (current_symbol.GetType() != SymbolCategories::Eof)
		{
			current_tocken = "";
			switch (current_symbol.GetType())
			{
			case SymbolCategories::Number:
				while (current_symbol.GetType() == SymbolCategories::Number)
				{
					current_tocken += current_symbol.GetValue();
					current_symbol.get(input_file);
					column++;
					
				}
				{
					auto it = constants_table.find(current_tocken);
					if (it != constants_table.end())
					{
						tocken_table.push_back({ bcolumn, bline, (*it).second, current_tocken });
					}
					else
					{
						tocken_table.push_back({bcolumn, bline, constants_id, current_tocken });
						constants_table[current_tocken] = constants_id;
						constants_id++;
					}
				}
				bcolumn = column;
				break;
			case SymbolCategories::ControlTaskSymbol:
				current_tocken += current_symbol.GetValue();
				current_symbol.get(input_file);
				column++;
				if (current_symbol.GetType() == SymbolCategories::Number)
				{
					while (current_symbol.GetType() == SymbolCategories::Number)
					{
						current_tocken += current_symbol.GetValue();
						current_symbol.get(input_file);
						column++;
					}
					if (current_symbol.GetValue() == '-')
					{
						current_tocken += '-';
						current_symbol.get(input_file);
						column++;
						while (current_symbol.GetType() == SymbolCategories::Leter)
						{
							current_tocken += current_symbol.GetValue();
							current_symbol.get(input_file);
							column++;
						}
						{
							auto it = control_task_table.find(current_tocken);
							if (it != control_task_table.end())
							{
								tocken_table.push_back({ bcolumn, bline, (*it).second, current_tocken });
							}
							else
							{
								tocken_table.push_back({ bcolumn, bline, control_task_id, current_tocken });
								control_task_table[current_tocken] = control_task_id;
								control_task_id++;
							}
						}
						bcolumn = column;
					}
					else
					{
						errors.push_back("Lexer: Error(line " + to_string(bline) + ", column " + to_string(bcolumn) + "): Expected '-'");
					}
				}
				else
				{
					errors.push_back("Lexer: Error(line " + to_string(bline) + ", column " + to_string(bcolumn) + "): Expected number");
					current_symbol.get(input_file);
				}
				break;
			case SymbolCategories::Leter:
				while (current_symbol.GetType() == SymbolCategories::Number || current_symbol.GetType() == SymbolCategories::Leter)
				{
					current_tocken += current_symbol.GetValue();
					current_symbol.get(input_file);
					column++;
				}
				{
					auto it = key_word_table.find(current_tocken);
					if (it != key_word_table.end())
					{
						tocken_table.push_back({ bcolumn, bline, (*it).second, current_tocken });
					}
					else
					{
						it = identifier_table.find(current_tocken);
						if (it != identifier_table.end())
						{
							tocken_table.push_back({ bcolumn, bline, (*it).second, current_tocken });
						}
						else
						{
							tocken_table.push_back({ bcolumn, bline, identifier_id, current_tocken });
							identifier_table[current_tocken] = identifier_id;
							identifier_id++;
						}
					}
				}
				bcolumn = column;
				break;
			case SymbolCategories::Delimiter:
				{
					current_tocken += current_symbol.GetValue();
					tocken_table.push_back({ bcolumn, bline, static_cast<int>(current_symbol.GetValue()), current_tocken });
				}
				column++;
				bcolumn = column;
				current_symbol.get(input_file);
				break;
			case SymbolCategories::CommentDelimiter:
				column++;
				if (current_symbol.get(input_file) && current_symbol.GetValue() == '*')
				{
					column++;
					while (true)
					{
						if (current_symbol.GetType() == SymbolCategories::Eof)
						{
							errors.push_back("Lexer: Error(line " + to_string(bline) + ", column " + to_string(bcolumn) + "): Not a closed comment");
							break;
						}
						else
						{
							if (current_symbol.GetValue() == '*')
							{
								current_symbol.get(input_file);
								column++;
								if (current_symbol.GetType() != SymbolCategories::Eof && current_symbol.GetValue() == ')')
								{	
									current_symbol.get(input_file);
									break;
								}
							}
							else
							{
								if (current_symbol.GetValue() == '\n')
								{
									column = 1;
									line++;
								}
								current_symbol.get(input_file);
								column++;
							}
						}
					}
				}
				else
				{
					errors.push_back("Lexer: Error(line " + to_string(bline) + ", column " + to_string(bcolumn) + "): Expected comment");
				}
				bcolumn = column;
				bline = line;
				break;
			case SymbolCategories::WhiteSpace:
				while (current_symbol.GetType() == SymbolCategories::WhiteSpace)
				{
					if (current_symbol.GetValue() == '\n')
					{
						column = 1;
						line++;
					}
					else
					{
						column++;
					}
					current_symbol.get(input_file);
				}
				bcolumn = column;
				bline = line;
 				break;
			case SymbolCategories::InvalidCharacters:
				errors.push_back("Lexer: Error(line " + to_string(line) + ", column " + to_string(column) +  "): Invalid character: '" + current_symbol.GetValue() + '\'');
				current_symbol.get(input_file);
				column++;
				break;
			}
		}
	}
	else
	{
		errors.push_back("Lexer: Error: Empty file");
	}
	input_file.close();
	if (errors.size() == 0)
	{
		return true;
	}
	return false;
}


void Lexer::Print(ofstream& os)
{
	for (const TockenTableField& elem : tocken_table)
	{
		os << elem.line << " " << elem.column << " " << elem.id << " " << elem.value << '\n';
	}
}

void Lexer::ShowErrors(ofstream& os)
{
	for (const auto& elem : errors)
	{
		os << elem << '\n';
	}
}