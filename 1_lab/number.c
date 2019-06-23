#include <stdio.h>
#include <math.h>

#include "number.h"

int long_number(int number[], int *znak, int *mantissa, int *count)
{
    char c = 0;
    char mantissa_count[MAX_MANTISSA];

    int point = 0, j = 0, k = 0;

    printf("Input number \n");

    scanf("%c", &c);
    if ('-' == c)
    {
        *znak = 1;
        scanf("%c", &c);
    }
    else if ('+' == c)
        scanf("%c", &c);
    else if (!(c <= '9' && c >= '0' || '.' == c))
    {
        printf("Incorrect input");
        return INCORRECT_INPUT;
    }

    while ('\n' != c && 'e' != c && 'E' != c)
    {
        if (c <= '9' && c >= '0')
        {
            number[*count] = (int)c - 48;
            (*count)++;

            if (point)
                (*mantissa)--;
        }
        else if (c == '.' && 0 == point)
            point = 1;
        else
        {
            printf("Incorrect input");
            return INCORRECT_INPUT;
        }
        scanf("%c", &c);
    }

    while (c != '\n')
    {
        scanf("%c", &c);
        mantissa_count[k] = c;
        k++;
        if (!(k > 1 && c <= '9' && c >= '0' || (1 == k && ('-' == c || '+' == c)) || '\n' == c))
        {
            printf("Incorrect input");
            return INCORRECT_INPUT;
        }
    }

    k--;

    if (*count > MAX_NUMBER || k > (MAX_MANTISSA-1))
    {
        printf("Incorrect input");
        return INCORRECT_INPUT;
    }

    if ('-' == mantissa_count[0])
    {
        for (j = 1; j < k; j++)
            (*mantissa) -= ((int)mantissa_count[j]-48)*pow(10,k-j-1);
    }
    else
    {
        for (j = 1; j < k; j++)
            (*mantissa) += ((int)mantissa_count[j]-48)*pow(10,k-j-1);
    }

    return SUCCESS;
}
