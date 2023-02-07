#include "parser.h"
#include <stack>

using namespace std;

void Parser::Parse()
{
	//DeleteTree(tree);
	int len = tocken_table.size();
	if (len == 0)
	{
		return;
	}
	int current_tocken_position = 0;
	ParseStages current_stage = ParseStages::ProgramFirstPart;
	ParserTreeNode* current_node;
	stack<ParserTreeNode*> return_nodes;
	stack<ParseStages> next_stages;
	bool is_end = false;
	TockenTableField tmp = { tocken_table[0].column, tocken_table[0].line, -1, "<signal-program>" };
	tree = AddNode(tree, tmp);
	tmp.value = "<program>";
	current_node = AddNode(tree, tmp);
	tmp = { tocken_table[len - 1].column, tocken_table[len - 1].line, -1, "end" };
	tocken_table.push_back(tmp);
	while (!is_end)
	{

		switch (current_stage)
		{
		case ParseStages::ProgramFirstPart:
			if (tocken_table[current_tocken_position].id == 401)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				next_stages.push(ParseStages::ProgramSeconPart);
				return_nodes.push(current_node);
				current_stage = ParseStages::ProcedureIdentifier;

			}
			else
			{
				make_error(current_tocken_position, "'PROGRAM'");
				is_end = true;
			}

			break;
		case ParseStages::ProgramSeconPart:
			if (tocken_table[current_tocken_position].id == ';')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				next_stages.push(ParseStages::ProgramThirdPart);
				current_stage = ParseStages::BlockPreFirstPart;
				return_nodes.push(current_node);
			}
			else
			{
				make_error(current_tocken_position, "';'");
				is_end = true;
			}
			break;
		
		case ParseStages::ProgramThirdPart:
			if (tocken_table[current_tocken_position].id == '.')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::End;
				//is_end = true;
			}
			else
			{
				make_error(current_tocken_position, "'.'");
				is_end = true;
			}
			break;
		case ParseStages::ProcedureIdentifier:
			tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<procedure-identifier>" };
			current_node = AddNode(current_node, tmp);
			current_stage = ParseStages::Identifier;
			break;
		case ParseStages::VariableIdentifier:
			tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<variable-identifier>" };
			current_node = AddNode(current_node, tmp);
			current_stage = ParseStages::Identifier;
			break;
		case ParseStages::Identifier:
			if (tocken_table[current_tocken_position].id > 1000)
			{
				tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<identifier>" };
				current_node = AddNode(current_node, tmp);
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{
				make_error(current_tocken_position, "<identifier>");
				is_end = true;
			}
			break;
		case ParseStages::BlockPreFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<block>" };
			current_node = AddNode(current_node, tmp);
			return_nodes.push(current_node);
			next_stages.push(ParseStages::BlockFirstPart);
			current_stage = ParseStages::LableDeclarationsFirstPart;
			break;
		case ParseStages::BlockFirstPart:
			//tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<block>" };
			//current_node = AddNode(current_node, tmp);
			return_nodes.push(current_node);
			next_stages.push(ParseStages::BlockSecondPart);
			current_stage = ParseStages::VariableDeclaration;
			break;
		case ParseStages::BlockSecondPart:
			if (tocken_table[current_tocken_position].id == 402)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				return_nodes.push(current_node);
				next_stages.push(ParseStages::BlockThirdPart);
				current_stage = ParseStages::StatementsList;
			}
			else
			{
				is_end = true;
				make_error(current_tocken_position, "'BEGIN'");
			}
			break;
		case ParseStages::BlockThirdPart:
			if (tocken_table[current_tocken_position].id == 403)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{
				is_end = true;
				make_error(current_tocken_position, "'END'");
			}
			break;
		case ParseStages::LableDeclarationsFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<label-declaration>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == 411)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				return_nodes.push(current_node);
				next_stages.push(ParseStages::LableDeclarationsSeconPart);
				current_stage = ParseStages::UnsignedInteger;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;
		case ParseStages::LableDeclarationsSeconPart:
			//if (tocken_table[current_tocken_position].id == ',')
			//{
				//AddNode(current_node, tocken_table[current_tocken_position]);
				//current_tocken_position++;
				next_stages.push(ParseStages::LableDeclarationsThirdPart);
				return_nodes.push(current_node);
				current_stage = ParseStages::LabelsList;
				//current_stage = ParseStages::Return;
			//}
			//else
			//{
			//	make_error(0, "");
			//	current_stage = ParseStages::End;
			//}
			break;
		case ParseStages::LableDeclarationsThirdPart:
			if (tocken_table[current_tocken_position].id == ';')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{
				is_end = true;
				make_error(current_tocken_position, "';'");
			}
			break;
		case ParseStages::LabelsList:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<labels-list>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == ',')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				return_nodes.push(current_node);
				next_stages.push(ParseStages::LabelsList);
				current_stage = ParseStages::UnsignedInteger;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;
		case ParseStages::VariableDeclaration:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<variable-declaration>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == 404)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::DeclarationsList;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;
		case ParseStages::DeclarationsList:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<declarations-list>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id > 1000)
			{
				return_nodes.push(current_node);
				next_stages.push(ParseStages::DeclarationsList);
				current_stage = ParseStages::DeclarationFirstPart;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;
		case ParseStages::DeclarationFirstPart:
			next_stages.push(ParseStages::DeclarationSecondPart);
			return_nodes.push(current_node);
			tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<declaration>" };
			current_node = AddNode(current_node, tmp);
			tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<variable-identifier>" };
			current_node = AddNode(current_node, tmp);
			current_stage = ParseStages::Identifier;
			break;
		case ParseStages::DeclarationSecondPart:
			if (tocken_table[current_tocken_position].id == ':')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				return_nodes.push(current_node);
				next_stages.push(ParseStages::DeclarationThirdPart);
				current_stage = ParseStages::Attribute;
			}
			else
			{
				make_error(current_tocken_position, "':'");
				is_end = true;
			}
			break;
		case ParseStages::DeclarationThirdPart:
			if (tocken_table[current_tocken_position].id == ';')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{
				make_error(current_tocken_position, "';'");
				is_end = true;
			}
			break;
		case ParseStages::Attribute:
			if (tocken_table[current_tocken_position].id == 405 || tocken_table[current_tocken_position].id == 406)
			{
				tmp = { tocken_table[current_tocken_position].column, tocken_table[current_tocken_position].line, -1, "<attribute>" };
				current_node = AddNode(current_node, tmp);
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{

				make_error(current_tocken_position, "'INTEGER' or 'FLOAT'");
				is_end = true;
			}
			break;
		case ParseStages::StatementsList:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<statements-list>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == 408)
			{
				return_nodes.push(current_node);
				next_stages.push(ParseStages::StatementsList);
				current_stage = ParseStages::StatementFirstPart;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;
		case ParseStages::StatementFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<statement>" };
			current_node = AddNode(current_node, tmp);
			return_nodes.push(current_node);
			next_stages.push(ParseStages::StatementSecondPart);
			current_stage = ParseStages::ConditionStatementFirstPart;
			break;
		case ParseStages::StatementSecondPart:
			if (tocken_table[current_tocken_position].id == 407)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				if (tocken_table[current_tocken_position].id == ';')
				{
					AddNode(current_node, tocken_table[current_tocken_position]);
					current_tocken_position++;
					current_stage = ParseStages::Return;
				}
				else
				{
					make_error(current_tocken_position, "';'");
					is_end = true;
				}
			}
			else
			{
				make_error(current_tocken_position, "'ENDIF'");
				is_end = true;
			}
			break;
		case ParseStages::ConditionStatementFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<condition-statement>" };
			current_node = AddNode(current_node, tmp);
			return_nodes.push(current_node);
			next_stages.push(ParseStages::ConditionStatementSecondPart);
			current_stage = ParseStages::IncompleteConditionStatementFirstPart;
			break;
		case ParseStages::ConditionStatementSecondPart:
			current_stage = ParseStages::AlternativePart;
			break;
		case ParseStages::IncompleteConditionStatementFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<incomplete-condition-statement>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == 408)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				next_stages.push(ParseStages::IncompleteConditionStatementSecondPart);
				return_nodes.push(current_node);
				current_stage = ParseStages::ConditionalExpressionFirstPart;

			}
			else
			{
				make_error(current_tocken_position, "'IF'");
				is_end = true;
			}
			break;

		case ParseStages::IncompleteConditionStatementSecondPart:
			if (tocken_table[current_tocken_position].id == 409)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::StatementsList;
			}
			else
			{
				make_error(current_tocken_position, "'THEN'");
				is_end = true;
			}
			break;

		case ParseStages::AlternativePart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<alternative-part>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id == 410)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::StatementsList;
			}
			else
			{
				tmp.value = "<empty>";
				AddNode(current_node, tmp);
				current_stage = ParseStages::Return;
			}
			break;

		case ParseStages::ConditionalExpressionFirstPart:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<conditional-expression>" };
			current_node = AddNode(current_node, tmp);
			return_nodes.push(current_node);
			next_stages.push(ParseStages::ConditionalExpressionSecondPart);
			current_stage = ParseStages::Expression;
			break;

		case ParseStages::ConditionalExpressionSecondPart:
			if (tocken_table[current_tocken_position].id == '=')
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Expression;
			}
			else
			{
				make_error(current_tocken_position, "=");
				is_end = true;
			}
			break;

		case ParseStages::Expression:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<expretion>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id > 1000)
			{
				current_stage = ParseStages::VariableIdentifier;
			}
			else
			{
				if (tocken_table[current_tocken_position].id > 500)
				{
					current_stage = ParseStages::UnsignedInteger;
				}
				else
				{
					make_error(current_tocken_position, "<identifier> or <unsigned-integer>");
					is_end = true;
				}
			}
			break;

		case ParseStages::UnsignedInteger:
			tmp = { tocken_table[current_tocken_position].column , tocken_table[current_tocken_position].line, -1, "<unsigned-integer>" };
			current_node = AddNode(current_node, tmp);
			if (tocken_table[current_tocken_position].id > 500)
			{
				AddNode(current_node, tocken_table[current_tocken_position]);
				current_tocken_position++;
				current_stage = ParseStages::Return;
			}
			else
			{
				make_error(current_tocken_position, "<unsigned-integer>");
				is_end = true;
			}
			break;
		case ParseStages::Return:
			current_stage = next_stages.top();
			current_node = return_nodes.top();
			next_stages.pop();
			return_nodes.pop();
			break;

		case ParseStages::End:
			if (tocken_table[current_tocken_position].id != -1)
			{
				make_error(current_tocken_position, "end of file");
			}
			is_end = true;
			break;
		}
	}
}

void Parser::Print(ofstream& os)
{
	os << tree;
	os << error;
}

void Parser::make_error(int pos, string expected)
{
	error = "Parser: Error(line " + to_string(tocken_table[pos].line) +
		", column " + to_string(tocken_table[pos].column) +
		"):";
	if (tocken_table[pos].id != -1)
	{
		error += "Found '" + tocken_table[pos].value +
			"' Expected " + expected;
	}
	else
	{
		error += "File end"
			" Expected " + expected;
	}

}