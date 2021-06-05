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
using std::endl;
using std::find;
using std::sort;

class BWT
{
	/* 구하는 순서.
	* 1. ref을 가지고 SA, bwt를 만듦.
	* 2. bwt를 가지고 front와 rank, keys 등을 생성.
	* 2. short_reads를 받아옴.
	* 
	* */
private:
	string ref;									// ref
	string bwt;									// bwt string
	string front;
	vector<int> bwt_rank;						// bwt rank vector. rank in order of bwt. used by bwt_front[make_pair(bwt[i], bwt_rank[i])]
	map< pair<char, int>, int> bwt_front;		// bwt front map. ((one of {'A','C','G','T'}, rank), index within bwt)
	map<char, int> base_max_count;				// base max count ex) 'A':3, 'C':5, 'G':5, 'T':3
	vector<char> base_keys;						// base keys
	vector<string> short_reads;
	vector<pair<string, pair<int, int>>> indexes;
	vector<int> SA;								// suffix array
	// vector<pair<pair<char, int>, int>> SA;
public:				
	void setData(string ref);
	string makeBWT();
	void setShortReads(vector<string> short_reads);		// set short reads
	vector<int> getsfx(string s);
	pair<int, int> searching(string pattern);	// return pair<start, end>
	string restore();
};

vector<int> getsfx(string s);
void searching(const string& BWT, string Pattern);//BWT, searching Pattern
int C(char Pi, string F);//C[P[i]] : P[i]보다 앞에 있는 문자의 개수
int Rank(int index, char Pi, string BWT);//Rank(i, c) : BWT에서 i위치 이전에 문자 c가 나타나는 횟수
int start(int index, char Pi, string F, string BWT);//C[P[i]] + rank(s - 1, P[i])+1
int end(int index, char Pi, string F, string BWT);//C[P[i]] + rank(e, P[i])
