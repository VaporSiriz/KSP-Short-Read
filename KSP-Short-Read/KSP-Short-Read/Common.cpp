#include "Common.h"
#include <fstream>

string readFile(string file_name)
{
    ifstream readFile(file_name);
    string output;
    if (readFile.is_open()) {
        readFile >> output;
    }
    return output;
}

void write_bwt(string fileName, vector<int> sa, string s)
{
    ofstream writeFile(fileName);
    string bwt = "";
    for (int i = 0;i < sa.size();i++) {
        int index = sa[i] == 0 ? sa.size() - 1 : sa[i] - 1;
        bwt += s[index];
    }
    writeFile.write(bwt.c_str(), bwt.length());
}