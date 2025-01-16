#include<stdio.h>
#include<stdlib.h>
int check_array(int* arr, int n){ // check array if it's member has continuous integer
    if(n <= 1) return 1; // if n<= 1 return true
    int min = arr[0], max = arr[0]; // variable for find maximum and minimum in array
    for(int i = 1; i<n; i++){ // search all array
        if(arr[i] < min){ // if arr[i] smaller than current minimum
            min = arr[i]; // change minimum to arr[i]
        }
        if(arr[i] > max){ // if arr[i] bigger than curren maximum
            max = arr[i]; // change maximum to arr[i]
        }
    }
    if(max - min != n - 1){ // if difference of maximum and minimum is different with n - 1, that array can't have all continuous value
        return 0; // return false
    }
    int* check = (int*)calloc(101, sizeof(int)); // allocating memory(n's maximum is 100) and initiating array by 0
    for(int i = 0; i<n; i++){ //search all array
        if(check[arr[i]] == 1){ // if arr[i] has already checked
            free(check);
            return 0; // return false
        }
        check[arr[i]] = 1; // change value 1 to check it
    }
    free(check); // free check array memory
    return 1; // all value checked by no duplication
}
int main(){
    int n; // array's number
    scanf("%d", &n); // input array's number
    int* arr = (int*)malloc(sizeof(int) * n); // allocating array
    for(int i = 0; i<n; i++){ // repeat for input all array
        scanf("%d", &arr[i]); // input array's value
    }
    printf("%d\n", check_array(arr, n)); // print value that check_array returns
    free(arr); // free array's memory
}