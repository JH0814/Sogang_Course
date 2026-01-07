#include <bits/stdc++.h>
#include<vector>
#include<queue>
using namespace std;

vector<int> BFS(int start, int N, vector<vector<int>>& arr){
    vector<int> dis(N + 1, -1);
    queue<int> q;
    dis[start] = 0;
    q.push(start);
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        for(int i = 0; i<arr[cur].size(); i++){
            int nei = arr[cur][i];
            if(dis[nei] == -1){
                dis[nei] = dis[cur] + 1;
                q.push(nei);
            }
        }
    }
    return dis;
}

int main(void) {
	
	ios::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);

    //write your code here
    int N, M;
    cin >> N >> M;
    vector<vector<int>> arr(N + 1);
    for(int i = 0; i < M; i++){
        int a, b;
        cin >> a >> b;
        arr[a].push_back(b);
        arr[b].push_back(a);
    }
    vector<pair<int, int>> red_ver;
    for(int i = 1; i<=N; i++){
        int d;
        cin >> d;
        if(d != -1){
            red_ver.push_back({i, d});
        }
    }
    if(red_ver.empty()){
        cout << N << endl;
        for(int i = 1; i<=N; i++){
            cout << i << " ";
        }
        cout << endl;
        return 0;
    }

    auto fir = red_ver[0];
    vector<int> distance = BFS(fir.first, N, arr);
    vector<int> ans;
    for(int i = 1; i<= N; i++){
        if(distance[i] == fir.second){
            ans.push_back(i);
        }
    }
    for(int i = 1; i<red_ver.size(); i++){
        if(ans.empty()){
            break;
        }
        auto cur = red_ver[i];
        distance = BFS(cur.first, N, arr);
        vector<int> next;
        for(int c : ans){
            if(distance[c] == cur.second){
                next.push_back(c);
            }
        }
        ans = next;
    }

    cout << ans.size() << endl;
    if(!ans.empty()){
        for(int i = 0; i<ans.size(); i++){
            cout << ans[i] << " ";
        }
        cout << endl;
    }
    return 0;
}