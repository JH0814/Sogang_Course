#include "header.h"
int main(){
    int T; // 테스트케이스 개수
    scanf("%d", &T);
    long long int arr[10] = {0, }; // 정답 저장해둘 배열
    long long int N; // 페이지 수
    for(int t = 0; t<T; t++){
        scanf("%lld", &N);
        memset(arr, 0, 10*sizeof(long long int)); // 정답 배열 0으로 초기화
        save(1, N, 1, arr); // 정답 배열에 값 저장하는 함수
        print(arr); // 정답 출력
    }
}