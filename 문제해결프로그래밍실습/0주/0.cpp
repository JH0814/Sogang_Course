#include<iostream>
using namespace std;

int max(int a, int b){
    return a > b ? a:b;
}

int main(){
    int num;
    cin >> num;
    int* arr = new int[num];
    for(int i = 0; i<num; i++){
        cin >> arr[i];
    }
    int tail = 0;
    int max_value = 0;
    for(int i = 0; i<num; i++){
        tail = max(0, tail + arr[i]);
        max_value = max(max_value, tail);
    }
    cout << max_value << endl;
    delete[] arr;
}