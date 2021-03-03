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

	bool isNumber(char a)
	{
		bool is_number = false;
		switch (a)
		{
			case 48:
				is_number = true;
				break;
			case 49:
				is_number = true;
				break;
			case 50:
				is_number = true;
				break;
			case 51:
				is_number = true;
				break;
			case 52:
				is_number = true;
				break;
			case 53:
				is_number = true;
				break;
			case 54:
				is_number = true;
				break;
			case 55:
				is_number = true;
				break;
			case 56:
				is_number = true;
				break;
			case 57:
				is_number = true;
				break;

		}

		return is_number;
	}


	string identifier_handler(string line, istream& infile)
	{
		string identifier = "";
		int i;

		if (isNumber(line[0]))
		{
			cout << "invalid variable name" << endl;
		} else
		{
			i = 0;
			while (i < line.length() && line[i] != '=' && line[i] != ';')
			{
				identifier += line[i];
				i++;
			}
			cout << "t_id" << " : " << identifier << endl;
			
			if (i >= line.length())
			{
				cout << "recursive call" << endl;
				infile >> line;
				identifier += identifier_handler(line, infile);
			}	
		}
		

		return identifier;
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
	// do not forget to update statements to output to a file,
	// not to console
	void scanFile(istream& infile)
	{
		string line;
		infile >> line;
		bool error = false;

		while (!infile.eof() && !error)
		{
			
			if(line == "begin")
			{
				cout << tokenmap["t_begin"] << " : " << line << endl;
			}
			else if(line == "end")
			{
				cout << tokenmap["t_end"] << " : " << line << endl;
			}else if(line == "else")
			{
				cout << tokenmap["t_else"] << " : " << line << endl;
			}else if(line == "if")
			{
				cout << tokenmap["t_if"] << " : " << line << endl;
			}else if(line == "input")
			{
				cout << tokenmap["t_input"] << " : " << line << endl;
			}else if(line == "integer")
			{
				cout << tokenmap["t_integer"] << " : " << line << endl;
				infile >> line;
				identifier_handler(line, infile);	
				
			}else if(line == "string")
			{
				cout << tokenmap["t_string"] << " : " << line << endl;
				if (sizeof(line[0] != 1))
						cout << "invalid variable name" << endl;
				else if(line[line.length() - 1] != ';')
				{
					identifier_handler(line, infile);	
				}


			}else if(line == "loop")
			{
				cout << tokenmap["t_loop"] << " : " << line << endl;
			}else if(line == "main()")
			{
				cout << tokenmap["t_main"] << " : " << "main" << endl;
				cout << tokenmap["s_lparen"] << " : " << "(" << endl;
				cout << tokenmap["s_rparen"] << " : " << ")" << endl;
			}else if(line == "main")
			{
				cout << tokenmap["t_main"] << " : " << line << endl;
			}else if(line == "output")
			{
				cout << tokenmap["t_output"] << " : " << line << endl;
			}else if(line == "then")
			{
				cout << tokenmap["t_then"] << " : " << line << endl;
			}else if(line == "var")
			{
				cout << tokenmap["t_var"] << " : " << line << endl;
			}else if(line == "while")
			{
				cout << tokenmap["t_while"] << " : " << line << endl;
			}else if (line == tokenmap["s_assign"])
			{
				cout << tokenmap["s_assign"] << " : " << line << endl;
			}else if (line == tokenmap["s_comma"])
			{
				cout << tokenmap["s_coma"] << " : " << line << endl;
			}else if (line == tokenmap["s_lparen"])
			{
				cout << tokenmap["s_lparen"] << " : " << line << endl;
			}else if (line == tokenmap["s_rparen"])
			{
				cout << tokenmap["s_rparen"] << " : " << line << endl;
			}else if (line == tokenmap["s_semi"])
			{
				cout << tokenmap["s_semi"] << " : " << line << endl;
			}else if (line == tokenmap["s_lt"])
			{
				cout << tokenmap["s_lt"] << " : " << line << endl;
			}else if (line == tokenmap["s_le"])
			{
				cout << tokenmap["s_le"] << " : " << line << endl;
			}else if (line == tokenmap["s_gt"])
			{
				cout << tokenmap["s_gt"] << " : " << line << endl;
			}else if (line == tokenmap["s_ge"])
			{
				cout << tokenmap["s_ge"] << " : " << line << endl;
			}else if (line == tokenmap["s_eq"])
			{
				cout << tokenmap["s_eq"] << " : " << line << endl;
			}else if (line == tokenmap["s_ne"])
			{
				cout << tokenmap["s_ne"] << " : " << line << endl;
			}else if (line == tokenmap["s_plus"])
			{
				cout << tokenmap["s_plus"] << " : " << line << endl;
			}else if (line == tokenmap["s_minus"])
			{
				cout << tokenmap["s_minus"] << " : " << line << endl;
			}else if (line == tokenmap["s_mult"])
			{
				cout << tokenmap["s_mult"] << " : " << line << endl;
			}else if (line == tokenmap["s_div"])
			{
				cout << tokenmap["s_div"] << " : " << line << endl;
			}else if (line == tokenmap["s_mod"])
			{
				cout << tokenmap["s_mod"] << " : " << line << endl;
			}else if (line == tokenmap["s_and"])
			{
				cout << tokenmap["s_and"] << " : " << line << endl;
			}else if (line == tokenmap["s_or"])
			{
				cout << tokenmap["s_or"] << " : " << line << endl;
			}else if (line == tokenmap["s_not"])
			{
				cout << tokenmap["s_not"] << " : " << line << endl;
			}


			infile >> line;
		}	
		//if (line[line.length()-1] != ';')
		// do we need to check if the program ends with a semicolon?
		}
				
	};
	



int main()
{
	ifstream file("tokenlexemedata.txt");

	string source_code;
	cout << "Please enter a source code file name, with extension: ";
	cin >> source_code;

	ifstream source(source_code);
	LexAnalyzer l(file);

	l.scanFile(source);


	// ARE RANDOM STRINGS CONSIDERED VALID SYNTAX?
	// ex:
	// 	main()
	// 	"hello"


	return 0;
}
