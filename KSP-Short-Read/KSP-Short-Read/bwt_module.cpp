#include "bwt_module.h"

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
        //orderToIdx[x]=idx; => pair���� second�������� �������� ��, second�������� x��°�� �ش��ϴ� index�� idx��� ���̴�.

        cnt.clear(); cnt.resize(lim, 0);
        for (int i = 0; i < n; i++) cnt[g[i]]++;
        for (int i = 1; i < lim; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sfx[--cnt[g[orderToIdx[i]]]] = orderToIdx[i];
        //sfx[x]=idx; => pair���� second�������� ������ first�������� �ٽ� stable sort�� ���� ��, ������ x��°�� �ش��ϴ� �ε����� idx��� ���̴�.

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

/*
    data generated in Constructor
    1. bwt : bwt string
    2. base_max_count : map storing max count of each bas
    3. base_keys : vector storing sort of base from bwt.
    4. bwt_rank : map storing base-rank in order of bwt string
*/
BWT::BWT(string s) {
    this->bwt = s;
    for (int i = 0;i < s.length();i++) {
        // if there is no character, add (base-rank1)
        if (base_max_count.find(s[i]) == base_max_count.end()) {
            base_keys.push_back(s[i]);
            base_max_count.insert(make_pair(s[i], 0));
        }
        bwt_rank.push_back(++base_max_count[s[i]]);
    }
    sort(base_keys.begin(), base_keys.end());
    // print front
    //string front = "";
    //int index = 0;
    //for (int i = 0;i < base_keys.size();i++) {
    //    cout << base_keys[i] << " : base_max_count : " << base_max_count[base_keys[i]] << '\n';
    //    for (int j = 1;j <= base_max_count[base_keys[i]];j++)
    //    {
    //        front += base_keys[i];
    //        bwt_front.insert({ make_pair(base_keys[i], j),index++ });
    //    }
    //}

    //cout << "front : " << front << '\n';
}

//// assume there is no overlap and mismatch
//int BWT::search(string s) {
//    int result = -1;
//    if (s.length() <= 0 || find(base_keys.begin(), base_keys.end(), s[0]) == base_keys.end()) return result;
//    
//    for (int i = 1;i <= base_max_count[s[0]];i++) {
//        int index = 0;
//        int route = 0;
//        while()
//        if (bwt_front[make_pair(s[index], i)] == s[index + 1]) {
//            cout <<
//        }
//    }
//    return 0;
//}

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



