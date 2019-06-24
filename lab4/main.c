#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SUCCESS 0
#define MAX_LEN 10

struct stack {
    int data;
    struct stack *next;
};

int *massiv_free[1000] = {0};
int free_kol = 0;

int menu()
{
    printf("Выберите, что хотите сделать?\n1. Добавить элемент\n2. Удалить элемент\n3. Распечатать стек\n4. Выйти из программы\n");
    int val = 0;
    char buf[255];
    while (1)
    {
        if (1 != scanf("%d", &val))
        {
            scanf("%s", buf);
        }
        if (1 == val || 2 == val || 3 == val || 4 == val)
            break;
        printf("Некорректный ввод. Попробуйте еще раз.\n");
    }

    return val;
}

struct stack *stack_push(struct stack *st, int data)
{
    struct stack *xst = calloc(1, sizeof(struct stack));
    xst->data = data;
    xst->next = st;
    printf("Элемент добавлен в %p\n", &xst->data);
    for (int i = 0; i < free_kol; i++)
    {
        if (&xst->data == massiv_free[i])
        {
            free_kol--;
            for (int j = i; j < free_kol; j++)
                massiv_free[j] = massiv_free[j + 1];
            massiv_free[free_kol] = 0;
        }
    }
    printf("Свободные области\n");
    for (int i = 0; i < free_kol; i++)
        printf("%p ", massiv_free[i]);
    printf("\n");
    return xst;
}

struct stack *stack_pop(struct stack *st, int *dst)
{
    massiv_free[free_kol] = st;
    free_kol++;
    printf("Свободные области\n");
    for (int i = 0; i < free_kol; i++)
        printf("%p ", massiv_free[i]);
    printf("\n");
    dst = &(st->data);
    struct stack *head = st->next;
    free(st);
    return head;
}

void stack_print_list(struct stack **st)
{
    int buf;
    int i = 0, k = 0;
    struct stack *st_copy = *st;
    clock_t begin, end;

    begin = clock();

    st_copy = *st;
    buf = (*st)->data;
    printf("%d ", buf);
    i++;
    (*st) = (*st)->next;
    free(st_copy);
    while (*st)
    {
        st_copy = *st;
        if ((*st)->data > buf)
            printf("\n");
        buf = (*st)->data;
        printf("%d ", buf);
        i++;
        (*st) = (*st)->next;
        free(st_copy);
    }

    end = clock();
    printf("\n");
    printf("Время выполнения для матрицы в виде списка %lu\n", end - begin);
    st = NULL;
}

void stack_print_massiv(int *st, int kol)
{
    int i = kol, k = 0;
    clock_t begin, end;

    if (0 == kol)
    {
        printf("Стек пустой\n");
        return;
    }

    begin = clock();
    printf("Стек\n");
    printf("%d ", st[0]);
    for (int j = 1; j < i; j++)
    {
        if (st[j] > st[j - 1])
            printf("\n");
        printf("%d ", st[j]);
    }
    end = clock();

    printf("\n");
    printf("Время выполнения для матрицы в виде массива %lu\n", end - begin);
}

int main()
{
    printf("Как будем хранить стек? \n1. В массиве \n2. В списке \n");
    int val, bool = 1;
    char buf[255];
    while (1)
    {
        if (1 != scanf("%d", &val))
        {
            scanf("%s", buf);
        }
        if (1 == val || 2 == val)
            break;
        printf("Некорректный ввод. Попробуйте еще раз.\n");
    }

    if (1 == val)
    {
        int *st = calloc(MAX_LEN, sizeof(int));
        int len = MAX_LEN, kol = 0;

        while (bool)
        {
            switch (menu())
            {
                case 1:
                    if (kol == len)
                    {
                        printf("Память заполнена, хотите аллоцировать дополнительно?\n1. да\n2. нет\n");
                        int val;
                        while (1)
                        {
                            if (1 != scanf("%d", &val))
                            {
                                scanf("%s", buf);
                            }
                            if (1 == val || 2 == val)
                                break;
                            printf("Некорректный ввод. Попробуйте еще раз.\n");
                        }
                        if (2 == val)
                            break;
                        if (1 == val)
                        {
                            st = realloc(st, len * 10);
                            len *= 10;
                        }
                    }
                    printf("Введите элемент\n");

                    int data = 0;
                    while (1)
                    {
                        if (1 != scanf("%d", &data))
                        {
                            scanf("%s", buf);
                        }
                        else
                            break;
                        printf("Некорректный ввод. Попробуйте еще раз.\n");
                    }
                    for (int i = kol; i > 0; i--)
                        st[i] = st[i - 1];
                    st[0] = data;
                    kol++;
                    printf("Элемент добавлен\n");
                    break;
                case 2:
                    if (0 == kol)
                    {
                        printf("Стек пустой\n");
                        break;
                    }
                    for (int i = 0; i < kol; i++)
                        st[i] = st[i + 1];
                    st[kol - 1] = 0;
                    kol--;
                    printf("Элемент удален\nОсвобожден адрес %p\n", st + kol);
                    break;
                case 3:
                    if (0 == kol)
                    {
                        printf("Стек пустой\n");
                        break;
                    }
                    stack_print_massiv(st, kol);
                    free(st);
                    st = calloc(MAX_LEN, sizeof(int));
                    len = MAX_LEN;
                    kol = 0;
                    break;
                case 4: bool = 0; break;
            }
        }
    }

    if (2 == val)
    {
        struct stack *st;
        int kol = 0;
        int *x;

        while(bool)
        {
            switch (menu())
            {
                case 1:
                    if (kol == 1000)
                    {
                        printf("Стек переполнен\n");
                        break;
                    }
                    int data = 0;
                    printf("Введите элемент\n");
                    while (1)
                    {
                        if (1 != scanf("%d", &data))
                        {
                            scanf("%s", buf);
                        }
                        else
                            break;
                        printf("Некорректный ввод. Попробуйте еще раз.\n");
                    }
                    st = stack_push(st, data);
                    kol++;
                    break;
                case 2:
                    if (0 == kol)
                    {
                        printf("Стек пустой\n");
                        break;
                    }
                    st = stack_pop(st, x);
                    kol--;
                    break;
                case 3:
                    if (0 == kol)
                    {
                        printf("Стек пустой\n");
                        break;
                    }
                    stack_print_list(&st);
                    kol = 0;
                    break;
                case 4: bool = 0; break;
            }
        }
    }
    return SUCCESS;
}
