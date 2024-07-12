//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h> // for isdigit()
//#include <math.h>  // for pow()
//
//// Function to return precedence of operators
//int prec(char c) {
//    if (c == '^')
//        return 3;
//    else if (c == '/' || c == '*')
//        return 2;
//    else if (c == '+' || c == '-')
//        return 1;
//    else
//        return -1;
//}
//
//// Function to return associativity of operators
//char associativity(char c) {
//    if (c == '^')
//        return 'R';
//    return 'L'; // Default to left-associative
//}
//
//// Function to check if a character is an operator
//int isOperator(char c) {
//    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
//}
//
//// Stack type for characters
//struct CharStack {
//    int top;
//    unsigned capacity;
//    char* array;
//};
//
//// Stack type for integers
//struct IntStack {
//    int top;
//    unsigned capacity;
//    int* array;
//};
//
//// Stack Operations for characters
//struct CharStack* createCharStack(unsigned capacity) {
//    struct CharStack* stack = (struct CharStack*)malloc(sizeof(struct CharStack));
//    if (!stack)
//        return NULL;
//    stack->top = -1;
//    stack->capacity = capacity;
//    stack->array = (char*)malloc(stack->capacity * sizeof(char));
//    if (!stack->array)
//        return NULL;
//    return stack;
//}
//
//int isCharStackEmpty(struct CharStack* stack) {
//    return stack->top == -1;
//}
//
//char peekChar(struct CharStack* stack) {
//    return stack->array[stack->top];
//}
//
//char popChar(struct CharStack* stack) {
//    if (!isCharStackEmpty(stack))
//        return stack->array[stack->top--];
//    return '$';
//}
//
//void pushChar(struct CharStack* stack, char op) {
//    stack->array[++stack->top] = op;
//}
//
//// Stack Operations for integers
//struct IntStack* createIntStack(unsigned capacity) {
//    struct IntStack* stack = (struct IntStack*)malloc(sizeof(struct IntStack));
//    if (!stack)
//        return NULL;
//    stack->top = -1;
//    stack->capacity = capacity;
//    stack->array = (int*)malloc(stack->capacity * sizeof(int));
//    if (!stack->array)
//        return NULL;
//    return stack;
//}
//
//int isIntStackEmpty(struct IntStack* stack) {
//    return stack->top == -1;
//}
//
//int popInt(struct IntStack* stack) {
//    if (!isIntStackEmpty(stack))
//        return stack->array[stack->top--];
//    return -1;
//}
//
//void pushInt(struct IntStack* stack, int value) {
//    stack->array[++stack->top] = value;
//}
//
//// Function to convert infix expression to postfix expression
//void infixToPostfix(char* exp, char* output) {
//    int i, k;
//    struct CharStack* stack = createCharStack(strlen(exp));
//    if (!stack)
//        return;
//
//    for (i = 0, k = 0; exp[i]; ++i) {
//        // If the scanned character is an operand, add it to output.
//        if (isdigit(exp[i]))
//            output[k++] = exp[i];
//
//            // If the scanned character is '(', push it to the stack.
//        else if (exp[i] == '(')
//            pushChar(stack, exp[i]);
//
//            // If the scanned character is ')', pop and output from the stack
//            // until an '(' is encountered.
//        else if (exp[i] == ')') {
//            while (!isCharStackEmpty(stack) && peekChar(stack) != '(')
//                output[k++] = popChar(stack);
//            if (!isCharStackEmpty(stack) && peekChar(stack) != '(')
//                return; // invalid expression
//            else
//                popChar(stack);
//        } else // an operator is encountered
//        {
//            while (!isCharStackEmpty(stack) && prec(exp[i]) <= prec(peekChar(stack))) {
//                if (associativity(exp[i]) == 'L' && prec(exp[i]) == prec(peekChar(stack)))
//                    break;
//                output[k++] = popChar(stack);
//            }
//            pushChar(stack, exp[i]);
//        }
//    }
//
//    // pop all the operators from the stack
//    while (!isCharStackEmpty(stack))
//        output[k++] = popChar(stack);
//
//    output[k] = '\0';
//}
//
//// Function to evaluate a postfix expression
//int evaluatePostfix(char* exp) {
//    struct IntStack* stack = createIntStack(strlen(exp));
//    int i;
//
//    if (!stack)
//        return -1;
//
//    for (i = 0; exp[i]; ++i) {
//        if (isdigit(exp[i]))
//            pushInt(stack, exp[i] - '0');
//        else {
//            int val1 = popInt(stack);
//            int val2 = popInt(stack);
//            switch (exp[i]) {
//                case '+':
//                    pushInt(stack, val2 + val1);
//                    break;
//                case '-':
//                    pushInt(stack, val2 - val1);
//                    break;
//                case '*':
//                    pushInt(stack, val2 * val1);
//                    break;
//                case '/':
//                    pushInt(stack, val2 / val1);
//                    break;
//                case '^':
//                    pushInt(stack, pow(val2, val1));
//                    break;
//            }
//        }
//    }
//    return popInt(stack);
//}
//
//// Driver code
//int main() {
//    char infixExp[100];
//    char postfixExp[100];
//    scanf("%s", infixExp);
//
//    infixToPostfix(infixExp, postfixExp);
//    printf("%s\n", postfixExp);
//
//    int result = evaluatePostfix(postfixExp);
//    printf("%d\n", result);
//
//    return 0;
//}
