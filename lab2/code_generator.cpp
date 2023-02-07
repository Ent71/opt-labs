#include "code_generator.h"

using namespace std;

TockenTableField CodeGenerator::get_identifier(ParserTreeNode* tree, int type)
{
	TockenTableField current_tocken = tree->children[0]->children[0]->value;
	if (taken_identifiers.count(current_tocken.id))
	{
		make_error("Variable: " + current_tocken.value + " is already defined", current_tocken.line, current_tocken.column);
	}
	else
	{
		taken_identifiers[current_tocken.id] = type;
	}
	return current_tocken;
}

TockenTableField CodeGenerator::get_identifier(ParserTreeNode* tree)
{
	TockenTableField current_tocken = tree->children[0]->children[0]->value;
	if (!taken_identifiers.count(current_tocken.id))
	{
		make_error("Variable: " + current_tocken.value + " is not defined", current_tocken.line, current_tocken.column);
		taken_identifiers[current_tocken.id] = -2;
	}
	return current_tocken;
}

void CodeGenerator::make_error(const string& message, int line, int column)
{
	errors.push_back("Code Generator : Error(line " + to_string(line) + ", column " + to_string(column) + "): " + message);
}

TockenTableField check_variable_declaration(ParserTreeNode* tree)
{
	return tree->children[0]->children[0]->value;
}

void CodeGenerator::generate_variable_declaration(ofstream& os, ParserTreeNode* tree)
{
	if (tree->children[0]->value.id != -2)
	{
		tree = tree->children[1];
		TockenTableField current_tocken;
		ParserTreeNode* declaration_tree;
		int type;
		while (tree->children[0]->value.id != -2)
		{
			declaration_tree = tree->children[0];
			type = declaration_tree->children[2]->children[0]->value.id;
			current_tocken = get_identifier(declaration_tree->children[0], type);
			os << "\t" << current_tocken.value << "\t";
			if (type == 405)
			{
				os << "DD"; 
			}
			else
			{
				os << "REAL4";
			}
			os << "\t?\n";
			tree = tree->children[1];
		}
	}
}

void CodeGenerator::generate_statements_list(ofstream& os, ParserTreeNode* tree)
{
	string current_lable1 = "$L" + to_string(lable_number), current_lable2 = "$L" + to_string(lable_number + 1);
	lable_number += 2;
	while (tree->children[0]->value.id != -2)
	{
		ParserTreeNode* conditional_statement_tree = tree->children[0]->children[0];
		generate_conditional_expression(os, conditional_statement_tree->children[0]->children[1]);
		os << "JNE " << current_lable1 << "\n";
		generate_statements_list(os, conditional_statement_tree->children[0]->children[3]);
		if (conditional_statement_tree->children[1]->children[0]->value.id != -2)
		{
			os << "JUMP " << current_lable2 << "\n";
			os << current_lable1 << ": " << "NOP\n";
			generate_statements_list(os, conditional_statement_tree->children[1]->children[1]);
			os << current_lable2 << ": " << "NOP\n";
		}
		else
		{
			os << current_lable1 << ": " << "NOP\n";
		}
		
		tree = tree->children[1];
	}
	os << "NOP\n";
}

void CodeGenerator::generate_conditional_expression(ofstream& os, ParserTreeNode* tree)
{
	pair<int, TockenTableField> lhs, rhs;
	os << "MOV AX, ";
	lhs = generate_expression(os, tree->children[0]);
	os << '\n';
	if (lhs.first == 406)
	{
		os << "FCOM AX, ";
	}
	else
	{
		os << "CMP AX, ";
	}
	rhs = generate_expression(os, tree->children[2]);
	os << '\n';
	if (lhs.first == -100)
	{
		make_error("In left part expression must be variable but found: " + lhs.second.value, lhs.second.line, lhs.second.column);
	}
	if (rhs.first != -100)
	{
		make_error("In right part expression must be unsigned integer but found: " + rhs.second.value, rhs.second.line, rhs.second.column);
	}
	if (lhs.first != rhs.first && !(lhs.first == -100 && rhs.first == 405 || lhs.first == 405 && rhs.first == -100))
	{
		make_error("Can not compare " + lhs.second.value + " and " + rhs.second.value + " different types", lhs.second.line, lhs.second.column);
	}
}

pair<int, TockenTableField> CodeGenerator::generate_expression(ofstream& os, ParserTreeNode* tree)
{
	if (tree->children[0]->value.id == -22)
	{
		os << tree->children[0]->children[0]->value.value;
		return { -100,  tree->children[0]->children[0]->value };
	}
	else
	{
		auto current_tocken = get_identifier(tree->children[0]);
		os << current_tocken.value;
		return { taken_identifiers[current_tocken.id], current_tocken };
	}
}

void CodeGenerator::analyse(ParserTreeNode* tree, ofstream& os)
{
	int lable_number = 0;
	string procedure_name;
	TockenTableField current_tocken;
	tree = tree->children[0];
	current_tocken = get_identifier(tree->children[1], -1);
	procedure_name = current_tocken.value;
	os << ".386\n.MODEL MEDIUM\n.DATA\n";
	tree = tree->children[3];
	generate_variable_declaration(os, tree->children[0]);
	os << ".CODE\n" << procedure_name << " PROC" << '\n';
	generate_statements_list(os, tree->children[2]);
	os << "ret\n" << procedure_name << " ENDP\n";

	for (const auto& error : errors)
	{
		os << error << '\n';
	}
}