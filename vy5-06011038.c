#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX 500

typedef struct stack {
    int data[MAX];
    int top;
} stack;

void push(stack *, int); //pushes data to stack

int pop(stack *); //gets data from stack

int precedence(char); //compares the operator priorities

void init(stack *); //initializes the stack

int empty(stack *); //checks the stack if it is empty

int full(stack *); //chekcs the stack if it is full

int top(stack *); //returns the top of the stack

void infix_to_postfix(char infix[], char postfix[]); //converts infix equation to postfix

int decode_postfix(char postfix[], stack *s_result); //processes the postfix equation

void print_stack_and_postfix(stack *, char postfix[]); //prints the present postfix equation and stack

void print_stack(stack *); //prints the stack

void read_data(char infix[]); //reads data from inpıt file


int main() {
    char infix[MAX], postfix[MAX]; //defines the infix and postfix arrays
    read_data(infix); //reading data from file

    printf("\nInfix expression :\n%s\n\n", infix);
    infix_to_postfix(infix, postfix); //data is sent to the function to convertion
    return 0;
}

void infix_to_postfix(char infix[], char postfix[]) {
    stack s, s_result; //defining stacks
    char token; //temporary variable
    char x; //temporary variable
    int i = 0, j = 0, k, v, m; //indices
    int result; //gets the return value from the function
    init(&s); //initializing stacks
    init(&s_result);

    while (infix[i] != '\0') { //function operates till end of the postfix equation
        m = i;
        printf("\nCurrent equation : ");
        while (infix[m] != '\n' && infix[m] != '\0') {
            printf("%c", infix[m]);
            m++;
        }
        printf("\n\n");
        while (infix[i] != '=') //finding the equation beginning
            i++;
        i++;

        for (k = i; infix[k] != ';'; k++) { // loop ends by the end of every equation
            token = infix[k]; //loop proceeds character by character
            if (isalnum(token) || isalpha(token)) { // checks the character if it is numeric ol alphabethic
                postfix[j++] = token;
                postfix[j] = '\0';
                print_stack_and_postfix(&s, postfix); //prints the stack and postfix
            } else if (token == '(') //pushes the variable if open parenthesis comes
                push(&s, '(');
            else if (token == ')') //close tag starts the pop operation
                while ((x = pop(&s)) != '(') {
                    postfix[j++] = x; //character is added to the postfix equation
                    postfix[j] = '\0';
                    print_stack_and_postfix(&s, postfix);
                }
            else {
                while ((precedence(token) <= precedence(s.data[s.top])) &&
                       !empty(&s)) { //comparing the priorities of the top of the stack and current character
                    x = pop(&s); //pop operation according to the comparison
                    postfix[j++] = x;
                    postfix[j] = '\0';
                    print_stack_and_postfix(&s, postfix);
                }
                push(&s, token);
            }
        }

        while (!empty(&s)) { //when the equation ends, the data is left in the stack will be popped
            x = pop(&s);
            postfix[j++] = x;
            postfix[j++] = ' ';
        }

        postfix[j] = '\0'; //adding the end character at the end of the postfix equation

        j = 0; //sets the beginning of the postfix again

        printf("\n\nPostfix for the current equation : %s\n", postfix);

        result = decode_postfix(postfix, &s_result); //prints the current postfix equation

        while (infix[i] != '=') //finding the variable that will be the result of the postfix aquation
            i--;
        while (!(isalpha(infix[i])))
            i--;

        push(&s_result, infix[i]); //the variable and the result is added to the stack
        push(&s_result, result);

        while (infix[i] != '\n') //moving throuhg the next line of the infix
            i++;
        i++;

        printf("Values of the variables : "); //printing the variables and values
        for (v = 0; v <= s_result.top; v = v + 2) {
            printf("\n %c = %d\n", s_result.data[v], s_result.data[v + 1]);
        }
        printf("\n");
    }
}

