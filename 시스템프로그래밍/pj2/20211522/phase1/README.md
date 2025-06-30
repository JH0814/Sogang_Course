[system programming lecture]

-project 2 phase 1

csapp.{c,h}
        CS:APP3e functions

myshell.c
        myshell
실행 방법 : make로 실행파일 만든 후 ./myshell 로 실행(csapp.c, csapp.h를 포함해야 함)
내용 : 단일 명령어들을 실행하는 shell이다. 입력된 명령어를 띄어쓰기를 기준으로 parsing한 후에 fork로 child process에서 execvp로 실행한다. execvp로 실행할 수 없는 exit이나 cd 등은 builtin_command로 따로 실행한다. exit이나 quit를 입력받으면 shell을 종료하며 그 전까지는 계속해서 명령어를 받는다.