#include <iostream>
#include <vector>
using namespace std;

int maxWater(vector<int>& arr) {
    int res = 0;
    int fir = 0;
    int lst = arr.size() - 1;
    int l_max = 0;
    int r_max = 0;
    while(fir < lst){
        if(arr[fir] <= arr[lst]){
            if(arr[fir] >= l_max){
                l_max = arr[fir];
            }
            else{
                res += (l_max - arr[fir]);
            }
            fir++;
        }
        else{  
            if(arr[lst] >= r_max){
                r_max = arr[lst];
            }
            else{
                res += (r_max - arr[lst]);
            }
            lst--;
        }
    }
    

    return res;
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int test_num;
    cin >> test_num; 

    for (int t = 0; t < test_num; ++t) {
        int B;
        cin >> B; 

        vector<int> arr(B);
        for (int i = 0; i < B; ++i) {
            cin >> arr[i];
        }

        cout << maxWater(arr) << endl;
    }

    return 0;
}