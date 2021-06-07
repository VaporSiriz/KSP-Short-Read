#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>

using namespace std;

/**
    그래프 구조 : vertax<vertax<int>> // 2차원 배열
    element1 : next_vertax1, next_vertax2, ...
    element2 : ...
    몇 번째 행의 무슨 vertax인지 알기 위해서 vertax를 따로 정의
**/
typedef struct Node {
    string name;
    vector<int> id;

} Node;

class Euler {
public:
    vector<vector<int>> graph;
    vector<Node> vertax;
    vector<int> startVertax;
    // struct Spectrum s;
    int mer;
    int edgeNum;

    Euler(vector<string> shortReads, int mMer) {
        mer = mMer;
        edgeNum = 0;
        vector<string> spec;
        for(int i = 0 ; i < shortReads.size() ; i++) { // shortRead 당 spectrum 생성
            for(int j = 0 ; j < shortReads[i].size() - mer + 1 ; j++) {
                spec.push_back(shortReads[i].substr(j, mer));
            }
            getGraph(spec, i);
            spec.clear();
        }
        cout << "===Graph 생성 완료===" << endl;
        printGraph();
        cout << "Edge 수 : " << edgeNum << endl;
        cout << "StartVertax Number : ";
        for(int i = 0 ; i < startVertax.size() ; i++) {
            cout << startVertax[i] << " ";    
        }
        cout << endl;
        
    }

    void restoreSpectrum(vector<string>& result) { // 복원 함수
        vector<vector<bool>> mark;
        stack<int> st;
        int edgeNum = 0; // 간선 개수
        
        // mark 초기화
        mark.resize(graph.size());
        for(int i = 0 ; i < mark.size() ; i++) {
            mark[i].resize(graph[i].size(), false);
        }
        
        // DFS 수행
        for(int i = 0 ; i < startVertax.size() ; i++) { // i : 시작 위치
            if(graph[i].empty()) continue; // 간선이 없는 경우 진행할 수 없으므로
            // mark 초기화
            for(int j = 0 ; j < mark.size() ; j++) {
                fill(mark[j].begin(), mark[j].end(), false);
            }
            
            st.push(startVertax[i]); // 시작 위치 push
            DFS(result, mark, st);
        }
    }

