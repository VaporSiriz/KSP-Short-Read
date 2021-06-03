#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::max;
using std::min;
using std::map;
using std::pair;
using std::make_pair;
using std::cout;
using std::find;
using std::sort;

class BWT
{
private:
	string bwt;									// bwt string
	vector<int> bwt_rank;						// bwt rank vector. rank in order of bwt. used by bwt_front[make_pair(bwt[i], bwt_rank[i])]
	map< pair<char, int>, int> bwt_front;		// bwt front map. ((one of {'A','C','G','T'}, rank), index within bwt)
	map<char, int> base_max_count;				// base max count ex) 'A':3, 'C':5, 'G':5, 'T':3
	vector<char> base_keys;						// base keys
public:
	BWT(string s);
	vector<int> getsfx(string s);
	int search(string s);
};

vector<int> getsfx(string s);