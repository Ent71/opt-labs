#pragma once
#include "parser_tree.h"
#include <map>

using namespace std;

class CodeGenerator
{
public:
	void analyse(ParserTreeNode* tree, ofstream& os);
private:
	void generate_variable_declaration(ofstream& os, ParserTreeNode* tree);
	void generate_statements_list(ofstream& os, ParserTreeNode* tree);
	void generate_conditional_expression(ofstream& os, ParserTreeNode* tree);
	pair<int, TockenTableField> generate_expression(ofstream& os, ParserTreeNode* tree);
	void make_error(const string& message, int line, int column);
	vector<string> errors;
	//void check_identifier(const TockenTableField& identifier);
	TockenTableField get_identifier(ParserTreeNode* tree, int type);
	TockenTableField get_identifier(ParserTreeNode* tree);
	map<int, int> taken_identifiers;
	int lable_number = 0;
	
};