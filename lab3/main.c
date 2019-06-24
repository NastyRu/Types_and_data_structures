#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0
#define FAILED 1

#define STEP 1000000
#define MAX_KOL 10000000

struct list {
    int data;
    struct list *next;
};

struct vector {
    int *data;
    int allocate;
    int size;
};

struct list *push_list(int data)
{
    struct list *buf = calloc(1, sizeof(struct list));
    buf->next = NULL;
    buf->data = data;
    return buf;
}

void list_add(struct list **list, int val)
{
   	for(; *list != NULL; list = &(*list)->next);
 	  *list = push_list(val);
}

void list_add_plus(struct list **list, int val)
{
    int data = (*list)->data;
   	for(; *list != NULL; data = (*list)->data, list = &(*list)->next);
 	  *list = push_list(val + data);
}

void list_change(struct list **list)
{
    int size, size0, before;
    size0 = (*list)->data;
    (*list)->data = 0;
    before = (*list)->data;
    list = &(*list)->next;
    for(; *list != NULL; list = &(*list)->next)
    {
        size = (*list)->data;
        (*list)->data = before + size0;
        size0 = size;
        before = (*list)->data;
    }
  }

void list_plus(struct list **list, int j)
{
   	for(int i = 0; i < j; i++, list = &(*list)->next);
 	  (**list).data += 1;
}

void push_vector(struct vector *a, int data)
{
    if (a->allocate == a->size)
    {
        a->allocate *= STEP;
        a->data = realloc(a->data, a->allocate);
    }
    a->data[a->size] = data;
    (a->size)++;
}

void print_list(const struct list *list)
{
    while (list)
    {
        printf("%d ", list->data);
        list = list->next;
    }
}

void print_vector(const struct vector vector)
{
    for (int i = 0; i < vector.size; i++)
    {
        printf("%d ", vector.data[i]);
    }
}

void list_free(struct list *list)
{
    while (list)
    {
        free(list);
        list = list->next;
    }
}

int number_list(struct list *list, int n)
{
    for (int i = 0; i < n; i++)
        list = list->next;
    return list->data;
}

void multiplication(struct list *ja, struct vector ia, struct vector a, struct vector column, struct vector row, int m, int n, struct vector *res, struct vector *res_col)
{
    clock_t begin, end;
    res->data = calloc(n, sizeof(int));
    res->allocate = n;
    res->size = n;

    res_col->data = calloc(n, sizeof(int));
    res_col->allocate = n;
    res_col->size = n;

    begin = clock();
    for (int i = 0; i < n; i++)
        res_col->data[i] = i;

    for (int i = 0; i < column.size; i++)
    {
        int k = row.data[i];
        for (int j = number_list(ja, k); j < number_list(ja, k+1); j++)
        {
            res->data[ia.data[j]] += column.data[i] * a.data[j];
        }
    }
    end = clock();
    printf("Время умножения матрицы на столбец в разреженном виде %d\n", (int)(end - begin));
}

void multiplication_matrix(struct list *ja, struct vector ia, struct vector a, struct vector column, struct vector row, int m, int n)
{
    clock_t begin, end;
    int matrix[n][m];
    int vector[m], res[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix[i][j] = 0;
    for (int j = 0; j < m; j++)
    {
        res[j] = 0;
        vector[j] = 0;
    }
    for (int i = 0; i < m; i++)
    {
            for (int j = number_list(ja, i); j < number_list(ja, i+1); j++)
            {
                matrix[ia.data[j]][i] = a.data[j];
            }
    }
    for (int i = 0; i < row.size; i++)
        vector[row.data[i]] = column.data[i];

    begin = clock();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            res[m] += matrix[i][j]*vector[j];
    }

    end = clock();

    printf("Время умножения матрицы на столбец в обычном виде %lu\n", (end - begin));
}

/*void print_matrix()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}*/

