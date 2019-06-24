#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILED 1
#define QMAX 100000

struct list *massiv_free[100000] = {0};
int free_kol = 0;

struct queue_massiv {
  int *qu[QMAX];
  int pin, pout;
};

void init_massiv(struct queue_massiv *q)
{
    q->pout = 1;
    q->pin = 0;
}

void push_massiv(struct queue_massiv *q, int *x)
{
    if (q->pin < QMAX-1) {
        q->pin++;
        q->qu[q->pin] = x;
    }
}

int isempty_massiv(struct queue_massiv *q)
{
    if (q->pin < q->pout)
        return 1;
    else
        return 0;
}

int *pop_massiv(struct queue_massiv *q)
{
    int *x;
    int h;

    if (isempty_massiv(q)==1)
        return NULL;

    x = q->qu[q->pout];
    for (h = q->pout; h < q->pin; h++)
    {
        q->qu[h] = q->qu[h+1];
    }
    (q->pin)--;
    return x;
}



struct list {
    struct list *next;
    int *data;
};

struct queue_list {
    int size;
    struct list *pout, *pin;
};

struct queue_list *init_list(void)
{
    struct queue_list *lt = calloc(1, sizeof(struct queue_list));
    lt->size = 0;
    lt->pout = NULL;
    lt->pin = lt->pout;
    return lt;
}

void push_list(struct queue_list *lt, int *data)
{
    struct list *node = calloc(1, sizeof(struct list));

    for (int i = 0; i < free_kol; i++)
    {
        if (massiv_free[i] == node)
        {
            free_kol--;
            for (int j = i; j < free_kol; j++)
                massiv_free[j] = massiv_free[j + 1];
            massiv_free[free_kol] = 0;
        }
    }
    node->data = data;
    if(lt->pin != NULL)
        lt->pin->next = node;
    else
    {
        lt->pout = node;
    }

    lt->pin = node;
    lt->size += 1;
}

int *pop_list(struct queue_list *lt)
{
    if (lt->size == 0)
    {
        return NULL;
    }

    struct list *node = lt->pout;
    int *ret_val = node->data;

    lt->size -= 1;
    lt->pout = node->next;
    massiv_free[free_kol] = node;
    free_kol++;
    free(node);

    if(lt->size == 0)
    {
        lt->pout = NULL;
        lt->pin = NULL;
    }
    return ret_val;
}

int main()
{
    srand(time(NULL));

    float t1 = (float)rand()/(float)(RAND_MAX/6), t2 = (float)rand()/(float)(RAND_MAX/1);
    float t1all = t1, t2all = t2;
    double time = 0;
    int n = 0, n1 = 0, n2 = 0;
    int l = 0;
    double step = 0.00001;
    char *buf;

    int var = 0;
    int t01, t02;

    printf("Как будем хранить очередь? \n1. Массивом \n2. Списком\n");
    if (1 != scanf("%d", &var) || (var != 1 && var != 2))
    {
        printf("Некорректное значение\n");
        return FAILED;
    }
    printf("Введите границу времени для поступления заявок\n");
    while (1 != scanf("%d", &t02) || t02 <= 0)
    {
        printf("Некорректное значение!\n");
        return FAILED;
    }

    printf("Введите границу времени для работы аппарата\n");
    while (1 != scanf("%d", &t01) || t01 <= 0)
    {
        printf("Некорректное значение!\n");
        return FAILED;
    }

    if (1 == var)
    {
        struct queue_massiv *q;
        int mas[1000000] = {0};
        int i = 0;
        int *a = NULL;
        int lo = 0, kol = 0, oa = 0;
        printf("Massiv\n");

        q = (struct queue_massiv *)calloc(1, sizeof(struct queue_massiv));
        init_massiv(q);
        push_massiv(q, mas + i);
        while (n < 1000)
        {
            time += step;
            if (time > t1all)
            {
                t1 = (float)rand()/(float)(RAND_MAX/t02);
                i++;
                push_massiv(q, mas + i);
                l++;
                t1all += t1;
            }
            if (time > t2all)
            {
                a = pop_massiv(q);
                lo += l;
                kol += 1;
                if (l > 0)
                    l--;
                if (NULL == a)
                    continue;
                oa++;
                t2 = (float)rand()/(float)(RAND_MAX/t01);
                t2all += t2;
                (*a) += 1;
                if ((*a) != 5)
                {
                    push_massiv(q, a);
                    l++;
                }
                else
                    n++;
                if (n % 100 == 0 && n != 0)
                {
                    printf("Очередь %d\n", l);
                    printf("Средняя очередь %d\n", lo / kol);
                }
            }
        }
        printf("Общее время моделирования %f\n", time);
        printf("Количество вышедших заявок %d\n", n);
        printf("Количество пришедших заявок %d\n", i + 1);
        printf("Количество срабатываний аппарата %d\n", oa);
        if ((t1all - t2all) < 0)
            printf("Время простоя %d\n", 0);
        else
            printf("Время простоя %f\n", t1all - t2all);
        return SUCCESS;
    }
    if (2 == var)
    {
        struct queue_list *q;
        int mas[1000000] = {0};
        int i = 0;
        int *a = NULL;
        int lo = 0, kol = 0, oa = 0;
        printf("List\n");

        q = init_list();
        push_list(q, mas + i);
        while (n < 1000)
        {
            time += step;
            if (time > t1all)
            {
                i++;
                t1 = (float)rand()/(float)(RAND_MAX/t02);
                push_list(q, mas + i);
                t1all += t1;
                l++;
            }
            if (time > t2all)
            {
                a = pop_list(q);
                lo += l;
                kol += 1;
                if (l > 0)
                    l--;
                if (NULL == a)
                    continue;
                oa++;
                t2 = (float)rand()/(float)(RAND_MAX/t01);
                t2all += t2;
                (*a) += 1;
                if ((*a) != 5)
                {
                    push_list(q, a);
                    l++;
                }
                else
                    n++;
                if (n % 100 == 0 && n != 0)
                {
                    printf("Очередь %d\n", l);
                    printf("Средняя очередь %d\n", lo / kol);
                }

            }
        }
        printf("Общее время моделирования %f\n", time);
        printf("Количество вышедших заявок %d\n", n);
        printf("Количество пришедших заявок %d\n", i + 1);
        printf("Количество срабатываний аппарата %d\n", oa);
        if ((t1all - t2all) < 0)
            printf("Время простоя %d\n", 0);
        else
            printf("Время простоя %f\n", t1all - t2all);

        printf("Количество ячеек памяти, которые были освобождены, но не заняты %d \n", free_kol);
        printf("Вывести?\n 1 - да\n 2 - нет\n");
        int y;
        scanf("%d", &y);
        if (1 == y)
        {
            for (int i = 0; i < free_kol; i++)
                printf("%p ", massiv_free[i]);
        }

        return SUCCESS;
    }

    printf("Некорректное значение\n");
    return FAILED;
}
