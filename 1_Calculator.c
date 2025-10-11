#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


int evaluateExpression(char *expression, int *error)
{
    int length = strlen(expression);
    long result = 0, lastNumber = 0, currentNumber = 0;
    char operation = '+';

    for (int i = 0; i < length; i++)
     {
        char currentChar = expression[i];

       
        if (isspace(currentChar)) 
            continue;

        if (isdigit(currentChar)) 
        {
            currentNumber = 0;
            while (i < length && isdigit(expression[i]))
             {
                currentNumber = currentNumber * 10 + (expression[i] - '0');
                i++;
            }
            i--; 
        }
       
        else if (currentChar != '+' && currentChar != '-' && currentChar != '*' && currentChar != '/') 
        {
            *error = 2;  
            return 0;
        }

        
        if (!isdigit(currentChar) || i == length - 1) 
        {
            if (operation == '+')
             {
                result += lastNumber;
                lastNumber = currentNumber;
            } else if (operation == '-') 
            {
                result += lastNumber;
                lastNumber = -currentNumber;
            } else if (operation == '*') 
            {
                lastNumber = lastNumber * currentNumber;
            } else if (operation == '/') 
            {
                if (currentNumber == 0) 
                {
                    *error = 1; 
                    return 0;
                }
                lastNumber = lastNumber / currentNumber;
            }
            operation = currentChar;
        }
    }

    result += lastNumber;
    return result;
}

int main() {
    char expression[1000];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);

   
    expression[strcspn(expression, "\n")] = '\0';

    int error = 0;
    int result = evaluateExpression(expression, &error);

    if (error == 1) {
        printf("Error: Division by zero.\n");
    } else if (error == 2) {
        printf("Error: Invalid expression.\n");
    } else {
        printf("Result: %d\n", result);
    }

    return 0;
}
