#pragma once
#include "lexer.h"
#include "parser_tree.h"
#include "tables.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

enum class ParseStages {
	ProgramFirstPart,
	ProgramSeconPart,
	ProgramThirdPart,
	BlockPreFirstPart,
	BlockFirstPart,
	BlockSecondPart,
	BlockThirdPart,
	VariableDeclaration,
	DeclarationsList,
	DeclarationFirstPart,
	DeclarationSecondPart,
	DeclarationThirdPart,
	StatementsList,
	Attribute,
	ProcedureIdentifier,
	VariableIdentifier,
	Identifier,
	StatementFirstPart,
	StatementSecondPart,
	ConditionStatementFirstPart,
	ConditionStatementSecondPart,
	IncompleteConditionStatementFirstPart,
	IncompleteConditionStatementSecondPart,
	AlternativePart,
	ConditionalExpressionFirstPart,
	ConditionalExpressionSecondPart,
	Expression,
	UnsignedInteger,
	Return,
	LableDeclarationsFirstPart,
	LableDeclarationsSeconPart,
	LableDeclarationsThirdPart,
	LabelsList,
	Label,
	End
};

class Parser {
public:
	void Parse();
	void Print(ofstream& os);
private:
	void make_error(int pos, string expected);
	ParserTreeNode* tree;
	string error = "";
};