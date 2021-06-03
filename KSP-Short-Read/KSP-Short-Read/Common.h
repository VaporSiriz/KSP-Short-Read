#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

string readFile(string file_name);
void write_bwt(string fileName, vector<int> sa, string s);
