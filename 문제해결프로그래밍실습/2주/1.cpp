#include <bits/stdc++.h>
#include <iostream>
#include <stack>
using namespace std;

int maximalRectangle(const vector<vector<int>>& mat) {
    int N = mat[0].size();
    vector<vector<int>> arr(N, vector<int>(N));
    int area = 0;
    
    // TODO
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            if(mat[i][j] == 1){
                arr[i][j] = (i > 0 ? arr[i-1][j] : 0) + 1;
            }
            else{
                arr[i][j] = 0;
            }
        }
    }
    for(int i = 0; i<N; i++){
        int max_cand = 0;
        stack<int> st;
        for(int j = 0; j<=N; j++){
            int cur = (j==N) ? 0 : arr[i][j];
            while(!st.empty() && cur < arr[i][st.top()]){
                int h = arr[i][st.top()];
                st.pop();
                int w = (st.empty()) ? j : j - st.top() - 1;
                max_cand = max(max_cand, h * w);
            }
            st.push(j);
        }
        area = max(area, max_cand);
    }

    return area;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<vector<int>> mat(N, vector<int> (N));
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++){
            
            cin >> mat[i][j];
        
        }
    }
    
    cout << maximalRectangle(mat) << "\n";
}