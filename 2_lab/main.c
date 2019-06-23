#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define SUCCESS 0
#define INCORRECT 1
#define LEN 50
#define LEN_TABLE 1000

struct perfomance {
    char theatre[LEN];
    char name[LEN];
    char producer[LEN];
    int begin_price;
    int end_price;
    char type[LEN];
    union {
        struct {
            int age;
            char type_child[LEN];
            char composer[LEN];
            char country[LEN];
            int duration;
        } child;
        struct {
            char type_adult[LEN];
        } adult;
    };
};

struct key_perfomance {
    int number;
    char name[LEN];
};

void functional(void)
{
    printf("1.Загрузка данных из файла\n\
2.Сохранение изменений в файл\n\
3.Добавление записи\n\
4.Удаление записи\n\
5.Сортировка таблицы\n\
6.Печать таблицы\n\
7.Сортировка таблицы ключей\n\
8.Печать таблицы ключей\n\
9.Печать таблицы в послдеовательности ключей\n\
10.Поиск музыкальных спекталей для детей по возрасту и продолжительности\n\
11.Закрыть\n");
}

int download_file(struct perfomance *poster, struct key_perfomance *key, int *size)
{
    int i = 0, j = 0;
    FILE *f;
    char filename;
    char c;

    printf("Введите имя файла\n");
    scanf("%s", &filename);
    f = fopen(&filename, "r");

    if (f == NULL)
    {
        printf("Нет файла\n\n");
        *size = 0;
        return INCORRECT;
    }

    fscanf(f, "%c", &c);
    while (!feof(f))
    {
        j = 0;
        while (c != '|')
        {
            poster[i].theatre[j] = c;
            j++;
            fscanf(f, "%c", &c);
        }
        poster[i].theatre[j] = '\0';

        j = 0;
        fscanf(f, "%c", &c);
        while (c != '|')
        {
            poster[i].name[j] = c;
            key[i].name[j] = c;
            j++;
            fscanf(f, "%c", &c);
        }
        poster[i].name[j] = '\0';
        key[i].name[j] = '\0';
        key[i].number = i + 1;

        j = 0;
        fscanf(f, "%c", &c);
        while (c != '|')
        {
            poster[i].producer[j] = c;
            j++;
            fscanf(f, "%c", &c);
        }
        poster[i].producer[j] = '\0';

        fscanf(f, "%d-%d|", &poster[i].begin_price, &poster[i].end_price);

        j = 0;
        fscanf(f, "%c", &c);
        while (c != '|')
        {
            poster[i].type[j] = c;
            j++;
            fscanf(f, "%c", &c);
        }
        poster[i].type[j] = '\0';

        if (0 == strcmp(poster[i].type, "детский"))
        {
            fscanf(f, "%d|", &poster[i].child.age);

            j = 0;
            fscanf(f, "%c", &c);
            while (c != '|' && c != '\n')
            {
                poster[i].child.type_child[j] = c;
                j++;
                fscanf(f, "%c", &c);
            }
            poster[i].child.type_child[j] = '\0';

            if (0 == strcmp(poster[i].child.type_child, "музыкальный"))
            {
                j = 0;
                fscanf(f, "%c", &c);
                while (c != '|')
                {
                    poster[i].child.composer[j] = c;
                    j++;
                    fscanf(f, "%c", &c);
                }
                poster[i].child.composer[j] = '\0';

                j = 0;
                fscanf(f, "%c", &c);
                while (c != '|')
                {
                    poster[i].child.country[j] = c;
                    j++;
                    fscanf(f, "%c", &c);
                }
                poster[i].child.country[j] = '\0';

                fscanf(f, "%d|", &poster[i].child.duration);
            }
        }
        if (strcmp(poster[i].type, "взрослый") == 0)
        {
            j = 0;
            fscanf(f, "%c", &c);
            while (c != '|')
            {
                poster[i].adult.type_adult[j] = c;
                j++;
                fscanf(f, "%c", &c);
            }
            poster[i].adult.type_adult[j] = '\0';
        }
        i++;
        fscanf(f, "%c", &c);
        fscanf(f, "%c", &c);
    }

    *size = i;

    fclose(f);
    printf("Файл загружен\n\n");
    return SUCCESS;
}

