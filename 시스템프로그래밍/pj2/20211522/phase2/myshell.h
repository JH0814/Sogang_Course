#include "csapp.h"
#include<errno.h>

// queue structure for pipeline
typedef struct Node{
    char command[MAXLINE];
    struct Node* next;
}Node;

typedef struct {
    int cnt;
    Node* front;
    Node* rear;
} Queue;

// queue function for pipeline
void initQueue(Queue* q); // queue 초기화화
int isEmpty(Queue* q);
void enqueue(Queue* q, char* command);
void dequeue(Queue* q, char* command);
void printQueue(Queue* q); // 디버깅 시 사용한 큐의 element 전체 출력
void freeQueue(Queue* q);

void eval(char *cmdline);// 입력받은 명령어 라인 판단하는 함수
int parseline(char *buf, char **argv); // 입력받은 명령어 ' '에 따라 나눠서 argv에 저장하는 함수
int builtin_command(char **argv); // fork 외로 실행되는 명령어 처리하는 함수