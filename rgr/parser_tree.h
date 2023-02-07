#pragma once
#include "tables.h"
#include <vector>

using namespace std;

struct ParserTreeNode {
	TockenTableField value;
	vector<ParserTreeNode*> children;
};

ParserTreeNode* AddNode(ParserTreeNode* tree, const TockenTableField& value);
void DeleteTree(ParserTreeNode* tree);
ofstream& operator<<(ofstream& os, ParserTreeNode* tree);

void DeleteSubTree(ParserTreeNode* tree);
void PrintTree(ofstream& os, ParserTreeNode* tree, int depth_count);