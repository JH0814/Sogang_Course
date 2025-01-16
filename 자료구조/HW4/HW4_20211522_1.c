#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef enum {head, entry} tagfield; // divide head and node
// struct for node
typedef struct matrix_node *matrix_pointer;
typedef struct entry_node{
    int row;
    int col;
    int value;
}node;
// struct for matrix's node
typedef struct matrix_node{
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    union{
        matrix_pointer next;
        node entry;
    }u;
}matrix_node;

//variable for each head of matrix
matrix_pointer* hdnode;
matrix_pointer* hdnode_t;
matrix_pointer mread(){ // function for input matrix
    FILE* fp = fopen("input.txt", "r"); // open file
    int num_rows, num_cols, num_terms, num_heads;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;
    fscanf(fp, "%d %d %d", &num_rows, &num_cols, &num_terms); // input number of row, col, terms
    num_heads = (num_cols > num_rows) ? num_cols : num_rows; // save row or col much bigger
    hdnode = (matrix_pointer*)malloc(sizeof(matrix_pointer) * (num_heads + 1)); // allocate node
    node = (matrix_pointer)malloc(sizeof(matrix_node)); // make head node
    // save data to head
    node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;
    node->u.entry.value = num_terms;
    if(!num_heads) node->right = node; // when row and col are 0
    else{
        for(int i = 0; i<num_heads; i++){ // make head to each of row or col
            temp = (matrix_pointer)malloc(sizeof(matrix_node));
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->u.next = temp;
        }
        current_row = 0;
        last = hdnode[0];
        for(int i = 0; i<num_terms; i++){
            fscanf(fp, "%d %d %d", &row, &col, &value); // input each of nonzero block
            if(row > current_row){ // when row is changed
                last->right = hdnode[current_row];
                current_row = row;
                last = hdnode[row];
            }
            temp = (matrix_pointer)malloc(sizeof(matrix_node)); // make new node
            // save data to node
            temp->tag = entry;
            temp->u.entry.row = row; temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp; // link to last node
            last = temp; // update last node
            hdnode[col]->u.next->down = temp;
            hdnode[col]->u.next = temp;
        }
        last->right = hdnode[current_row]; // link last node to head
        for(int i = 0; i<num_cols; i++){ 
            hdnode[i]->u.next->down = hdnode[i]; // link all down
        }
        for(int i = 0; i<num_heads-1; i++){
            hdnode[i]->u.next = hdnode[i + 1]; // link all head node
        }
        hdnode[num_heads-1]->u.next = node; // last headnode link to head
        node->right = hdnode[0];
    }
    fclose(fp);
    return node;
}
void mwrite(matrix_pointer node){ // function for writing to file
    FILE* fp = fopen("output.txt", "w");
    matrix_pointer temp, head1 = node->right;
    fprintf(fp, "%d %d %d\n", node->u.entry.row, node->u.entry.col, node->u.entry.value); // write number of row, col, term
    for(int i = 0; i<node->u.entry.row; i++){ // search to row-major
        for(temp = head1->right; temp != head1; temp = temp->right){ // search by col
            fprintf(fp, "%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value); // print to file
        }
        head1 = head1->u.next; // move head1 to next head
    }
    fclose(fp);
}
matrix_pointer mtranspose(matrix_pointer head1){
    matrix_pointer node = (matrix_pointer)malloc(sizeof(matrix_node)); // make head node
    // save number of row, col, term by switching row and col
    node->u.entry.row = head1->u.entry.col;
    node->u.entry.col = head1->u.entry.row;
    node->u.entry.value = head1->u.entry.value;
    int num_heads = (head1->u.entry.col > head1->u.entry.row) ? head1->u.entry.col : head1->u.entry.row; // number of node
    int* last_row = (int*)malloc(node->u.entry.col * sizeof(int)); // array for saving last node of row
    int* last_col = (int*)malloc(node->u.entry.row * sizeof(int)); // array for saving last node of col
    for(int i = 0; i<node->u.entry.col; i++) last_row[i] = -1; // initiating
    for(int i = 0; i<node->u.entry.row; i++) last_col[i] = -1; // initiating
    hdnode_t = (matrix_pointer*)malloc(sizeof(matrix_pointer) * (num_heads + 1)); // make head node array
    matrix_pointer temp, ptr = head1->right, ptr_temp;
    if(!num_heads) node->right = node; // when row and col are 0
    else{
        for(int i = 0; i<num_heads; i++){ // initiating each head node
            temp = (matrix_pointer)malloc(sizeof(matrix_node));
            hdnode_t[i] = temp;
            hdnode_t[i]->tag = head;
            hdnode_t[i]->right = temp;
            hdnode_t[i]->down = temp;
            hdnode_t[i]->u.next = temp;
        }
        for(int i = 0; i<head1->u.entry.row; i++){ // search matrix by row
            for(ptr_temp = ptr->right; ptr_temp != ptr; ptr_temp = ptr_temp->right){ // search matrix by col
                temp = (matrix_pointer)malloc(sizeof(matrix_node)); // make new node for transpose
                // save data by switching row and col
                temp->u.entry.row = ptr_temp->u.entry.col;
                temp->u.entry.col = ptr_temp->u.entry.row;
                temp->u.entry.value = ptr_temp->u.entry.value;
                temp->tag = entry;
                matrix_pointer pre, cur;
                if(last_col[temp->u.entry.row] == -1){ // when there is no node in col
                    hdnode_t[temp->u.entry.row]->right = temp; // link to right
                    temp->right = hdnode_t[temp->u.entry.row]; // link temp's right to head
                    last_col[temp->u.entry.row] = temp->u.entry.col; // update last node
                }
                else if(last_col[temp->u.entry.row] < temp->u.entry.col){ // when col is larger than last node
                    for(cur = hdnode_t[temp->u.entry.row] -> right; cur->right != hdnode_t[temp->u.entry.row];){ // move cur to last
                        cur = cur->right;
                    }
                    temp->right = cur->right; // link head to temp
                    cur->right = temp; // linkt temp to cur
                    last_col[temp->u.entry.row] = temp->u.entry.col; // update last node
                }
                else{ // other case
                    for(cur = hdnode_t[temp->u.entry.row] -> right; cur->right != hdnode_t[temp->u.entry.row];){ // move cur until temp is smaller than cur
                        if(temp->u.entry.col < cur->u.entry.col){
                            temp->right = cur; // link cur to temp
                            pre->right = temp; // link temp to pre
                            break;
                        }
                        pre = cur;
                        cur = cur->right;
                    }
                }
                // it only change row method is same with above
                if(last_row[temp->u.entry.col] == -1){
                    hdnode_t[temp->u.entry.col]->down = temp;
                    temp->down = hdnode_t[temp->u.entry.col];
                    last_row[temp->u.entry.col] = temp->u.entry.row;
                    hdnode_t[temp->u.entry.col]->u.next = temp;
                }
                else if(last_row[temp->u.entry.col] < temp->u.entry.row){
                    for(cur = hdnode_t[temp->u.entry.col] -> down; cur->down != hdnode_t[temp->u.entry.col];){
                        cur = cur->down;
                    }
                    temp->down = cur->down;
                    cur->down = temp;
                    last_row[temp->u.entry.col] = temp->u.entry.row;
                    hdnode_t[temp->u.entry.col]->u.next = temp;
                }
                else{
                    for(cur = hdnode_t[temp->u.entry.col] -> down; cur->down != hdnode_t[temp->u.entry.col];){
                        if(temp->u.entry.row < cur->u.entry.row){
                            temp->down = cur;
                            pre->down = temp;
                            break;
                        }
                        pre = cur;
                        cur = cur->down;
                    }
                }
            }
            // move haed node
            ptr = ptr->u.next;
        }
        for(int i = 0; i<node->u.entry.col; i++){
            hdnode_t[i]->u.next->down = hdnode_t[i];
        }
        for(int i = 0; i<num_heads-1; i++){ // link head node
            hdnode_t[i]->u.next = hdnode_t[i + 1];
        }
        hdnode_t[num_heads - 1]->u.next = node;
        node->right = hdnode_t[0];
    }
    free(last_col);
    free(last_row);
    return node;
}
int main(){
    matrix_pointer head_ori = mread(); // input matrix from file
    matrix_pointer head_t = mtranspose(head_ori); // save transpose to head_t
    mwrite(head_t); // save transpose matrix to file
    // free memory of both matrix
    matrix_pointer temp, del, cur;
    cur = head_ori->right;
    for(int i = 0; i<head_ori->u.entry.row; i++){
        for(temp = cur->right; temp != cur; ){
            del = temp;
            temp = temp->right;
            free(del);
        }
        cur = cur->u.next;
    }
    free(hdnode);
    cur = head_t->right;
    for(int i = 0; i<head_t->u.entry.row; i++){
        for(temp = cur->right; temp != cur; ){
            del = temp;
            temp = temp->right;
            free(del);
        }
        cur = cur->u.next;
    }
    free(hdnode_t);
}