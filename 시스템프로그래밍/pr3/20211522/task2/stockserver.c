/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"
#define SBUFSIZE 16
#define NTHREADS 100

void echo(int connfd);
sem_t mutex;

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

typedef struct {
    int *buf;        /* Buffer array */
    int n;           /* Maximum number of slots */
    int front;       /* buf[(front+1)%n] is first item */
    int rear;        /* buf[rear%n] is last item */
    sem_t mutex;     /* Protects accesses to buf */
    sem_t slots;     /* Counts available slots */
    sem_t items;     /* Counts available items */
} sbuf_t;
sbuf_t sp;

void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;                      /* Buffer holds max of n items */
    sp->front = sp->rear = 0;      /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1);    /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);    /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0);    /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}

/* Insert item onto the rear of shared buffer sp */
void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);                                     /* Wait for available slot */
    P(&sp->mutex);                                     /* Lock the buffer */
    sp->buf[(++sp->rear) % (sp->n)] = item;            /* Insert the item */
    V(&sp->mutex);                                     /* Unlock the buffer */
    V(&sp->items);                                     /* Announce available item */
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                                     /* Wait for available item */
    P(&sp->mutex);                                     /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)];           /* Remove the item */
    V(&sp->mutex);                                     /* Unlock the buffer */
    V(&sp->slots);                                     /* Announce available slot */
    return item;
}

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

void trade(char* buf){ // executing buy and sell command or print invalid command
    if(strncmp(buf, "buy ", 4) == 0){ // buy command
        int id, cnt;
        sscanf(buf, "buy %d %d\n", &id, &cnt);
        P(&mutex);
        Node* target = search(id);
        if(target && target->obj->left_stock >= cnt){
            target->obj->left_stock -= cnt;
            strcpy(buf, "[buy] success\n");
        }
        else{
            strcpy(buf, "Not enough left stock\n");
        }
        V(&mutex);
    }
    else if(strncmp(buf, "sell ", 5) == 0){ // sell command
        int id, cnt;
        sscanf(buf, "sell %d %d\n", &id, &cnt);
        P(&mutex);
        Node* target = search(id);
        if(target){
            target->obj->left_stock += cnt;
            strcpy(buf, "[sell] success\n");
        }
        else{
            buf[0] = '\0';
        }
        V(&mutex);
    }
    else{
        P(&mutex);
        strcpy(buf, "invalid command\n");
        V(&mutex);
    }
}

static void init_echo_cnt(void){
    Sem_init(&mutex, 0, 1);
}

void echo_cnt(int connfd){
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    Pthread_once(&once, init_echo_cnt);
    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0){
        P(&mutex);
        printf("server received %d bytes\n", n);
        V(&mutex);
        if(strcmp(buf, "show\n") == 0){ // show command
            P(&mutex);
            buf[0] = '\0';
            show(root, buf);
            V(&mutex);
        }
        else if(strncmp(buf, "exit\n", 5) == 0){ // exit command
            P(&mutex);
            buf[0] = '\0';
            save_data();
            V(&mutex);
            return;
        }
        else{ // buy, sell or non-exist command
            trade(buf);
        }
        Rio_writen(connfd, buf, MAXLINE); // write response
    }
}


void *thread(void* vargp){
    Pthread_detach(pthread_self()); 
    while(1){
        int connfd = sbuf_remove(&sp);
        echo_cnt(connfd);
        Close(connfd);
    }
    
}


int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];
    pthread_t tid;

    // handle SIGINT
    Signal(SIGINT, sigint_handler);

    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    make_tree();
    sbuf_init(&sp, SBUFSIZE);
    for (int i = 0; i < NTHREADS; i++){ /* Create worker threads */
        Pthread_create(&tid, NULL, thread, NULL);
    }
    while (1) {
        clientlen = sizeof(struct sockaddr_storage); 
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sp, connfd); /* Insert connfd in buffer */
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
    }
    exit(0);
}
/* $end echoserverimain */
