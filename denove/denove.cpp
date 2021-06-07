#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>
#include <chrono>
#include <cmath>
using namespace std;
using namespace chrono;

string greedyScs(vector<string> reads, int k);
int overlap(string s1, string s2, int minLen);
pair<pair<string, string>, int> selectMaximalOverlap(vector<string> reads, int k);
vector<pair<string, string>> permute(vector<string> reads, int r);

void print(vector<string> reads) {
    for(int i = 0 ; i < reads.size() ; i++) {
        cout << reads[i] << endl;
    }
    cout << endl;
}

string greedyScs(vector<string> reads, int k) {
    pair<pair<string, string>, int> best = selectMaximalOverlap(reads, k);
    string s1 = best.first.first;
    string s2 = best.first.second;
    int olen = best.second;
    // print(reads);
    while(olen > 0) {
        reads.erase(find(reads.begin(), reads.end(), s1));
        reads.erase(find(reads.begin(), reads.end(), s2));
        reads.push_back(s1 + s2.substr(olen, s2.length()-olen));
        // print(reads);
        best = selectMaximalOverlap(reads, k);
        s1 = best.first.first;
        s2 = best.first.second;
        olen = best.second;
    }
    string result = "";
    for(int i = 0 ; i < reads.size() ; i++) {
        result += reads[i];
    }
    return result;
}

int overlap(string s1, string s2, int minLen) { // minLen : 겹치는 prefix/suffix minLength
    int start = 0;
    while(true) {
        start = s1.find(s2.substr(0, minLen), start);
        if(start == string::npos) { // can't find
            return 0;
        }

        string s1Suffix = s1.substr(start, s1.length()-start);
        string s2Prefix = s2.substr(0, s1Suffix.length());
        // found, check s1's suffix <-> s2'x prefix
        if(s1Suffix.compare(s2Prefix) == 0) {
            return s1.length() - start;
        }
        start += 1;
    }
}

vector<pair<string, string>> permute(vector<string> reads, int r) { // r : 순열 요소 개수
    vector<pair<string, string>> v;
    string s1, s2;
    for(int i = 0 ; i < reads.size() ; i++) {
        s1 = reads[i];
        for(int j = 0 ; j < reads.size() ; j++) {
            if(i == j) continue;
            s2 = reads[j];
            v.push_back(make_pair(s1, s2));
        }
    }
    return v;
}

pair<pair<string, string>, int> selectMaximalOverlap(vector<string> reads, int k) { // 가장 최적의 overlap 찾기
    string s1;
    string s2;
    string bestS1 = "";
    string bestS2 = "";
    int bestOlen = 0;
    int olen;
    
    vector<pair<string, string>> permutationList = permute(reads, 2); // 순열 계산
    for(int i = 0 ; i < permutationList.size() ; i++) {
        // cout << s1 << " " << s2 << endl;
        s1 = permutationList[i].first;
        s2 = permutationList[i].second;
        olen = overlap(s1, s2, k); // 얼마나 겹치는지
        if(olen > bestOlen) { // 기존에 최고로 많이 겹친 것보다도 더 겹친다면
            bestS1 = s1;
            bestS2 = s2;
            bestOlen = olen;
        }
    }
    return make_pair(make_pair(bestS1, bestS2), bestOlen);
}


int main() {

    int len;

    vector<string> shortReads;
    string buffer;

    ifstream file("/Users/songhyemin/Documents/GitHub/KSP-Short-Read/denove/short-reads.txt");
    while (file.peek() != EOF) {
        getline(file, buffer);
        shortReads.push_back(buffer);
    }
    auto start = system_clock::now();
    sort(shortReads.begin(), shortReads.end());
    // permute(shortReads, 2);
    string result = greedyScs(shortReads, 3);
    auto end = system_clock::now();
    cout << result << endl;
    cout << "걸린 시간 : " << duration_cast<duration<double, milli>>(end-start).count() << " milliseconds" << endl;

    return 0;
}
