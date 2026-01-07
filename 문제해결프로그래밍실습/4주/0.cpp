#include <bits/stdc++.h>
using namespace std;

long long ask(long long y, long long x) {
	cout << '?' << ' ' << y << ' ' << x << endl;
	long long ret;
	cin >> ret;
	return ret;
}

int main(void) {
	
	ios::sync_with_stdio(0);
	cin.tie(0); cout.tie(0);

	int N; cin >> N;
	// write your code here
    for(int i = 0; i<N; i++){
        long long l = 0;
        long long r = 10000000 - 1;
        for(int j = 0; j<50; j++){
            long long mid1 = l + (r - l) / 3;
            long long mid2 = r - (r - l) / 3;
            if(ask(0, mid1) < ask(0, mid2)){
                r = mid2;
            }
            else{
                l = mid1;
            }
        }
        long long ans_x = l;
        long long min_dis = ask(0, ans_x);
        for(long long dx = l - 3; dx <= l + 3; dx++){
            if(dx < 0 || dx >= 10000000) continue;
            long long cur = ask(0, dx);
            if(cur < min_dis){
                min_dis = cur;
                ans_x = dx;
            }
        }
        long long ans_y = round(sqrt(min_dis));
        if(ans_y >= 10000000) ans_y = 10000000 - 1;
        ask(ans_y, ans_x);
    }
	return 0;
}