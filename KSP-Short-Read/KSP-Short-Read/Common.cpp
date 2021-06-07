#include "Common.h"

string readFile(string file_name)
{
    ifstream readFile(file_name);
    string output;
    if (readFile.is_open()) {
        readFile >> output;
    }
    readFile.close();
    return output;
}

int get_rand(int low, int high)
{
    random_device rd;

    mt19937 gen(rd());

    uniform_int_distribution<int> dis(low, high);

    return dis(gen);
}

char getOppositeBase(char base) {
    char b = base;
    switch (base)
    {
    case 'A':
        b = 'T';
        break;
    case 'C':
        b = 'G';
        break;
    case 'G':
        b = 'C';
        break;
    case 'T':
        b = 'A';
        break;
    default:
        break;
    }
    return b;
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

void make_bwt(string read_file_name, string write_file_name)
{
    string s = readFile(read_file_name);
    vector<int> sfx = getsfx(s);
    write_bwt(write_file_name, sfx, s);
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

vector<string> getShortReads(string fileName) {
    string buffer;
    vector<string> shortReads;
    ifstream file(fileName);
    while (file.peek() != EOF) {
        getline(file, buffer);
        shortReads.push_back(buffer);
        //cout << buffer << '\n';
    }
    file.close();
    return shortReads;
}

vector<int> getSNPLocations(string refFile, int snp_distance)
{
    string ref = readFile(refFile);
    vector<int> SNPS;
    for (int i = 0;i < ref.length()-snp_distance;i += snp_distance) {
        int site = get_rand(i, i + snp_distance - 1);
        SNPS.push_back(site);
    }
    return SNPS;
}

// myGenome을 생성. SNP이 몇개 단위로 존재할 것인지도 포함
void makeMyGenome(string ref_file, string my_file, vector<int> SNPS)
{
    ofstream writeFile(my_file);
    string myGenome = readFile(ref_file);
    for (int i = 0;i < SNPS.size();i++) {
        myGenome[SNPS[i]] = getOppositeBase(myGenome[SNPS[i]]);
    }
    writeFile.write(myGenome.c_str(), myGenome.length());
    writeFile.close();
}

void makeShortReads(string my_file, string short_reads_file, int k, int n)
{   // 균등하게 생성
    cout << "<makeShortReads>\n";
    ofstream writeFile(short_reads_file);
    vector<string> short_reads;
    set<int> short_reads_indexes;
    string genome = readFile(my_file);
    int start_indx = 0;

    short_reads_indexes.insert(start_indx);
    short_reads_indexes.insert(genome.length() - k);

    // index 0부터 시작하는 short read 과 가장 끝의 short read
    string start = genome.substr(start_indx, k);
    string end = genome.substr(genome.length() - k, k);
    short_reads.push_back(genome.substr(start_indx, k));
    short_reads.push_back(genome.substr(genome.length() - k, k));
    //cout << start_indx << " : " << start << "\n" << genome.length() - k << " : " << end << '\n';

    int i = 0, count = 2;
    // i*k를 거점으로 i*k ~ (i+1)*k 에 하나씩은 무조건 들어가야함.
    while (start_indx < genome.length() - k)
    {
        int rand = get_rand(i*k + k/2, (i + 1) * k);
        start_indx = rand;
        if (start_indx >= genome.length() - k) break;
        string short_read = genome.substr(start_indx, k);
        //cout << start_indx << " : " << short_read << '\n';
        short_reads.push_back(short_read);
        short_reads_indexes.insert(start_indx);
        count++;
        i++;
    }

    // 남은 갯수 채우기
    //cout << "-----------------------\n";
    i = 0;
    int div = (genome.length() / k);
    while (count < n) {
        int rand = get_rand(i * k + 1, (i + 1) * k);
        if (rand < genome.length() - k && short_reads_indexes.find(rand) == short_reads_indexes.end()) {
            short_reads_indexes.insert(rand);
            string substr = genome.substr(rand, k);
            //cout << rand << " : " << substr << '\n';
            short_reads.push_back(substr);
            count++;
        }
        //i++;
        //i %= div;
        i = (i + 1) % (genome.length() / k);
    }

    // 랜덤으로 섞고 
    //for (int i = 0;i < short_reads.size();i++)
    //{
    //    int rand = get_rand(0, short_reads.size() - 1);
    //    string tmp = short_reads[i];
    //    short_reads[i] = short_reads[rand];
    //    short_reads[rand] = tmp;
    //}
    sort(short_reads.begin(), short_reads.end());

    // short reads 파일에 write
    for (int i = 0;i < short_reads.size();i++) {
        string short_read = short_reads[i] + "\n";
        writeFile.write(short_read.c_str(), short_read.length());
    }
    writeFile.close();

    //cout << "\ncount : " << count << " start_index : " << start_indx << '\n';
}
