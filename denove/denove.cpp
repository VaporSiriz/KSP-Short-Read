#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>

using namespace std;

typedef struct Spectrum { // Spectrum 구조체
    int mer;
    vector<string> data;
} Spectrum;

void printGraph(vector<vector<int>>& graph, vector<string>& vertax);
void getGraph(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax);
void DFS(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax, vector<string>& result, vector<vector<bool>>& mark, stack<int>& st, int edgeNum);
void restoreSpectrum(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax, vector<string>& result);
string getString(Spectrum s, vector<string>& vertax, stack<int> st);
void printResult(vector<string>& result);

int main() {

    int len;
    vector<vector<int>> graph;
    vector<string> vertax;
    vector<string> result; // 결과 모은 벡터
    vector<string> shortReads;
    string buffer;

    ifstream file("/Users/songhyemin/Documents/과제/알고리즘 실습/프로젝트/short-reads.txt");
    while (file.peek() != EOF) {
        getline(file, buffer);
        shortReads.push_back(buffer);
    }
    len = shortReads.size();
    
    struct Spectrum s;
    s.mer = shortReads[0].length();
    s.data = shortReads;
    cout << "mer : " << s.mer << endl;
    for(int i = 0 ; i < s.data.size() ; i++) {
        cout << s.data[i] << " ";
    }
    cout << endl;
    getGraph(s, graph, vertax);
    printGraph(graph, vertax);
    restoreSpectrum(s, graph, vertax, result);
    printResult(result);
    
    return 0;
}

void restoreSpectrum(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax, vector<string>& result) { // 복원 함수
    vector<vector<bool>> mark;
    stack<int> st;
    int edgeNum = 0; // 간선 개수
    
    // mark 초기화
    mark.resize(graph.size());
    for(int i = 0 ; i < mark.size() ; i++) {
        mark[i].resize(graph[i].size(), false);
        edgeNum += graph[i].size();
    }
    
    // DFS 수행
    for(int i = 0 ; i < graph.size() ; i++) { // i : 시작 위치
        if(graph[i].empty()) continue; // 간선이 없는 경우 진행할 수 없으므로
        // mark 초기화
        for(int j = 0 ; j < mark.size() ; j++) {
            fill(mark[j].begin(), mark[j].end(), false);
        }
        
        st.push(i); // 시작 위치 push
        DFS(s, graph, vertax, result, mark, st, edgeNum);
    }
}

void DFS(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax, vector<string>& result, vector<vector<bool>>& mark, stack<int>& st, int edgeNum) {
    // cout << getString(s, vertax, st) << endl;
    int top = st.top();
    if(st.size() == edgeNum + 1) { // 모든 간선을 지났을 때 (+1은 시작지점)
        result.push_back(getString(s, vertax, st));
        // 이전 상태로 되돌아가기 (top을 push하기 전으로)
        st.pop();
        // top의 mark Index 계산, graph의 top의 몇 번째 연결되어있는 요소인지
        auto markIndex = distance(graph[st.top()].begin(), find(graph[st.top()].begin(), graph[st.top()].end(), top));
        mark[st.top()][markIndex] = false;
        return;
    }
    for(int i = 0 ; i < graph[top].size() ; i++) { // 연결된 간선들에 대해
        if(!mark[top][i]) { // 지나가지 않은 간선이라면
            st.push(graph[top][i]);
            mark[top][i] = true;
            DFS(s, graph, vertax, result, mark, st, edgeNum);
        }
    }
    st.pop();
    if(!st.empty()) {
        auto markIndex = distance(graph[st.top()].begin(), find(graph[st.top()].begin(), graph[st.top()].end(), top));
        mark[st.top()][markIndex] = false;
    }
}

void getGraph(Spectrum s, vector<vector<int>>& graph, vector<string>& vertax) { // 그래프 생성 함수

    string front = (s.data[0]).substr(0, s.mer-1);
    string back = (s.data[0]).substr(1, s.mer-1);

    // 초기 vertax 설정
    vertax.push_back(front);
    vertax.push_back(back);
    
    // vertax 생성
    for(int i = 1 ; i < s.data.size() ; i++) {
        front = (s.data[i]).substr(0, s.mer-1);
        back = (s.data[i]).substr(1, s.mer-1);

        auto fVertax = find(vertax.begin(), vertax.end(), front); // 앞 부분 ex) ATG => AT
        if(fVertax == vertax.end()) { // 존재하지 않는 경우
            vertax.push_back(front);
        }

        auto bVertax = find(vertax.begin(), vertax.end(), back); // 뒷 부분 ex) ATG => TG
        if(bVertax == vertax.end()) { // 존재하지 않는 경우
            vertax.push_back(back);
        }
    }

    // 간선 생성 (그래프 생성)
    graph.resize(vertax.size());
    for(int i = 0 ; i < s.data.size() ; i++) {
        front = (s.data[i]).substr(0, s.mer-1);
        back = (s.data[i]).substr(1, s.mer-1);

        auto fVertax = find(vertax.begin(), vertax.end(), front);
        auto bVertax = find(vertax.begin(), vertax.end(), back);

        int fIndex = distance(vertax.begin(), fVertax); // front index
        int bIndex = distance(vertax.begin(), bVertax); // back index
        if(find(graph[fIndex].begin(), graph[fIndex].end(), bIndex) == graph[fIndex].end()) { // 간선이 연결되어 있지 않은 경우
            graph[distance(vertax.begin(), fVertax)].push_back(distance(vertax.begin(), bVertax));
        }
    }
}

void printGraph(vector<vector<int>>& graph, vector<string>& vertax) { // 디버깅용 그래프 출력 함수
    for(int i = 0 ; i < graph.size() ; i++) {
        cout << vertax[i] << " : ";
        for(int j = 0 ; j < graph[i].size() ; j++) {
            cout << vertax[graph[i][j]] << " ";
        }
        cout << endl;
    }
}

string getString(Spectrum s, vector<string>& vertax, stack<int> st) { // stack으로부터 복원한 string return
    string result = vertax[st.top()];
    st.pop();
    while(!st.empty()) {
        result = vertax[st.top()].front() + result;
        st.pop();
    }
    return result;
}

void printResult(vector<string>& result) { // 결과 출력 함수
    for(auto i = result.begin() ; i != result.end() ; i++) {
        cout << *i << endl;
    }
    if(result.empty()) cout << "EMPTY" << endl;
}