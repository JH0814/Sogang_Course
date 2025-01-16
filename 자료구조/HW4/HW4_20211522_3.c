#include<stdio.h>
#include<stdlib.h>
// array for direction
int row_dir[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int col_dir[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int map[10000][10000]; // array for saving maze
int mark[10000][10000]; // array for saving visited
int EXIT_ROW, EXIT_COL; // variable for saving exit
// node struct
typedef struct node* node_pointer;
typedef struct node{
    node_pointer llink;
    int row, col, dir;
    node_pointer rlink;
}node;
node_pointer top = NULL; // stack's top
int isEmpty(){ // return 1 or 0 according to stack's elements
    return !top;
}
void push(int row, int col, int dir){ // push item to stack
    // declare node that made new
    node_pointer temp = (node_pointer)malloc(sizeof(node));
    temp->row = row;
    temp->col = col;
    temp->dir = dir;
    if(top == NULL){ // if temp is first element
        temp->rlink = NULL;
        temp->llink = NULL;
        top = temp;
    }
    else{ // other case
        temp->rlink = top; // link to existing top
        top->llink = temp; // top's llink is temp
        temp->llink = NULL; // temp's llink is NULL because temp is new top
        top = temp; // change top
    }
    
}
node_pointer pop(){ // pop element in stack
    node_pointer temp = top; // save top to temp
    if(isEmpty()){ // if stack is empty
        temp = NULL;
        return temp; // return NULL
    }
    else if(top->rlink != NULL){ 
        top = temp->rlink; // move top to rlink
        top->llink = NULL; // change top's llink to NULL
    }
    else{ // when there is only one element in stack
        top = temp->rlink; // move top to temp->rlink
    }
    return temp;
}
void path(){
    node_pointer position; // variable for saving popped element
    int row, col, dir; // variable for saving location and direction
    int nextRow, nextCol; // variable for saving next location
    int found = 0; // flag for finding exit
    mark[1][1] = 1; // note first move to mark array
    push(1, 1, 1); // push first location
    while(top != NULL && !found){ // when stack is not empty and before exit founded
        position = pop(); // pop element
        //save location to variable
        row = position->row;
        col = position->col;
        dir = position->dir;
        while(dir < 8 && !found){ // until dir < 8 and before exit founded
            // calculate next location
            nextRow = row + row_dir[dir];
            nextCol = col + col_dir[dir];
            if(nextRow == EXIT_ROW && nextCol == EXIT_COL){ // when it entered exit
                found = 1; // change flag
                push(row, col, dir); // push to stack
            }
            else if(!map[nextRow][nextCol] && !mark[nextRow][nextCol]){ // maze is road and not visited
                mark[nextRow][nextCol] = 1; // note visited
                dir++; // increase dir
                push(row, col, dir); // push to stack
                // update location
                row = nextRow;
                col = nextCol;
                dir = 0;
            }
            else{ // other case
                ++dir; // change direction
            }
        }
        free(position); // free memory
    }
    if(found){ // when it can reach to exit
        FILE* fp = fopen("path.txt", "w"); // open file
        node_pointer ptr = top;
        while(ptr->rlink != NULL){ // go to end of list
            ptr = ptr->rlink;
        }
        while(ptr->llink != NULL){ // print list to file upward
            fprintf(fp, "%d %d\n", ptr->row, ptr->col);
            ptr = ptr->llink;
        }
        fprintf(fp, "%d %d\n", ptr->row, ptr->col); // print top of stack
        fprintf(fp, "%d %d\n", EXIT_ROW, EXIT_COL); // print exit
        fclose(fp);
    }
    else{ // when path doesn't exist
        printf("The maze does not have a path\n");
    }
}
int main(){
    FILE* fp = fopen("maze.txt", "r"); // file open for input
    // input maze to map array
    int i = 0, j = 0;
    while(1){
        char c;
        c = fgetc(fp);
        if(c == '1' || c == '0'){
            map[i][j++] = c - '0';
        }
        else if(c == '\n'){
            j = 0;
            i++;
        }
        else if(c == EOF){
            break;
        }
    }
    fclose(fp);
    // save exit
    EXIT_ROW = i-1;
    EXIT_COL = j-2;
    path(); // call path()
    // free stack
    node_pointer pre, ptr = top;
    while(ptr != NULL){
        pre = ptr;
        ptr = ptr->rlink;
        free(pre);
    }
    free(ptr);
}