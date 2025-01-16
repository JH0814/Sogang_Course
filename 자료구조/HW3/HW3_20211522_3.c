#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int isFull(int top, int max){ // return stack is already full(in stack ADT)
    if(top >= max){
        return 1;
    }
    else{
        return 0;
    }
}
int isEmpty(int top){// return stack is empty(in stack ADT)
    if(top < 0){// stack is empty when top < 0
        return 1;
    }
    else{
        return 0;
    }
}
void push(int* stack, int item, int* top, int max){// push integer in stack(in stack ADT)
    if(isFull(*top, max)) return;// when stack is already full, end function
    stack[++(*top)] = item;// put item to stack array and increase top
}
int pop(int* stack, int* top){// pop integer in stack(in stack ADT)
    if(isEmpty(*top)) return -1;// when stack is empty, end function
    return stack[(*top)--];// return stack[*top] and decrease top
}
int main(){
    char* num; //strnig to save number
    int max_len = 1000001; // set max_len to 10001
    int k; // integer to save number of deleting from num
    int* stack; // stack array
    int top = -1; // integer for top
    num = (char*)malloc(sizeof(char) * 1000001); // dynamic allocating to num
    int idx = 0; // inputed length to num
    while(1){
        char c = getchar(); // input to num one by one
        num[idx] = c; // save to num
        if(num[idx] == '\n'){ // input until \n inputed
            num[idx] = '\0'; // change to null
            break; // break the loop
        }
        idx++; // increase number of input
        if(idx + 1 >= max_len){ // when input data over max_len
            // save to temp
            char* temp = (char*)malloc(sizeof(char) * (max_len));
            for(int i = 0; i<idx; i++){
                temp[i] = num[i];
            }
            // reallocate memory to increase it's memory
            free(num);
            num = (char*)malloc(sizeof(char) * (max_len + 1));
            for(int i = 0; i<idx; i++){
                num[i] = temp[i];
            }
            free(temp); // free temp
            max_len++; // increase max_len
        }
    }
    scanf("%d", &k); // input k
    stack = (int*)malloc(sizeof(int) * (strlen(num) + 1)); // dynamic allocate to stack
    for(int i = 0; i<strlen(num); i++){ // for loop for searching all num
        while(!isEmpty(top) && k > 0 && stack[top] > (num[i] - '0')){ // until stack is not empty and k > 0 && stack[top] is bigger than num[i]
            pop(stack, &top); // delete stack's top
            k--; // decrease k when pop called
        }
        push(stack, num[i] - '0', &top, strlen(num) + 1); // push to stack
    }
    while(!isEmpty(top) && k > 0){ // if k don't become 0 at above while loop
        pop(stack, &top); // delete stack's top
        k--; // decrease k when pop called
    }
    if(isEmpty(top)){ // if there is no element in stack
        printf("0\n"); // print 0
        return 0; // end program
    }
    int fir = 0; // flag for when filtering when 0 is first number
    for(int i = 0; i<=top; i++){ // for loop for print answer
        if(fir == 0 && stack[i] == 0 && top != 0) continue; // when first integer is 0 continue
        else fir = 1; // when not zero integer is printed, denote to flag
        printf("%d", stack[i]); // print answer
    }
    printf("\n");
    free(num); // free num
    free(stack); // free stack
}