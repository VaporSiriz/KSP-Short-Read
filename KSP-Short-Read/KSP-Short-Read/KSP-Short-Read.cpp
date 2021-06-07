#include "bwt_module.h"
#include "Common.h"
#include <time.h>
using std::cin;
using std::cout;
using std::ios;
using std::to_string;

const int SNP_DISTANCE = 100;
const int MISMATCH = 1;

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

    /* SNP 생성 */
    //vector<int> SNPS = getSNPLocations("test.txt", SNP_DISTANCE);
    //vector<int> SNPS = getSNPLocations("covid-19.txt", SNP_DISTANCE);
    vector<int> SNPS = getSNPLocations("GenomeRef.txt", SNP_DISTANCE);

    /* MyGenome 생성 */
    cout << "==== create MyGenome.txt from GenomeRef.txt ====\n";
    //makeMyGenome("test.txt", "myGenome.txt", SNPS);
    //makeMyGenome("covid-19.txt", "myGenome.txt", SNPS);
    makeMyGenome("GenomeRef.txt", "myGenome.txt", SNPS);
    cout << "myGenome.txt created Successfully.\n";

    /* Short Reads 생성 */
    //cout << "==== create Short Reads from myGenome.txt ====\n";
    //makeShortReads("myGenome.txt", "myGenome-short-reads.txt", 6, 30);
    // makeShortReads("myGenome.txt", "myGenome-short-reads.txt", 20, 2500);
    makeShortReads("myGenome.txt", "myGenome-short-reads.txt", 40, 30000);
    //cout << "myGenome-short-reads.txt created Successfully\n";

    /* bwt를 이용해 short reads 를 원래대로 복원 */
    clock_t start;
    start = clock();
    //string s = readFile("test.txt") + "$";
    //string s = readFile("covid-19.txt")+"$";
    string s = readFile("GenomeRef.txt") + "$";
    BWT bwt = BWT();
    bwt.setData(s, SNPS, MISMATCH);
    bwt.setShortReads(getShortReads("myGenome-short-reads.txt"));
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

