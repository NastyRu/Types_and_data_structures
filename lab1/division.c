#include <stdio.h>
#include <math.h>

#include "division.h"
#include "number.h"

int division(int result[], int *mantissa_res, int *count_res, int number1[],
             int number2[], int mantissa1, int mantissa2, int count1, int count2)
{
    int number[MAX_RESULT];
    int i = 0, calc = 0;
    int count = count1;
    int k = 0;

    if (1 == count2 && 0 == number2[0])
    {
        printf("\nDivision by zero");
        return DIVISION_INTO_ZERO;
    }

    for(i = 0; i < count1; i++)
        number[i] = number1[i];

    while (count1 < MAX_RESULT)
    {
        number[count1] = 0;
        count1++;
        mantissa1--;
    }

    *mantissa_res = mantissa1 - mantissa2 + 1;

    while ((count2+i) < count1)
    {
        number2[count2+i] = 0;
        i++;
    }

    *count_res = count1 - count2;

    for(int j = 0; j < *count_res; j++)
    {
        calc = 0;
        i = 0;

        while (number[k] >= 0)
        {
            for (i = count1 - 1; i >= k; i--)
            {
                number[i] -= number2[i];
                if (number[i] < 0 && i > k)
                {
                    number[i-1]--;
                    number[i]+=10;
                }
            }
            if (number[k] >= 0)
                calc++;
        }

        for (i = count1 - 1; i >= k; i--)
        {
            number[i] += number2[i];

            if (number[i] > 9 && i > 0)
            {
                number[i-1]++;
                number[i]-=10;
            }
        }

        result[j] = calc;

        for (i = count1-1; i>0; i--)
            number2[i] = number2[i-1];

        number2[k] = 0;

        if (calc && 0 == number[k])
        {
            k+=1;

            if (j > 0 && 0 == result[j-1] && 0 == number[k])
                k++;
        }
    }

    return SUCCESS;
}
