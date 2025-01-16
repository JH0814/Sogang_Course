#include<stdio.h>
#include<stdlib.h>
//tree struct
typedef struct node *treePointer;
typedef struct node {
int key;
treePointer parent;
treePointer leftChild, rightChild;
}node;

treePointer root; // root of tree
int size; // variable for save number of node in tree

int search(int item){ // function for search item
    int top = -1;
    // search by stack
    treePointer* stack = (treePointer*)calloc(size, sizeof(treePointer)); // allocate stack
    treePointer ptr = root; // variable for searching
    for(;;){
        for(; ptr; ptr = ptr->leftChild){ // search all left Child
            stack[++top] = ptr; // push to stack
        }
        if(top >= 0){
            ptr = stack[top--]; // pop
        }
        if(!ptr) break; // if end of tree then break
        if(ptr->key == item) { // if item already in tree
            free(stack); // free stack
            return 1; // return true
        }
        ptr = ptr->rightChild; // move to right Child
    }
    free(stack); // free stack
    return 0; // return false
}

void inte_to_string(char* arr, int num){ // int to binary number string
    int idx = 0;
    while(1){
        // int to binary
        arr[idx] = num % 2 + '0';
        num /= 2;
        idx++; // index for save to arr
        if(num == 0){
            break;
        }
    }
    arr[idx] = '\0'; // add null to made to string
}
treePointer insert_loca(){ // function for find last node
    if(size == 0) return NULL; // if size is 0, return NULL
    char path[32]; // variable for save binary string
    treePointer ptr = root; // variable for search tree
    inte_to_string(path, size); // save binary to path
    for(int i = 0; path[i] != '\0'; i++){ // search all binary
        if(path[i] == '1'){ // when bit is 1
            if(ptr->leftChild != NULL)
                ptr = ptr->leftChild; // move to leftChild
        }
        else{
            if(ptr->rightChild != NULL)
                ptr = ptr->rightChild; // move to rightChild
        }
    }
    return ptr; // return ptr;
}
void exchange_up(treePointer node){ // function for exchange to node up
    while(node->parent && node->parent->key < node->key){ // when node's parent is not null and parent key is smaller than node
        // swap each node's key
        int temp = node->key;
        node->key = node->parent->key;
        node->parent->key = temp;
        node = node->parent;
    }
}
void exchange_down(treePointer node){ // function for exchange to node down
    while(1){
        treePointer max = node;
        if(node->leftChild && node->leftChild->key > max->key){ // node's leftChild is not null and bigger than max->key
            max = node->leftChild;
        }
        if(node->rightChild && node->rightChild->key > max->key){// node's rightChild is not null and bigger than max->key
            max = node->rightChild;
        }
        if(max == node) break; // if node is biggest break
        // swap each node's key
        int temp = node->key;
        node->key = max->key;
        max->key = temp;
        node = max;
    }
}
int insert(int item){ // insert node to tree
    if(size != 0){ // if size is not 0
        if(search(item) == 1) return 0; // find already item is in tree
    }
    // made new node
    treePointer newnode = (treePointer)malloc(sizeof(struct node));
    newnode->key = item;
    newnode->leftChild = newnode->parent = newnode->rightChild = NULL;
    if(size == 0){ // when size is 0
        // make node to root
        root = newnode;
        size = 1;
        return 1;
    }
    
    treePointer last = insert_loca(); // save last node's pointer
    newnode->parent = last; // link newnode to last
    size++;
    if(last->leftChild == NULL){ // link left first else right
        last->leftChild = newnode;
    }
    else{
        last->rightChild = newnode;
    }
    exchange_up(newnode); // sort value
    return 1;
}

int delete(){ // delete node from tree
    if(size == 0) return -1; // if size is 0, return -1
    int max = root->key;
    if(size == 1){ // if size is 1, free root
        free(root);
        root = NULL;
        size--;
        return max;
    }
    treePointer last = insert_loca(); // search last node
    root->key = last->key; // exchange value

    // find it is link at left or right
    if(last->parent->rightChild == last){
        last->parent->rightChild = NULL;
    }
    else{
        last->parent->leftChild = NULL;
    }

    free(last); // free memory
    size--;

    exchange_down(root); // sort value
    return max;
}

int main(){
    // open file
    FILE* fp1 = fopen("input1.txt", "r");
    FILE* fp2 = fopen("output1.txt", "w");
    while(1){
        char order;
        int num;
        int flag = 0; // quit flag
        int ins, del;
        fscanf(fp1, "%c", &order); // input order
        switch(order){
            case 'i':
                fscanf(fp1, "%d", &num);
                ins = insert(num); // call insert
                // print result
                if(ins == 0) fprintf(fp2, "Exist number\n");
                else fprintf(fp2, "Insert %d\n", num);
                break;
            case 'd':
                del = delete(); // call delete
                // print result
                if(del == -1) fprintf(fp2, "The heap is empty\n");
                else fprintf(fp2, "Delete %d\n", del);
                break;
            case 'q':
                // change flag
                flag = 1;
                break;
            default:
                continue;
        }
        if(flag == 1) break; // if flag is changed, end loop
    }
    // file close
    fclose(fp1);
    fclose(fp2);
    // free all node
    while(1){
        int del = delete();
        if(del == -1) break;
    }
}