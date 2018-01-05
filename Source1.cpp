#include <iostream>
#include <cctype>
#include <sstream>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

void read_dictionary(ifstream &in_file, set<string> &dictionary, int &bad){

	string word;

	bool bad_word = false;

	const size_t LIMIT = 3;

	if (in_file.fail())
	{
		cout << "\nCouldn't load file\n\n" << endl;

		bad = 0;
	}
	else{
		while (in_file >> word)
		{
			if (word.size() > LIMIT){

				for (size_t spot = 0; spot < word.size(); spot++){

					if (!isalpha(word[spot])){

						spot = word.size();

						bad_word = true;
					}
					else{
						if (isupper(word[spot])){

							word[spot] = tolower(word[spot]);
						}
					}
				}
				if (bad_word == false){

					dictionary.insert(word);
				}
				bad_word = false;
			}
		}
		in_file.close();
	}
}
//---------------------------------------------------------------------------------------

void read_document(ifstream &in_file, vector<string> &document, int &bad){

	string word;

	string line;

	if (in_file.fail())
	{
		cout << "\nCouldn't load file\n\n" << endl;

		bad = 0;
	}
	else{
		while (in_file >> word)
		{
			for (size_t spot = 0; spot < word.size(); spot++){

				if (!isalpha(word[spot])){

					spot = word.size();
				}
				else{
					if (isupper(word[spot])){

						word[spot] = tolower(word[spot]);
					}
				}
			}
			document.push_back(word);
		}
		in_file.close();
	}
}
//---------------------------------------------------------------------------------------

void find2(string& c, size_t& a, size_t& b, vector<vector<string>>& table, set<string>& dictionary, vector<vector<bool>>& used, set <string>& words, set <string> ::iterator& d){
	if (used[a][b] == true ){
		d=dictionary.end();
	}
	else{
		c = c + table[a][b];
		if (dictionary.count(c) == 1){
			words.insert(c);
		}
		d = dictionary.lower_bound(c);
	}
}
//---------------------------------------------------------------------------------------
	
string find(string c, size_t a, size_t b, int f, vector<vector<string>>& table, set<string>& dictionary, size_t size_v, vector<vector<bool>>& used, set <string>& words){
	set <string> ::iterator d;

	if ((a < size_v && b < size_v) && (a >= 0 && b >= 0)){

		find2(c, a, b, table, dictionary, used, words, d);

		if (d != dictionary.end()){

			string p = *d;
			if (p.length() >= c.length()){

				if (p[f] == c[f]){
					f++;
					used[a][b] = true;
					find(c, a - 1, b - 1, f, table, dictionary, size_v, used, words);
					find(c, a - 1, b, f, table, dictionary, size_v, used, words);
					find(c, a - 1, b + 1, f, table, dictionary, size_v, used, words);
					find(c, a, b + 1, f, table, dictionary, size_v, used, words);
					find(c, a, b - 1, f, table, dictionary, size_v, used, words);
					find(c, a + 1, b - 1, f, table, dictionary, size_v, used, words);
					find(c, a + 1, b, f, table, dictionary, size_v, used, words);
					find(c, a + 1, b + 1, f, table, dictionary, size_v, used, words);
					used[a][b] = false;
					return "";
				}
				else{
					c.pop_back();
					return "";
				}
			}
			else{

				return "";
			}
		}
		else{
			return "";
		}
	}
	else{
		return "";
	}
}
//---------------------------------------------------------------------------------------

void report(ofstream& out_file, vector<string>& document,vector<vector<string>>& table, set<string>& dictionary, size_t& v_size){
	for (size_t spot = 0; spot < v_size; spot++){

		for (size_t spot1 = 0; spot1 < v_size; spot1++){

			table[spot][spot1] = document[0];

			out_file << table[spot][spot1] << " ";

			document.erase(document.begin());
		}
		out_file << endl;
	}
	out_file << endl;
	set <string> words;
	vector<vector<bool>> used(v_size, vector<bool>(v_size));	
	for (size_t spot = 0; spot < v_size; spot++){
		for (size_t spot1 = 0; spot1 < v_size; spot1++){
			used[spot][spot1] = false;	
		}	
	}
	for (size_t spot = 0; spot < v_size; spot++){
		
		for (size_t spot1 = 0; spot1 < v_size; spot1++){
			string c;
			
			find(c,spot, spot1, 0, table, dictionary, v_size, used, words);		
		}
	}	
	for (auto c : words){

		out_file << c << endl;
	}
}
//---------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

	set<string> dictionary;

	vector<string> document;

	ifstream in_file;

	ofstream out_file;

	int bad = 1;

	in_file.open(argv[1]);

	read_dictionary(in_file, dictionary, bad);

	if (bad == 0){
		return 0;
	}
	in_file.open(argv[2]);

	read_document(in_file, document, bad);

	if (bad == 0){
		return 0;
	}
	size_t v_size = document.size();

	v_size = sqrt(v_size);

	vector<vector<string>> table(v_size, vector<string>(v_size));
	out_file.open(argv[3]);
	report(out_file, document, table, dictionary, v_size);
	out_file.close();
	return 0;
}

//---------------------------------------------------------------------------------------

