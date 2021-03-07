/*
Assignment 2 for CSCE 306, Spring 2021 (Dr. St. Clair)
Written by David Rudenya
*/

# include <fstream>
# include <iostream>
# include <vector>
# include <map>
using namespace std;

class LexAnalyzer // error messages JUST name the type of error
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
		
		while (!infile.eof())
		{
			while (i < line.length() && !error)
			{

				if (line[i] == '"')
				{
					i++;
					parsed += getString(line, i);	
					while (i >= line.length() && !infile.eof()) 
					{
						getline(infile, line);
						parsed += ' ';
						i = 0;
						parsed += getString(line, i);
					}

					if (infile.eof() && line[i] != '"')
					{	outfile << "Error: Unclosed String" << endl;
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
							outfile << "Error: Invalid Symbol" << endl;
							error = true;
						} 
						else if (search_map_for(parsed))
						{
							outfile << tokenmap[parsed] << " : " << parsed << endl;
							//converter[0] = line[i];
							//outfile << tokenmap[converter] << " : " << converter << endl; 
							//converter = " ";
						} else
						{
							outfile << "t_id : " << parsed << endl;
							//converter[0] = line[i];
							//outfile << tokenmap[converter] << " : " << converter << endl; 
							//converter = " ";

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
					i--;
				} else if(isSymbol(line[i])) // add conditions to check for compound symbols, <=, >=, ==, etc
				{

					if (line[i] == '&' && i+1 < line.length() && line[i+1] == '&')
					{
						outfile << tokenmap["&&"] << ": &&" << endl;
						i++;	
					} else if (line[i] == '|' && i+1 < line.length() && line[i+1] == '|')
					{

						outfile << tokenmap["||"] << ": ||" << endl;
						i++;	
					} else if (search_map_for(line[i]))
					{
						if (i+1 < line.length() && isSymbol(line[i+1]))
						{
							if (line[i+1] == '=')
							{
								if (line[i] == '<')
								{
									outfile << tokenmap["<="] << ": <=" << endl;
									i++;
								}
								else if (line[i] == '>')
								{
									outfile << tokenmap[">="] << ": >=" << endl;
									i++;
								}
								else if (line[i] == '=')
								{
									outfile << tokenmap["=="] << ": ==" << endl;
									i++;
								}
								else if (line[i] == '!')
								{	
									outfile << tokenmap["!="] << ": !=" << endl;
									i++;
								}
							} else
							{
								converter[0] = line[i];
								outfile << tokenmap[converter] << " : " << converter << endl;
								converter = " ";
							}
							
						} else
						{
							converter[0] = line[i];
							outfile << tokenmap[converter] << " : " << converter << endl;
							converter = " ";
						}
						
						
						
					} else
					{
						outfile << "Error: Invalid Symbol" << endl;
						error = true;
					}
						
							
				} else if(isNumber(line[i]))
				{
					parsed += line[i];
					i++;
					while (i < line.length() && !finished_parsing && !isWhiteSpace(line[i]))
					{
						if (isSymbol(line[i]))
						{
							if (search_map_for(line[i]))
							{
								outfile << "t_int : " << parsed << endl;	
								finished_parsing = true;
							} else
							{
								outfile << "Error: Invalid Symbol" << endl;
								error = true;
							}

							finished_parsing = true;
						} else if (isAlpha(line[i]))
						{
							outfile << "Error: Invalid Identifier" << endl;
							error = true;
							finished_parsing = true;

						} else
							parsed += line[i];
						i++;
					}
					
					// we exited the loop due to reading until the end of the line,
					// or we ran into whitespace along the way. Either way, the number
					// didn't break any rules
					if (!finished_parsing)
					{
						outfile << "t_int : " << parsed << endl;
					}
					
					parsed = "";
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
	ifstream file("tokenlexemedata.txt"); // do NOT hard code this

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

	file.close();
	source_code.close();
	output.close();	

	return 0;
}
