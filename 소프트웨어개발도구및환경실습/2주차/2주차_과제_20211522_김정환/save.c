#include "header.h"

void calc(long long int N, long long int ten, long long int* arr){ // 들어온 자리 수만큼 N에 나오는 0 ~ 9값을 정답 배열에 넣어준다.
    while(N > 0){
        arr[N % 10] += ten;
        N /= 10;
    }
}

void save(long long int fir, long long int lst, long long int ten, long long int* arr){
    // fir를 1씩 늘려주면서 10으로 나눈 값이 0이 되게 만든다.
    while(fir % 10 != 0 && fir <= lst){
        // fir를 증가시킬 때마다 calc를 통해서 정답 배열에 값을 늘려준다.
        // 이를 통해서 일의 자리를 0으로 만들면서 지나가는 수들의 자리 수별 숫자를 계산해둘 수 있다.
        calc(fir, ten, arr);
        fir++;
    }
    if(fir > lst) return;
    // lst는 10으로 나눈 값이 9가 될 때까지 1씩 줄여준다.
    while(lst % 10 != 9 && lst >= fir){
        // lst도 줄여줄 때마다 calc를 통해서 정답 배열에 값을 늘려준다.
        // 이를 통해서 일의 자리를 9로 만들면서 지나가는 수들의 자리 수별 숫자를 계산해둘 수 있다.
        calc(lst, ten, arr);
        lst--;
    }
    // cnt는 fir의 일의 자리가 0, lst의 일의 자리가 9인 수가 되었을 때 그 사이에서 일의 자리에 0~9가 나오는 횟수이다.
    long long int cnt = (lst/10 - fir/10 + 1);
    for(int i = 0; i<10; i++){
        arr[i] += cnt * ten; // 0 ~ 9가 일의 자리일 때 몇 번 나왔는지 구한 후 자리 수만큼 곱해주어야 해당 자리 수에 0 ~ 9까지 몇 번 나오는지 알 수 있다.
    }
    // 재귀로 다음 자리 수의 0 ~ 9 개수 세기
    save(fir/10, lst/10, ten * 10, arr);
}