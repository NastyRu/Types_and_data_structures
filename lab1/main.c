#include <stdio.h>
#include <math.h>

#include "division.h"
#include "nules.h"
#include "number.h"

int main()
{
    int number1[MAX_NUMBER] = {0}, number2[MAX_NUMBER] = {0}, result[MAX_RESULT] = {0};
    int mantissa1 = 0, mantissa2 = 0, mantissa_res = 0;
    int znak1 = 0, znak2 = 0, znak_res = 0;
    int count1 = 0, count2 = 0, count_res = 0;

    printf("Division real numbers \n");
    printf("Format (+/-)m.n(e/E)+/-k \n");

    printf("|                            |\n");

    if (SUCCESS == long_number(number1, &znak1, &mantissa1, &count1) &&
            SUCCESS == long_number(number2, &znak2, &mantissa2, &count2))
    {
        nules(number1, &mantissa1, &count1);
        nules(number2, &mantissa2, &count2);

        if (znak1 != znak2)
            znak_res = 1;

        printf("\n");

        if (SUCCESS == division(result, &mantissa_res, &count_res, number1, number2,
                                mantissa1, mantissa2, count1, count2))
        {
            int k;

            nules(result, &mantissa_res, &count_res);

            if (result[MAX_NUMBER-1] > 4)
            {
                int vspom[MAX_NUMBER] = {0};
                vspom[MAX_NUMBER - 2] = 1;

                for (int i = MAX_NUMBER - 2; i >= 0; i--)
                {
                    result[i] += vspom[i];

                    if (result[i] > 9 && i > 0)
                    {
                        result[i-1]++;
                        result[i]-=10;
                    }
                }
            }



            if (abs(mantissa_res+count_res) < 100000)
            {
                printf("Answer:\n");

                if (znak_res)
                    printf("-");
                else
                    printf("+");
                printf("0.");

                k = count_res;

                if (k > (MAX_NUMBER-1))
                    k = MAX_NUMBER-1;



                for (int i = 0; i < k; i++)
                {
                    printf("%d",result[i]);
                }
                if (mantissa_res+count_res < 0)
                    printf("e%d", (mantissa_res+count_res));
                else
                    printf("e+%d", (mantissa_res+count_res));

                return SUCCESS;
            }
            else
            {
                printf("OVERFLOW");

                return DIVISION_IMPOSSIBLE;
            }
        }
    }
}
