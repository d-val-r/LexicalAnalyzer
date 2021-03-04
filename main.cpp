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
		if (!isNumber(a) && !isAlpha(a))
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

		if (a == TAB && a == SPACE)
			is_white_space = true;
		
		return is_white_space;
	}

	string identifier_handler(string line, istream& infile)
	{
		string identifier = "";
		int i;
		bool has_alpha = false;

		if (isNumber(line[0]))
		{
			identifier = "invalid variable name";
		} else
		{
			i = 0;
			while (i < line.length() && line[i] != '=' && line[i] != ';')
			{
				identifier += line[i];
				has_alpha = isAlpha(line[i]);
				i++;
			}

		if (!has_alpha)	
			identifier = "invalid variable name";
		}
		

		return identifier;
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
					if (start_quote == -1) // find a way to handle one line with multiple strings
							      // if user writes "asdlflhjk" as;dlk ", then what do I do with the
							      // stray double quote?
						start_quote = i;
					else
						end_quote = i;	
				} else if (isAlpha(line[i]))
				{
					while (!isSymbol(line[i]) && i < line.length())
					{
						parsed += line[i]; 
						i++;
					}

					if (isSymbol(line[i]) && !search_map_for(line[i]))
					{
						outfile << "invalid identifier";
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
