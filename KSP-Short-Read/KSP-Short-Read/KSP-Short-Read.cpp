#include "bwt_module.h"
#include "Common.h"
#include <time.h>
using std::cin;
using std::cout;
using std::ios;

void make_bwt(string read_file_name, string write_file_name)
{
    string s = readFile(read_file_name) + "$";
    vector<int> sfx = getsfx(s);
    write_bwt(write_file_name, sfx, s);
}

int main()
{
    ios::sync_with_stdio(false); cin.tie(0);

    /*  measure time of BWT generating
        test.txt : length 7. time ~ 0.001
        covid-19.txt : length 29,904. time ~ 2.0
        myGenome.txt : length 1,000,000. time 25 ~ 27
    */
    //clock_t start;
    //start = clock();
    //make_bwt("test.txt", "test_bwt.txt");
    //cout << (clock() - (double)start) << '\n';
    //start = clock();
    //make_bwt("covid-19.txt", "covid-19_bwt.txt");
    //cout << (clock() - (double)start) << '\n';
    //start = clock();
    //make_bwt("myGenome.txt", "myGenome_bwt.txt");
    //cout << (clock() - (double)start) << '\n';

    string s = readFile("myGenome_bwt.txt");
    //cout << s << endl;
    BWT bwt = BWT(s);
    searching(s, "TGAATAGTCAACTACGATGTTTATGACACTGAGGCTCTCCCACAAGCTGGAGCTGCGTAGGTATGTCTACGCC");
    return 0;
}

