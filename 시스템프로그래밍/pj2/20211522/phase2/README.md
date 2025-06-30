[system programming lecture]

-project 2 phase 2

csapp.{c,h}
        CS:APP3e functions

myshell.c
        myshell
실행 방법 : make로 실행파일 만든 후 ./myshell 로 실행(csapp.c, csapp.h를 포함해야 함)
내용 : 단일 명령어들을 실행하는 shell에서 pipeline이 추가되었다. 입력된 명령어를 띄어쓰기를 기준으로 parsing한 후에 fork로 child process에서 execvp로 실행한다. execvp로 실행할 수 없는 exit이나 cd 등은 builtin_command로 따로 실행한다. exit이나 quit를 입력받으면 shell을 종료하며 그 전까지는 계속해서 명령어를 받는다. 이 때 명령어들을 |로 연결하여 전달 시에 이전 명령어의 출력값을 다음 명령어의 입력값으로 전달한다.