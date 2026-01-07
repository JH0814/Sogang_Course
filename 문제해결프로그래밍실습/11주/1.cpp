#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int INF = 1e9;

int travelling(int N, const vector<vector<int>> &cost_graph) {
    int answer;

    // TODO: Implement the travelling function
    if (N == 1) return 0;
    vector<vector<int>> dp(1 << N, vector<int>(N, INF));
    dp[1][0] = 0;
    for(int t = 1; t< (1<<N); t++){
        for(int i = 0; i<N; i++){
            if((t & (1 << i)) == 0) continue;
            if(dp[t][i] == INF) continue;

            for(int j = 0; j<N; j++){
                if((t & (1 << j))) continue;
                if(cost_graph[i][j] == 0) continue;
                int next = t | (1 << j);
                dp[next][j] = min(dp[next][j], dp[t][i] + cost_graph[i][j]);
            }
        }
    }
    answer = INF;
    int full = (1 << N) - 1;
    for(int i = 1; i<N; i++){
        if(dp[full][i] != INF && cost_graph[i][0] > 0){
            answer = min(answer, dp[full][i] + cost_graph[i][0]);
        }
    }

    return answer;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<int>> cost_graph(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> cost_graph[i][j];
        }
    }

    int ans = travelling(N, cost_graph);
    cout << ans << '\n';

    return 0;
}