    void DFS(vector<string>& result, vector<vector<bool>>& mark, stack<int>& st) {
        // cout << getString(st) << " " << st.size() << " / " << edgeNum+1 << endl;
        int top = st.top();
        if(st.size() == edgeNum + 1) { // 모든 간선을 지났을 때 (+1은 시작지점)
            result.push_back(getString(st));
            cout << "FIND !!" << endl;
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
                DFS(result, mark, st);
            }
        }
        st.pop();
        if(!st.empty()) {
            auto markIndex = distance(graph[st.top()].begin(), find(graph[st.top()].begin(), graph[st.top()].end(), top));
            mark[st.top()][markIndex] = false;
        }
    }

    void getGraph(vector<string> spec, int mid) { // 그래프 생성 함수
        string front;
        string back;
        
        // vertax 생성
        for(int i = 0 ; i < spec.size() ; i++) {
            front = (spec[i]).substr(0, mer - 1);
            back = (spec[i]).substr(1, mer - 1);
            Node* fVertax = NULL;
            int fVertaxIndex;
            for(int j = 0 ; j < vertax.size() ; j++) {
                if(vertax[j].name == front) {
                    fVertax = &vertax[j];
                    fVertaxIndex = j;
                }
            }

            if(i == 0 && fVertax == NULL) {
                startVertax.push_back(vertax.size());
            } else if(i == 0 && find(startVertax.begin(), startVertax.end(), fVertaxIndex) == startVertax.end()) {
                startVertax.push_back(fVertaxIndex);
            }

            if(fVertax == NULL) { // 존재하지 않는 경우
                Node v;
                v.name = front;
                v.id.push_back(mid);
                vertax.push_back(v);
            } else {
                if(find(fVertax->id.begin(), fVertax->id.end(), mid) == fVertax->id.end()){
                    fVertax->id.push_back(mid);
                }
            }


            Node* bVertax = NULL;
            int bVertaxIndex;
            for(int j = 0 ; j < vertax.size() ; j++) {
                if(vertax[j].name == back) {
                    bVertax = &vertax[j];
                    bVertaxIndex = j;
                }
            }
            // auto bVertax = find(vertax.begin(), vertax.end(), back); // 뒷 부분 ex) ATG => TG
            if(bVertax == NULL) { // 존재하지 않는 경우
                Node v;
                v.name = back;
                v.id.push_back(mid);
                vertax.push_back(v);
            } else {
                
                if(find(bVertax->id.begin(), bVertax->id.end(), mid) == bVertax->id.end()){
                    bVertax->id.push_back(mid);
                }
            }
        }

        // 간선 생성 (그래프 생성)
        graph.resize(vertax.size());
        for(int i = 0 ; i < spec.size() ; i++) {
            front = (spec[i]).substr(0, mer-1);
            back = (spec[i]).substr(1, mer-1);

            
            Node* fVertax = NULL;
            int fVertaxIndex;
            for(int j = 0 ; j < vertax.size() ; j++) {
                if(vertax[j].name == front) {
                    fVertax = &vertax[j];
                    fVertaxIndex = j;
                }
            }

            Node* bVertax = NULL;
            int bVertaxIndex;
            for(int j = 0 ; j < vertax.size() ; j++) {
                if(vertax[j].name == back) {
                    bVertax = &vertax[j];
                    bVertaxIndex = j;
                }
            }

            if(i == 0) {
                vector<vector<bool>> mark;
                // mark 초기화
                mark.resize(graph.size());
                for(int j = 0 ; j < mark.size() ; j++) {
                    mark[j].resize(graph[j].size(), false);
                }
                // 겹치는지 확인
                bool is_exist = false;
                // Node temp = *fVertax;
                Node* v = fVertax;
                for(int j = 0 ; j < fVertax->id.size() ; j++) { // fVertax의 전체 id에 대하여
                    if(fVertax->id[j] == mid) continue; // 자기자신이면 패스
                    int k = 0;
                    while(k < spec.size()) {
                        string f = (spec[k]).substr(0, mer-1);
                        string b = (spec[k]).substr(1, mer-1);
                        // if(find(v->id.begin(), v->id.end(), j) != v->id.end()) {
                        //     v = 
                        // }
                        k++;
                    }

                }

            } else {
                graph[fVertaxIndex].push_back(bVertaxIndex);
                edgeNum++;
            }
        }
    }

    void printGraph() { // 디버깅용 그래프 출력 함수
        for(int i = 0 ; i < graph.size() ; i++) {
            cout << vertax[i].name << "( ";
            for(int j = 0 ; j < vertax[i].id.size() ; j++) {
                cout << vertax[i].id[j] << " ";
            }
            cout << ") : ";
            for(int j = 0 ; j < graph[i].size() ; j++) {
                cout << vertax[graph[i][j]].name << " ";
            }
            cout << endl;
        }
    }

    string getString(stack<int> st) { // stack으로부터 복원한 string return
        string result = vertax[st.top()].name;
        st.pop();
        while(!st.empty()) {
            result = vertax[st.top()].name.front() + result;
            st.pop();
        }
        return result;
    }

    void printResult(vector<string>& result) { // 결과 출력 함수
        cout << "===Result===" << endl;
        for(auto i = result.begin() ; i != result.end() ; i++) {
            cout << *i << endl;
        }
        if(result.empty()) cout << "EMPTY" << endl;
    }
};


int main() {

    int len;
    vector<string> result;
    vector<string> shortReads;
    string buffer;

    ifstream file("/Users/songhyemin/Documents/GitHub/KSP-Short-Read/denove/short-reads.txt");
    while (file.peek() != EOF) {
        getline(file, buffer);
        shortReads.push_back(buffer);
    }
    len = shortReads.size();
    Euler euler(shortReads, 3);
    
    // cout << "mer : " << euler.s.mer << endl;
    // for(int i = 0 ; i < euler.s.data.size() ; i++) {
    //     cout << euler.s.data[i] << " ";
    // }
    // // cout << endl;
    euler.restoreSpectrum(result);
    euler.printResult(result);
    
    return 0;
}



