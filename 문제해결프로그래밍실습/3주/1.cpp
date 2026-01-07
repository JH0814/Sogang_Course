#include <bits/stdc++.h>
#include<vector>
using namespace std;
long long N, L, R, arr[40005][3];

struct Interval{
    long long l;
    long long r;
};
vector<vector<Interval>> bob_route;

long long min(long long a, long long b){
    return a < b ? a : b;
}

long long max(long long a, long long b){
    return a > b ? a : b;
}

void set_sorting(vector<Interval>& v){
    if(v.size() < 2) return;
    for(int i = 0; i<v.size() - 1; i++){
        for(int j = 0; j<v.size() - i - 1; j++){
            if(v[j].l > v[j + 1].l){
                Interval temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

vector<Interval> intersect_sets(vector<Interval>& a, vector<Interval>& b){
    vector<Interval> res;
    int i = 0, j = 0;
    while(i < a.size() && j < b.size()){
        long long fir = max(a[i].l, b[j].l);
        long long lst = min(a[i].r, b[j].r);
        if(fir <= lst){
            res.push_back({fir, lst});
        }
        if(a[i].r < b[j].r){
            i++;
        }
        else{
            j++;
        }
    }
    return res;
}

vector<Interval> union_sets(vector<Interval>& a, vector<Interval>& b){
    vector<Interval> res = a;
    for(int i = 0; i<b.size(); i++){
        res.push_back(b[i]);
    }
    if(res.empty()){
        return {};
    }
    vector<Interval> merged;
    set_sorting(res);
    merged.push_back(res[0]);
    for(int i = 1; i<res.size(); i++){
        if(res[i].l <= merged.back().r + 1){
            merged.back().r = max(merged.back().r, res[i].r);
        }
        else{
            merged.push_back(res[i]);
        }
    }
    return merged;
}

// 4개의 함수의 "함수 원형(인자/반환형/이름)"은 절대 바꾸면 안됩니다.
// 전역 변수/함수/클래스 정의 등은 자유롭게 수정하거나 추가해도 됩니다.
// 정답을 최종 제출할 때 I/O(입출력)는 진행하면 안됩니다. 채점기는 이 4개의 함수만 호출하며 I/O는 엄격히 금지합니다.
// 디버그, 테스트 목적의 출력은 허용됩니다. (e.g. printf, cout)
// I/O를 조작하여 부당한 점수를 얻은 경우, 향후 재채점에서 0점 처리됩니다.

bool need_debug() {

    // 이 함수는 딱 한번 호출됩니다.
    // 실제 게임이 어떻게 되었는지 확인하고 싶으면 true를 return하면 됩니다. 이외에는 false를 return하면 됩니다.
    // true를 return해도 채점이 되긴 하지만, 정답을 최종 제출할 땐 false를 return하는 상태로 제출하는것을 권장합니다.

    // return false; // 정답을 제출할 때에는 false로 바꿔주세요.
    return false;
    
}

void alice_init(long long n, long long l, long long r, const std::vector<std::vector<long long>>& v) {
    
    N = n, L = l, R = r;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) arr[i][j] = v[i][j];
    }
    bob_route.assign(N + 1, vector<Interval>());
    bob_route[N] = {{L, R}};
    for(int i = N - 1; i>=0; i--){
        long long a = arr[i][0];
        long long b = arr[i][1];
        long long c = arr[i][2];
        vector<Interval> next_intervals;
        vector<Interval>& next_win_set = bob_route[i + 1];
        vector<Interval> set_A_prime, set_B_prime, set_C_prime;
        for(int j = 0; j<next_win_set.size(); j++){
            const Interval& p = next_win_set[j];
            set_A_prime.push_back({p.l - a, p.r - a});
            set_B_prime.push_back({p.l - b, p.r - b});
            set_C_prime.push_back({p.l - c, p.r - c});
        }
        vector<Interval> inter_AB = intersect_sets(set_A_prime, set_B_prime);
        vector<Interval> inter_BC = intersect_sets(set_B_prime, set_C_prime);
        vector<Interval> inter_CA = intersect_sets(set_C_prime, set_A_prime);
        vector<Interval> union1 = union_sets(inter_AB, inter_BC);
        bob_route[i] = union_sets(union1, inter_CA);
    }

    for(int i = 0; i<N; i++){
        cout << "Turn " << i << endl;
        for(int j = 0; j<bob_route[i].size(); j++){
            cout << bob_route[i][j].l << " " << bob_route[i][j].r << endl;
        }
    }
    // 이 함수는 딱 한번 호출됩니다. 
    // 여기서 입력을 보관하고, 필요한 무거운 연산들인 전처리/사전 계산 등을 모두 끝내야 합니다.
    // 이후 alice_ban()를 반복적으로 실행할 때, 충분히 빠르게 동작해야 합니다.
    // 총 수행시간 중 채점기와 Bob의 코드의 총 수행시간은 5초 미만임을 보장합니다.
    // 즉 alice_init, is_alice_win을 한번 실행하고 alice_ban을 N번 실행하는 시간의 총 합은 15초 미만이어야 합니다.

}

bool is_alice_win() {

    // 이 함수는 alice_init 이후에 딱 한번 호출됩니다. 
    // 앨리스가 승리할 수 있으면 true, 아니면 false를 return해야 합니다.
    // 정답과 다른 값을 return하면 오답으로 판정됩니다.
    if(bob_route[0].empty()){
        return true;
    }
    for(int i = 0; i<bob_route[0].size(); i++){
        Interval& p = bob_route[0][i];
        if(p.l <= 0 && p.r >= 0){
            return false;
        }
    }
    return true;
}

int alice_ban(int turn, long long S) { //turn은 0-base로 들어옵니다.

    // is_alice_win()에서 true를 return하면 호출되는 함수이며, 정확히 N번 호출되는 함수입니다.
    // 매 호출시 현재 진행중인 턴수와 S값이 주어집니다. 이번 턴에 앨리스가 밴할 인덱스를 return해야 합니다.
    // main함수에서는 alice_ban과 bob_pick함수가 서로 번갈아가면서 실행됩니다. 
    // turn은 0-base로 들어옵니다. 즉 turn = [0..N-1]이 순차적으로 들어옵니다.
    // 즉 현재 밴한 결과는 다음 bob_pick 함수 호출때 영향을 미치며 다음 alice_ban 함수에도 연쇄적으로 영향을 끼칩니다.
    // indx는 0-base입니다. 즉 [0..2]사이의 int를 return해야합니다.
    // 그 외의 값을 return할 시 채점 결과는 오답으로 판정되고 프로그램을 즉시 종료합니다.
    // 최종 S값이 [L..R]사이에 들어오면 오답으로 판정됩니다, 들어오지 않는다면 정답으로 판정됩니다.
    long long a = arr[turn][0];
    long long b = arr[turn][1];
    long long c = arr[turn][2];
    vector<Interval>& next_turn = bob_route[turn + 1];
    bool check_C = false;
    for(int i = 0; i<next_turn.size(); i++){
        Interval& p = next_turn[i];
        if((p.l <= S + a && p.r >= S + a) || (p.l <= S + b && p.r >= S + b)){
            check_C = true;
            break;
        }
    }
    if(!check_C){
        return 2;
    }
    bool check_B = false;
    for(int i = 0; i<next_turn.size(); i++){
        Interval& p = next_turn[i];
        if((p.l <= S + a && p.r >= S + a) || (p.l <= S + c && p.r >= S + c)){
            check_B = true;
            break;
        }
    }
    if(!check_B){
        return 1;
    }
    return 0;
}