#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN_CELL 10

typedef struct
{
    int rows;
    int cols;
    char ***data;

} matrix_t;

matrix_t mat_init()
{
    matrix_t tab;
    tab.cols = 0;
    tab.rows = 0;
    tab.data = NULL;
    return tab;
}

void mat_clear(matrix_t *tab, int r, int c)
{
    tab->cols = 0;
    tab->rows = 0;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            free(tab->data[i][j]);
        }
        free(tab->data[i]);
        printf("Dealokace %d\n", i);
    }

    free(tab->data);
    printf("Dealokace tabulky\n");
}
void mat_alloc(matrix_t *tab)
{
    tab->data = malloc(tab->rows * sizeof(char *));
    if (tab->data == NULL)
    {
        printf("Malloc fail!");
        return;
    }
    for (int i = 0; i < tab->rows; i++)
    {
        tab->data[i] = malloc(tab->cols * sizeof(char *));
        if (tab->data[i] == NULL)
        {
            printf("Malloc fail!");
            mat_clear(tab, i, 0);
            return;
        }
        for (int j = 0; j < tab->cols; j++)
        {
            tab->data[i][j] = calloc(MIN_CELL, sizeof(char));
            if (tab->data[i][j] == NULL)
            {
                printf("Calloc fail!");
                mat_clear(tab, i, j);
                return;
            }
        }
    }
}
int mat_reall(matrix_t *tab, int size, int row, int col, int type)
{
    if (type == 1)
    {
        char ***p = realloc(tab->data, sizeof(char) * size);
        if (p == NULL)
        {
            printf("Realloc fail!");
            free(p);
            mat_clear(tab, tab->rows, tab->cols);
            return 0;
        }
        tab->data = p;
        return 1;
    }
    else if (type == 2)
    {
       char **p = realloc(tab->data[row], sizeof(char) * size);
        if (p == NULL)
        {
            printf("Realloc fail!");
            free(p);
            mat_clear(tab, tab->rows, tab->cols);
            return 0;
        }
        tab->data[row] = p;
        return 1;  
    }
    else
    {
        char *p = realloc(tab->data[row][col], sizeof(char) * size);
        if (p == NULL)
        {
            printf("Realloc fail!");
            free(p);
            mat_clear(tab, tab->rows, tab->cols);
            return 0;
        }
        tab->data[row][col] = p;
        return 1;
    }
}

int open_file(FILE **f, const char name[], char *meth)
{
    *f = fopen(name, meth);
    if (f == NULL)
    {
        printf("Not opened!");
        return 0;
    }
    else
    {
        printf("Soubor otevren!\n");
        return 1;
    }
}
void count_rows_cols(FILE *f, matrix_t *tab, const char del[])
{
    char c;
    int cols = 0;
    while (1)
    {
        c = fgetc(f);
        if (feof(f))
        {
            tab->cols += 1;
            break;
        }
        if (c == '\n')
        {
            tab->rows += 1;
            tab->cols = cols;
            cols = 0;
        }
        if (strchr(del, c) != NULL)
        {
            cols += 1;
        }
    }
}
void read_to_mat(FILE *f, matrix_t *tab, const char del[])
{
    count_rows_cols(f, tab, del);

    printf("Pocet sloupcu: %d\n", tab->cols);
    printf("Pocet radku: %d\n", tab->rows);

    mat_alloc(tab);

    printf("Alokace se provedla\n");

    fseek(f, 0, SEEK_SET);

    int row = 0;
    int col = 0;
    int size = 1;
    int is_delim = 0;
    char c;

    while (1)
    {
        c = fgetc(f);
        if (c == '\n')
        {
            tab->data[row][col][size - 1] = '\0';
            row++;
            col = 0;
            size = 1;
            is_delim = 1;
            if (row == tab->rows)
            {
                break;
            }
            if (!mat_reall(tab, MIN_CELL, row, col, 0))
            {
                return;
            }
        }
        for (unsigned i = 0; i < strlen(del); i++)
        {
            if (del[i] == c)
            {
                tab->data[row][col][size - 1] = '\0';
                col++;
                size = 1;
                is_delim = 1;
                if (!mat_reall(tab, MIN_CELL, row, col, 0))
                {
                    return;
                }
                break;
            }
            else
            {
                is_delim = 0;
            }
        }
        if (feof(f))
        {
            break;
        }

        if (!is_delim)
        {
            if (c != '\n')
            {

                tab->data[row][col][size - 1] = c;
                size++;
                if (size > MIN_CELL)
                {
                    if (!mat_reall(tab, size, row, col, 0))
                    {
                        return;
                    }
                }
            }
        }
    }
}
/*void mat_add_row(matrix_t *tab, int r)
{
    int real = 0;
    int i;
    real = mat_reall(tab, tab->rows + 1, 0, 0, 1);
    if (real)
    {
        tab->rows += 1;
        for (i = tab->rows - 1; i >= r; i--)
        {
            tab->data[i] = tab->data[i - 1]; //FIXME
        }
        tab->data[i] = malloc(tab->cols * sizeof(char *));
        if (tab->data[i] == NULL)
        {
            printf("Malloc fail!");
            mat_clear(tab, i, 0);
            return;
        }
        for (int j = 0; j < tab->cols; j++)
        {
            tab->data[i][j] = calloc(MIN_CELL, sizeof(char));
            if (tab->data[i][j] == NULL)
            {
                printf("Calloc fail!");
                mat_clear(tab, i, j);
                return;
            }
        }
    }
}
void mat_add_col()
{
}*/
void close_file(FILE *f)
{
    printf("Soubor zavren!\n");
    fclose(f);
}
void mat_print(matrix_t *t)
{
    for (int i = 0; i < t->rows; i++)
    {
        for (int j = 0; j < t->cols; j++)
        {
            printf("%s ", t->data[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char const **argv)
{
    int opened = 0;
    if (argc > 2)
    {
        FILE *f;
        matrix_t tab;
        tab = mat_init();
        if (open_file(&f, argv[argc - 1], "r"))
        {
            if ((strcmp(argv[1], "-d")) && (argc == 3))
            {
                const char *del = " ";
                read_to_mat(f, &tab, del);
            }
            if ((!strcmp(argv[1], "-d")) && (argc == 5))
            {
                const char *del = argv[2];
                read_to_mat(f, &tab, del);
            }
            mat_print(&tab);
            // mat_add_row(&tab, 6);
            mat_print(&tab);
            mat_clear(&tab, tab.rows, tab.cols);
            close_file(f);
        }
        else
        {
            return 1;
        }
    }
    if (opened)
    {
        /* code */
    }

    return 0;
}
