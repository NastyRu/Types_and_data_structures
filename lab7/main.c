#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0
#define INCORRECT 1

#define MAX 1000000

int **init_matrix(int n)
{
    int **ukazat, *data;
    ukazat = calloc(n, sizeof(int *));
    if (ukazat == NULL)
        return NULL;
    data = calloc(n * n, sizeof(int));
    if (data == NULL)
    {
        free(ukazat);
        return NULL;
    }
    for (int i = 0; i < n; i++)
        ukazat[i] = data + i * n;

    return ukazat;
}

void free_matrix(int **ukazat)
{
    if (ukazat == NULL)
        return;
    free(ukazat[0]);
    free(ukazat);
}

int read_kol(FILE *f, int *n)
{
    if (1 != fscanf(f, "%d\n", n) || *n <= 0)
    {
        printf("Некорректные данные\n");
        return INCORRECT;
    }
    return SUCCESS;
}

int **allocate_matrix(const char *argv, int *n)
{
    FILE *f;
    char c;
    f = fopen(argv, "r");
    if (SUCCESS != read_kol(f, n))
        return NULL;

    int **matrix = init_matrix(*n);
    if (NULL == matrix)
    {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < (*n) - 1; i++)
    {
        for (int j = i + 1; j < (*n); j++)
        {
            if (1 != fscanf(f, "%d", &matrix[i][j]) && matrix[i][j] < 0)
            {
                printf("Некорректные данные\n");
                return NULL;
            }
            matrix[j][i] = matrix[i][j];
        }
    }

    fclose(f);
    return matrix;
}

int write_matrix(const int n, int **matrix)
{
    if (matrix == NULL)
        return INCORRECT;


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%4d ", matrix[i][j]);
        printf("\n");
    }
    return SUCCESS;
}

int dot(const char *filename, const int n, int **matrix)
{
    char filename_p[100];
    snprintf(filename_p, 100, "%s.gv", filename);
    FILE *f = fopen(filename_p, "w");
    fprintf(f, "%s\n", "graph test_tree {");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            if (matrix[i][j] != 0 && j > i)
                fprintf(f, "%d--%d[label=%d]\n", i + 1, j + 1, matrix[i][j]);
    }
    fprintf(f, "}\n");
    fclose(f);

    char str[1000];
    snprintf(str, 1000, "dot -Tpng %s.gv -o %s.png\n", filename, filename);

    system(str);

    return SUCCESS;
}

int find_way(int kol, int **matrix, int start, int end)
{
    int min_way[kol];
    int visited[kol];
    int temp;

    for (int i = 0; i < kol; i++)
    {
        min_way[i] = MAX;
        visited[i] = 1;
    }
    min_way[start - 1] = 0;

    int minindex = MAX;
    int min = MAX;

    do
    {
        minindex = MAX;
        min = MAX;
        for (int i = 0; i < kol; i++)
        {
            if ((visited[i] == 1) && (min_way[i] < min))
            {
                min = min_way[i];
                minindex = i;
            }
        }
        if (minindex != MAX)
        {
            for (int i = 0; i < kol; i++)
            {
                if (matrix[minindex][i] > 0)
                {
                    temp = min + matrix[minindex][i];
                    if (temp < min_way[i])
                    {
                        min_way[i] = temp;
                    }
                }
            }
            visited[minindex] = 0;
        }
    } while (minindex < MAX);

    if (MAX == min_way[end - 1])
    {
        return INCORRECT;
    }
    return SUCCESS;
}

int delete_ways(int kol, int **matrix, int start, int end)
{
    int buf1, buf2, buf3;

    for (int i = 0; i < kol; i++)
        for (int j = 0; j < kol; j++)
        {
            for (int k = i; k < kol; k++)
                for (int m = 0; m < kol; m++)
                {
                    for (int n = k; n < kol; n++)
                        for (int l = 0; l < kol; l++)
                        {
                            buf1 = matrix[i][j];
                            buf2 = matrix[k][m];
                            buf3 = matrix[n][l];

                            matrix[i][j] = 0;
                            matrix[k][m] = 0;
                            matrix[n][l] = 0;
                            matrix[j][i] = 0;
                            matrix[m][k] = 0;
                            matrix[l][n] = 0;

                            if (INCORRECT == find_way(kol, matrix, start, end))
                            {
                                return SUCCESS;
                            }
                            matrix[i][j] = buf1;
                            matrix[k][m] = buf2;
                            matrix[n][l] = buf3;
                            matrix[j][i] = buf1;
                            matrix[m][k] = buf2;
                            matrix[l][n] = buf3;
                        }
                }
        }
        return INCORRECT;
}

struct spisok {
    int top;
    int data;
    struct spisok *next;
};

void list_print(struct spisok *head)
{
    while (head != NULL)
    {
        printf("%d ", head->top + 1);
        head = head->next;
    }
    printf("\n");
}

void spisok_print(struct spisok *table[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("[%d] = ", i + 1);
        if (table[i] == NULL)
            printf("NULL\n");
        else
            list_print(table[i]);
    }
}

struct spisok *list_push(struct spisok *st, int data, int top)
{
    struct spisok *xst = calloc(1, sizeof(struct spisok));
    xst->data = data;
    xst->top = top;
    xst->next = st;
    return xst;
}

void list(const char *argv, struct spisok *st[])
{
    FILE *f = fopen(argv, "r");

    int n = 0, number;
    fscanf(f, "%d\n", &n);

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (1 != fscanf(f, "%d", &number) && number < 0)
            {
                printf("Некорректные данные\n");
                return;
            }
            if (number != 0)
            {
                st[i] = list_push(st[i], number, j);
                st[j] = list_push(st[j], number, i);
            }
        }
    }
    fclose(f);
}

