#include "bwt_module.h"

/*
    data generated in Constructor
    1. ref
    2. SA(Suffix Array)
    3. bwt : bwt string
    4. base_max_count : map storing max count of each bas
    5. base_keys : vector storing sort of base from bwt.
    6. bwt_rank : map storing base-rank in order of bwt string
    7. bwt_front : map sotring <<base-rank>-index>
*/
void BWT::setData(string ref) {
    this->ref = ref;
    SA = getsfx(ref);
    bwt = makeBWT();
    front = bwt;
    sort(front.begin(), front.end());
    for (int i = 0;i < bwt.length();i++) {
        // if there is no character, add (base-rank1)
        if (base_max_count.find(bwt[i]) == base_max_count.end()) {
            base_keys.push_back(bwt[i]);
            base_max_count.insert(make_pair(bwt[i], 0));
        }
        bwt_rank.push_back(++base_max_count[bwt[i]]);
    }
    sort(base_keys.begin(), base_keys.end());
    // print front
    //string front = "";
    int index = 0;
    for (int i = 0;i < base_keys.size();i++) {
        cout << base_keys[i] << " : base_max_count : " << base_max_count[base_keys[i]] << '\n';
        for (int j = 1;j <= base_max_count[base_keys[i]];j++)
        {
            // front += base_keys[i];
            bwt_front.insert({ make_pair(base_keys[i], j), index++ });
        }
    }
}

string BWT::makeBWT() {
    string bwt = "";
    for (int i = 0;i < SA.size();i++) {
        int index = SA[i] == 0 ? SA.size() - 1 : SA[i] - 1;
        bwt += this->ref[index];
    }
    return bwt;
}

void BWT::setShortReads(vector<string> short_reads) {
    this->short_reads = short_reads;
}

