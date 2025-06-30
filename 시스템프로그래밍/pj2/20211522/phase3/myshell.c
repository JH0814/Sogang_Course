#include "myshell.h"
#define MAXARGS 128

int pid_cnt = 1;
job job_arr[MAXARGS];
job f_arr;

void sigtstp_handler(int sig){
    int olderrno = errno;
    pid_t pid;
    sigset_t mask, prev;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    sio_puts("[");
    sio_putl(pid_cnt);
    sio_puts("] Stopped ");
    Sio_puts(f_arr.command);
    // foreground process 추가
    f_arr.job_id = pid_cnt;
    job_arr[pid_cnt] = f_arr;
    pid_cnt++;
    for (int i = 1; i < pid_cnt; i++) {
        if (job_arr[i].state == 0) {  // Foreground job found
            Kill(job_arr[i].pid, SIGTSTP);  // Send SIGTSTP to the job
            job_arr[i].state = -1;  // Mark job as stopped
            break;
        }
    }
    Sigprocmask(SIG_SETMASK, &prev, NULL);
    errno = olderrno;
}
void sigchld_handler(int sig){
    int olderrno = errno;
    pid_t pid;
    sigset_t mask, prev;
    Sigfillset(&mask);

    while(1){
        pid = waitpid(-1, NULL, WNOHANG);
        if (pid <= 0) { // sigchld에 대해서는 process가 없는 경우에 따라서 예외처리를 하였다.
            if (pid == 0 || errno == ECHILD)
                break;
            else
                perror("waitpid error");
        } else {
            Sigprocmask(SIG_BLOCK, &mask, &prev);
            for(int i = 1; i < pid_cnt; i++){
                if(job_arr[i].pid == pid && job_arr[i].state == 1){ // background인 pid를 갖는 process를 완료처리함
                    job_arr[i].state = -7;
                    break;
                }
            }
            Sigprocmask(SIG_SETMASK, &prev, NULL);
        }
    }

    errno = olderrno;
}
void sigint_handler(int sig){
    int olderrno = errno;
    pid_t pid;
    sigset_t mask, prev;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev);
    f_arr.job_id = pid_cnt;
    job_arr[pid_cnt] = f_arr;
    pid_cnt++;
    for(int i = 1; i<pid_cnt; i++){
        if(job_arr[i].state == 0){ // foreground process를 찾아서 SIGINT로 종료
            Kill(job_arr[i].pid, SIGINT);
            job_arr[i].state = -5;
        }
    }
    Sigprocmask(SIG_SETMASK, &prev, NULL);
    errno = olderrno;
}

void initQueue(Queue* q) { // queue 초기화
    q->front = q->rear = NULL;
    q->cnt = 0;
}

int isEmpty(Queue* q) { // queue가 비었는지 확인
    return q->front == NULL;
}

void enqueue(Queue* q, char* command) { // queue에 element 추가
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("malloc failed");
        exit(1);
    }
    strcpy(newNode->command, command);
    if(newNode->command[strlen(newNode->command) - 1] == '\n'){ // 마지막에 개행문자 존재 시 띄어쓰기로 바꿔줌(parseline 작동에 문제 없도록)
        newNode->command[strlen(newNode->command) - 1] = ' ';
    }
    newNode->next = NULL;
    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->cnt++;
}
void dequeue(Queue* q, char* command) { // queue에서 element 제거 후 인자에 받은 문자열에 복사
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }

    Node* temp = q->front;
    strcpy(command, temp->command);
    q->front = q->front->next;
    q->cnt--;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

void freeQueue(Queue* q) { // queue의 element 전부 제거
    while (!isEmpty(q)) {
        char command[MAXLINE];
        dequeue(q, command);
    }
}

