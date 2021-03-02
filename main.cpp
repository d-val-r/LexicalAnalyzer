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

	string scanFile(istream& infile)
	{
		string message = "";
		string buffer;
		int start_quote = -1;
	        int end_quote = -1;	
		int i = 0;
		while (!infile.eof())
		{
			getline(infile, buffer);
			while (buffer[i] != ';' && i < buffer.length())	
			{
				if (buffer[i] == '"' && start_quote == -1)
					start_quote = i;
				else if (buffer[i] == '"' && start_quote != -1)
					end_quote = i;
				message += buffer[i];
				i++;
			}
			
			// the thing causing the program to not read a semicolon inside
			// quotes improperly is most likely here
			if (start_quote != -1 && end_quote == -1 || i >= buffer.length())
				continue;
			else
			{
				start_quote = end_quote = -1;	
				i = 0;
			}
		}		
		cout << start_quote << endl;
		cout << end_quote << endl;
		cout << i << endl;
		return message;
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

	string x = l.scanFile(source);
	cout << x << endl;

	// ARE RANDOM STRINGS CONSIDERED VALID SYNTAX?
	// ex:
	// 	main()
	// 	"hello"


	return 0;
}
