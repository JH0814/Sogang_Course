#include<stdio.h>
#include<stdlib.h>
//tree struct
typedef struct node *treePointer;
typedef struct node {
    int key;
    treePointer leftChild, rightChild;
}node;

treePointer root = NULL; // pointer for tree's root
FILE* fp1, *fp2; // FILE pointer

treePointer iter_search(treePointer tree, int key){ // search tree for key is already in tree
    while(tree){ // until tree is not null
        if(key == tree->key) return tree; // if tree-key is same with key, return
        if(key<tree->key){ // if for search tree
            tree = tree->leftChild;
        }
        else{
            tree = tree->rightChild;
        }
    }
    return NULL; // else return NULL
}

treePointer modified_search(treePointer tree, int key){ // find last node for insert
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
void inorder(treePointer ptr){ // function for print inorder
    if(ptr){
        inorder(ptr->leftChild);
        fprintf(fp2, "%d ", ptr->key);
        inorder(ptr->rightChild);
    }
}
void postorder(treePointer ptr){ // fucntion for print postorder
    if(ptr){
        postorder(ptr->leftChild);
        postorder(ptr->rightChild);
        fprintf(fp2, "%d ", ptr->key);
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
    fp1 = fopen("input2.txt", "r");
    fp2 = fopen("output2.txt", "w");
    int n, key;
    fscanf(fp1, "%d", &n); // input n from file
    for(int i = 0; i<n; i++){
        fscanf(fp1, "%d", &key); // input key from file
        if(root == NULL){ // when root is null
            // make root node
            root = (treePointer)malloc(sizeof(node));
            root->key = key;
            root->leftChild = root->rightChild = NULL;
        }
        else{
            int flag = insert(&root, key); // call insert
            if(flag == 0){ // when same value is exist
                fprintf(fp2, "cannot construct BST\n");
                fclose(fp1);
                fclose(fp2);
                return 0;
            }
        }
    }
    fclose(fp1);
    // print inorder
    fprintf(fp2, "Inorder: ");
    inorder(root);
    fprintf(fp2, "\n");
    // print postorder
    fprintf(fp2, "Postorder: ");
    postorder(root);
    fprintf(fp2, "\n");
    fclose(fp2);
    free_all(root); // free all memory
}