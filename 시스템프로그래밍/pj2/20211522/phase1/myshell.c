#include "myshell.h"
#define MAXARGS 128

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execvp() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	    return;   /* Ignore empty lines */
    if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
        pid = Fork();
        if(pid < 0){ // Fork error 시 표시 후 종료
            printf("fork error: %s\n", strerror(errno));
            exit(0);
        }
        if(pid == 0){ // 자식 프로세스일 때 명령어 실행
            if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

	    /* Parent waits for foreground job to terminate */
	    if (!bg){ 
	        int status;
            if(Waitpid(pid, &status, 0) < 0){ // 부모 프로세스는 자식 프로세스 종료까지 대기하도록 함
                unix_error("eval: Waitpid error");
            }
	    }
	    else//when there is backgrount process!
	        printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "exit")) /* exit command */ // exit에 대해서도 실행하도록 하였음
	    exit(0);  
    if (!strcmp(argv[0], "quit")) /* quit command */
	    exit(0);  
    if (!strcmp(argv[0], "cd")){ /* cd command */
        int ch;
        if(argv[1] == NULL || argv[1][0] == '~'){ // Home directory로 이동하는 경우 구분
            ch = chdir(getenv("HOME"));
        }
        else{
            ch = chdir(argv[1]);
        }
        if(ch != 0){
            printf("cd error!\n");
        }
        return 1;
    }
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
	    return 1;
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *ptr = buf;
    int argc = 0;
    int bg = 0;

    if (buf[strlen(buf)-1] == '\n') { // 개행문자 제거
        buf[strlen(buf)-1] = ' ';
    }

    while (*ptr && *ptr == ' ') ptr++; // 문자열 맨 앞의 ' ' 제거

    while (*ptr) {
        while (*ptr && *ptr == ' ') ptr++;
        if (*ptr == '\0') break; // NULL 만나면 종료

        if (*ptr == '"' || *ptr == '\'') { // 따옴표 구분을 위한 처리
            char quote = *ptr; // 같은 기호일 때 지우기 위한 저장
            ptr++;
            argv[argc++] = ptr;
            while (*ptr && *ptr != quote) ptr++;
            if (*ptr == quote) {
                *ptr = '\0';
                ptr++;
            }
        } else {
            argv[argc++] = ptr;
            while (*ptr && *ptr != ' ') ptr++;
            if (*ptr) {
                *ptr = '\0';
                ptr++;
            }
        }
    }

    argv[argc] = NULL;

    if (argc == 0)
        return 1;

    if ((bg = (strcmp(argv[argc-1], "&") == 0)) != 0)
        argv[--argc] = NULL;

    return bg;
}
/* $end parseline */

int main(){
    char cmdline[MAXLINE]; /* Command line */

    while (1) {
	    /* Read */
	    printf("CSE4100-SP-P2> ");                   
	    char* in = fgets(cmdline, MAXLINE, stdin); 
	    if (feof(stdin))
	        exit(0);

	    /* Evaluate */
	    eval(cmdline);
    } 
}