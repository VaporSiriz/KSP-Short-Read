#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <fstream>
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::set;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

string readFile(string file_name);
int get_rand(int low, int high);
void write_bwt(string fileName, vector<int> sa, string s);
vector<string> getShortReads(string fileName);
void makeShortReads(string my_file, string short_reads_file, int k, int n);
