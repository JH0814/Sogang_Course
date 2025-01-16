#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int N, M; // WIDTH, HEIGHT 저장
int* hori, *vert; // 한 줄의 벽 여부 저장
int* map; // 미로 내 칸 별 set 숫자 저장
int set_num = 0; // 현재 set 숫자
FILE* fp; // file 변수
void vert_print(){ // 세로 벽 출력하는 함수
    fprintf(fp, "|"); // 외벽
    for(int i = 0; i<N - 1; i++){
        fprintf(fp, " ");
        if(vert[i] == 1){ // vert 배열 1이면 출력
            fprintf(fp, "|");
        }
        else{
            fprintf(fp, " "); // 아닐 때는 공백
        }
    }
    fprintf(fp, " |\n"); // 외벽 출력 후 줄바꿈
}
void vert_line(){ // 세로 벽 배열에 저장하는 함수
    int line_flag; // 벽의 여부를 결정할 랜덤이 될 flag
    int pre_set_num;
    for(int i = 0; i<N - 1; i++){ // i+1과 비교하므로 N-1까지 탐색
        if(map[i] != map[i + 1]){ // 다음 칸과 다르면
            line_flag = rand() % 2; // 랜덤으로 벽 여부 결정
            if(line_flag){ // 1일 경우에
                pre_set_num = map[i + 1]; // 우선 map[i+1]의 set 숫자 저장
                map[i + 1] = map[i]; // map[i]로 바꿔줌
                for(int j = 0; j<N; j++){ // map[i + 1]과 같은 값 모두 통일시켜줌
                    if(map[j] == pre_set_num){
                        map[j] = map[i];
                    }
                }
                vert[i] = 0; // 벽 아니게 함
            }
            else{
                vert[i] = 1; // 이외에는 모두 벽
            }
        }
        else{
            vert[i] = 1; // 이외에는 모두 벽
        }
    }
    vert_print(); // 벽 출력
}
void hori_line(){ // 수평 벽 여부 결정
    int pre_set_num = map[0]; // 제일 처음 것으로 우선 저장
    int flag = 0; // 같은 집합 당 최소 하나는 있는지 판별할 flag
    for(int i = 0; i<N; i++){ // 한 줄 모두 탐색
        hori[i] = rand() % 2; // 랜덤으로 가로 벽 결정
        if(hori[i] == 0){ // 길이 되면 flag 1로 변경
            flag = 1;
        }
        if(i < N - 1){ // 끝을 제외한 칸에서는
            if(pre_set_num != map[i + 1]) hori[i] = 0; // set 숫자가 달라지는 지점에서 길이 하나도 없으면
            else flag = 0; // flag 초기화
            pre_set_num = map[i + 1]; // set 숫자 update
        }
        if(i == N - 1 && !flag){ // 마지막까지 길이 안 만들어지면 길 만들기
            hori[i] = 0;
        }
        if(hori[i]){ // 벽이 있을 때
            map[i] = set_num; // set update
            set_num++;
        }
    }
    // 가로 벽 출력
    fprintf(fp, "+");
    for(int i = 0; i<N; i++){
        if(hori[i]){
            fprintf(fp, "-+");
        }
        else{
            fprintf(fp, " +");
        }
    }
    fprintf(fp, "\n");
}
void maze_gene(){
    // 첫 줄은 모두 출력
    for(int i = 0; i<N; i++){
        fprintf(fp, "+-");
    }
    fprintf(fp, "+\n");
    srand((unsigned int)time(NULL)); // 랜덤 사용 위한 설정
    // 첫 번째 세로 벽은 모두 랜덤으로 설정
    int pre = -1;
    for(int i = 0; i<N; i++){
        vert[i] = rand() % 2; // 랜덤으로 벽 여부 결정
        if(vert[i]){
            for(int j = pre + 1; j<=i; j++){ // set 숫자 이어지는 칸은 동일하게 설정
                map[j] = set_num;
            }
            pre = i;
            set_num++;
        }
    }
    // 나머지 set의 숫자 설정
    for(int i = pre + 1; i<N; i++){
        map[i] = set_num;
    }
    set_num++;
    vert_print(); // 첫 번째 세로 벽 출력

    for(int i = 0; i<M - 2; i++){ // 2번째 ~ M -1번째 줄은 반복적이므로 함수 호출로
        hori_line();
        vert_line();
    }
    hori_line(); // 마지막 가로 벽 생성
    // 마지막 세로벽 생성
    int pre_set_num;
    for(int i = 0; i<N - 1; i++){
        if(map[i] != map[i + 1]){
            vert[i] = 0;
            pre_set_num = map[i + 1];
            map[i + 1] = map[i];
            for(int j = 0; j<N; j++){
                if(map[j] == pre_set_num){
                    map[j] = map[i];
                }
            }
        }
        else{
            vert[i] = 1;
        }
    }
    vert_print();
    //마지막 외벽 출력
    for(int i = 0; i<N; i++){
        fprintf(fp, "+-");
    }
    fprintf(fp, "+\n");
}
int main(){
    fp = fopen("maze.maz", "w"); // 파일 열기
    // 가로 세로 입력받기
    printf("width: ");
    scanf("%d", &N);
    printf("height: ");
    scanf("%d", &M);
    // 필요한 배열 할당
    hori = (int*)malloc(sizeof(int) * N);
    vert = (int*)malloc(sizeof(int) * (N - 1));
    map = (int*)malloc(sizeof(int) * N);
    memset(map, 0, sizeof(int)); // map 초기화
    maze_gene(); // 함수 호출
    printf("result file: maze.maz\n");
    // 파일과 메모리 모두 해제
    free(hori);
    free(vert);
    free(map);
    fclose(fp);
}