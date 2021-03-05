/*
Assignment 2 for CSCE 306, Spring 2021 (Dr. St. Clair)
Written by David Rudenya
*/

# include <fstream>
# include <iostream>
# include <vector>
# include <map>
using namespace std;

class LexAnalyzer
{
private:
	vector<string> lexemes;
	vector<string> tokens;
	map<string, string> tokenmap;

	bool isNumber(int a)
	{
		bool is_number = false;

		if (a > 47 && a < 58)
			is_number = true;	

		return is_number;
	}

	bool isAlpha(char a)
	{
		a = (int) a;
		bool is_alpha = false;

		if (a > 64 && a < 91 || a > 96 && a < 123)
			is_alpha = true;

		return is_alpha;
	}


		

	bool isSymbol(char a)
	{
		bool is_symbol = false;
		if (!isNumber(a) && !isAlpha(a) && !isWhiteSpace(a) && a != '"')
			is_symbol = true;

		return is_symbol;
	}

	

	bool isWhiteSpace(char a)
	{	
		a = (int) a;
		bool is_white_space = false;

		// ASCII codes for tab and space whitespace chars	
		// is it better to make variables to make it obvious, or
		// to just use the numbers in the comparison to save memory?
		int TAB = 9;
		int SPACE = 32;

		if (a == TAB || a == SPACE)
			is_white_space = true;
		
		return is_white_space;
	}

	
	int findLexeme(string target)
	{
		int i = 0;
		int index = -1;
		bool found = false;
		while (i < lexemes.size() && !found)
		{
			if (lexemes[i] == target)
				found = true;
		}

		return index;
	}
		


	bool search_map_for(string target)
	{
		map<string, string>::iterator itr = tokenmap.begin();
		bool found = false;
		while (!found && itr != tokenmap.end())
		{
			if (itr->first == target)
				found = true;
			itr++;
		}	

		return found;
	}

	bool search_map_for(char a)
	{
		string x = " ";
		x[0] = a;
		return search_map_for(x);
	}

	string getString(string line, int& i)
	{
		string parsed = "";
		while (line[i] != '"' && i < line.length())
			{
				parsed += line[i];
				i++;
			}
		
		return parsed;

	}
	
public:
	LexAnalyzer(istream& infile)
	{
		string lexeme;
		string token;		
		
		infile >> token >> lexeme;
		while (!infile.eof())
		{
			tokens.push_back(token);
			lexemes.push_back(lexeme);
			tokenmap[lexeme] = token;
			infile >> token >> lexeme;
		}	

	}
	
	
	void scanFile(istream& infile, ostream& outfile)
	{	
		string converter = " ";	
		bool error = false;
		bool finished_parsing = false;
		int i = 0;
		string parsed = "";
		
		
		
		string line;
		getline(infile, line);

		int start_quote = -1;
		int end_quote = -1;		
		
		while (!infile.eof())
		{
			while (i < line.length() && !error)
			{
				if (line[i] == '"')
				{
					i++;
					parsed += getString(line, i);	
					while (i >= line.length() && line[i] != '"' && !infile.eof()) 
					{
						getline(infile, line);
						parsed += ' ';
						i = 0;
						parsed += getString(line, i);
					}

					if (infile.eof() && line[line.length()-1] != '"')
					{	outfile << "Error: unclosed string";
						error = true;
					} else
					{
						outfile << "t_string : " << parsed << endl; 
						parsed = "";
					}
					
				} else if (isAlpha(line[i]))
				{
					while (i < line.length() && !isSymbol(line[i]) && !isWhiteSpace(line[i]) && line[i] != '"')
					{
						parsed += line[i]; 
						i++;
					}
					
					if (isSymbol(line[i]) && line[i] != '\0')
					{
						if (!search_map_for(line[i]))
						{
							outfile << "Error: invalid identifier " << parsed << line[i] << endl;
							error = true;
						}
						else
						{
							outfile << tokenmap[parsed] << " : " << parsed << endl;
							converter[0] = line[i];
							outfile << tokenmap[converter] << " : " << converter << endl; 
							converter = " ";
						}
					} else if(search_map_for(parsed))
					{
						outfile << tokenmap[parsed] << " : " << parsed << endl;
					}
					else
					{
						outfile << "t_id : " << parsed << endl;
					}
					parsed = "";
				} else if(isSymbol(line[i]))
				{
					if (search_map_for(line[i]))
					{
						converter[0] = line[i];
						outfile << tokenmap[converter] << " : " << converter << endl;
						converter = " ";
					}
						
							
				} else if(isNumber(line[i]))
				{
					while (i < line.length() && !finished_parsing)
					{
						parsed += line[i];
						if (isSymbol(line[i]))
						{
							if (search_map_for(line[i]))
							{
								outfile << "t_int : " << parsed << endl;	
							} else
							{
								outfile << "Error: Invalid Symbol " << line[i] << endl;
								error = true;
							}

							finished_parsing = true;
						} else if (isAlpha(line[i]))
						{
							outfile << "Error: Invalid Identifier " << parsed << endl;
							error = true;
							finished_parsing = true;

						} else if (isWhiteSpace(line[i]) || line[i] == '\0')
						{
							outfile << "t_id : " << parsed << endl;	
							finished_parsing = true;
						}

						i++;
					}

					finished_parsing = false;
				}	
				i++;
			}

			i = 0;
			getline(infile, line);
		}
	}
				
};
	



int main()
{
	ifstream file("tokenlexemedata.txt");

	string source_name;
	cout << "Please enter a source code file name, with extension: ";
	cin >> source_name;

	ifstream source_code(source_name);
	string out_name;
	cout << "Please enter an output file name, with extension: ";
	cin >> out_name;

	ofstream output(out_name);


	LexAnalyzer l(file);
	l.scanFile(source_code, output);


	// ARE RANDOM STRINGS CONSIDERED VALID SYNTAX?
	// ex:
	// 	main()
	// 	"hello"

	

	return 0;
}
