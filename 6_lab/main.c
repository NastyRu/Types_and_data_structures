#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define SUCCESS 0
#define max(a, b) ((a) > (b) ? (a) : (b))

struct tree {
    int data;
    struct tree *left;
    struct tree *right;
};

struct avl_tree {
    int data;
    int height;
    struct avl_tree *left;
    struct avl_tree *right;
};

struct table {
    int data;
    struct table *next;
};

int _print_t_a(struct avl_tree *tree, int is_left, int offset, int depth, char s[60][255])
{
    char b[60];
    int width = 6;

    if (!tree) return 0;

    sprintf(b, "(%-4d)", tree->data);

    int left  = _print_t_a(tree->left,  1, offset, depth + 1, s);
    int right = _print_t_a(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left)
    {
        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';
        s[depth - 1][offset + left + width/2] = '.';
    }
    else if (depth && !is_left)
    {
        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';
        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left)
    {
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '/';
        s[2 * depth - 1][offset + left + width + right + width/2] = '.';
    }
    else if (depth && !is_left)
    {
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '\\';
        s[2 * depth - 1][offset - width/2 - 1] = '.';
    }
#endif
    return left + width + right;
}
void print_t_a(struct avl_tree *tree)
{
    char s[60][255];
    for (int i = 0; i < 60; i++)
        sprintf(s[i], "%200s", " ");

    _print_t_a(tree, 0, 0, 0, s);

    for (int i = 0; i < 60; i++)
    {
        int kol = 0;
        for (int j = 0; j < 200; j++)
        {
            if (s[i][j] == ' ')
                kol++;
        }
        if (200 == kol)
            break;
        printf("%s\n", s[i]);
    }
}
int _print_t_t(struct tree *tree, int is_left, int offset, int depth, char s[60][255])
{
    char b[60];
    int width = 6;

    if (!tree) return 0;

    sprintf(b, "(%-4d)", tree->data);

    int left  = _print_t_t(tree->left,  1, offset, depth + 1, s);
    int right = _print_t_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left)
    {
        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';
        s[depth - 1][offset + left + width/2] = '.';
    }
    else if (depth && !is_left)
    {
        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';
        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left)
    {
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '/';
        s[2 * depth - 1][offset + left + width + right + width/2] = '.';
    }
    else if (depth && !is_left)
    {
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '\\';
        s[2 * depth - 1][offset - width/2 - 1] = '.';
    }
#endif
    return left + width + right;
}
void print_t_t(struct tree *tree)
{
    char s[60][255];
    for (int i = 0; i < 60; i++)
        sprintf(s[i], "%200s", " ");

    _print_t_t(tree, 0, 0, 0, s);

    for (int i = 0; i < 60; i++)
    {
        int kol = 0;
        for (int j = 0; j < 200; j++)
        {
            if (s[i][j] == ' ')
                kol++;
        }
        if (200 == kol)
            break;
        printf("%s\n", s[i]);
    }
}

// Двоичное дерево поиска
struct tree *tree_add(struct tree *root, struct tree *r, int data)
{
    if(!r)
    {
        r = (struct tree *) calloc(1, sizeof(struct tree));
        if (r == NULL)
        {
            printf("Нет памяти\n");
            return NULL;
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        if (root == NULL)
            return r;
        if (data < root->data)
            root->left = r;
        else if (data > root->data)
            root->right = r;

        return r;
    }

    if (data < r->data)
        tree_add(r, r->left, data);
    else if (data > r->data)
        tree_add(r, r->right, data);

    return root;
}
struct tree *tree_find(struct tree *root, int data, int *finds)
{
    if (root == NULL || root->data == data)
    {
        (*finds)++;
        return root;
    }
    else if (root->data > data)
    {
        (*finds)++;
        return tree_find(root->left, data, finds);
    }
    else
    {
        (*finds)++;
        return tree_find(root->right, data, finds);
    }
    return NULL;
}

// Сбалансированное дерево
struct avl_tree *node_new(const int data)
{
    struct avl_tree *node = calloc(1, sizeof(struct avl_tree));
    if (node == NULL)
        return NULL;
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}
int tree_dif(struct avl_tree *tree)
{
    int right_height = 0;
    int left_height = 0;
    if (tree->right != NULL)
        right_height = tree->right->height;
    if (tree->left != NULL)
        left_height = tree->left->height;
    return right_height - left_height;
}
void tree_height(struct avl_tree *tree)
{
    int right_height = 0;
    int left_height = 0;

    if (tree == NULL)
        return;
    if (tree->right != NULL)
        right_height = tree->right->height;
    if (tree->left != NULL)
        left_height = tree->left->height;

    tree->height = max(left_height, right_height) + 1;
}
struct avl_tree *rotate_right(struct avl_tree *tree)
{
    struct avl_tree *left;
    if (NULL == tree)
        return NULL;
    left = tree->left;
    if (NULL == left)
        return NULL;

