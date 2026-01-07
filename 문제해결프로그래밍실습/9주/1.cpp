#include<iostream>
#include<queue>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

string Gifts(const string& d, int n, int m) {
    string answer;
    string d_sort = d;
    sort(d_sort.begin(), d_sort.end());

    string s_m = to_string(m);
    int len_m = s_m.length();

    vector<bool> visited(n + 1, false);
    vector<int> from(n + 1, -1);
    vector<int> how(n + 1, 0);
    vector<int> dist(n + 1, 0);

    queue<int> q;
    for(char c : d_sort){
        int digit = c - '0';
        if(digit == 0) continue;
        int r = digit % n;
        int state = r;
        if(r == m){
            if(len_m == 1 && digit == m){
                state = n;
            }
        }
        if(!visited[state]){
            visited[state] = true;
            from[state] = -1;
            how[state] = c;
            dist[state] = 1;
            q.push(state);
        }
    }
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        if(cur == m){
            answer = "";
            int temp = m;
            while(temp != -1){
                answer += (char)how[temp];
                temp = from[temp];
            }
            reverse(answer.begin(), answer.end());
            return answer;
        }
        int cur_r = (cur == n) ? m : cur;
        for(char c : d_sort){
            int digit = c - '0';
            int n_r = (cur_r * 10 + digit) % n;
            int next_len = dist[cur] + 1;
            int next_state = n_r;
            if(n_r == m){
                if(next_len == len_m){
                    string s = "";  
                    int temp = cur;
                    while(temp != -1){
                        s += (char)how[temp];
                        temp = from[temp];
                    }
                    reverse(s.begin(), s.end());
                    s += c;
                    if(s == s_m){
                        next_state = n;
                    }
                }
            }
            if(!visited[next_state]){
                visited[next_state] = true;
                from[next_state] = cur;
                how[next_state] = c;
                dist[next_state] = next_len;
                q.push(next_state);
            }
        }
    }
    answer = "IMPOSSIBLE";
    return answer;
}

int main() {
    ios_base::sync_with_stdio(0);
    int test_num;
    cin >> test_num;

    while (test_num--) {
        string d;
        int n, m;
        cin >> d >> n >> m;

        cout << Gifts(d, n, m) << "\n";
    }

    return 0;
}