/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd);

typedef struct item{
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex;
}item;

typedef struct node{
    item* obj;
    struct node* left;
    struct node* right;
}Node;
Node* root = NULL;

typedef struct pool{
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
    int maxi;
    int clientfd[FD_SETSIZE];
    rio_t clientrio[FD_SETSIZE];
}pool;


void make_tree(){ // make initial binary tree
    FILE* fp = fopen("stock.txt", "r");
    while(1){
        item* newitem = (item*)malloc(sizeof(item));
        if(fscanf(fp, "%d %d %d", &(newitem->ID), &(newitem->left_stock), &(newitem->price)) == EOF){ // input until file ends
            free(newitem);
            break;
        }
        Node* newnode = (Node*)malloc(sizeof(Node));
        newnode->obj = newitem;
        newnode->left = newnode->right = NULL;
        if(root == NULL){
            root = newnode;
        }
        else{
            Node* ptr = root;
            while(1){
                if(ptr->obj->ID < newitem->ID){
                    if(!ptr->right){
                        ptr->right = newnode;
                        break;
                    }
                    ptr = ptr->right;
                }
                else{
                    if(!ptr->left){
                        ptr->left = newnode;
                        break;
                    }
                    ptr = ptr->left;
                }
            }
        }
    }
    fclose(fp);
} 

Node* search(int ID) { // search Node in binary tree
    Node* ptr = root;
    while (ptr) {
        if (ptr->obj->ID < ID) {
            ptr = ptr->right;
        } else if (ptr->obj->ID > ID) {
            ptr = ptr->left;
        } else {
            return ptr;
        }
    }
    return NULL;
}

void show(Node* ptr, char* buf){ // show command executing
    if(!ptr){
        return;
    }
    show(ptr->left, buf);
    char info[64];
    sprintf(info, "%d %d %d\n", ptr->obj->ID, ptr->obj->left_stock, ptr->obj->price);
    strcat(buf, info);
    show(ptr->right, buf);
}

void save_data(){ // save stock data to "stock.txt"
    FILE* fp = fopen("stock.txt", "w");
    if(fp == NULL) return;
    char buf[MAXLINE] = "\0";
    show(root, buf);
    fprintf(fp, "%s", buf);
    fclose(fp);
}

void free_tree(Node* node) { // free memory allocated in tree
    if (node == NULL) return;

    free_tree(node->left);
    free_tree(node->right);

    free(node->obj);
    free(node);
}

void sigint_handler(int sig){ // when SIGINT occur, free memory of tree and end program
    free_tree(root);
    exit(0);
}

void init_pool(int listenfd, pool* p){
    int i;
    p->maxi = -1;
    for(i = 0; i<FD_SETSIZE; i++){
        p->clientfd[i] = -1;
    }
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool* p){
    int i;
    p->nready--;
    for(i = 0; i<FD_SETSIZE; i++){
        if(p->clientfd[i] < 0){
            p->clientfd[i] = connfd;
            Rio_readinitb(&p->clientrio[i], connfd);

            FD_SET(connfd, &p->read_set);

            if(connfd > p->maxfd){
                p->maxfd = connfd;
            }
            if(i > p->maxi){
                p->maxi = i;
            }
            break;
        }
    }
    if(i == FD_SETSIZE){
        app_error("add client error: Too many clients");
    }
}


void trade(char* buf){ // executing buy and sell command or print invalid command
    if(strncmp(buf, "buy ", 4) == 0){
        int id, cnt;
        sscanf(buf, "buy %d %d\n", &id, &cnt);
        Node* target = search(id);
        if(target && target->obj->left_stock >= cnt){ 
            target->obj->left_stock -= cnt;
            strcpy(buf, "[buy] success\n");
        }
        else{
            strcpy(buf, "Not enough left stock\n");
        }
    }
    else if(strncmp(buf, "sell ", 5) == 0){
        int id, cnt;
        sscanf(buf, "sell %d %d\n", &id, &cnt);
        Node* target = search(id);
        if(target){
            target->obj->left_stock += cnt;
            strcpy(buf, "[sell] success\n");
        }
        else{
            buf[0] = '\0';
        }
    }
    else{
        strcpy(buf, "invalid command\n");
    }
}

void check_clients(pool* p){ // check client's input and response command
    int i, connfd, n;
    char buf[MAXLINE];
    rio_t rio;

    for(i = 0; (i <= p->maxi) && (p->nready > 0); i++){
        connfd = p->clientfd[i];
        rio = p->clientrio[i];

        if((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))){
            p->nready--;
            n = rio_readlineb(&rio, buf, MAXLINE);
            if(n > 0){
                printf("server received %d bytes\n", n);
                if(strcmp(buf, "show\n") == 0){ // show command
                    buf[0] = '\0';
                    show(root, buf);
                }
                else if(strncmp(buf, "exit\n", 5) == 0){ // exit command
                    buf[0] = '\0';
                    Close(connfd);
                    FD_CLR(connfd, &p->read_set);
                    p->clientfd[i] = -1;
                    return;
                }
                else{ // buy, sell or non-exist command
                    trade(buf);
                }
                Rio_writen(connfd, buf, MAXLINE); // write response
            }
            else{
                Close(connfd);
                FD_CLR(connfd, &p->read_set);
                p->clientfd[i] = -1;
            }
        }
    }
}


int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    static pool pool;

    // handle SIGINT
    Signal(SIGINT, sigint_handler);

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    make_tree();
    init_pool(listenfd, &pool);
    while (1) {
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);
        if(FD_ISSET(listenfd, &pool.ready_set)){
            clientlen = sizeof(struct sockaddr_storage); 
	        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            add_client(connfd, &pool);
            Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
            printf("Connected to (%s, %s)\n", client_hostname, client_port);
        }
	    check_clients(&pool);
        int check_flag = 0; // if flag is 1, there is client connected to server
        for(int i = 0; i<=pool.maxi; i++){ // check there is any-client connected
            if(pool.clientfd[i] > 0){
                check_flag = 1;
                break;
            }
        }
        if(!check_flag){ // save data when there is no client in server
            save_data();
        }
    }
    exit(0);
}
/* $end echoserverimain */
