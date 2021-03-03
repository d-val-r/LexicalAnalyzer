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
		if (a < 65 || a > 122)
		{
			is_number = true;	
		}

		return is_number;
	}

	bool hasAlpha(int a)
	{
		bool has_alpha = false;
		if (a > 65 && a < 122)
			has_alpha = true;
		return has_alpha;
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
				has_alpha = hasAlpha(line[i]);
				i++;
			}

		if (!has_alpha)	
			identifier = "invalid variable name";
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
	void scanFile(istream& infile, ostream& outfile)
	{
		string line;
		infile >> line;
		bool error = false;

		while (!infile.eof() && !error)
		{
			
			if(line == "begin")
			{
				outfile << tokenmap["t_begin"] << " : " << line << endl;
			}
			else if(line == "end")
			{
				outfile << tokenmap["t_end"] << " : " << line << endl;
			}else if(line == "else")
			{
				outfile << tokenmap["t_else"] << " : " << line << endl;
			}else if(line == "if")
			{
				outfile << tokenmap["t_if"] << " : " << line << endl;
			}else if(line == "input")
			{
				outfile << tokenmap["t_input"] << " : " << line << endl;
			}else if(line == "integer")
			{
				outfile << tokenmap["t_integer"] << " : " << line << endl;
				infile >> line;
				line = identifier_handler(line, infile);	
				if (line == "invalid variable name")
					error = true;	
				 
				outfile << "t_id" << " : " << line << endl;
			}else if(line == "string")
				// is string a keyword for variable declaration or
				// is it meant to identiy string literals?
			{
				outfile << tokenmap["t_string"] << " : " << line << endl;
				infile >> line;
				line = identifier_handler(line, infile);	
				if (line == "invalid variable name")
					error = true;	
				outfile << "t_id" << " : " << line << endl; 
			}else if(line == "loop")
			{
				outfile << tokenmap["t_loop"] << " : " << line << endl;
			}else if(line == "main()")
			{
				outfile << tokenmap["t_main"] << " : " << "main" << endl;
				outfile << tokenmap["s_lparen"] << " : " << "(" << endl;
				outfile << tokenmap["s_rparen"] << " : " << ")" << endl;
			}else if(line == "main")
			{
				outfile << tokenmap["t_main"] << " : " << line << endl;
			}else if(line == "output")
			{
				outfile << tokenmap["t_output"] << " : " << line << endl;
			}else if(line == "then")
			{
				outfile << tokenmap["t_then"] << " : " << line << endl;
			}else if(line == "var")
			{
				outfile << tokenmap["t_var"] << " : " << line << endl;
			}else if(line == "while")
			{
				outfile << tokenmap["t_while"] << " : " << line << endl;
			}else if (line == tokenmap["s_assign"])
			{
				outfile << tokenmap["s_assign"] << " : " << line << endl;
			}else if (line == tokenmap["s_comma"])
			{
				outfile << tokenmap["s_coma"] << " : " << line << endl;
			}else if (line == tokenmap["s_lparen"])
			{
				outfile << tokenmap["s_lparen"] << " : " << line << endl;
			}else if (line == tokenmap["s_rparen"])
			{
				outfile << tokenmap["s_rparen"] << " : " << line << endl;
			}else if (line == tokenmap["s_semi"])
			{
				outfile << tokenmap["s_semi"] << " : " << line << endl;
			}else if (line == tokenmap["s_lt"])
			{
				outfile << tokenmap["s_lt"] << " : " << line << endl;
			}else if (line == tokenmap["s_le"])
			{
				outfile << tokenmap["s_le"] << " : " << line << endl;
			}else if (line == tokenmap["s_gt"])
			{
				outfile << tokenmap["s_gt"] << " : " << line << endl;
			}else if (line == tokenmap["s_ge"])
			{
				outfile << tokenmap["s_ge"] << " : " << line << endl;
			}else if (line == tokenmap["s_eq"])
			{
				outfile << tokenmap["s_eq"] << " : " << line << endl;
			}else if (line == tokenmap["s_ne"])
			{
				outfile << tokenmap["s_ne"] << " : " << line << endl;
			}else if (line == tokenmap["s_plus"])
			{
				outfile << tokenmap["s_plus"] << " : " << line << endl;
			}else if (line == tokenmap["s_minus"])
			{
				outfile << tokenmap["s_minus"] << " : " << line << endl;
			}else if (line == tokenmap["s_mult"])
			{
				outfile << tokenmap["s_mult"] << " : " << line << endl;
			}else if (line == tokenmap["s_div"])
			{
				outfile << tokenmap["s_div"] << " : " << line << endl;
			}else if (line == tokenmap["s_mod"])
			{
				outfile << tokenmap["s_mod"] << " : " << line << endl;
			}else if (line == tokenmap["s_and"])
			{
				outfile << tokenmap["s_and"] << " : " << line << endl;
			}else if (line == tokenmap["s_or"])
			{
				outfile << tokenmap["s_or"] << " : " << line << endl;
			}else if (line == tokenmap["s_not"])
			{
				outfile << tokenmap["s_not"] << " : " << line << endl;
			} // include an else portion that will parse a line
			  // like, say, x5=;, because otherwise, it will read
			  // it as one line and it won't match anyhthing above


			infile >> line;
		}	
		//if (line[line.length()-1] != ';')
		// do we need to check if the program ends with a semicolon?
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