int save_to_file(struct perfomance *poster, int *size)
{
    FILE *f;
    char filename;
    printf("Введите имя файла ");
    scanf("%s", &filename);
    f = fopen(&filename, "w");

    if ((*size) == 0)
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    for (int i = 0; i < *size; i++)
    {
        fprintf(f, "%s|%s|%s|%d-%d|%s|", poster[i].theatre, poster[i].name,
        poster[i].producer, poster[i].begin_price, poster[i].end_price, poster[i].type);

        if (0 == strcmp(poster[i].type, "детский"))
        {
            if (0 == strcmp(poster[i].child.type_child, "музыкальный"))
            {
                fprintf(f, "%d|%s|%s|%s|%d|\n",
                poster[i].child.age, poster[i].child.type_child,
                poster[i].child.composer, poster[i].child.country,poster[i].child.duration);
            }
            else
            {
                fprintf(f, "%d|%s|\n", poster[i].child.age, poster[i].child.type_child);
            }
        }

        if (strcmp(poster[i].type, "взрослый") == 0)
        {
            fprintf(f, "%s|\n",poster[i].adult.type_adult);
        }
    }
    printf("Файл сохранен\n\n");
    fclose(f);
    return SUCCESS;
}

int add_record(struct perfomance *poster, struct key_perfomance *key, int *size)
{
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }

    if ((*size) >= LEN_TABLE)
    {
        printf("Таблица переполнена\n\n");
        return INCORRECT;
    }

    int i = *size;
    char c;
    int j = 1;
    poster[i].begin_price = 0;
    poster[i].end_price = 0;
    int k, m;

    printf("Введите театр(меньше 50 символов) ");
    m = 0;
    scanf("%c", &c);
    scanf("%c", &c);
    while (c != '\n')
    {
        poster[i].theatre[m] = c;
        scanf("%c", &c);
        m++;
    }
    poster[i].theatre[m] = '\0';

    printf("Введите название спектакля(меньше 50 символов) ");
    m = 0;
    scanf("%c", &c);
    while (c != '\n')
    {
        poster[i].name[m] = c;
        scanf("%c", &c);
        m++;
    }
    poster[i].name[m] = '\0';
    int point = 0;
    while (poster[i].name[point] != '\0')
    {
        key[i].name[point] = poster[i].name[point];
        point++;
    }

    printf("Введите режиссера(меньше 50 символов) ");
    m = 0;
    scanf("%c", &c);
    while (c != '\n')
    {
        poster[i].producer[m] = c;
        scanf("%c", &c);
        m++;
    }
    poster[i].producer[m] = '\0';

    printf("Введите минимальную цену ");
    k = scanf("%d", &poster[i].begin_price);
    char buf[255];
    while (!(k == 1 && poster[i].begin_price > 0))
    {
        printf("Некорректный ввод, попробуйте еще раз ");
        if (k != 1)
            scanf("%s", buf);
        k = scanf("%d", &poster[i].begin_price);
    }

    printf("Введите максимальную цену ");
    k = scanf("%d", &poster[i].end_price);
    while (!(k == 1 && poster[i].end_price > 0 && poster[i].begin_price < poster[i].end_price))
    {
        printf("Некорректный ввод, попробуйте еще раз ");
        if (k != 1)
            scanf("%s", buf);
        k = scanf("%d", &poster[i].end_price);
    }

    printf("Введите тип спектакля (взрослый/детский) ");
    scanf("%s", poster[i].type);

    while (0 != strcmp(poster[i].type, "детский") &&
    0 != strcmp(poster[i].type, "взрослый"))
    {
        printf("Некорректный ввод, попробуйте еще раз ");
        scanf("%s", poster[i].type);
    }
    if (0 == strcmp(poster[i].type, "детский"))
    {
        printf("Введите возраст ребенка ");
        k = scanf("%d", &poster[i].child.age);
        while (!(k == 1 && poster[i].child.age >= 0))
        {
            printf("Некорректный ввод, попробуйте еще раз ");
            if (k != 1)
                scanf("%s", buf);
            k = scanf("%d", &poster[i].child.age);
        }

        printf("Введите тип спектакля(сказка/пьеса/музыкальный) ");
        scanf("%s", poster[i].child.type_child);
        while (0 != strcmp(poster[i].child.type_child, "музыкальный") &&
        0 != strcmp(poster[i].child.type_child, "сказка") &&
        0 != strcmp(poster[i].child.type_child, "пьеса"))
        {
            printf("Некорректный ввод, попробуйте еще раз ");
            scanf("%s", poster[i].child.type_child);
        }

        if (0 == strcmp(poster[i].child.type_child, "музыкальный"))
        {
            printf("Введите композитора ");
            m = 0;
            scanf("%c", &c);
            scanf("%c", &c);
            while (c != '\n')
            {
                poster[i].child.composer[m] = c;
                scanf("%c", &c);
                m++;
            }
            poster[i].child.composer[m] = '\0';
            printf("Введите страну ");
            m = 0;
            scanf("%c", &c);
            while (c != '\n')
            {
                poster[i].child.country[m] = c;
                scanf("%c", &c);
                m++;
            }
            poster[i].child.country[m] = '\0';
            printf("Введите продолжительность ");
            k = scanf("%d", &poster[i].child.duration);
            while (!(k == 1 && poster[i].child.duration > 0))
            {
                printf("Некорректный ввод, попробуйте еще раз ");
                if (k != 1)
                    scanf("%s", buf);
                k = scanf("%d", &poster[i].child.duration);
            }
        }
    }
    else if (strcmp(poster[i].type, "взрослый") == 0)
    {
        printf("Введите тип спектакля(драма/пьеса/комедия) ");
        scanf("%s", poster[i].adult.type_adult);
        while (0 != strcmp(poster[i].adult.type_adult, "драма") &&
        0 != strcmp(poster[i].adult.type_adult, "комедия") &&
        0 != strcmp(poster[i].adult.type_adult, "пьеса"))
        {
            printf("Некорректный ввод, попробуйте еще раз ");
            scanf("%s", poster[i].adult.type_adult);
        }
    }

    (*size)++;
    key[i].number = *size;
    printf("Запись добавлена\n\n");

    return SUCCESS;
}
int delete_record(struct perfomance *poster, struct key_perfomance *key, int *size)
{
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    printf("Какой спектакль удалить из таблицы? ");
    char s[LEN];
    int kol = *size;
    scanf("%s", s);
    for (int i = 0; i < *size; i++)
    {
        if (0 == strcmp(poster[i].name, s))
        {
            for (int j = i; j < (*size) - 1; j++)
            {
                poster[j] = poster[j+1];
                key[j] = key[j+1];
            }
            (*size)--;
        }
    }
    if (kol == *size)
        printf("Запись не найдена\n\n");
    else
        printf("Запись удалена\n\n");
    return SUCCESS;
}

