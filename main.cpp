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

	


	bool search_map_for(string target)
	{
		map<string, string>::iterator itr = tokenmap.begin();
		bool found = false;
		while (!found && itr != tokenmap.end())
		{
			if (itr->second == target)
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
		string key;
		string value;		
		
		infile >> key >> value;
		while (!infile.eof())
		{
			tokenmap[key] = value;
			infile >> key >> value;
		}	

	}
	
	
	void scanFile(istream& infile, ostream& outfile)
	{	
				
		bool error = false;
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
					while (i <= line.length() && line[line.length()-1] != '"' && !infile.eof()) 
					{
						getline(infile, line);
						parsed += ' ';
						i = 0;
						parsed += getString(line, i);
					}

					if (infile.eof() && line[line.length()-1] != '"')
					{	error = true;
						outfile << "Error: unclosed string";
					} else
						outfile << "t_string : " << parsed << endl; 
					
				} else if (isAlpha(line[i]))
				{
					while (i < line.length() && !isSymbol(line[i]) && !isWhiteSpace(line[i]) && line[i] != '"')
					{
						parsed += line[i]; 
						i++;
					}
					
					if (isSymbol(line[i]) && !search_map_for(line[i]))
					{
						outfile << "invalid identifier" << endl;
					} else if(search_map_for(parsed))
					{
						outfile << tokenmap[parsed] << " : " << parsed << endl;
					}
					else
					{
						outfile << "t_id : " << parsed << endl;
						parsed = "";
					}
				} else if(isSymbol(line[i]))
				{
					if (!(i > start_quote && i < end_quote))
					{
						if (!(search_map_for(line[i])))
							outfile << "Invalid symbol";
					}
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
