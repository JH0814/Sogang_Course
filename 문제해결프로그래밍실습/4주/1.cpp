#include "env.h"
#include<cmath>
#include<queue>
using namespace std;

vector<int> find_pair(int N){
    vector<int> partner(N, -1);
    vector<bool> visited(N, false);
    for(int i = 0; i<N; i++){
        if(!visited[i]){
            bool found = false;
            for(int j = i + 1; j<N; j++){
                if(!visited[j] && check_connectivity(i, j)){
                    partner[i] = j;
                    partner[j] = i;
                    visited[i] = true;
                    visited[j] = true;
                    found = true;
                    break;
                }
            }
            if(!found){
                visited[i] = true;
            }
        }
    }
    return partner;
}

pair<vector<int>, int> find_pair_uncon(int N){
    vector<int> part(N, -1);
    vector<bool> visited(N, false);
    int uncon = -1;
    for(int i = 0; i<N; i++){
        if(!visited[i]){
            bool found = false;
            for(int j = i + 1; j<N; j++){
                if(!visited[j] && check_connectivity(i, j)){
                    part[i] = j;
                    part[j] = i;
                    visited[i] = true;
                    visited[j] = true;
                    found = true;
                    break;
                }
            }
            if(!found){
                visited[i] = true;
                uncon = i;
            }
        }
    }
    return {part, uncon};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    std::cin >> T;
    
    while (T--) {
        int N;
        std::cin >> N;

        // TODO: write your logic here!
        // You can use the functions defined above.
        // Any invocation of print functions rather than the given
        // functions will result in 0 points for this question.
        if(N == 3){
            vector<int> res(N, -1);
            for(int i = 0; i<N/2; i++){
                link_wire(2 * i, 2 * i + 1);
            }
            move_to_opposite();
            auto result1 = find_pair_uncon(N);
            vector<int> pair1 = result1.first;
            int unlinked_a = result1.second;
            res[N - 1] = unlinked_a;
            vector<int> b_to_link;
            b_to_link.push_back(unlinked_a);
            for(int i = 0; i<N; i++){
                if(i != unlinked_a){
                    b_to_link.push_back(i);
                }
            }
            for(int i = 0; i<N/2; i++){
                link_wire(b_to_link[2 * i], b_to_link[2 * i + 1]);
            }
            move_to_opposite();
            clean_wires();
            auto result2 = find_pair_uncon(N);
            vector<int> pair2 = result2.first;
            int unlinked_b = result2.second;
            vector<int> part_B(N, -1);
            for(int i = 0; i<N/2; i++){
                part_B[b_to_link[2 * i]] = b_to_link[2 * i + 1];
                part_B[b_to_link[2 * i + 1]] = b_to_link[2 * i];
            }
            queue<int> q;
            q.push(N - 1);
            if(res[unlinked_b] == -1){
                res[unlinked_b] = b_to_link.back();
            }
            if(unlinked_b != N - 1){
                q.push(unlinked_b);
            }
            while(!q.empty()){
                int cur_A = q.front();
                q.pop();
                if(res[cur_A] == -1) continue;
                int cur_B = res[cur_A];
                if(cur_A < N - 1){
                    int partner_A1 = (cur_A % 2 == 0) ? cur_A + 1 : cur_A - 1;
                    if(res[partner_A1] == -1 && pair1[cur_B] != -1){
                        res[partner_A1] = pair1[cur_B];
                        q.push(partner_A1);
                    }
                }
                int partner_A2 = pair2[cur_A];
                if(partner_A2 != -1 && res[partner_A2] == -1 && part_B[cur_B] != -1){
                    res[partner_A2] = part_B[cur_B];
                    q.push(partner_A2);
                }
            }
            vector<pair<int, int>> ans;
            for(int i = 0; i<N; i++){
                ans.push_back({i, res[i]});
            }
            answer(ans);
        }
        else if(N % 2 != 0){
            for(int i = 0; i<N/2; i++){
                link_wire(2 * i, 2 * i + 1);
            }
            move_to_opposite();
            vector<int> pair1 = find_pair(N);
            move_to_opposite();
            clean_wires();
            for(int i = 0; i<(N - 1)/2; i++){
                link_wire(2 * i + 1, 2 * i + 2);
            }
            move_to_opposite();
            vector<int> pair2 = find_pair(N);
            vector<int> res(N);
            int start_node = -1;
            for(int i = 0; i<N; i++){
                if(pair2[i] == -1){
                    start_node = i;
                    break;
                }
            }
            int cur = start_node;
            for(int i = 0; i<N; i++){
                res[i] = cur;
                if(i + 1 < N){
                    if(i % 2 == 0){
                        cur = pair1[cur];
                    }
                    else{
                        cur = pair2[cur];
                    }
                }
            }
            vector<pair<int, int>> ans;
            for(int i = 0; i<N; i++){
                ans.push_back({i, res[i]});
            }
            answer(ans);
        }
        else{
            vector<int> res(N);
            for(int i = 1; i<N - 1; i++){
                link_wire(i, i + 1);
            }
            move_to_opposite();
            vector<int> temp(N, -1);
            for(int i = 0; i<N; i++){
                for(int j = 0; j<N; j++){
                    if(i != j && temp[i] == -1 && check_connectivity(i, j)){
                        temp[i] = j;
                    }
                }
            }
            int a0_find = -1;
            for(int i = 0; i<N; i++){
                if(temp[i] == -1){
                    a0_find = i;
                    break;
                }
            }
            res[0] = a0_find;
            move_to_opposite();
            clean_wires();
            vector<int> a_sub, b_sub;
            for(int i = 1; i<N; i++){
                a_sub.push_back(i);
            }
            for(int i = 0; i<N; i++){
                if(i != a0_find) b_sub.push_back(i);
            }
            int M = N - 1;
            for(int i = 0; i<M/2; i++){
                link_wire(a_sub[2 * i], a_sub[2 * i + 1]);
            }
            move_to_opposite();
            vector<int> sub_pair1 = find_pair(N);
            move_to_opposite();
            clean_wires();
            for(int i = 0; i<(M - 1)/2; i++){
                link_wire(a_sub[2 * i + 1], a_sub[2 * i + 2]);
            }
            move_to_opposite();
            vector<int> sub_pair2 = find_pair(N);
            int start_node = -1;
            int a1 = a_sub[0];
            for(int b : b_sub){
                if(sub_pair2[b] == -1){
                    start_node = b;
                    break;
                }
            }
            int cur = start_node;
            for(int i = 0; i<M; i++){
                int cur_a = a_sub[i];
                res[cur_a] = cur;
                if(i + 1 < M){
                    if(i % 2 == 0){
                        cur = sub_pair1[cur];
                    }
                    else{
                        cur = sub_pair2[cur];
                    }
                }
            }
            vector<pair<int, int>> ans;
            for(int i = 0; i<N; i++){
                ans.push_back({i, res[i]});
            }
            answer(ans);
        }
    }
    return 0;
}