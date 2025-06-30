[system programming lecture]

-project 2 phase 3

csapp.{c,h}
        CS:APP3e functions

myshell.c
        myshell
실행 방법 : make로 실행파일 만든 후 ./myshell 로 실행(csapp.c, csapp.h를 포함해야 함)
내용 : phase 2에서 background 명령어를 실행할 수 있는 기능이 추가되었다. 명령어 뒤에 &를 붙여서 background로 실행할 수 있고, builtin_command에 jobs, bg, fg, kill이 추가되었다. jobs는 background process의 상태를 포함하여 목록으로 출력한다. bg와 fg는 각각 중지되었던 background process를 background에서 foreground에서 running으로 바꿔준다. kill은 process에 SIGKILL을 보내 종료시킨다. bg, fg, kill은 % 뒤에 jobs에서 확인한 job id를 붙여 보내서 대상을 정할 수 있다.