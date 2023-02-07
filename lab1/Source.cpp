#include "lexer.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** args)
{
	fill_symbol_categories();
	for (int i = 0; i < argc; i++)
	{
		string path(args[i]);
		Lexer l;
		l.Parse(path + "/input.sig");
		ofstream of(path + "/generated.txt");
		l.Print(of);
		l.ShowErrors(of);
		of.close();
	}
}