    tree->left = left->right;
    left->right = tree;
    tree_height(tree);
    tree_height(left);
    return left;
}
struct avl_tree *rotate_left(struct avl_tree *tree)
{
    struct avl_tree *right;
    if (NULL == tree)
        return NULL;
    right = tree->right;
    if (NULL == right)
        return NULL;

    tree->right = right->left;
    right->left = tree;
    tree_height(tree);
    tree_height(right);
    return right;
}
struct avl_tree *tree_balance(struct avl_tree *tree)
{
    if (NULL == tree)
        return NULL;

    tree_height(tree);
    switch (tree_dif(tree))
    {
        case 2:
            if (tree_dif(tree->right) < 0)
                tree->right = rotate_right(tree->right);
            return rotate_left(tree);
        case -2:
            if (tree_dif(tree->left) > 0)
                tree->left = rotate_left(tree->left);
            return rotate_right(tree);
        default:
            return tree;
    }
}
struct avl_tree *tree_insert(struct avl_tree *tree, const int data)
{
    if (tree == NULL)
        return node_new(data);

    else if (tree->data == data)
        return tree;
    else if (tree->data > data)
        tree->left = tree_insert(tree->left, data);
    else
        tree->right = tree_insert(tree->right, data);

    return tree_balance(tree);
}
struct avl_tree *tree_from_file()
{
    FILE * f;
    struct avl_tree *tree = NULL;
    int n;

    if (NULL == ( f = fopen("file.txt", "r")))
        return NULL;

    while (fscanf(f, "%d", &n) == 1 )
        tree = tree_insert(tree, n);

