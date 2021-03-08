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
	// pre: parameter accepts a character argument and auto-casts it into an int
	// post: returns true if the argument is a number; false if not
	{
		bool is_number = false;

		// check the ASCII value of a to see if it's a valid number
		if (a > 47 && a < 58)
			is_number = true;	

		return is_number;
	}


	bool isAlpha(char a)
	// pre: parameter is any character
	// post: returns true if the argument is a letter; false otherwise
	{
		a = (int) a;
		bool is_alpha = false;

		// checks if the argument's ASCII code corresponds to a valid letter
		if (a > 64 && a < 91 || a > 96 && a < 123)
			is_alpha = true;

		return is_alpha;
	}

	bool isWhiteSpace(char a)
	// pre: parameter is any character
	// post: returns true if the character is either a tab or space character
	{	
		a = (int) a;
		bool is_white_space = false;

		// ASCII codes for tab and space whitespace chars	
		int TAB = 9;
		int SPACE = 32;

		if (a == TAB || a == SPACE)
			is_white_space = true;
		
		return is_white_space;
	}

		

	bool isSymbol(char a)
	// pre: parameter is any character
	// post: returns true if the character is not a number, letter, whitespace, or double-quote
	{
		bool is_symbol = false;
		if (!isNumber(a) && !isAlpha(a) && !isWhiteSpace(a) && a != '"')
			is_symbol = true;

		return is_symbol;
	}

		
	bool search_map_for(string target)
	// pre: parameter refers to any string
	// post: returns true if the string is found within the map; false if not
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
	// pre: parameter refers to any character
	// post: returns true if the character is found in the map; false if not
	// desc: overloaded method, converts a character into a string to search
	//       the map for 
	{
		string x = " ";
		x[0] = a;
		return search_map_for(x);
	}

	string getString(string line, int& i)
	// pre: 1st parameter is a line of text, second parameter is a reference to 
	//      the integer that was iterating through the string at the time of calling the function
	// post: returns all characters it finds in the first parameter until
	//       it hits either the end of the text or finds a double quote and
	//       increments the integer, i, that was originally passed in 
	{
		string parsed = "";
		while (line[i] != '"' && i < line.length())
			{
				parsed += line[i];
				i++;
			}
		
		return parsed;

	}


	void write(ostream& outfile, bool error)
	// pre: first parameter refers to an empty output file to write to,
	//      second parameter is a Boolean indicating whether or not
	//      an error was encountered while populating the tokens and lexemes
	//      vectors
	// post: valid token/lexeme pairs and/or errors written to output file
	//       and error output to console if necessary
	{
		for (int i = 0; i < tokens.size(); i++) 
		{
			outfile << tokens[i] << " : " << lexemes[i] << endl; 
		}

		if (error)
			cout << "Error encountered. File processing aborted" << endl;
		else
			cout << "Finished processing file." << endl;
		
	}
	
