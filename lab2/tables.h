#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct TockenTableField
{
	int column, line, id;
	string value;
};
extern map<string, int> key_word_table;
extern map<string, int> identifier_table;
extern map<string, int> constants_table;
extern map<string, int> control_task_table;
extern vector<TockenTableField> tocken_table;