// NLogN
vector<int> BWT::getsfx(string s) {
    vector<int> g, ng, sfx, cnt, orderToIdx;
    int n = s.size();
    int lim = max(257, n + 1);
    g.resize(n + 1); ng.resize(n + 1); sfx.resize(n); orderToIdx.resize(n + 1);
    for (int i = 0; i < n; i++) sfx[i] = i, g[i] = s[i];
    for (int t = 1; t < n; t <<= 1) {
        cnt.clear(); cnt.resize(lim, 0);
        for (int i = 0; i < n; i++) cnt[g[min(i + t, n)]]++;
        for (int i = 1; i < lim; i++) cnt[i] += cnt[i - 1];
        for (int idx = n - 1; idx >= 0; idx--) orderToIdx[--cnt[g[min(idx + t, n)]]] = idx;

        cnt.clear(); cnt.resize(lim, 0);
        for (int i = 0; i < n; i++) cnt[g[i]]++;
        for (int i = 1; i < lim; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sfx[--cnt[g[orderToIdx[i]]]] = orderToIdx[i];

        auto cmp = [&](int i, int j) {
            if (g[i] == g[j]) return g[i + t] < g[j + t];
            else return g[i] < g[j];
        };
        ng[sfx[0]] = 1;
        for (int i = 1; i < n; i++) {
            if (cmp(sfx[i - 1], sfx[i])) ng[sfx[i]] = ng[sfx[i - 1]] + 1;
            else ng[sfx[i]] = ng[sfx[i - 1]];
        }
        g = ng;
    }
    return sfx;
}

// mismatch가 없음을 가정
pair<int, int> BWT::searching(string Pattern) {
    //초기값 s = 1, e = 주어진 문자열의 길이
    int s = 1;
    int e = bwt.length();
    //패턴 길이만큼 반복
    for (int i = 0; i < Pattern.length(); i++)
    {
        s = start(s, Pattern[Pattern.length() - 1 - i], front, bwt);
        e = end(e, Pattern[Pattern.length() - 1 - i], front, bwt);
    }

    cout << "F Index From " << s << " to " << e;
    return make_pair(s, e);
}

struct by_value {
    bool operator()(pair<string, pair<int, int>> const &a, pair<string, pair<int, int>> const& b) const {
        return a.second.first < b.second.first;
    }
};

string BWT::restore() {
    cout << "ref   : " << ref << '\n';
    cout << "bwt   : " << bwt << '\n';
    cout << "front : " << front << '\n';
    for (int i = 0;i < SA.size();i++) cout << ref[SA[i]];
    cout << '\n';
    string rt;
    rt.resize(bwt.size());
    for (int i = 0;i < short_reads.size();i++) {
        cout << "---searching---\n";
        cout << short_reads[i] << "\n";
        pair<int, int> start_to_end = searching(short_reads[i]);
        if (start_to_end.first == 0 && start_to_end.second == -1) {
            cout << i << " : " << short_reads[i] << '\n';
        }
        else {
            indexes.push_back(make_pair(short_reads[i], make_pair(start_to_end.first, start_to_end.second)));
            cout << " => ";
            int index = SA[start_to_end.first];
            cout << " index : " << index << ' ';
            for (int j = 0;j < short_reads[i].length();j++) {
                int location = index+j;
                rt[location] = short_reads[i].at(j);
                cout << short_reads[i].at(j);
            }
            cout << '\n';
        }
    }
    sort(indexes.begin(), indexes.end(), by_value());
    cout << rt << '\n';
    return rt;
}

// NLogN
vector<int> getsfx(string s) {
    vector<int> g, ng, sfx, cnt, orderToIdx;
    int n = s.size();
    int lim = max(257, n + 1);
    g.resize(n + 1); ng.resize(n + 1); sfx.resize(n); orderToIdx.resize(n + 1);
    for (int i = 0; i < n; i++) sfx[i] = i, g[i] = s[i];
    for (int t = 1; t < n; t <<= 1) {
        cnt.clear(); cnt.resize(lim, 0);
        for (int i = 0; i < n; i++) cnt[g[min(i + t, n)]]++;
        for (int i = 1; i < lim; i++) cnt[i] += cnt[i - 1];
        for (int idx = n - 1; idx >= 0; idx--) orderToIdx[--cnt[g[min(idx + t, n)]]] = idx;

        cnt.clear(); cnt.resize(lim, 0);
        for (int i = 0; i < n; i++) cnt[g[i]]++;
        for (int i = 1; i < lim; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sfx[--cnt[g[orderToIdx[i]]]] = orderToIdx[i];

        auto cmp = [&](int i, int j) {
            if (g[i] == g[j]) return g[i + t] < g[j + t];
            else return g[i] < g[j];
        };
        ng[sfx[0]] = 1;
        for (int i = 1; i < n; i++) {
            if (cmp(sfx[i - 1], sfx[i])) ng[sfx[i]] = ng[sfx[i - 1]] + 1;
            else ng[sfx[i]] = ng[sfx[i - 1]];
        }
        g = ng;
    }
    return sfx;
}

// mismatch가 없음을 가정
void searching(const string& BWT, string Pattern) {
    cout << "Searching " << Pattern << endl;
    string F;//Front
    //cout << text << endl;
    F = BWT;
    sort(F.begin(), F.end());
    //cout << F << endl;

    //초기값 s = 1, e = 주어진 문자열의 길이
    int s = 1;
    int e = BWT.length();
    //패턴 길이만큼 반복
    for (int i = 0; i < Pattern.length(); i++)
    {
        s = start(s, Pattern[Pattern.length() - 1 - i], F, BWT);
        e = end(e, Pattern[Pattern.length() - 1 - i], F, BWT);
    }

    cout << "F Index From " << s << " to " << e;
}

int C(char Pi, string F)
{
    return F.find(Pi);
}

int Rank(int index, char Pi, string BWT)
{
    int count = 0;
    for (int i = 0; i < index; i++)
        if (Pi == BWT[i])
            count++;
    return count;
}

int start(int index, char Pi, string F, string BWT) {
    int s;
    s = C(Pi, F) + Rank(index - 1, Pi, BWT) + 1;
    return s;
}
int end(int index, char Pi, string F, string BWT) {
    int e;
    e = C(Pi, F) + Rank(index, Pi, BWT);
    return e;
}