int find_way_sp(int kol, struct spisok *table[], int start, int end)
{
    int min_way[kol];
    int visited[kol];
    int temp;
    struct spisok *table_copy[kol];
    for (int i = 0; i < kol; i++)
        table_copy[i] = table[i];

    for (int i = 0; i < kol; i++)
    {
        min_way[i] = MAX;
        visited[i] = 1;
    }
    min_way[start - 1] = 0;

    int minindex = MAX;
    int min = MAX;

    do
    {
        minindex = MAX;
        min = MAX;
        for (int i = 0; i < kol; i++)
        {
            if ((visited[i] == 1) && (min_way[i] < min))
            {
                min = min_way[i];
                minindex = i;
            }
        }
        if (minindex != MAX)
        {
            while (table_copy[minindex] != NULL)
            {
                if (table_copy[minindex]->data > 0)
                {
                    temp = min + table_copy[minindex]->data;
                    if (temp < min_way[table_copy[minindex]->top])
                    {
                        min_way[table_copy[minindex]->top] = temp;
                    }
                }
                table_copy[minindex] = table_copy[minindex]->next;
            }
           visited[minindex] = 0;
        }
    } while (minindex < MAX);

    if (MAX <= min_way[end - 1])
    {
        return INCORRECT;
    }
    return SUCCESS;
}

void insert(struct spisok **head, struct spisok *elem, struct spisok *before)
{
    if (NULL == head || NULL == (*head) || NULL == elem || NULL == before)
        return;
    struct spisok *buffer = (*head);

    if (buffer == before)
    {
        (*head) = elem;
        (*head)->next = before;
        return;
    }
    while (buffer->next != before)
    {
        buffer = buffer->next;
        if (NULL == buffer)
            return;
    }
    buffer->next = elem;
    buffer = buffer->next;
    buffer->next = before;
}

struct spisok* delete_list(struct spisok *head, int top)
{
    struct spisok *temp = head;

    if (temp->top == top)
    {
        temp->data = temp->data + MAX;
        return head;
    }
    /*if (NULL == temp)
        return head;*/
    while (temp != NULL && temp->top != top)
        temp = temp->next;
    if (NULL == temp)
        return head;
    temp->data = temp->data + MAX;
    return head;
}

struct spisok *number_list(struct spisok *list, int n)
{
    for (int i = 0; i < n; i++)
        list = list->next;
    return list;
}

int kol_list(struct spisok *list)
{
    int i = 0;
    while(list != NULL)
    {
        i++;
        list = list->next;
    }
    return i;
}

int check(int kol, int start, int end, struct spisok *table[], int i, struct spisok *bufi, int k, struct spisok *bufk, int n, struct spisok *bufn)
{
    struct spisok *table_copy[1000] = {};
    for (int i = 0; i < kol; i++)
        for (int j = 0; j < kol_list(table[i]); j++)
            table_copy[i] = list_push(table_copy[i], number_list(table[i], j)->data, number_list(table[i], j)->top);


    table_copy[i] = delete_list(table_copy[i], bufi->top);
    table_copy[bufi->top] = delete_list(table_copy[bufi->top], i);
    table_copy[k] = delete_list(table_copy[k], bufk->top);
    table_copy[bufk->top] = delete_list(table_copy[bufk->top], k);
    table_copy[n] = delete_list(table_copy[n], bufn->top);
    table_copy[bufn->top] = delete_list(table_copy[bufn->top], n);

    if (SUCCESS == find_way_sp(kol, table_copy, start, end))
    {
        return SUCCESS;
    }
    return INCORRECT;
}

int delete_ways_sp(int kol, struct spisok *table[], int start, int end)
{
    for (int i = 0; i < kol; i++)
        for (int j = 0; j < kol_list(table[i]); j++)
            for (int k = i; k < kol; k++)
                for (int m = 0; m < kol_list(table[k]); m++)
                    for (int n = k; n < kol; n++)
                        for (int l = 0; l < kol_list(table[n]); l++)
                        {
                            if (INCORRECT == check(kol, start, end, table, i, number_list(table[i], j), k, number_list(table[k], m), n, number_list(table[n], l)))
                            {
                                //printf("yes\n");
                                return  SUCCESS;
                            }
                        }

    //printf("Черт\n");
    return INCORRECT;
}

int main()
{
    int n = 0;
    int **matrix = allocate_matrix("file.txt", &n);

    int b = 1000000, e = 1000000;
    char buf[255];

    printf("Введите 2 вершины через пробел\n");
    while (1)
    {
        if (2 != scanf("%d %d", &b, &e))
        {
            scanf("%s", buf);
            printf("Некорректный ввод попробуйте еще раз\n");
        }
        if (b <= n && e <= n)
            break;
        else
            printf("Некорректный ввод попробуйте еще раз\n");
    }

    write_matrix(n, matrix);
    dot("graph", n, matrix);
    clock_t begin, end;
    begin = clock();
    int q = delete_ways(n, matrix, b, e);
    end = clock();
    printf("Время работы для матрицы %lu\n", end - begin);
    free_matrix(matrix);

    struct spisok *spisok[100] = {};
    list("file.txt", spisok);
    spisok_print(spisok, n);
    begin = clock();
    delete_ways_sp(n, spisok, b, e);
    end = clock();
    printf("Время работы для списка %lu\n", (end - begin) / 2);
    //spisok_print(spisok, n);

    if (q == SUCCESS)
        printf("Можно удалить\n");
    else
        printf("Нельзя удалить\n");
    return 0;
}
