#include "header.h"
void print(long long int* arr){
    for(int i = 0; i<10; i++){ // 정답 배열 끝까지 반복
        printf("%lld ", arr[i]); // 띄어쓰기로 구분
    }
    printf("\n");
}