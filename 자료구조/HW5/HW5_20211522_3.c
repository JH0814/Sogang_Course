#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// tree struct
typedef struct node *treePointer;
typedef struct node {
    int key;
    treePointer leftChild, rightChild;
}node;

treePointer root = NULL; // pointer for root
FILE* fp1, *fp2; // file pointer

treePointer iter_search(treePointer tree, int key){ // function for check key is already in tree
    while(tree){ // until tree is not NULL
        if(key == tree->key) return tree; // when key is same with tree's key then return
        if(key<tree->key){ // move to correct child node
            tree = tree->leftChild;
        }
        else{
            tree = tree->rightChild;
        }
    }
    return NULL; // when there is no key in tree return NULL
}

treePointer modified_search(treePointer tree, int key){ // find node for insert
    if(!tree) return NULL; // if tree is null return NULL
    while(!(tree->leftChild == NULL && tree->rightChild == NULL)){ // until left or right is not null
        if(!tree) break; // if tree is null break
        if(key == tree->key) return NULL; // if already same key is in tree, return NULL
        if(key < tree->key){ // if for move correct child node
            if(tree->leftChild == NULL) break;
            tree = tree->leftChild;
        }
        else{
            if(tree->rightChild == NULL) break;
            tree = tree->rightChild;
        }
    }
    return tree; // return pointer for insert
}

int insert(treePointer* node, int num){ // function for insert node to tree
    treePointer flag = iter_search(*node, num); // flag for num is already in tree
    if(flag != NULL) return 0; // if it is not NULL end function
    treePointer ptr, temp = modified_search(*node, num); // save last node to temp
    if(temp || !(*node)){
        // make newnode
        ptr = (treePointer)malloc(sizeof(struct node));
        ptr->key = num;
        ptr->leftChild = ptr->rightChild = NULL;
        if(*node){
            if(num < temp->key){ // link ptr to correct place
                temp->leftChild = ptr;
            }
            else{
                temp->rightChild = ptr;
            }
        }
        else{
            *node = ptr;
        }
    }
}

treePointer top(){ // return top of priority queue
    treePointer ptr = root;
    // search top until ptr->rightChild is not NULL
    while(ptr->rightChild){
        ptr = ptr->rightChild;
    }
    return ptr; // return ptr
}

treePointer pop(){ // pop top of priority queue
    treePointer ptr = root;
    if(ptr == NULL){ // if root is NULL
        return NULL; // return NULL
    }
    treePointer pre;
    while(ptr->rightChild){ // move to most right node
        pre = ptr;
        ptr = ptr->rightChild;
    }
    if(ptr == root){ // if ptr is root
        root = ptr->leftChild;
        return ptr;
    }
    else if(ptr->leftChild){ // if left is not NULL
        pre->rightChild = ptr->leftChild; // move left to right
        return ptr;
    }
    else{
        pre->rightChild = NULL; // change right to NULL
        return ptr;
    }
}

void free_all(treePointer ptr){ // free all memory
    if(ptr == NULL) return; // if ptr is NULL end function

    free_all(ptr->leftChild); // recursive call
    free_all(ptr->rightChild);
    free(ptr); // free memory after recursive call
}

int main(){
    // file open
    fp1 = fopen("input3.txt", "r");
    fp2 = fopen("output3.txt", "w");
    while(1){
        char order[5];
        int num;
        int flag = 0;
        fscanf(fp1, "%s", order); // input order from file
        if(strcmp(order, "push") == 0){ // when order is push
            fscanf(fp1, "%d", &num); // input num from file
            int res = insert(&root, num); // call insert
            // print result
            if(res == 0) fprintf(fp2, "Exist number\n");
            else{
                fprintf(fp2, "Push %d\n", num);
            }
        }
        else if(strcmp(order, "pop") == 0){ // when order is pop
            treePointer temp = pop(); // call pop
            // print result
            if(temp == NULL) fprintf(fp2, "The queue is empty\n");
            else{
                fprintf(fp2, "Pop %d\n", temp->key);
                free(temp); // free poped memory
            }
        }
        else if(strcmp(order, "top") == 0){ // when order is top
            treePointer temp = top(); // call top
            // print result
            if(temp == NULL) fprintf(fp2, "The queue is empty\n");
            else{
                fprintf(fp2, "The top is %d\n", temp->key);
            }
        }
        else if(strcmp(order, "q") == 0){ // when order is q
            flag = 1; // change flag
        }
        else{
            continue;
        }
        if(flag) break; // when flag is changed, end loop
    }
    // close file
    fclose(fp1);
    fclose(fp2);
    free_all(root); // free all memory
}