int comparator_table(const void *a, const void *b)
{
    struct perfomance *ia = (struct perfomance *)a;
    struct perfomance *ib = (struct perfomance *)b;
    return strcmp(ia->name, ib->name);
}

int sort_table(struct perfomance *poster, int *size)
{
    clock_t begin, end;

    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }

    struct perfomance copy[LEN_TABLE];

    for (int i = 0; i < (*size); i++)
    {
        copy[i] = poster[i];
    }

    struct perfomance buf;
    begin = clock();
    for (int i = 1; i < (*size); i++)
    {
        for (int j = 0; j < (*size) - i; j++)
        {
            if (strcmp(poster[j].name, poster[j + 1].name) > 0)
            {
                buf = poster[j];
                poster[j] = poster[j + 1];
                poster[j + 1] = buf;
            }
        }
    }
    end = clock();
    printf("Время сортировки пузырьком %d такт\n", (int)(end - begin));
    begin = clock();
    qsort(copy, *size, sizeof(struct perfomance), comparator_table);
    end = clock();
    printf("Время быстрой сортировки %d такт\n\n", (int)(end - begin));
    return SUCCESS;
}
int print_table(struct perfomance *poster, int *size)
{
    if ((*size) == 0)
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    for (int i = 0; i < *size; i++)
    {
        printf("Номер: %d\nТеатр: %s\nСпектакль: %s\nРежиссер: %s\nДиапазон цен: %d-%d\nДля кого: %s\n", i+1,
        poster[i].theatre, poster[i].name,
        poster[i].producer, poster[i].begin_price, poster[i].end_price, poster[i].type);

        if (0 == strcmp(poster[i].type, "детский"))
        {
            if (0 == strcmp(poster[i].child.type_child, "музыкальный"))
            {
                printf("Возраст: %d\nТип: %s\nКомпозитор: %s\nСтрана: %s\nПродолжительность: %d мин\n\n",
                poster[i].child.age, poster[i].child.type_child,
                poster[i].child.composer, poster[i].child.country,poster[i].child.duration);
            }
            else
            {
                printf("Возраст: %d\nТип: %s\n\n", poster[i].child.age, poster[i].child.type_child);
            }
        }

        if (strcmp(poster[i].type, "взрослый") == 0)
        {
            printf("Тип: %s\n\n",poster[i].adult.type_adult);
        }
    }
    return SUCCESS;
}

int comparator_key(const void *a, const void *b)
{
    struct key_perfomance *ia = (struct key_perfomance *)a;
    struct key_perfomance *ib = (struct key_perfomance *)b;
    return strcmp(ia->name, ib->name);
}

