#include "csapp.h"
#include<errno.h>

void eval(char *cmdline); // 입력받은 명령어 라인 판단하는 함수
int parseline(char *buf, char **argv); // 입력받은 명령어 ' '에 따라 나눠서 argv에 저장하는 함수
int builtin_command(char **argv); // fork 외로 실행되는 명령어 처리하는 함수