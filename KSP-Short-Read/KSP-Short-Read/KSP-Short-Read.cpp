#include "bwt_module.h"
#include "Common.h"
#include <time.h>
using std::cin;
using std::cout;
using std::ios;
using std::to_string;

void make_bwt(string read_file_name, string write_file_name)
{
    string s = readFile(read_file_name);
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
    // start = clock();
    // make_bwt("test.txt", "test_bwt.txt");
    //cout << (clock() - (double)start) << '\n';
    //start = clock();
    //make_bwt("covid-19.txt", "covid-19_bwt.txt");
    //cout << (clock() - (double)start) << '\n';
    //start = clock();
    //make_bwt("myGenome.txt", "myGenome_bwt.txt");
    //cout << (clock() - (double)start) << '\n';

    //makeShortReads("myGenome.txt", "myGenome-short-reads.txt", 40, 30000);
    //makeShortReads("covid-19.txt", "covid-19-short-reads.txt", 15, 2000);
    clock_t start;
    start = clock();
    string s = readFile("myGenome.txt") + "$";
    //string s = readFile("covid-19.txt")+"$";
    BWT bwt = BWT();
    bwt.setData(s);
    bwt.setShortReads(getShortReads("myGenome-short-reads.txt"));
    //bwt.setShortReads(getShortReads("covid-19-short-reads.txt"));
    string restore = bwt.restore();
    int same = 0;
    for (int i = 0;i < s.length()-1;i++) {
        if (s[i] == restore[i]) same++;
    }
    cout << (double)same << "/" << (double)s.length() << '\n';
    cout << (clock() - (double)start) << '\n';

    //denovo(getSortReads("covid-19-short-reads.txt"));
    return 0;
}

