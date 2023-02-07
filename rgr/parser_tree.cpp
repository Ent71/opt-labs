#include "parser_tree.h"

using namespace std;

ParserTreeNode* AddNode(ParserTreeNode* tree, const TockenTableField& value)
{
	ParserTreeNode* new_node = new ParserTreeNode;
	new_node->value = value;
	if (tree == nullptr)
	{
		tree = new_node;
	}
	else
	{
		tree->children.push_back(new_node);
	}
	return new_node;
}

void DeleteTree(ParserTreeNode* tree)
{
	if (tree != nullptr)
	{
		DeleteSubTree(tree);
	}
}


void DeleteSubTree(ParserTreeNode* tree)
{

	for (ParserTreeNode* elem : tree->children)
	{
		DeleteSubTree(elem);
	}
	delete tree;
	
}

ofstream& operator<<(ofstream& os, ParserTreeNode* tree)
{
	if (tree != nullptr)
	{
		PrintTree(os, tree, 0);
	}
	else
	{
		os << "Empty tree!\n";
	}
	return os;
}

void PrintTree(ofstream& os, ParserTreeNode* tree, int depth_count)
{
	for (int i = 0; i < depth_count * 2; i++)
	{
		os << ".";
	}
	if (tree->value.id == -1)
	{
		os << tree->value.value << "\n";
	}
	else
	{
		os << tree->value.id << " " << tree->value.value << "\n";
	}
	for (ParserTreeNode* elem : tree->children)
	{
		PrintTree(os, elem, depth_count + 1);
	}
}