#include<stdio.h>
#include<stdlib.h>
void combination(int* sel, int start, int n, int r){ // function that return n combination r
    if(r == 0){ // if recursive function end print all cases
        printf("{"); // open braces
        for(int i = 0; i<n; i++){ // research all arrays
            if(sel[i] == 1){ // print only array sel[i] has 1
                printf("%c ", i + 'a'); // use ASCII to print alphabet to show it's order
            }
        }
        printf("}\n"); // close brases
        return; // end function
    }
    for(int i = start; i<n; i++){ // start at argument 'start' and it ends at n - 1
        sel[i] = 1; // denote 'i' is included
        combination(sel, i + 1, n, r - 1); // call function that value i increase 1 and r decrease 1
        sel[i] = 0; // denote 'i' is not included
    }
}
void PowerSet(int cnt, int N){ // function that call function 'combination' by it's ascending order
    if(cnt == 0){ // show recursive call end
        printf("{}\n"); // print empty set
        return; // end function
    }
    PowerSet(cnt - 1, N); // call own function that has value cnt 1 smaller
    int* sel = (int*)calloc(N, sizeof(int)); // allocating array that expresses including in combination
    combination(sel, 0, N, cnt); // call combination function
    free(sel); // free sel memory
}
int main(){
    int N; // variable that input number
    scanf("%d", &N); // input
    PowerSet(N, N); // call PowerSet and print all
}