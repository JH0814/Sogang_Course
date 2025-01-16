#include<stdio.h>
#include<stdlib.h>
typedef enum {lparen, rparen, plus, minus, unary, times, divide, mod, eos, operand} precedence; // divide operator by integer
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
        case '#': return unary;
        case '\0': return eos;
        default: return operand;
    }
}
void printToken(precedence symbol, char* post, int* idx){ // change token to operator and save to string
    switch(symbol){ // token to operator
        case plus: post[(*idx)++] = '+'; printf("%c", '+'); break; // save to string and print it
        case minus: post[(*idx)++] = '-'; printf("%c", '-'); break;
        case divide: post[(*idx)++] = '/'; printf("%c", '/'); break;
        case times: post[(*idx)++] = '*'; printf("%c", '*'); break;
        case unary: post[(*idx)++] = '#'; printf("%c", '#'); break;
        case mod: post[(*idx)++] = '%'; printf("%c", '%'); break;
        case eos: post[(*idx)++] = '\0'; printf("%c", '\0'); break;
        default: return; 
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
    if(top < 0){ // stack is empty when top < 0
        return 1;
    }
    else{
        return 0;
    }
}
void push(int* stack, int item, int* top){ // push integer in stack(in stack ADT)
    if(isFull(*top)) return; // when stack is already full, end function
    stack[++(*top)] = item; // put item to stack array and increase top
}
int pop(int* stack, int* top){// pop integer in stack(in stack ADT)
    if(isEmpty(*top)) exit(1); // when stack is empty, end function
    return stack[(*top)--]; // return stack[*top] and decrease top
}
int eval(int* stack, char* str, int* top){ // calculate postfix
    precedence token; // integer that divide operator
    char symbol; // character that save operator or operand
    int op1, op2; // save operand in integer
    int n = 0; // index for searching from string
    token = getToken(str, &symbol, &n); // first character
    while(token != eos){ // until string end
        if(token == operand){ // when character is operand
            push(stack, symbol - '0', top); // push to stack
        }
        else if(token == unary){ // when string is unary operator
            op1 = pop(stack, top); // pop one operand
            push(stack, op1 * (-1), top); // and multiple -1 to operand and push to stack
        }
        else{ // when character is operator
            op2 = pop(stack, top); // pop two operand
            op1 = pop(stack, top);
            switch(token){ // divide case by operator and push item which is caculated to stack
                case plus: push(stack, op1 + op2, top); break;
                case minus: push(stack, op1 - op2, top); break;
                case times: push(stack, op1 * op2, top); break;
                case divide: push(stack, op1/op2, top); break;
                case mod: push(stack, op1 % op2, top); break;
            }
        }
        token = getToken(str, &symbol, &n); // search next character
    }
    return pop(stack, top); // return calculated value
}

void postfix(int* stack, char* str, int* top, char* post){ // infix to postfix and save to post array
    char symbol; // save character from string
    int n = 0; // index for searching string
    int idx = 0; // index for saving to post
    // compare operator's priority
    int isp[] = {0, 19, 12, 12, 12, 13, 13, 13, 0};
    int icp[] = {20, 19, 12, 12, 12, 13, 13, 13, 0};
    precedence token;
    stack[0] = eos; // make first item in stack eos
    (*top)++; // make top = 0
    for(token = getToken(str, &symbol, &n); token != eos; token = getToken(str, &symbol, &n)){ // for loof for searching string
        if(token == operand){ // when it is operand
            post[idx++] = symbol; // save to post
            printf("%c", symbol); // print out
        }
        else if(token == rparen){ // when it is rparen
            while(stack[*top] != lparen){ // until top in stack is not lparen
                printToken(pop(stack, top), post, &idx); // print and save it
            }
            pop(stack, top); // pop lparen
        }
        else{ // when token is operator(except rparen)
            if(token == minus){ // when token is minus
                if(n == 0 || !(str[n - 2] >= '0' && str[n - 2] <= '9') && str[n - 2] != ')'){ // if - is first or before - is not neither integer nor )
                    token = unary; // change token to unary
                }
            }
            while(isp[stack[*top]] >= icp[token]){ // until in stack operator is higher than token
                printToken(pop(stack, top), post, &idx); // print and save 
            }
            push(stack, token, top); // push toekn to stack
        }
    }
    while((token = pop(stack, top)) != eos){ // pop all the token remained
        printToken(token, post, &idx);
    }
    printf("\n"); // print \n
    post[idx] = '\0'; // save NULL to end of post
}
int main(){
    char string[21]; // variable for save expression
    scanf("%s", string); // input expression
    int stack[21]; // stack array
    int top = -1; // top for stack
    char post[21]; // variable for save postfix
    postfix(stack, string, &top, post); // call postfix
    top = -1; // initiating top
    printf("%d\n", eval(stack, post, &top)); // print out caculated result
}