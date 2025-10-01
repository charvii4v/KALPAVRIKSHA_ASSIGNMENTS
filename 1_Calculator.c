#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int evaluate(char *exp, int *error) {
    int n = strlen(exp);
    long result = 0, lastNo = 0, currNo = 0;
    char perform = '+'; 

    for (int i = 0; i < n; i++) {
        char c = exp[i];

        if (isspace(c)) 
        continue;

        if (isdigit(c)) {
            currNo = 0;
            while (i < n && isdigit(exp[i])) {

                currNo = currNo * 10 + (exp[i] - '0');
                i++;
            }

            i--; 
        }
        else if (c != '+' && c != '-' && c != '*' && c != '/') {
            *error = 2; 
            return 0;
        }

        if (!isdigit(c) || i == n - 1) {
            if (perform  == '+') {
                result += lastNo;
                lastNo = currNo;
            } else if (perform  == '-') {
                result += lastNo;
                lastNo = -currNo;
            } else if (perform  == '*') {
                lastNo = lastNo * currNo;
            } else if (perform  == '/') {
                if (currNo == 0) {
                    *error = 1; 
                    return 0;
                }
                lastNo = lastNo / currNo;
            }
            perform  = c;
        }
    }
    result += lastNo;
    return result;
}

int main() {
    char exp[1000];
    printf("Enter expression: ");
    fgets(exp, sizeof(exp), stdin);

    exp[strcspn(exp, "\n")] = '\0';

    int error = 0;
    int result = evaluate(exp, &error);

    if (error == 1) {
        printf("Error: Division by zero.\n");
    } else if (error == 2) {
        printf("Error: Invalid expression.\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}