public:
	LexAnalyzer(istream& infile)
	// pre: parameter refers to open data file consisting of token and lexeme
	//      pairs i.e. a_and and t_begin begin t_int 27. Each pair appears on its
	//      own line of input
	// post: tokenmap has been populated
	{
		string lexeme;
		string token;		
		
		infile >> token >> lexeme;
		while (!infile.eof())
		{
			tokenmap[lexeme] = token;
			infile >> token >> lexeme;
		}	

	}
	
	
	void scanFile(istream& infile, ostream& outfile)
	// pre: first parameter refers to an open text file that contains
	//      source code in the language, second parameter refers to an open
	//      empty output file
	// post: If no error, the token and lexeme pairs for the given input
	//       file have been written to the output file. If there is an error,
	//       the incomplete token/lexeme pairs, as well as an error message
	//       are written to the output file. A success or fail message has printed
	//       to the console
	{	
		
		
		// used to change chars into strings of length 1	
		string converter = " ";	

		// helps control main loop, true if error encountered
		bool error = false;

		// used to parse substrings when a potential int value is found
		bool finished_parsing = false;

		// helps control main loop, used to iterate through lines of text
		int i = 0;

		// used to store a substring when parsing string literals,
		// variable names, numbers, etc.
		string parsed = "";
		
		
		
		string line;
		getline(infile, line);
		
		while (!infile.eof())
		{
			while (i < line.length() && !error)
			{

				if (line[i] == '"')
				{

					// immediately increment i, as line[i] is, at the moment,
					// a double quote, and need not be added to the parsed
					// substring
					i++;


					parsed += getString(line, i);	

					// if the getString method increments i past the length
					// of the line of text, that means it never found the other double quote,
					// and the process needs to be repeated until either the end of the file is
					// reached, or the other double quote is found
					while (i >= line.length() && !infile.eof()) 
					{
						getline(infile, line);

						// reset i to 0, as it will now parse a new line
						i = 0;
						parsed += getString(line, i);
					}

					
					// if we broken out of the loop due to running out
					// of text in the file to parse and the other double quote
					// still wasn't found, output an error
					if (infile.eof() && line[i] != '"')
					{	tokens.push_back("Error: Unclosed String");
						lexemes.push_back("");
						error = true;
					} else
					{
						tokens.push_back("t_str");
						lexemes.push_back(parsed);

						// resets parsed for future use
						parsed = "";
					}
					
				} else if (isAlpha(line[i]))
				{
					while (i < line.length() && !isSymbol(line[i]) && !isWhiteSpace(line[i]) && line[i] != '"')
					{
						parsed += line[i]; 
						i++;
					}
					

					// checks if the above loop ran into a symbol, not including
					// the terminating character
					if (isSymbol(line[i]) && line[i] != '\0')
					{
						if (!search_map_for(line[i]))
						{
							tokens.push_back("Error: Invalid Symbol");
							lexemes.push_back("");
							error = true;
						} 
						// the above statement did not flag the symbol as
						// invalid, so either the string of characters is a keyword
						// or an identifier
						else if (search_map_for(parsed))
						{
							tokens.push_back(tokenmap[parsed]);
							lexemes.push_back(parsed);
						} else
						{
							tokens.push_back("t_id"); 
							lexemes.push_back(parsed);

						}
					} 
					// we didn't find a symbol, so the loop either
					// reached the end of the line or found whitespace
					else if(search_map_for(parsed))
					{
						tokens.push_back(tokenmap[parsed]);
						lexemes.push_back(parsed);
					}
					else
					{
						tokens.push_back("t_id"); 
						lexemes.push_back(parsed);
					}
					parsed = "";


					// decrement i, because if it was a symbol, the next
					// iteration of the main loop has to process it, but will skip it
					// when it increments i on line 413; if whitespace,
					// the loop will just skip it
					i--;
				} else if(isSymbol(line[i])) 
				{
					
					// check for compound operators that MUST have two in a row;
					// will find error if running the search_map_for method on just
					//'&' or '|'
					if (line[i] == '&' && i+1 < line.length() && line[i+1] == '&')
					{
						tokens.push_back(tokenmap["&&"]);
						lexemes.push_back("&&");	

						// increments i to avoid re-processing next '&' symbol on 
						// the next main loop iteration
						i++;	
					} else if (line[i] == '|' && i+1 < line.length() && line[i+1] == '|')
					{
						tokens.push_back(tokenmap["||"]);
					        lexemes.push_back("||");	

						// increments i to avoid re-processing next '|' symbol on 
						// the next main loop iteration
						i++;	
					} else if (search_map_for(line[i]))
					{
						if (i+1 < line.length() && isSymbol(line[i+1]))
						{
							// check for the various *= compound operators,
							// where * is any of the below characters
							// increments i to avoid re-processing the second
							// character of the compound operator on subsequent
							// iterations of the main loop
							if (line[i+1] == '=')
							{
								if (line[i] == '<')
								{
									tokens.push_back(tokenmap["<="]); 
									lexemes.push_back("<=");
									i++;
								}
								else if (line[i] == '>')
								{
									tokens.push_back(tokenmap[">="]);
								        lexemes.push_back(">=");	
									i++;
								}
								else if (line[i] == '=')
								{
									tokens.push_back(tokenmap["=="]); 
									lexemes.push_back("==");
									i++;
								}
								else if (line[i] == '!')
								{	
									tokens.push_back(tokenmap["!="]); 
									lexemes.push_back("!=");
									i++;
								} else
								{
									converter[0] = line[i];
									tokens.push_back(tokenmap[converter]);
								        lexemes.push_back(converter);	
									converter = " ";
								}
							} 
							// no compound operators were found, but the symbol 
							// is valid
							else
							{
								// convert the character to a string of length
								// 1 to index the map with
								converter[0] = line[i];
								tokens.push_back(tokenmap[converter]);
							        lexemes.push_back(converter);	
								converter = " ";
							}
							
						} 
						
						// at the cost of repeating the above else clause,
						// we avoid forcing the loop to check all the above if-statements
						// when it runs into a single valid operand that's 
						// not followed by another symbol 
						else
						{
							converter[0] = line[i];
							tokens.push_back(tokenmap[converter]);
						        lexemes.push_back(converter);	
							converter = " ";
						}
						
						
						
					} 
					// the symbol was not found in tokenmap
					else
					{
						tokens.push_back("Error: Invalid Symbol"); 
						lexemes.push_back("");
						error = true;
					}
						
							
				} else if(isNumber(line[i]))
				{
					parsed += line[i];

					// increment i, as the current number has already 
					// been added to the parsed substring
					i++;
					while (i < line.length() && !finished_parsing && !isWhiteSpace(line[i]))
					{
						if (isSymbol(line[i]))
						{
							if (search_map_for(line[i]))
							{
								tokens.push_back("t_int");
								lexemes.push_back(parsed);
								finished_parsing = true;


								// decrement i to let the next iteration of the main
								// loop handle processing of the symbol currently at line[i]
								// this is done because the symbol could be a compound operator 
								// that needs more complicated processing
								i--;
							} else
							{
								tokens.push_back("Error: Invalid Symbol");
								lexemes.push_back("");
								error = true;
								finished_parsing = true;
							}

							finished_parsing = true;
						} else if (isAlpha(line[i]))
						{
							tokens.push_back("Error: Invalid Identifier"); 
							lexemes.push_back("");
							error = true;
							finished_parsing = true;

						} else
							parsed += line[i];
						

						// only increment i in this while-loop as long
						// as there is a reason to repeat the loop; otherwise,
						// avoid doing so to let the main loop increment and
						// avoid skipping any characters
						if (!finished_parsing)
							i++;
					}
					
					// exited the loop due to reading until the end of the line,
					// or ran into whitespace along the way. Either way, the number
					// didn't break any rules and is a valid integer
					if (!finished_parsing)
					{
						tokens.push_back("t_int"); 
						lexemes.push_back(parsed);
					}
					
					parsed = "";
					finished_parsing = false;
				}	
				i++;
			}

			// reset the iterating variable parsing the line
			// and update the line
			i = 0;
			getline(infile, line);
		}

		write(outfile, error);
	}
				
};
	



int main()
{
	string language;
	cout << "Please enter the name of the file defining the language, with the extension: ";
	cin >> language;
	ifstream language_file(language); 

	string source_name;
	cout << "Please enter a source code file name, with the extension: ";
	cin >> source_name;

	ifstream source_code(source_name);
	string out_name;
	cout << "Please enter an output file name, with extension: ";
	cin >> out_name;
	ofstream output(out_name);

	if (!language_file || !source_code || !output)
	{
		cout << "Error opening file" << endl;
		exit(-1);
	}


	LexAnalyzer l(language_file);
	l.scanFile(source_code, output);

	language_file.close();
	source_code.close();
	output.close();	

	return 0;
}
