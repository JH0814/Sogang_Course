#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void failure(char* arr, char* pat, int* fail){ // function that notice same in pattern
    int i, n = strlen(pat); // declare variable
    fail[0] = -1; // first array is -1
    for(int j = 1; j<n; j++){ // search all array which starting point is second value
        i = fail[j - 1]; // checking point is fail[j - 1];
        while(pat[j] != pat[i + 1] && i >= 0){ // while pat[j] and pat[i + 1] is same and not out of range
            i = fail[i]; // i jumpt to fail[i]
        }
        if(pat[j] == pat[i + 1]){ // compare pat[j] and pat[i + 1]
            fail[j] = i + 1; // if index is j it start to check at i + 1
        }
        else{
            fail[j] = -1; // if there is no charactor which is same with pattern
        }
    }
}
int pmatch_all(char* str, char* pat, int* cnt, int* ans){
    int i = 0, j = 0; // it starts at both 0
    int lens = strlen(str); // save str's length at variable
    int lenp = strlen(pat); // save pat's length at variable
    int* fail = (int*)calloc(lenp, sizeof(int)); // allocate fail array memory with initiating by 0
    failure(str, pat, fail); // call failure function
    while(i < lens){ // it changes from textbook's code that j<lenp is deleted
        if(str[i] == pat[j]){// if str[i] and pat[j] is same keep i and j increase
            i++;
            j++;
            if(j == lenp){ // if j get to the end of pattern
                ans[*cnt] = i - lenp; // plus index to answer array
                (*cnt)++; // increase anser count
                j = 0; // initiating j
                i = i - lenp + 1; // initiating i to next to the answer
            }
        }
        else if(j == 0){ // if it is not same and j == 0
            i++;
        }
        else{
            j = fail[j - 1] + 1; // change j to jump point
        }
    }
    free(fail); // free memory of fail array
}
int main(){
    char str[31]; // string that want to search pattern(max length is 30)
    char pat[31]; // string that the pattern is given(max length is 30)
    int cnt = 0; // answer count variable
    scanf("%s", str); // input string
    scanf("%s", pat); // input pattern
    for(int i = 0; i<strlen(str); i++){ // loop for delete '\n'
        if(str[i] == '\n'){
            str[i] = '\0';
        }
    }
    for(int i = 0; i<strlen(pat); i++){ // loop for delete '\n'
        if(pat[i] == '\n'){
            pat[i] = '\0';
        }
    }
    int* ans = (int*)malloc(sizeof(int) * strlen(str)); // allocating array to save answers
    int res = pmatch_all(str, pat, &cnt, ans); // call pmatch_all to find answer
    if(cnt == 0){ // if there is no pattern in str
        free(ans); // free memory of ans
        return 0; // end of program
    }
    else{
        for(int i = 0; i<cnt; i++){ // loop for print all answers
            printf("%d\n", ans[i]); // print ansers
        }
    }
    free(ans); // free memory of ans
}