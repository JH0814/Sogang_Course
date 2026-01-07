#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;
vector<vector<int>> adj;
vector<bool> visited;
int cctv_cnt = 0;

int dfs(int u){
    visited[u] = true;
    bool child_unwatch = false;
    bool child_ins = false;
    for(int v : adj[u]){
        if(visited[v]) continue;
        int state = dfs(v);
        if(state == 0) child_unwatch = true;
        if(state == 2) child_ins = true;
    }
    if(child_unwatch){
        cctv_cnt++;
        return 2;
    }
    if(child_ins){
        return 1;
    }
    return 0;
}

int solve(int G) {
	int answer = 0;
	// TO DO //
    cctv_cnt = 0;
    visited = vector<bool>(G, false);
    for(int i = 0; i<G; i++){
        if(!visited[i]){
            int state = dfs(i);
            if(state == 0){
                cctv_cnt++;
            }
        }
    }

	return cctv_cnt;
}

int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int C, H, G, p1, p2;
	cin >> C;

	while (C--) {
		cin >> G >> H;
		adj = vector<vector<int>>(G);

		for (int i = 0; i < H; i++) {
			cin >> p1 >> p2;

			adj[p1].push_back(p2);
			adj[p2].push_back(p1);
		}

		cout << solve(G) << endl;
	}

	return 0;
}
