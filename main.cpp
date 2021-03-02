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
	
};


int main()
{
	ifstream file("tokenlexemedata.txt");
	l.read();
	

	return 0;
}