void printQueue(Queue* q) { // queue의 element 전부 출력
    Node* current = q->front;
    while (current) {
        printf("%s -> ", current->command);
        current = current->next;
    }
    printf("NULL\n");
}

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execvp() */
    char buf[MAXLINE];   /* Holds modified command line */
    char buf2[MAXLINE];
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    Queue q; // pipeline을 위한 명령어를 저장할 queue
    initQueue(&q); // queue 초기화화
    strcpy(buf, cmdline);
    strcpy(buf2, cmdline);
    // |를 기준으로 구분하여 queue에 삽입
    char* temp = strtok(buf2, "|");
    while(temp){
        enqueue(&q, temp);
        temp = strtok(NULL, "|");
    }
    int tot = q.cnt;
    if(tot == 1){
        freeQueue(&q); // |가 없을 때는 queue 필요없으므로 제거
        bg = parseline(buf, argv);
        if (argv[0] == NULL)  
	        return;   /* Ignore empty lines */
        if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
            pid = Fork();
            if(pid < 0){// Fork error 시 표시 후 종료
                printf("fork error: %s\n", strerror(errno));
                exit(0);
            }
            if(pid == 0){// 자식 프로세스일 때 명령어 실행
                if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                }
            }
    
            /* Parent waits for foreground job to terminate */
            if (!bg){ 
                int status;
                f_arr.pid = pid;
                strcpy(f_arr.command, cmdline);
                f_arr.state = 0;
                f_arr.job_id = 0;
                if (Waitpid(pid, &status, 0 | WUNTRACED) < 0) {
                    perror("waitpid error (pipeline)");
                }
            }
            else{//when there is backgrount process!
                // job_arr에 추가
                job_arr[pid_cnt].pid = pid;
                strcpy(job_arr[pid_cnt].command, cmdline);
                job_arr[pid_cnt].state = 1;
                job_arr[pid_cnt].job_id = pid_cnt;
                printf("[%d] %d\n", pid_cnt, job_arr[pid_cnt].pid);
                pid_cnt++;
            }
        }
    }
    else{
        int fd[tot][2]; // variable for file descriptor
        pid_t pid_arr[tot]; // pid를 저장할 variable
        for(int i = 0; i<tot; i++){ // 총 개수만큼 for 순회
            char command[MAXLINE];
            dequeue(&q, command); // command를 queue에서 얻어옴
            if(pipe(fd[i]) < 0){
                printf("pipe error!\n");
            }
            bg = parseline(command, argv);   
            if (argv[0] == NULL) {
                continue;
            }
            if (!builtin_command(argv)) { //quit -> exit(0), & -> ignore, other -> run
                pid_arr[i] = Fork();
                if(pid_arr[i] < 0){
                    printf("fork error: %s\n", strerror(errno));
                    exit(0);
                }
                if(pid_arr[i] == 0){ // 자식 프로세스일 때 명령어 수행
                    if(i == 0){ // pipeline의 첫 명령어일 때는 처음의 읽어오는 fd를 쓰지 않음
                        Close(fd[i][0]);
                        Dup2(fd[i][1], STDOUT_FILENO);
                        Close(fd[i][1]);
                    }
                    else if(i == tot - 1){ // pipeline의 마지막은 쓰는 fd를 쓰지 않음
                        Close(fd[i - 1][1]);
                        Dup2(fd[i - 1][0], STDIN_FILENO);
                        Close(fd[i - 1][0]);
                    }
                    else{ // 평소에는 앞의 fd에서 읽어오고 다음 fd에 쓰는 방식
                        Close(fd[i - 1][1]);
                        Dup2(fd[i - 1][0], STDIN_FILENO);
                        Close(fd[i - 1][0]);
                        Close(fd[i][0]);
                        Dup2(fd[i][1], STDOUT_FILENO);
                        Close(fd[i][1]);
                    }
                    if (execvp(argv[0], argv) < 0) {	//ex) /bin/ls ls -al &
                        printf("%s: Command not found.\n", argv[0]);
                        exit(0);
                    }
                }
            }
            if(i > 0){
                Close(fd[i - 1][0]);
                Close(fd[i - 1][1]);
            }
            if (!bg && i == tot - 1){ 
                int status;
                f_arr.pid = pid_arr[i];
                strcpy(f_arr.command, cmdline);
                f_arr.state = 0;
                f_arr.job_id = 0;
                if (Waitpid(pid_arr[i], &status, 0 | WUNTRACED) < 0) {
                    perror("waitpid error (pipeline)");
                }
            }
            else if(i == tot - 1){//when there is backgrount process!
                job_arr[pid_cnt].pid = pid_arr[i];
                strcpy(job_arr[pid_cnt].command, cmdline);
                job_arr[pid_cnt].state = 1;
                job_arr[pid_cnt].job_id = pid_cnt;
                printf("[%d] %d\n", pid_cnt, job_arr[pid_cnt].pid);
                pid_cnt++;
            }
        }
        Close(fd[tot - 1][0]);
        Close(fd[tot - 1][1]);
        freeQueue(&q); // queue의 element 모두 제거
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
    
    if(!strcmp(argv[0], "jobs")){ // job command
        sigset_t mask_all, prev;
        Sigfillset(&mask_all);
        Sigprocmask(SIG_BLOCK, &mask_all, &prev);
        for(int i = 1; i<pid_cnt; i++){ // -5, 즉 정상 종료 후 한 번 출력되었거나 SIGINT로 종료된 process들 제거
            if(job_arr[i].state == -5){
                for(int j = i; j<pid_cnt; j++){
                    job_arr[j] = job_arr[j + 1];
                }
                pid_cnt--;
                i--;
            }
        }
        for(int i = 1; i<pid_cnt; i++){ // 1은 background에서 running, -1은 SIGTSTP에 의해 멈춘 상태, -7은 정상 종료된 상태
            if(job_arr[i].state == 1){
                printf("[%d] %s %s",job_arr[i].job_id, "Running", job_arr[i].command);
            }
            else if(job_arr[i].state == -1){
                printf("[%d] %s %s",job_arr[i].job_id, "Stopped", job_arr[i].command);
            }
            else if(job_arr[i].state == -7){
                job_arr[i].state = -5;
                printf("[%d] %s %s",job_arr[i].job_id, "Done", job_arr[i].command);
            }
        }
        Sigprocmask(SIG_SETMASK, &prev, NULL);
        return 1;
    }
    if(!strcmp(argv[0], "kill")){ // kill command
        if(argv[1][0] == '%'){
            int number = atoi(argv[1] + 1);
            for(int i = 1; i<pid_cnt; i++){
                if(job_arr[i].job_id == number){
                    Kill(job_arr[i].pid, SIGKILL);
                    for(int j = i; j<pid_cnt; j++){
                        job_arr[j] = job_arr[j + 1];
                    }
                    pid_cnt--;
                    break;
                }
            }
        }
        return 1;
    }
    if(!strcmp(argv[0], "bg")){ // bg command
        if(argv[1][0] == '%'){
            int number = atoi(argv[1] + 1);
            for(int i = 1; i<pid_cnt; i++){
                if(job_arr[i].job_id == number){
                    Kill(job_arr[i].pid, SIGCONT);
                    job_arr[i].state = 1;
                    break;
                }
            }
        }
        return 1;
    }
    if(!strcmp(argv[0], "fg")){ // fg command
        if(argv[1][0] == '%'){
            int number = atoi(argv[1] + 1);
            for(int i = 1; i<pid_cnt; i++){
                if(job_arr[i].job_id == number){
                    Kill(job_arr[i].pid, SIGCONT); // foreground로 살리는 process
                    job_arr[i].state = 0;
                    Waitpid(job_arr[i].pid, NULL, 0); // 이 떄는 foreground이므로 부모 process가 종료까지 기다려야 함
                    for(int j = i; j<pid_cnt; j++){ // 종료 후 job list에서 삭제
                        job_arr[j] = job_arr[j + 1];
                    }
                    pid_cnt--;
                    break;
                }
            }
        }
        return 1;
    }
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

    while (*ptr && *ptr == ' ') ptr++; // 맨 앞의 띄어쓰기 제거
    char *end = buf + strlen(buf) - 1;
    while (end > buf && (*end == ' ' || *end == '\t')) end--; // 끝 공백 무시

    if (*end == '&') {
        bg = 1;
        *end = '\0'; // & 제거
    }
    while (*ptr) {
        while (*ptr && *ptr == ' ') ptr++;
        if (*ptr == '\0') break; // NULL일 때 반복문 종료

        if (*ptr == '"' || *ptr == '\'') { // 따옴표 처리 위한 처리
            char quote = *ptr; // 같은 따옴표임을 확인하기 위한 저장
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
    return bg;
}
/* $end parseline */



int main(){
    char cmdline[MAXLINE]; /* Command line */

    Signal(SIGTSTP, sigtstp_handler);
    Signal(SIGINT, sigint_handler);
    Signal(SIGCHLD, sigchld_handler);

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