int decode_postfix(char postfix[], stack *s_result) {
    stack s; //the stack that saves the data
    char token;
    int i = 0, x = 0, y = 0; //indices
    int op_1, op_2, op_3; //operation variables
    int len = 0;
    int count = 0;
    int v = 0;

    init(&s); //initializing stack

    while (postfix[i] != '\0') { //finding the length of the postfix
        len++;
        i++;
    }

    i = 0;

    while (isdigit(postfix[i]) || postfix[i] == ' ') { //checks the postfix if it is consist of all characters are numeric or alphabethic
        count++;
        i++;
    }

    i = 0;

    if (len == count) { //calculating the multi-digit variables
        while ((postfix[i] >= '0') && (postfix[i] <= '9')) {
            while (postfix[i] != ' ') {
                x = postfix[i] - '0';
                y = y + x;
                y = y * 10;
                i++;
            }
            return (y / 10);
        }
    }

    i = 0;

    while (postfix[i] != '\0') { //operation runs untill the end of the postfix
        token = postfix[i];
        if (token == '*') { //checks the operands
            op_1 = pop(&s); //gets data from the stack and makes the operation
            op_2 = pop(&s);

            op_3 = op_2 * op_1;

            printf("\nOperators = %d ,  %d  ", op_2, op_1);
            printf("Operation : Multiplication\n");
            push(&s, op_3); //result is pushed to the stack
            print_stack(&s);
        }

        if (token == '/') {
            op_1 = pop(&s);
            op_2 = pop(&s);

            op_3 = op_2 / op_1;

            printf("\nOperators = %d ,  %d  ", op_2, op_1);
            printf("Operation : Division\n");
            push(&s, op_3);
            print_stack(&s);
        }

        if (token == '+') {
            op_1 = pop(&s);
            op_2 = pop(&s);

            op_3 = op_2 + op_1;

            printf("\nOperators = %d ,  %d  ", op_2, op_1);
            printf("Operation : Addition\n");
            push(&s, op_3);
            print_stack(&s);
        }

        if (token == '-') {
            op_1 = pop(&s);
            op_2 = pop(&s);

            op_3 = op_2 - op_1;

            printf("\nOperators = %d ,  %d  ", op_2, op_1);
            printf("Operation : Substraction\n");
            push(&s, op_3);
            print_stack(&s);
        }
        while ((token >= '0' && token <= '9') || isalpha(token)) { //enters the loop if the character is alphabethic or numeric
            if (isalpha(token)) { //searching the previous variables in the stack
                v = s_result->data[s_result->top];
                while (token != s_result->data[v]) {
                    v--;
                }
                token = s_result->data[v + 1]; //variable is added to the stack
                push(&s, token);
                break;
            }
            while (token != ' ') { //converting the multidigit variables
                x = token - '0';
                y = y + x;
                y = y * 10;
                i++;
                token = postfix[i];
            }
            y = y / 10;
            push(&s, y);
            print_stack(&s); //result is pushed to the stack
            y = 0;
            x = 0;
        }
        i++;
    }
    printf("\nResult of the equation is : %d\n", s.data[s.top]);
    return s.data[s.top];
}

int precedence(char x) { //returns the operation priorities
    if (x == '(')
        return (0);
    if (x == '+' || x == '-')
        return (1);
    if (x == '*' || x == '/' || x == '%')
        return (2);
    return 3;
}

void init(stack *s) { //initializes the stack
    int i = 0;
    while (i < MAX) {
        s->data[i] = 0;
        i++;
    }
    s->top = -1;
}

int empty(stack *s) {
    if (s->top < 0)
        return (1);
    return (0);
}

void push(stack *s, int x) {
    s->top = s->top + 1;
    s->data[s->top] = x;
}

int pop(stack *s) {
    int x;
    x = s->data[s->top];
    s->top = s->top - 1;
    return (x);
}

void print_stack_and_postfix(stack *s, char postfix[]) {
    printf("Postfix expression : %s \t\t\t", postfix);
    printf("Stack : ");
    int k;
    for (k = 0; k < s->top; k++) {
        if ((isalpha(s->data[k]) || isdigit(s->data[k])) || ((s->data[k] >= '(') && (s->data[k]) <= '/')) {
            printf(" %c ", s->data[k]);
        }
    }
    printf("\n");
}

void print_stack(stack *s) {
    printf("Stack : ");
    int k;
    for (k = 0; k <= s->top; k++) {
        printf(" %d ", s->data[k]);
    }
    printf("\n");
}

void read_data(char infix[]) {
    FILE *f;
    char c;
    int i = 0;
    f = fopen("input.txt", "r");
    if (!f) {
        printf("File not found\n");
        exit(0);
    }

    while ((c = fgetc(f)) != EOF) {
        infix[i] = (char) c;
        i++;
    }
    infix[i] = '\0';
}