    fclose(f);
    return tree;
}
struct avl_tree *avl_tree_find(struct avl_tree *root, int data, int *finds)
{
    if (root == NULL || root->data == data)
    {
        (*finds)++;
        return root;
    }
    else if (root->data > data)
    {
        (*finds)++;
        return avl_tree_find(root->left, data, finds);
    }
    else
    {
        (*finds)++;
        return avl_tree_find(root->right, data, finds);
    }
    return NULL;
}

// Хеш-таблица
void list_print(struct table *head)
{
    while (head != NULL)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}
struct table *list_push(struct table *st, int data)
{
    struct table *xst = calloc(1, sizeof(struct table));
    xst->data = data;
    xst->next = st;
    return xst;
}
void hash_table_push(struct table *table[], int data, int kol)
{
    int i = abs(data) % kol;
    table[i] = list_push(table[i], data);
}
void hash_table_print(struct table *table[], int kol)
{
    for (int i = 0; i < kol; i++)
    {
        printf("[%d] = ", i);
        if (table[i] == NULL)
            printf("NULL\n");
        else
            list_print(table[i]);
    }
}
struct table *hash_table_find(struct table *table[], int kol, int data, int *finds)
{
    struct table *head = table[abs(data) % kol];
    while (head != NULL)
    {
        (*finds)++;
        if (head->data == data)
            return head;
        head = head->next;
    }
    return NULL;
}

// Файл
int file_find(FILE *f, int data, int *finds)
{
    while(!feof(f))
    {
        int num;
        fscanf(f, "%d\n", &num);
        (*finds)++;
        if (data == num)
            return 1;
    }
    return 0;
}

int main(void)
{
    int buffer = 0;
    int i = 0, kol_elem = 0;
    struct tree *root = NULL;
    FILE *f = fopen("file.txt", "r");

    while (!feof(f))
    {
        if (1 == fscanf(f, "%d\n", &buffer))
            i++;
        kol_elem++;
        root = tree_add(root, root, buffer);
    }
    if (0 == i)
    {
        printf("Файл пустой \n");
        return 1;
    }
    printf("Дерево\n");
    print_t_t(root);
    printf("\n");
    fclose(f);

    struct avl_tree *tree = tree_from_file();
    printf("Сбалансированное дерево\n");
    print_t_a(tree);
    printf("\n");

    struct table *hash_table[10000] = {};
    int prost = 1;
    for (int j = i; j < 10000; j++)
    {
        prost = 1;
        for (int k = 2; k * k <= j; k++)
            if (j % k == 0)
                prost = 0;
        if (1 == prost)
        {
            i = j;
            break;
        }
    }
    f = fopen("file.txt", "r");
    while (!feof(f))
    {
        fscanf(f, "%d\n", &buffer);
        hash_table_push(hash_table, buffer, i);
    }
    printf("Хеш-таблица\n");
    hash_table_print(hash_table, i);
    fclose(f);

    printf("\n");
    int number;
    char buf[255];
    printf("Введите число которое нужно найти\n");
    while (1)
    {
        if (1 == scanf("%d", &number))
            break;
        printf("Некорректное значение! Попробуйте еще раз\n");
        scanf("%s", buf);
    }

    int t_s = 0, a_s = 0, h_s = 0, f_s = 0;
    clock_t begin, end;
    begin = clock();
    if (NULL == tree_find(root, number, &t_s))
        printf("\nТакого числа нет \n\n");
    end = clock();
    printf("\nДвоичное дерево поиска\n");
    printf("Время %lu\nКоличество сравнений %d\nПамять %d\n\n", end - begin, t_s, 8 * 3 * kol_elem);

    begin = clock();
    avl_tree_find(tree, number, &a_s);
    end = clock();
    printf("Сбалансированное дерево\n");
    printf("Время %lu\nКоличество сравнений %d\nПамять %d\n\n", end - begin, a_s, 8 * 4 * kol_elem);

    begin = clock();
    hash_table_find(hash_table, i, number, &h_s);
    end = clock();
    printf("Хеш таблица\n");
    printf("Время %lu\nКоличество сравнений %d\nПамять %d\n\n", end - begin, h_s, 8 * 2 * kol_elem);

    f = fopen("file.txt", "r");
    begin = clock();
    file_find(f, number, &f_s);
    end = clock();
    fclose(f);
    printf("Файл\n");
    printf("Время %lu\nКоличество сравнений %d\nПамять %lu\n\n", end - begin, f_s, sizeof(int));


    printf("Среднее время поиска\n");
    int time_t = 0, time_a = 0, time_h = 0, time_f = end - begin;
    FILE *fp = fopen("file.txt", "r");
    int a;
    while (!feof(fp))
    {
        int t = 0;
        fscanf(fp, "%d\n", &a);

        begin = clock();
        tree_find(root, number, &t_s);
        end = clock();
        t = end - begin;

        time_t += t;

        begin = clock();
        avl_tree_find(tree, number, &a_s);
        end = clock();
        t = end - begin;

        time_a += t;

        begin = clock();
        hash_table_find(hash_table, i, number, &h_s);
        end = clock();
        t = end - begin;
        time_h += t;
    }

    printf("Для двоичного дерева поиска %f\n", (float) time_t / kol_elem);
    printf("Для сбалансированного дерева %f\n", (float) time_a / kol_elem);
    printf("Для хеш-таблицы %f\n", (float) time_h / kol_elem);
    printf("Для файла %f\n", (float) time_f);
    fclose(fp);


    return SUCCESS;
}
