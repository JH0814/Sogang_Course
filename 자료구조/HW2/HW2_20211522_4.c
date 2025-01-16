#include<stdio.h>
#include<stdlib.h>
int compare(char* a, char* b){ // compare string a and b by lexical order
    int i = 0, j = 0; // at first both of string starts at 0
    while(1){
        if((i == j) && a[i] > b[j]){ // if it's order has difference
            return 1; // when a is more higher order
        }
        else if((i == j) && a[i] < b[j]){ // if its order has difference
            return -1; // when b is more higher order
        }
        else{
            if(a[i] != '\0'){ // increase i when a is not end
                i++;
            }
            if(b[j] != '\0'){ // increase j when b is not end
                j++;
            }
            if(a[i] == '\0' && b[j] == '\0') break;
        }
    }
    if(i == j){ // if i and j is same until both of string ends, it is same
        return 0; // return 0
    }
    else if(i > j){ // if a is longer than b, it's order is higher than b
        return 1;
    }
    else{
        return -1; // if b is longer than a, it's order is higher than a
    }
}
void swap(char** a, char** b){ // exchange string a and b
    char* tmp = *a; // save *a to tmp
    *a = *b; // connect *b to *a
    *b = tmp; // *b change to tmp
}
void sorting(char** arr, int N){ // sorting by bubble sort
    for(int i = 0; i<N - 1; i++){ // search all array except end index
        for(int j = 0; j<N - 1 - i; j++){ // search from 0 to N - 1 -i
            if(compare(arr[j], arr[j + 1]) == 1){ // swap when arr[j] > arr[j + 1]
                swap(&arr[j], &arr[j + 1]); // call swap function
            }
        }
    }
}
int main(){
    int N; // number of student
    scanf("%d", &N); // input N
    getchar(); // remove '\n'
    getchar(); // remove '\0'
    char** arr = (char**)malloc(sizeof(char*) * N); // allocating string's array
    for(int i = 0; i<N; i++){ // loop for input student's name
        arr[i] = (char*)malloc(sizeof(char) * 101); // allocating name array string(usually name less then 100 characters)
        int cnt = 0; // input character count
        int max_size = 101; // save array's max_length
        while(1){
            char c = getchar(); // name input by one-by-one
            arr[i][cnt] = c; // save it to array
            if(arr[i][cnt] == EOF){ // if file end's
                arr[i][cnt] = '\0'; // express string's end point
                break; // stop loop
            }
            if(arr[i][cnt] == '\n'){ // if '\n' exist, there is a point of name ends
                arr[i][cnt] = '\0'; // it change to express string ends
                break; // stop while loop
            }
            cnt++; // increase current cnt
            if(cnt + 1 >= max_size){ // when name longer than max_size
                char* temp = (char*)malloc(sizeof(char) * (max_size)); // new memory to save longer array
                for(int j = 0; j<cnt; j++){ // loop for save current array
                    temp[j] = arr[i][j]; // save name to temp array
                }
                free(arr[i]); // free current arr[i]
                arr[i] = (char*)malloc(sizeof(char) * (max_size + 1)); // allocate new memory that increase size 1
                for(int j = 0; j<cnt; j++){ // loop for save string that inputed before
                    arr[i][j] = temp[j]; // save string to arr[i]
                }
                free(temp); // free temp
                max_size += 1; // increase max_size
            }
        }
    }
    sorting(arr, N); // call sorting function
    for(int i = 0; i<N; i++){ // loop for print array
        printf("%s\n", arr[i]); // print student name
    }
    for(int i = 0; i<N; i++){ // free memory
        free(arr[i]);
    }
    free(arr);
}