#include "lexer.h"
#include "parser.h"
#include "parser_tree.h"
#include "code_generator.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** args)
{
	fill_symbol_categories();
	for (int i = 1; i < argc; i++)
	{
		bool is_not_error;
		string path(args[i]);
		Lexer l;
		is_not_error = l.Parse(path + "/input.sig");
		ofstream of(path + "/generated.txt");
		Parser p;
		is_not_error = p.Parse() && is_not_error;
		if (is_not_error)
		{
			CodeGenerator cd;
			cd.analyse(p.getTree(), of);
			of << "\n";
		}
		l.ShowErrors(of);
		of << "\n";
		p.ShowError(of);
		of.close();
	}
}