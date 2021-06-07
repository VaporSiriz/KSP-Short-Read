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
using std::max;
using std::min;

string readFile(string file_name);
int get_rand(int low, int high);
char getOppositeBase(char base);
vector<int> getsfx(string s);
void write_bwt(string fileName, vector<int> sa, string s);
void make_bwt(string read_file_name, string write_file_name);
vector<string> getShortReads(string fileName);
vector<int> getSNPLocations(string refFile, int snp_distance);
void makeMyGenome(string ref_file, string my_file, vector<int> SNPS);
void makeShortReads(string my_file, string short_reads_file, int k, int n);
