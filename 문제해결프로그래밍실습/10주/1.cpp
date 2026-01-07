#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> pebbles(int N, int K, string S) {
    vector<int> ans;
    // TODO
    // You should choose the indices and put them into
    // the vector `ans`.
    // The indices you insert should be 0-based, but
    // the skeleton code will convert them to 1-based
    // and print them.

    int fir = 0;
    int white = 0;
    int max_len = 0;
    int ans_fir = 0;
    int ans_lst = -1;
    
    for(int lst = 0; lst<N; lst++){
        if(S[lst] == 'W'){
            white++;
        }
        while(white > K){
            if(S[fir] == 'W'){
                white--;
            }
            fir++;
        }
        int cur = lst - fir + 1;
        if(cur > max_len){
            max_len = cur;
            ans_fir = fir;
            ans_lst = lst;
        }
    }
    if(ans_lst != -1){
        for(int i = ans_fir; i<=ans_lst; i++){
            if(S[i] == 'W'){
                ans.push_back(i);
            }
        }
    }
    return ans;
}

int main() {
    int N, K;
    string S;

    cin >> N >> K >> S;

    vector<int> ans = pebbles(N, K, S);

    cout << ans.size() << '\n';
    for (int x: ans) cout << x + 1 << ' ';
    cout << '\n';

    return 0;
}