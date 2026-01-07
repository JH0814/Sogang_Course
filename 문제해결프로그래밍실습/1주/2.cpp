#include<iostream>
#include<vector>
#include<queue>
using namespace std;

vector<int> min_multiplications(int n) {
    vector<int> powers;

    //TODO : complete min multiplication
    if(n == 1){
        return {1};
    }
    queue<vector<int>> q;
    vector<bool> visited(n+1, false);
    q.push({1});
    visited[1] = true;
    while(!q.empty()){
        powers = q.front();
        q.pop();
        int cur_max = powers.back();
        for(int i = 0; i<powers.size(); i++){
            for(int j = i; j<powers.size(); j++){
                int n_exp = powers[i] + powers[j];
                if(n_exp > cur_max && n_exp <= n && !visited[n_exp]){
                    vector<int> new_power = powers;
                    new_power.push_back(n_exp);
                    if(n == n_exp){
                        return new_power;
                    }
                    visited[n_exp] = true;
                    q.push(new_power);
                }
            }
        }
    }
    return powers;
}

int main(){

    int n;
    cin >> n;

    
    vector<int> steps = min_multiplications(n);
    cout << steps.size() - 1 << " ";
    for (int step : steps) {
        cout << step << " ";
    }

    return 0;
}