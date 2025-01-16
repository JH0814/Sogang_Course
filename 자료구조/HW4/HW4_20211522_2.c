#include<stdio.h>
#include<stdlib.h>
// struct for polynomials
typedef struct poly_node* poly_pointer;
typedef struct poly_node{
    int coef;
    int expon;
    poly_pointer link;
}poly_node;

poly_pointer pmult(poly_pointer a, poly_pointer b){ // function for calculating multiple of two polynomials
    poly_pointer a_idx = a; // variable for searching a
    poly_pointer b_idx = b; // variable for searching b
    poly_pointer d = NULL; // to save multiple
    while(a_idx){
        b_idx = b; // initiating b in each of loop
        while(b_idx){
            poly_pointer d_idx = d; // for searching d
            int flag = 0; // flag for already added
            while(d_idx){
                if(d_idx->expon == a_idx->expon + b_idx->expon){ // if there is node that has same expon
                    d_idx->coef += (a_idx->coef) * (b_idx->coef); // add coef to it
                    flag = 1; // update flag and break loop
                    break;
                }
                d_idx = d_idx->link;
            }
            if(!flag){ // when flag is 0
                if(d == NULL){ // when d is empty
                    d = (poly_pointer)malloc(sizeof(poly_node)); // make new node
                    // calculate value
                    d->coef = (a_idx->coef) * (b_idx->coef);
                    d->expon = a_idx->expon + b_idx->expon;
                    d->link = NULL; // make link NULL to express end
                }
                else{ // other case
                    poly_pointer temp = (poly_pointer)malloc(sizeof(poly_node)); // make new node
                    // calculate value
                    temp->coef = (a_idx->coef) * (b_idx->coef);
                    temp->expon = a_idx->expon + b_idx->expon;
                    d_idx = d; // initiating d_idx
                    if(temp->expon > d->expon){ // when temp's expon is most biggest
                        temp->link = d;
                        d = temp; // change head to temp
                    }
                    else{
                        poly_pointer pre;
                        while(d_idx != NULL && d_idx->expon > temp->expon){ // search d until temp is smaller
                            pre = d_idx;
                            d_idx = d_idx->link;
                        }
                        temp->link = d_idx; // link d_idx to temp
                        pre->link = temp; // link temp to pre
                    }
                }
            }
            b_idx = b_idx->link;
        }
        a_idx = a_idx->link;
    }
    poly_pointer pre = NULL;
    poly_pointer d_idx = d;
    while(d_idx != NULL){
        if(pre == NULL && d_idx->coef == 0){ // when first node's expon is 0
            // change head and free pre
            pre = d_idx;
            d = d->link;
            d_idx = d_idx->link;
            free(pre);
            continue;
        }
        else if(d_idx->coef == 0){ // when node's expon is 0
            // delete that node and free memory
            pre->link = d_idx->link;
            poly_pointer del = d_idx;
            d_idx = d_idx->link;
            free(del);
        }
        else{
            pre = d_idx;
            d_idx = d_idx->link;
        }
    }
    return d; // return d
}
void pwrite(poly_pointer d){ // write polynomials to file
    FILE* fp = fopen("d.txt", "w"); // open file
    poly_pointer d_idx = d;
    int cnt = 0; // variable for counting node
    while(d_idx){
        cnt++;
        d_idx = d_idx->link;
    }
    fprintf(fp, "%d\n", cnt); // write node's count
    d_idx = d;
    while(d_idx){
        fprintf(fp, "%d %d\n", d_idx->coef, d_idx->expon); // write node's coef and expon
        d_idx = d_idx->link;
    }
    fclose(fp);
}
int main(){
    poly_pointer a = NULL, b = NULL, ptr;
    FILE* fp = fopen("a.txt", "r"); // open file for input a
    int num;
    fscanf(fp, "%d", &num); // number of term in a
    ptr = a;
    for(int i = 0; i<num; i++){
        int coef, expo;
        fscanf(fp, "%d %d", &coef, &expo); // input coef, expo from a.txt
        if(a == NULL){ // if a is null
            a = (poly_pointer)malloc(sizeof(poly_node)); // make new node
            // save data to a
            a->coef = coef;
            a->expon = expo;
            a->link = NULL;
            ptr = a;
        }
        else{
            poly_pointer temp = (poly_pointer)malloc(sizeof(poly_node)); // make new node
            // save data to temp
            temp->coef = coef;
            temp->expon = expo;
            temp->link = NULL;
            // link to ptr
            ptr->link = temp;
            ptr = ptr->link; // move next node
        }
    }
    fclose(fp);
    fp = fopen("b.txt", "r");
    fscanf(fp, "%d", &num); // number of term in b
    ptr = b;
    for(int i = 0; i<num; i++){
        int coef, expo;
        fscanf(fp, "%d %d", &coef, &expo); // input coef, expo from b.txt
        if(b == NULL){
            b = (poly_pointer)malloc(sizeof(poly_node)); // make new node
            // save data to b
            b->coef = coef;
            b->expon = expo;
            b->link = NULL;
            ptr = b;
        }
        else{
            poly_pointer temp = (poly_pointer)malloc(sizeof(poly_node)); // make new node
            // save data to temp
            temp->coef = coef;
            temp->expon = expo;
            temp->link = NULL;
            ptr->link = temp; // link temp to ptr
            ptr = ptr->link; // move to next node
        }
    }
    fclose(fp);
    poly_pointer d = pmult(a, b); // call pmult and save to d
    pwrite(d); // call pwrite
    // free a, b, d's memory
    poly_pointer pre;
    ptr = a;
    while(ptr){
        pre = ptr;
        ptr = ptr->link;
        free(pre);
    }
    pre = NULL;ptr = b;
    while(ptr){
        pre = ptr;
        ptr = ptr->link;
        free(pre);
    }
    pre = NULL;ptr = d;
    while(ptr){
        pre = ptr;
        ptr = ptr->link;
        free(pre);
    }
}