int main()
{
    srand(time(NULL));
    char buf[255];
    FILE *f1, *f2;
    struct vector a, ia, column, row, res, res_col;
    int n = 0, m = 0;
    struct list *ja = calloc(1, sizeof(struct list));
    int i, j, data;
    int var;

    ia.data = calloc(STEP, sizeof(int));
    ia.allocate = STEP;
    ia.size = 0;

    a.data = calloc(STEP, sizeof(int));
    a.allocate = STEP;
    a.size = 0;

    column.data = calloc(STEP, sizeof(int));
    column.allocate = STEP;
    column.size = 0;

    row.data = calloc(STEP, sizeof(int));
    row.allocate = STEP;
    row.size = 0;

    printf("Использовать заранее приготовленные данные? \nда - 1, нет - 2\n");
    if (1 != scanf("%d", &var))
    {
        char buf[255];
        scanf("%s", buf);
        printf("Некорректный ввод");
        return 1;
    }
    if (var != 1 && var != 2)
    {
        printf("Некорректный ввод");
        return 1;
    }
    if (1 == var)
    {
        printf("Введите номер файла\n");
        int filename;
        scanf("%d", &filename);
        switch (filename)
        {
            case 1:
            f1 = fopen("in_11.txt", "r");
            f2 = fopen("in_12.txt", "r");
            break;
            case 2:
            f1 = fopen("in_21.txt", "r");
            f2 = fopen("in_22.txt", "r");
            break;
        }
        if (f1 == NULL || f2 == NULL)
        {
            printf("Нет файла\n\n");
            return FAILED;
        }

        fscanf(f1, "%d %d\n", &n, &m);

        for (int k = 0; k < m; k++)
            list_add(&ja, 0);

        while (!feof(f1))
        {
            fscanf(f1, "%d", &i);
            fscanf(f1, "%d", &j);
            fscanf(f1, "%d\n", &data);
            push_vector(&a, data);
            push_vector(&ia, i);
            list_plus(&ja, j);
        }
        list_change(&ja);

        while (!feof(f2))
        {
            fscanf(f2, "%d", &i);
            fscanf(f2, "%d\n", &data);
            push_vector(&column, data);
            push_vector(&row, i);
        }
        fclose(f1);
        fclose(f2);
    }

    if (2 == var)
    {
        printf("Введите количество строк и стобцов\n");
        if (2 != scanf("%d %d", &n, &m))
        {
            char buf[255];
            scanf("%s", buf);
            printf("Некорректный ввод");
            return 1;
        }
        if (n <= 0 || m <= 0)
        {
            printf("Некорректный ввод");
            return 1;
        }

        printf("1. Ручной ввод \n2. Aвтозаполение\n");
        int var;
        if (1 != scanf("%d", &var))
        {
            char buf[255];
            scanf("%s", buf);
            printf("Некорректный ввод");
            return 1;
        }
        if (var != 1 && var != 2)
        {
            printf("Некорректный ввод");
            return 1;
        }
        if (2 == var)
        {
            printf("Введите количество ненулевых элементов\n");
            int percent;

            if (1 != scanf("%d", &percent))
            {
                char buf[255];
                scanf("%s", buf);
                printf("Некорректный ввод");
                return 1;
            }
            if (percent < 0 || percent > n*m)
            {
                printf("Некорректный ввод");
                return 1;
            }

            int del = percent / m, ost = percent % m, copy_ost = ost;

            for (int i = 0; i < percent; i++)
            {
                push_vector(&a, 1 + rand()%100);
            }

            for (int i = 1; i < m; i++)
            {
                if (ost > 0)
                {
                    list_add_plus(&ja, del + 1);
                    ost--;
                }
                else
                    list_add_plus(&ja, del);
            }

            list_add(&ja, percent);
            int x;

            for (int i = 0; i < m; i++)
            {
                for (int j = number_list(ja, i); j < number_list(ja, i+1) - 1;)
                {
                    x = 0 + rand() % n;
                    for (int k = number_list(ja, i); k <= j; k++)
                    {
                        if(ia.data[k] == x)
                            break;
                        else if (j == k)
                        {
                            j++;
                            push_vector(&ia, x);
                            break;
                        }
                    }
                }
                push_vector(&ia, 0);
            }

            /*int x = 0;
            for (int i = 0; i < m; i++)
            {
                for (int j = number_list(ja, i); j < number_list(ja, i + 1); j++)
                {
                    push_vector(&ia, x);
                    x++;
                }
                x = 0;
            }*/

            printf("\nЭлементы\n");
            print_vector(a);
            printf("\n");
            printf("Строки\n");
            print_vector(ia);
            printf("\n");
            printf("Столбцы\n");
            print_list(ja);
            printf("\n\n");
        }
        if (1 == var)
        {
            for (int k = 0; k < m; k++)
                list_add(&ja, 0);
            printf("Введите ненулевые элементы и их позицию, вводить по столбцу \nСтрока Столбец Значение элемента (индексация с 0)\nВвод до некорректного\n");
            while (1 == scanf("%d", &i) && i < n && 1 == scanf("%d", &j) && j < m && 1 == scanf("%d", &data))
            {
                push_vector(&a, data);
                push_vector(&ia, i);
                list_plus(&ja, j);
            }

            list_change(&ja);

            printf("\nЭлементы\n");
            print_vector(a);
            printf("\n");
            printf("Строки\n");
            print_vector(ia);
            printf("\n");
            printf("Столбцы\n");
            print_list(ja);
            printf("\n\n");
            scanf("%s", buf);
        }

        printf("Как ввести вектор-столбец?\n1. Ручной ввод \n2. Aвтозаполение\n");
        if (1 != scanf("%d", &var))
        {
            char buf[255];
            scanf("%s", buf);
            printf("Некорректный ввод");
            return 1;
        }
        if (var != 1 && var != 2)
        {
            printf("Некорректный ввод");
            return 1;
        }
        if (2 == var)
        {
            printf("Введите количество ненулевых элементов\n");
            int percent;

            if (1 != scanf("%d", &percent))
            {
                scanf("%s", buf);
                printf("Некорректный ввод");
                return 1;
            }
            if (percent < 0 || percent > m)
            {
                printf("Некорректный ввод");
                return 1;
            }

            for (int i = 0; i < percent; i++)
            {
                push_vector(&column, 1 + rand()%100);
            }

            int i = 1, x;
            while (i < percent)
            {
                x = 0 + rand() % n;
                for (int j = 0; j <= i; j++)
                {
                    if(row.data[j] == x)
                        break;
                    else if (j == i)
                    {
                        ++i;
                        push_vector(&row, x);
                        break;
                    }
                }

            }
            push_vector(&row, 0);

            int del = percent / m, ost = percent % m;

            printf("\nЭлементы\n");
            print_vector(column);
            printf("\n");
            printf("Строки\n");
            print_vector(row);
            printf("\n\n");
        }
        if (1 == var)
        {
            printf("Введите ненулевые элементы вектора столбца \nСтрока Значение элемента (индексация с 0)\nВвод до некорректного\n");
            while (1 == scanf("%d", &i) && i < m && 1 == scanf("%d", &data))
            {
                push_vector(&column, data);
                push_vector(&row, i);
            }
            scanf("%s", buf);

            printf("\nЭлементы\n");
            print_vector(column);
            printf("\n");
            printf("Строки\n");
            print_vector(row);
            printf("\n\n");
        }
    }

    printf("Что вы хотите сделать \n1. Вычислить произведение \n2. Сравнить время работы\n");
    int param;
    if (1 != scanf("%d", &param))
    {
        scanf("%s", buf);
        param = 0;
    }
    switch (param)
    {
        case 1:
        multiplication(ja, ia, a, column, row, m, n, &res, &res_col);
        printf("\nРезультат\n");
        print_vector(res);
        printf("\n");
        break;
        case 2:
        multiplication(ja, ia, a, column, row, m, n, &res, &res_col);
        multiplication_matrix(ja, ia, a, column, row, m, n);
        break;
        default: printf("Некорректная операция\n\n"); break;
    }

    free(a.data);
    free(ia.data);
    free(ja);
    free(column.data);
    free(row.data);
    free(res.data);
    free(res_col.data);

    return SUCCESS;
}