int sort_key_table(struct key_perfomance *key, int *size)
{
    clock_t begin, end;
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }

    struct key_perfomance tmp;

    struct key_perfomance copy[LEN_TABLE];

    for (int i = 0; i < (*size); i++)
    {
        copy[i] = key[i];
    }
    begin = clock();
    for (int i = 1; i < (*size); i++)
    {
        for (int j = 0; j < (*size) - i; j++)
        {
            if (strcmp(key[j].name, key[j + 1].name) > 0)
            {
                tmp = key[j];
                key[j] = key[j + 1];
                key[j + 1] = tmp;
            }
        }
    }
    end = clock();
    printf("Время сортировки пузырьком %d такт\n", (int)(end - begin));
    begin = clock();
    qsort(copy, *size, sizeof(struct key_perfomance), comparator_key);
    end = clock();
    printf("Время быстрой сортировки %d такт\n\n", (int)(end - begin));
    return SUCCESS;
}
int print_key_table(struct key_perfomance *key, int *size)
{
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    for (int i = 0; i < *size; i++)
        printf("%d %s\n", key[i].number, key[i].name);
    return SUCCESS;
    printf("\n");
}
int print_table_by_key(struct perfomance *poster, struct key_perfomance *key, int *size)
{
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    for (int j = 0; j < *size; j++)
    {
        int i = key[j].number - 1;
        printf("Номер: %d\nТеатр: %s\nСпектакль: %s\nРежиссер: %s\nДиапазон цен: %d-%d\nДля кого: %s\n", i+1,
        poster[i].theatre, poster[i].name,
        poster[i].producer, poster[i].begin_price, poster[i].end_price, poster[i].type);

        if (0 == strcmp(poster[i].type, "детский"))
        {
            if (0 == strcmp(poster[i].child.type_child, "музыкальный"))
            {
                printf("Возраст: %d\nТип: %s\nКомпозитор: %s\nСтрана: %s\nПродолжительность: %d мин\n\n",
                poster[i].child.age, poster[i].child.type_child,
                poster[i].child.composer, poster[i].child.country,poster[i].child.duration);
            }
            else
            {
                printf("Возраст: %d\nТип: %s\n\n", poster[i].child.age, poster[i].child.type_child);
            }
        }

        if (strcmp(poster[i].type, "взрослый") == 0)
        {
            printf("Тип: %s\n\n",poster[i].adult.type_adult);
        }
    }
    return SUCCESS;
}
int search(struct perfomance *poster, int *size)
{
    if (0 == (*size))
    {
        printf("Нет загруженной таблицы\n\n");
        return INCORRECT;
    }
    char buf[255];
    int age, duration, num = 0;
    printf("Введите возраст ребенка ");
    if (1 != scanf("%d", &age))
    {
        printf("Некорректный ввод\n\n");
        scanf("%s", buf);
        return INCORRECT;
    }
    printf("Введите максимальную продолжительность ");
    if (1 != scanf("%d", &duration))
    {
        printf("Некорректный ввод\n\n");
        scanf("%s", buf);
        return INCORRECT;
    }

    for (int i = 0; i < *size; i++)
    {
        if (0 == strcmp(poster[i].type, "детский") && poster[i].child.age <= age &&
        0 == strcmp(poster[i].child.type_child, "музыкальный") && poster[i].child.duration < duration)
        {
            printf("Номер: %d\nТеатр: %s\nСпектакль: %s\nРежиссер: %s\nДиапазон цен: %d-%d\nДля кого: %s\n", i+1,
            poster[i].theatre, poster[i].name,
            poster[i].producer, poster[i].begin_price, poster[i].end_price, poster[i].type);

            printf("Возраст: %d\nТип: %s\nКомпозитор: %s\nСтрана: %s\nПродолжительность: %d мин\n\n",
                    poster[i].child.age, poster[i].child.type_child,
                    poster[i].child.composer, poster[i].child.country,poster[i].child.duration);

            num ++;
        }
    }

    if (0 == num)
        printf("Спектаклей по указанным параметрам нет\n\n");

    return SUCCESS;
}


int main()
{
    struct key_perfomance key[LEN_TABLE];
    struct perfomance poster[LEN_TABLE];
    int param;
    char buf[255];
    int flag = 1;
    int size = 0;

    while(flag)
    {
        functional();
        if (1 != scanf("%d", &param))
        {
            scanf("%s", buf);
            param = 0;
        }
        switch (param)
        {
            case 1: download_file(poster, key, &size); break;
            case 2: save_to_file(poster, &size); break;
            case 3: add_record(poster, key, &size); break;
            case 4: delete_record(poster, key, &size); break;
            case 5: sort_table(poster, &size); break;
            case 6: print_table(poster, &size); break;
            case 7: sort_key_table(key, &size); break;
            case 8: print_key_table(key, &size); break;
            case 9: print_table_by_key(poster, key, &size); break;
            case 10: search(poster, &size); break;
            case 11: flag = 0; break;
            default: printf("Некорректная операция\n\n"); break;
        }
    }

    return SUCCESS;
}
