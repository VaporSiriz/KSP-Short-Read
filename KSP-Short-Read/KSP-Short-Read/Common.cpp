#include "Common.h"

string readFile(string file_name)
{
    ifstream readFile(file_name);
    string output;
    if (readFile.is_open()) {
        readFile >> output;
    }
    return output;
}

int get_rand(int low, int high)
{
    random_device rd;

    mt19937 gen(rd());

    uniform_int_distribution<int> dis(low, high);

    return dis(gen);
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
        cout << buffer << '\n';
    }
    file.close();
    return shortReads;
}

void makeShortReads(string my_file, string short_reads_file, int k, int n)
{   // �յ��ϰ� ����
    cout << "<makeShortReads>\n";
    ofstream writeFile(short_reads_file);
    vector<string> short_reads;
    set<int> short_reads_indexes;
    string genome = readFile(my_file);
    int start_indx = 0;
    int end_indx = k;
    int remain_count = genome.length() - 2;

    short_reads_indexes.insert(start_indx);
    short_reads_indexes.insert(genome.length() - k - 1);

    // index 0���� �����ϴ� short read �� ���� ���� short read
    short_reads.push_back(genome.substr(start_indx, k));
    short_reads.push_back(genome.substr(genome.length() - k -1, k));

    int i = 0, count = 2;
    // i*k�� �������� i*k ~ (i+1)*k �� �ϳ����� ������ ������.
    while (start_indx < genome.length() - k)
    {
        int rand = get_rand(i * k + 1, (i + 1) * k);
        start_indx = rand;
        if (start_indx >= genome.length() - k) break;
        string short_read = genome.substr(start_indx, k);
        cout << start_indx << " : " << short_read << '\n';
        short_reads.push_back(short_read);
        short_reads_indexes.insert(start_indx);
        count++;
        i++;
    }

    // ���� ���� ä���
    cout << "-----------------------\n";
    i = 0;
    int div = (genome.length() / k);
    while (count < n) {
        int rand = get_rand(i * k + 1, (i + 1) * k);
        cout << "count : " << short_reads.size() << " rand is " << rand << " : " << genome.substr(rand, k) << '\n';
        if (rand < genome.length() - k && short_reads_indexes.find(rand) == short_reads_indexes.end()) {
            short_reads_indexes.insert(rand);
            string substr = genome.substr(rand, k);
            cout << rand << " : " << substr << '\n';
            short_reads.push_back(substr);
            count++;
        }
        //i++;
        //i %= div;
        i = (i + 1) % (genome.length() / k);
    }

    // �������� ���� 
    //for (int i = 0;i < short_reads.size();i++)
    //{
    //    int rand = get_rand(0, short_reads.size() - 1);
    //    string tmp = short_reads[i];
    //    short_reads[i] = short_reads[rand];
    //    short_reads[rand] = tmp;
    //}
    sort(short_reads.begin(), short_reads.end());

    // short reads ���Ͽ� write
    for (int i = 0;i < short_reads.size();i++) {
        string short_read = short_reads[i] + "\n";
        writeFile.write(short_read.c_str(), short_read.length());
    }
    writeFile.close();

    cout << "\ncount : " << count << "start_index : " << start_indx << '\n';
}
