#include <iostream>
#include <vector>
using namespace std;

int min_steps(int n) {
    int i = 2;
    int ans = 0;
    while(n > 1){
        while(n % i == 0){
            ans += i;
            n /= i;
        }
        i++;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) {
        cout << 0 << "\n";
        return 0;
    }
    cout << min_steps(n) << endl;
    return 0;
}