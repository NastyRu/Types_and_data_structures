#include <stdio.h>
#include <math.h>

#include "number.h"
#include "nules.h"

int nules(int number[], int *mantissa, int *count)
{
    int i = 0;

    while(0 == number[i] && *count > 1)
    {
        (*count)--;
        for(int j = 0; j < (*count); j++)
            number[j] = number[j+1];
    }

    i = (*count) - 1;

    while(0 == number[i] && i > 0)
    {
        (*count)--;
        (*mantissa)++;
        i--;
    }

    if (0 == number[0])
        *mantissa = 0;

    return SUCCESS;
}
