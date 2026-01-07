#include <bits/stdc++.h>
using namespace std;

int N, K;
vector<pair<int, int>> arr[20005];
int parent[20005];
long long max_branch[20005];
long long dist_from_start[20005];

void diameter(int u, int p, long long cur_dis, pair<long long, int>& far_node){
    if(cur_dis > far_node.first){
        far_node = {cur_dis, u};
    }
    for(auto& e : arr[u]){
        int v = e.first;
        int w = e.second;
        if(v == p) continue;
        diameter(v, u, cur_dis + w, far_node);
    }
}

void component_dfs(int u, int p, int forb, long long cur_dis, pair<long long, int>& far_node){
    if(cur_dis > far_node.first){
        far_node = {cur_dis, u};
    }
    for(auto& e : arr[u]){
        int v = e.first;
        int w = e.second;
        if(v == p || v == forb) continue;
        component_dfs(v, u, forb, cur_dis + w, far_node);
    }
}

long long get_path(int start, int forb){
    if(start == 0) return 0;
    pair<long long, int> far = {-1, -1};
    component_dfs(start, 0, forb, 0, far);
    int A = far.second;
    if(A == -1) return 0;
    pair<long long, int> comp = {-1, -1};
    component_dfs(A, 0, forb, 0, comp);
    return comp.first;
}

pair<long long, int> find_path(int u, int p){
    parent[u] = p;
    pair<long long, int> far = {0, u};
    for(auto& e : arr[u]){
        int v = e.first;
        int w = e.second;
        if(v == p) continue;

        auto res = find_path(v, u);
        res.first += w;
        if(res.first > far.first){
            far = res;
        }
    }
    return far;
}

long long branch_dfs(int u, int p){
    long long max_d = 0;
    for(auto& e : arr[u]){
        int v = e.first;
        int w = e.second;
        if(v == p) continue;
        max_d = max(max_d, branch_dfs(v, u) + w);
    }
    return max_d;
}



int main(void) {
	
	ios::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);

    //write your code here
    cin >> N >> K;
    for(int i = 0; i<N - 1; i++){
        int a, b, c;
        cin >> a >> b >> c;
        arr[a].push_back({b, c});
        arr[b].push_back({a, c});
    }
    if(K == 1){
        pair<long long, int> far_ori = {-1, -1};
        diameter(1, 0, 0, far_ori);
        int A = far_ori.second;
        pair<long long, int> dia = {-1, -1};
        diameter(A, 0, 0, dia);
        cout << dia.first << endl;
    }
    else{
        auto res_A = find_path(1, 0);
        int A = res_A.second;
        auto res_B = find_path(A, 0);
        int B = res_B.second;

        vector<int> path;
        vector<bool> on_diameter(N + 1, false);
        int cur = B;
        while(cur != 0){
            path.push_back(cur);
            on_diameter[cur] = true;
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());
        long long cur_dist = 0;
        for(int i = 0; i<path.size(); i++){
            int u = path[i];
            dist_from_start[u] = cur_dist;
            for(auto& e : arr[u]){
                int v = e.first;
                int w = e.second;
                if(!on_diameter[v]){
                    max_branch[u] = max(max_branch[u], branch_dfs(v, u) + w);
                }
            }
            if(i < path.size() - 1){
                for(auto& e : arr[u]) if(e.first == path[i + 1]) cur_dist += e.second;
            }
        }
        vector<long long> left_dia(path.size(), 0);
        vector<long long> right_dia(path.size(), 0);
        long long max_from_left = 0;
        long long max_val = -999999999;
        for(int i = 0; i<path.size(); i++){
            int u = path[i];
            if(i > 0) left_dia[i] = left_dia[i - 1];
            left_dia[i] = max(left_dia[i], max_branch[u]);
            left_dia[i] = max(left_dia[i], dist_from_start[u] + max_branch[u]);
            left_dia[i] = max(left_dia[i], max_val + dist_from_start[u]);
            max_val = max(max_val, max_branch[u] - dist_from_start[u]);
        }
        max_val = -999999999;
        for(int i = path.size() - 1; i>=0; i--){
            int u = path[i];
            long long dist_from_right = res_B.first - dist_from_start[u];
            if(i < path.size() - 1) right_dia[i] = right_dia[i + 1];
            right_dia[i] = max(right_dia[i], max_branch[u]);
            right_dia[i] = max(right_dia[i], dist_from_right + max_branch[u]);
            right_dia[i] = max(right_dia[i], dist_from_right + max_branch[u] + max_val);
            max_val = max(max_val, max_branch[u] - dist_from_right);
        }
        long long ans = -1;
        for(int i = 0; i<path.size() - 1; i++){
            long long d1 = left_dia[i];
            long long d2 = right_dia[i + 1];
            long long cur_max = max(d1, d2);
            if(ans == -1 || cur_max < ans){
                ans = cur_max;
            }
        }
        cout << ans << endl;
    }
    

    return 0;
}