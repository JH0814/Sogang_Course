#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef enum {lparen, rparen, plus, minus, times, divide, mod, eos, operand} precedence; // divide operator by integer
precedence getToken(char* str, char* symbol, int* n){ // get integer from operator
    *symbol = str[(*n)++]; // get character from string and increase index
    switch(*symbol){ // return value that same with character
        case '(': return lparen;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '/': return divide;
        case '*': return times;
        case '%': return mod;
        case '\0': return eos;
        default: return operand;
    }
}
char printToken(precedence symbol){ // change token to operator and return it
    switch(symbol){ // token to operator
        case plus: return '+';
        case minus: return '-';
        case divide: return '/';
        case times: return '*';
        case mod: return '%';
        case eos: return '\0';
        default: return '\0';
    }
}

int isFull(int top){ // return stack is already full(in stack ADT)
    if(top >= 21){ // Max stack size is 21
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
void push(int* stack, int item, int* top){// push integer in stack(in stack ADT)
    if(isFull(*top)) return;// when stack is already full, end function
    stack[++(*top)] = item;// put item to stack array and increase top
}
int pop(int* stack, int* top){// pop integer in stack(in stack ADT)
    if(isEmpty(*top)) exit(1);// when stack is empty, end function
    return stack[(*top)--];// return stack[*top] and decrease top
}
void char_push(char stack[][21], char item, int* top){ // push character in stack(in stack ADT)
    if(isFull(*top)) return; // when stack is already full, end function
    stack[++(*top)][0] = item; // put character to array with increase top
    stack[*top][1] = '\0'; // express end of string
}
void str_push(char stack[][21], char* item, int* top){ // push string in stack(in stack ADT)
    if(isFull(*top)) return; // when stack is alredy full, end function
    strcpy(stack[++(*top)], item); // copy to stack[++(*top)]
}
char* str_pop(char stack[][21], int* top){ // pop string in stack(in stack ADT)
    char* temp = '\0'; // temp for return
    if(isEmpty(*top)) return temp; // when stack is empty, return '\0'
    return stack[(*top)--]; // return top string of stack
}
void prefix(char num_stack[][21], int* ope_stack, char* str, int* num_top, int* ope_top){ // change infix expression to prefix expression
    char symbol; // variable for searching string
    int n = 0; // index for searching
    // priority of operator
    int isp[] = {0, 0, 12, 12, 13, 13, 13, 0};
    int icp[] = {0, 0, 12, 12, 13, 13, 13, 0};
    precedence token; // integer for divide operator
    for(token = getToken(str, &symbol, &n); token != eos; token = getToken(str, &symbol, &n)){ // for loop for searching string
        if(token == operand){ // when token is operand
            char_push(num_stack, symbol, num_top); // push to num_stack
        }
        else if(token == lparen){ // when token is lparen
            push(ope_stack, token, ope_top); // push to ope_stack
        }
        else if(token == rparen){ // when token is rparen
            while(isEmpty(*ope_top) == 0 && ope_stack[*ope_top] != lparen){ // while loop for pop until lparen become top
                char op1[21], op2[21]; // variable to save operand
                strcpy(op1, str_pop(num_stack, num_top));
                strcpy(op2, str_pop(num_stack, num_top));
                char op = printToken(pop(ope_stack, ope_top)); // variable to save operator
                // connect string that operator and two of operand
                char temp[21];
                temp[0] = op;temp[1] = '\0';
                strcat(temp, op2);
                strcat(temp, op1);
                str_push(num_stack, temp, num_top); // push to num_stack
            }
            pop(ope_stack, ope_top); // pop lparen
        }
        else{
            while(isEmpty(*ope_top) == 0 && icp[token] <= isp[ope_stack[*ope_top]]){ // while loop for pop until lower priority found
                char op1[21], op2[21]; // variable to save operand
                strcpy(op1, str_pop(num_stack, num_top));
                strcpy(op2, str_pop(num_stack, num_top));
                char op = printToken(pop(ope_stack, ope_top)); // variable to save operator
                // connect string that operator and two of operand
                char temp[21];
                temp[0] = op;temp[1] = '\0';
                strcat(temp, op2);
                strcat(temp, op1);
                str_push(num_stack, temp, num_top); // push to num_stack
            }
            push(ope_stack, token, ope_top); // push to ope_stack
        }
    }
    while(isEmpty(*ope_top) == 0){ // pop remained operator and operand
        char op1[21], op2[21]; // variable to save operand
        strcpy(op1, str_pop(num_stack, num_top));
        strcpy(op2, str_pop(num_stack, num_top));
        char op = printToken(pop(ope_stack, ope_top)); // variable to save operator
        // connect string that operator and two of operand
        char temp[21];
        temp[0] = op;temp[1] = '\0';
        strcat(temp, op2);
        strcat(temp, op1);
        str_push(num_stack, temp, num_top); // push to num_stack
    }
}
int main(){
    char str[21]; // variable to save infix expression
    scanf("%s", str); // input string
    char num_stack[21][21]; // stack array for push operand
    int ope_stack[21]; // stack array for push operator
    int num_top = -1, ope_top = -1; // top index of each stack
    prefix(num_stack, ope_stack, str, &num_top, &ope_top); // call prefix
    printf("%s\n", num_stack[num_top]); // print prefix expression
}