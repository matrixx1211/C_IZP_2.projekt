/*                            IZP projekt č.2                                */
/*                            Marek Bitomský                                 */
/*                                 2020                                      */
/*                                                                           */
/*      Cílem projektu je vytvořit program, který bude implementovat         */
/*                   základní operace tabulkových procesorů.                 */
/*                 Program umí pracovat s daty v celé tabulce.               */

//Hlavičkové soubory
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 5
#define MINARGS 3
#define DELIMPOS 1
#define MINCELL 1

//Definice struktur
typedef struct
{
    int rowCount;
    int colCount;
    int cellCount;
    char ***cell;
} array;

typedef struct
{
    int cmdCount;
    char **cell;
} cmdArray;

typedef struct
{
    int selRow1, selCol1, selRow2, selCol2;
    char *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8, *_9;
} selection;

/* Konstruktor pro výběr buněk a dočasných proměnných */
void selectionConstruct(selection *selected)
{
    selected->selRow1 = 0;
    selected->selCol1 = 0;
    selected->selRow2 = 0;
    selected->selCol2 = 0;
    selected->_0 = NULL;
    selected->_1 = NULL;
    selected->_2 = NULL;
    selected->_3 = NULL;
    selected->_4 = NULL;
    selected->_5 = NULL;
    selected->_6 = NULL;
    selected->_7 = NULL;
    selected->_8 = NULL;
    selected->_9 = NULL;
}

/* Konstruktor pro pole příkazů */
void cmdConstruct(cmdArray *cmds)
{
    cmds->cell = NULL;
    cmds->cmdCount = 0;
    return;
}

/* Destruktor pro pole příkazů - uvolnění paměti */
void cmdDestruct(cmdArray *cmds)
{
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        if (cmds->cell[i] != NULL)
            free(cmds->cell[i]);
    }
    if (cmds->cell != NULL)
        free(cmds->cell);
    return;
}

/* Konstruktor pro tabulku */
void tableConstruct(array *table)
{
    table->cell = NULL;
    table->rowCount = 0;
    table->colCount = 0;
    table->cellCount = 0;
    return;
}

/* Destruktor pro tabulku */
void tableDestruct(array *table)
{
    //pro všechny řádky
    for (int i = 0; i < table->rowCount; i++)
    {
        //pro všechny sloupce
        for (int j = 0; j < table->colCount; j++)
        {
            //uvolní paměť buněk
            if (table->cell[i][j] != NULL)
                free(table->cell[i][j]);
        }
        //uvolní pamět sloupců
        if (table->cell[i] != NULL)
            free(table->cell[i]);
    }
    //uvolní pamět sloupců
    if (table->cell != NULL)
        free(table->cell);
    return;
}

/* Alokace pole příkazů */
void initArray(cmdArray *cmds)
{
    //alokace řádků
    cmds->cell = malloc(sizeof(char *) * cmds->cmdCount);
    if (cmds->cell == NULL)
    {
        fprintf(stderr, "Chyba pri alokaci pameti.");
        cmdDestruct(cmds);
        return;
    }

    //alokace buněk na velikost MINCELL
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        cmds->cell[i] = calloc(1, sizeof(char) * MINCELL);
        if (cmds->cell[i] == NULL)
        {
            fprintf(stderr, "Chyba pri alokaci pameti.");
            cmdDestruct(cmds);
            return;
        }
    }
    return;
}

/* Zvětšení buňky pro příkaz */
void resizeArray(cmdArray *cmds, int row, int newSize)
{
    char *cellPointer;
    //zvětší buňku na novou velikost
    cellPointer = realloc(cmds->cell[row], sizeof(char) * newSize);
    if (cellPointer == NULL)
    {
        fprintf(stderr, "Chyba pri realokaci pameti.");
        free(cellPointer);
        return;
    }
    cmds->cell[row] = cellPointer;
    return;
}

/* Zjistí počet příkazů */
void countCommands(cmdArray *cmds, const char *argValue[], int seqPos)
{
    //zjistí kolik je zadáno příkazů
    for (unsigned int i = 0; i <= strlen(argValue[seqPos]); i++)
        if (argValue[seqPos][i] == ';' || argValue[seqPos][i] == '\0')
            cmds->cmdCount++;
    return;
}

/* Rozdělí příkazy do buněk v poli */
void commandsToArray(cmdArray *cmds, int argCount, const char *argValue[])
{
    //Podle počtu argumentů nastaví pozici CMD_Sequence
    int seqPos = argCount - 2;
    cmdConstruct(cmds);
    countCommands(cmds, argValue, seqPos);
    initArray(cmds);

    int j = 0, character = 0;
    //pro všechny příkazy
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        //zatímco existuje ještě nějaký příkaz respektive znak
        while (argValue[seqPos][j] != '\0')
        {
            //pokud to není oddělovač příkazu
            if (argValue[seqPos][j] != ';')
            {
                //do buňky vloží znak
                cmds->cell[i][character] = argValue[seqPos][j];
                character++;
                j++;
                if (j >= MINCELL)
                    resizeArray(cmds, i, character + 1);
            }
            //pokud je to oddělovač
            else
            {
                //nastaví na poslední znak v buňce terminátor
                cmds->cell[i][character] = '\0';
                j++;
                i++;
                character = 0;
            }
        }
        //pokud už je na posledním znaku, tak nastaví do poslední buňky terminátor
        if (argValue[seqPos][j] == '\0')
            cmds->cell[i][character] = '\0';
    }
}

/* Výpis příkazů */
void printCommands(cmdArray *cmds)
{
    //vypíše příkaz po příkazu
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        printf("%d. %s\n", i + 1, cmds->cell[i]);
    }
}

/* Změní velikost řádku o zadaný počet */
void resizeRowBy(array *table, int by)
{
    char ***rowPointer;
    char **colPointer;
    char *cellPointer;
    table->rowCount += by;
    //pokud bylo zadáno záporné číslo
    if (by < 0)
        for (int i = 0; i < table->rowCount + 1; i++)
            if (i + 1 > table->rowCount)
            {
                //uvlonit poslední řádek
                for (int j = 0; j < table->colCount; j++)
                    free(table->cell[table->rowCount][j]);
                free(table->cell[table->rowCount]);
            }

    //realokace ukazatelu řádků na novou velikost
    rowPointer = realloc(table->cell, sizeof(char *) * table->rowCount);
    if (rowPointer == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    else
        table->cell = rowPointer;

    //alokace ukazatelů sloupců na nových řádcích
    for (int i = (table->rowCount - by); i < (table->rowCount); i++)
    {
        colPointer = malloc(sizeof(char *) * table->colCount);
        if (colPointer == NULL)
        {
            fprintf(stderr, "Alokace se nepovedla.");
            tableDestruct(table);
            return;
        }
        else
            table->cell[i] = colPointer;

        //alokace buněk na velikost MINCELL
        for (int j = 0; j < table->colCount; j++)
        {
            //calloc vyčistí alokovaný prostor
            cellPointer = calloc(1, MINCELL);
            if (cellPointer == NULL)
            {
                fprintf(stderr, "Realokace se nepovedla.");
                tableDestruct(table);
                return;
            }
            else
                table->cell[i][j] = cellPointer;
        }
    }
}

/* Změní velikost sloupce o zadaný počet */
void resizeColBy(array *table, int by)
{
    char **colPointer;
    char *cellPointer;
    table->colCount += by;

    //pro všechny řádky
    for (int i = 0; i < table->rowCount; i++)
    {
        //pokud je by zadáno jako záporné číslo
        if ((by < 0) && (table->rowCount >= i))
        {
            free(table->cell[i][table->colCount]);
        }
        //realokace sloupců na novou velikost
        colPointer = realloc(table->cell[i], table->colCount * sizeof(char *));
        if (colPointer == NULL)
        {
            fprintf(stderr, "Alokace se nepovedla.");
            tableDestruct(table);
            return;
        }
        else
            table->cell[i] = colPointer;

        //alokace bunek od první neinicializované až po poslední
        for (int j = table->colCount - by; j < table->colCount; j++)
        {
            //calloc vyčistí alokovany prostor
            cellPointer = calloc(1, MINCELL);
            if (cellPointer == NULL)
            {
                fprintf(stderr, "Realokace se nepovedla.");
                tableDestruct(table);
                return;
            }
            else
                table->cell[i][j] = cellPointer;
        }
    }
}

/* Změní velikost buňky o zadaný počet */
int resizeCellBy(array *table, int by, int row, int col)
{
    char *cellPointer;
    //spočítá novou velikost buňky pomocí délky buňky + o kolik se má zvětšít (by) + terminátor (+1)
    int newSize = strlen(table->cell[row][col]) + 1 + by;
    //realokuje velikost paměti na novou velikost
    cellPointer = realloc(table->cell[row][col], sizeof(char) * newSize);
    if (cellPointer == NULL)
    {
        fprintf(stderr, "Chyba pri realokaci.");
        tableDestruct(table);
        return 0;
    }
    table->cell[row][col] = cellPointer;
    return 1;
}

/* Změní velikost buňky na zadaný počet */
int resizeCellOn(array *table, int newSize, int row, int col)
{
    char *cellPointer;
    //realokuje buňku na novou velikost
    cellPointer = realloc(table->cell[row][col], sizeof(char) * newSize);
    if (cellPointer == NULL)
    {
        fprintf(stderr, "Chyba pri realokaci.");
        tableDestruct(table);
        return 0;
    }
    table->cell[row][col] = cellPointer;
    return 1;
}

/* Nastavení výběru buněk */
void selectionSet(cmdArray *cmds, selection *selected, int i)
{
    char number[1000];
    int dotCount = 0, numberCount = 0;
    //pro všechny znaky v poli
    for (unsigned int j = 1; j < strlen(cmds->cell[i]); j++)
    {
        //pokud se rovnají , nebo ]
        if (cmds->cell[i][j] == ',' || cmds->cell[i][j] == ']')
        {
            //podle počtu teček vlož do dané proměnné
            number[numberCount] = '\0';
            numberCount = 0;
            dotCount++;
            switch (dotCount - 1)
            {
            case 0:
                selected->selRow1 = atoi(number);
                break;
            case 1:
                selected->selCol1 = atoi(number);
                break;
            case 2:
                selected->selRow2 = atoi(number);
                break;
            case 3:
                selected->selCol2 = atoi(number);
                break;
            }
        }
        //pokud se rovnají _, tak nastav vyběr na 0
        else if (cmds->cell[i][j] == '_')
        {
            number[numberCount] = 0;
            numberCount++;
        }
        //pokud je znak číslo tak ho vlož do pole
        else
        {
            number[numberCount] = cmds->cell[i][j];
            numberCount++;
        }
    }
    return;
}

/* Zkontroluje  */
void checkSelection(array *table, selection *selected)
{
    //zvětšení pole ukazatelů na řádek
    if (selected->selRow1 > table->rowCount)
        resizeRowBy(table, selected->selRow1 - table->rowCount);
    if (selected->selRow2 > table->rowCount)
        resizeRowBy(table, selected->selRow2 - table->rowCount);

    //zvětšení pole ukazatelů na sloupec
    if (selected->selCol1 > table->colCount)
        resizeColBy(table, selected->selCol1 - table->colCount);
    if (selected->selCol2 > table->colCount)
        resizeColBy(table, selected->selCol2 - table->colCount);
}

/* Vloží řádek nad vybrané buňky */
void insertRow(array *table, selection *selected)
{
    //pokud je zadaný řádek větší jak 0
    if (selected->selRow1 > 0)
    {
        char **tmp;
        //zvěč pole ukazatelů na řádek
        resizeRowBy(table, 1);
        //posuň řádky na zadané místo
        for (int i = table->rowCount - 1; i >= selected->selRow1; i--)
        {
            tmp = table->cell[i];
            table->cell[i] = table->cell[i - 1];
            table->cell[i - 1] = tmp;
        }
    }
}

/* Vloží řádek pod vybrané buňky */
void appendRow(array *table, selection *selected)
{
    char **tmp;
    //zvěč pole ukazatelů na řádek
    resizeRowBy(table, 1);
    //posuň řádky na zadané místo
    for (int i = table->rowCount - 1; i > selected->selRow1; i--)
    {
        tmp = table->cell[i];
        table->cell[i] = table->cell[i - 1];
        table->cell[i - 1] = tmp;
    }
}

/* Odstraní vybraný řádek */
void deleteRow(array *table, selection *selected)
{
    char **tmp;
    //ten co má být smazán se posune na konec
    for (int i = selected->selRow1 - 1; i < table->rowCount - 1; i++)
    {
        tmp = table->cell[i];
        table->cell[i] = table->cell[i + 1];
        table->cell[i + 1] = tmp;
    }
    //zmenší pole ukazatelů na řádek
    resizeRowBy(table, -1);
}

/* Vloží sloupec nalevo */
void insertCol(array *table, selection *selected)
{
    char *tmp;
    //zvěč pole ukazatelů na sloupec
    resizeColBy(table, 1);
    //pro všechny řádky
    for (int i = 0; i < table->rowCount; i++)
    {
        //posuň sloupce na zadané místo
        for (int j = table->colCount - 1; j >= selected->selCol1; j--)
        {
            tmp = table->cell[i][j];
            table->cell[i][j] = table->cell[i][j - 1];
            table->cell[i][j - 1] = tmp;
        }
    }
}

/* Vloží sloupec napravo */
void appendCol(array *table, selection *selected)
{
    char *tmp;
    //zvěč pole ukazatelů na sloupec
    resizeColBy(table, 1);
    //pro všechny řádky
    for (int i = 0; i < table->rowCount; i++)
    {
        //posuň sloupce na zadané místo
        for (int j = table->colCount - 1; j > selected->selCol1; j--)
        {
            tmp = table->cell[i][j];
            table->cell[i][j] = table->cell[i][j - 1];
            table->cell[i][j - 1] = tmp;
        }
    }
}

/* Odstraní vybraný sloupec */
void deleteCol(array *table, selection *selected)
{
    char *tmp;
    //pro všechny řádky
    for (int i = 0; i < table->rowCount; i++)
    {
        //posuň zadaný sloupec na konec
        for (int j = selected->selCol1 - 1; j < table->colCount - 1; j++)
        {
            tmp = table->cell[i][j];
            table->cell[i][j] = table->cell[i][j + 1];
            table->cell[i][j + 1] = tmp;
        }
    }
    //zmenší pole ukazatelů na sloupec
    resizeColBy(table, -1);
}

/* V příkazech pro úpravu buněk najde indexy buněk */
void editCmdsSel(cmdArray *cmds, int i, int *row, int *col, int begin)
{
    char number[1000];
    int dotCount = 0, numberCount = 0;
    //pro všechny znaky v poli
    for (unsigned int j = begin; j < strlen(cmds->cell[i]); j++)
    {
        //pokud se rovnají , nebo ]
        if (cmds->cell[i][j] == ',' || cmds->cell[i][j] == ']')
        {
            //podle počtu teček vlož do dané proměnné
            number[numberCount] = '\0';
            numberCount = 0;
            dotCount++;
            switch (dotCount - 1)
            {
            case 0:
                *row = atoi(number);
                break;

            case 1:
                *col = atoi(number);
                break;
            }
        }
        //pokud se rovnají _, tak nastav vyběr na 0
        else if (cmds->cell[i][j] == '_')
        {
            number[numberCount] = 0;
            numberCount++;
        }
        //pokud je znak číslo tak ho vlož do pole
        else
        {
            number[numberCount] = cmds->cell[i][j];
            numberCount++;
        }
    }
}

/* Odstraní obsah buňky */
void clearCell(array *table, selection *selected)
{
    char *cellPointer;
    //realokuje velikost buňky na 1 znak
    cellPointer = realloc(table->cell[selected->selRow1][selected->selCol1], sizeof(char) * MINCELL);
    if (cellPointer == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    table->cell[selected->selRow1][selected->selCol1] = cellPointer;
    //na první znak vloží terminátor
    table->cell[selected->selRow1][selected->selCol1][0] = '\0';
}

/* Prohodí dvě buňky */
void swapCells(array *table, selection *selected, cmdArray *cmds, int i)
{
    int row, col;
    char *tmp;
    //vyhledá row a col
    editCmdsSel(cmds, i, &row, &col, 6);
    //vymění buňky
    tmp = table->cell[selected->selRow1 - 1][selected->selCol1 - 1];
    table->cell[selected->selRow1 - 1][selected->selCol1 - 1] = table->cell[row - 1][col - 1];
    table->cell[row - 1][col - 1] = tmp;
}

/* Uloží součet hodnot z rozsahu do konkrétní buňky */
void sumCells(array *table, selection *selected, cmdArray *cmds, int i)
{
    int row, col;
    double sum = 0;
    char *tmp;
    //vyhledá row a col
    editCmdsSel(cmds, i, &row, &col, 5);
    //pro vybrané řádky a sloupce
    for (int i = selected->selRow1 - 1; i <= selected->selRow2 - 1; i++)
    {
        for (int j = selected->selCol1 - 1; j <= selected->selCol2 - 1; j++)
        {
            sum += atof(table->cell[i][j]);
        }
    }
    //realokuju na velikost integeru
    tmp = realloc(table->cell[row - 1][col - 1], sizeof(int));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    //uloží hodnotu do buňky
    table->cell[row - 1][col - 1] = tmp;
    sprintf(table->cell[row - 1][col - 1], "%g", sum);
}

/* Nastaví zadaný řetezec na vybrané místo */
void setString(array *table, selection *selected, cmdArray *cmds, int i)
{
    char string[1000];
    int strLen = 0;
    char *tmp;
    //uložím zadaný řetezec do string
    for (unsigned int j = 4; j <= strlen(cmds->cell[i]) - 1; j++)
    {
        string[strLen] = cmds->cell[i][j];
        strLen++;
    }
    //vloží terminátor
    string[strLen] = '\0';
    //realokace na novou velikost
    tmp = realloc(table->cell[selected->selRow1 - 1][selected->selCol1 - 1], sizeof(char) * (strLen + 1));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    //vloží řetezec do zadané buňky
    table->cell[selected->selRow1 - 1][selected->selCol1 - 1] = tmp;
    sprintf(table->cell[selected->selRow1 - 1][selected->selCol1 - 1], "%s", string);
}

/* Uloží průměr do zadané buňky */
void avgCells(array *table, selection *selected, cmdArray *cmds, int i)
{
    int row, col, count = 0;
    double sum = 0, atofValue;
    char *tmp;
    //vyhledá row a col
    editCmdsSel(cmds, i, &row, &col, 5);
    //pro vybrané řádky a sloupce
    for (int i = selected->selRow1 - 1; i <= selected->selRow2 - 1; i++)
    {
        for (int j = selected->selCol1 - 1; j <= selected->selCol2 - 1; j++)
        {
            atofValue = atof(table->cell[i][j]);
            if (atofValue != 0.0)
            {
                sum += atofValue;
                count++;
            }
        }
    }
    //realokuju na velikost integeru
    tmp = realloc(table->cell[row - 1][col - 1], sizeof(double) + sizeof(char));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    //uloží hodnotu do buňky
    table->cell[row - 1][col - 1] = tmp;
    sprintf(table->cell[row - 1][col - 1], "%f", sum / count);
}

/* Uloží počet neprázdných buněk do zadané buňky */
void countCells(array *table, selection *selected, cmdArray *cmds, int i)
{
    int row, col, count = 0;
    char *tmp;
    //vyhledá row a col
    editCmdsSel(cmds, i, &row, &col, 7);
    //pro vybrané řádky a sloupce
    for (int i = selected->selRow1 - 1; i <= selected->selRow2 - 1; i++)
    {
        for (int j = selected->selCol1 - 1; j <= selected->selCol2 - 1; j++)
        {
            if (table->cell[i][j][0] != '\0')
                count++;
        }
    }
    //realokuju na velikost integeru
    tmp = realloc(table->cell[row - 1][col - 1], sizeof(int));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    //uloží hodnotu do buňky
    table->cell[row - 1][col - 1] = tmp;
    sprintf(table->cell[row - 1][col - 1], "%d", count);
}

/* Uloží délku vybrané buňky do zadané buňky */
void lenCells(array *table, selection *selected, cmdArray *cmds, int i)
{
    int row, col, length = 0;
    char *tmp;
    //vyhledá row a col
    editCmdsSel(cmds, i, &row, &col, 5);
    length = strlen(table->cell[selected->selRow1 - 1][selected->selCol1 - 1]);
    //realokace paměti na novou velikost
    tmp = realloc(table->cell[row - 1][col - 1], sizeof(double) + sizeof(char));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    //uloží hodnotu do buňky
    table->cell[row - 1][col - 1] = tmp;
    sprintf(table->cell[row - 1][col - 1], "%d", length);
}

/* Rozšifrování CMD_sequence */
void processCommands(cmdArray *cmds, selection *selected, array *table, int argCount, const char *argValue[])
{
    //rozdělí přikazy do pole
    commandsToArray(cmds, argCount, argValue);
    //konstrukce výběru buněk
    selectionConstruct(selected);
    //pro všechny příkazy
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        //pokud je první znak [ a poslední znak ]
        if ((cmds->cell[i][0] == '[') && (cmds->cell[i][strlen(cmds->cell[i]) - 1] == ']') && (strstr(cmds->cell[i], "[set]") == NULL))
        {
            //nastavení výběru
            selectionSet(cmds, selected, i);
            //zvětšení pole na základě výběru
            checkSelection(table, selected);
            //TODO: přidat ještě [min], [max] a [find STR], pokud stihnu
        }
        else
        {
            //uprava struktury
            if (!strcmp(cmds->cell[i], "irow"))
                insertRow(table, selected);
            if (!strcmp(cmds->cell[i], "arow"))
                appendRow(table, selected);
            if (!strcmp(cmds->cell[i], "drow"))
                deleteRow(table, selected);
            if (!strcmp(cmds->cell[i], "icol"))
                insertCol(table, selected);
            if (!strcmp(cmds->cell[i], "acol"))
                appendCol(table, selected);
            if (!strcmp(cmds->cell[i], "dcol"))
                deleteCol(table, selected);

            //uprava obsahu bunek
            if ((strstr(cmds->cell[i], "set") != NULL) && (strstr(cmds->cell[i], "[set]") == NULL))
                setString(table, selected, cmds, i);
            if (strstr(cmds->cell[i], "clear") != NULL)
                clearCell(table, selected);
            if (strstr(cmds->cell[i], "swap") != NULL)
                swapCells(table, selected, cmds, i);
            if (strstr(cmds->cell[i], "sum") != NULL)
                sumCells(table, selected, cmds, i);
            if (strstr(cmds->cell[i], "avg") != NULL)
                avgCells(table, selected, cmds, i);
            if (strstr(cmds->cell[i], "count") != NULL)
                countCells(table, selected, cmds, i);
            if (strstr(cmds->cell[i], "len") != NULL)
                lenCells(table, selected, cmds, i);

            //příkazy pro práci s dočasnými proměnnými
            //if(strstr(cmds->cell[i], "def") != NULL) def();
            //if(strstr(cmds->cell[i], "use") != NULL) use();
            //if(strstr(cmds->cell[i], "inc") != NULL) inc();
            //TODO: vypadá to, že nestíhám
            //if(strstr(cmds->cell[i], "[set]") != NULL) tmpSet();
        }
    }
}

/* Otevře soubor podle argumentu FILE */
int openFile(int lastArg, char const *fileName[], FILE **inputFile, char mode[2])
{
    *inputFile = fopen(fileName[lastArg - 1], mode);
    //pokud se nepovede otevřít, tak vypíše chybu
    if (*inputFile == NULL)
    {
        fprintf(stderr, "Chyba pri otevirani souboru.\n");
        return 0;
    }
    else
        return 1;
}

/* Uzavře soubor */
void closeFile(FILE *inputFile)
{
    int closed;
    closed = fclose(inputFile);
    if (closed != 0)
    {
        fprintf(stderr, "Chyba při zavření souboru.");
    }
    return;
}

/* Kontroluje, zda argumenty byli zadány správně */
int checkArgs(int argCount, const char *argValue[])
{
    //Pokud ukazatel na .txt není NULL a zároveň byl zadán DELIM, tak musí být počet argumentů 5 nebo počet argumentů musí být 3
    if ((strstr(argValue[argCount - 1], ".txt") != NULL) && ((!strcmp("-d", argValue[DELIMPOS]) && argCount == MAXARGS) || argCount == MINARGS))
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "Program spusten spatne.\n"
                        "Je potreba zadat [-d DELIM] CMD_SEQUENCE FILE.\n"
                        "------------------------------------------------------------\n"
                        "Parametr [-d DELIM] je volitelny.\n"
                        "------------------------------------------------------------\n"
                        "Parametr CMD_SEQUENCE je povinny.\n"
                        "------------------------------------------------------------\n"
                        "Parametr FILE je take povinny, zadejte ve formatu nazev.txt.\n"
                        "------------------------------------------------------------\n");
        return 0;
    }
}

/* Nastavení delimu */
char const *findDelim(int argCount, const char *argValue[])
{
    //pokud bylo zadáno -d nastav hodnotu
    if (!strcmp(argValue[DELIMPOS], "-d") && (argCount == MAXARGS))
    {
        char const *delim = argValue[DELIMPOS + 1];
        return delim;
    }
    //pokud zadáno nebylo nastav na mezeru
    else
    {
        char *delim = " ";
        return delim;
    }
}

/* Spočítá počet řádků a počet sloupců */
void countRowsAndCols(array *table, FILE *inputFile, const char *delim)
{
    char character;
    //pokud je na vstupu nějaký znak
    while ((character = fgetc(inputFile)) != EOF)
    {
        //pokud je znak konec řádků
        if (character == '\n')
        {
            table->rowCount++;
            table->colCount = 0;
        }

        //pokud je znak z delimu
        if (strchr(delim, character) != NULL)
        {
            //počet nalezených znaků delim
            table->cellCount++;
            table->colCount++;
        }
    }
    //počet delimů + počet řádků = počet buněk
    table->cellCount += table->rowCount;
    table->colCount = table->cellCount / table->rowCount;
    return;
}

/* Inicializace tabulky */
void initTable(array *table, FILE *inputFile)
{
    fseek(inputFile, 0, SEEK_SET);
    //alokace ukazatelu na řádek
    table->cell = malloc(sizeof(char *) * table->rowCount);
    if (table->cell == NULL)
    {
        fprintf(stderr, "Chyba pri alokaci mista v pameti.");
        tableDestruct(table);
        return;
    }
    for (int i = 0; i < table->rowCount; i++)
    {
        //alokace ukazatelů na sloupcce
        table->cell[i] = malloc(sizeof(char *) * table->colCount);
        if (table->cell == NULL)
        {
            fprintf(stderr, "Chyba pri alokaci mista v pameti.");
            tableDestruct(table);
            return;
        }
        for (int j = 0; j < table->colCount; j++)
        {
            //alokace buněk na velikost MINCELL a vyčištení
            table->cell[i][j] = calloc(1, MINCELL);
            if (table->cell == NULL)
            {
                fprintf(stderr, "Chyba pri alokaci mista v pameti.");
                tableDestruct(table);
                return;
            }
        }
    }
    return;
}

/* Rozdělí data ze souboru do 2D pole */
int fileToTable(array *table, FILE *inputFile, const char *delim)
{
    //konstukce tabulky
    tableConstruct(table);
    //spočítá počet řádků a sloupců
    countRowsAndCols(table, inputFile, delim);
    //vytvoří tabulku
    initTable(table, inputFile);
    //posunutí ukazatele na začátek souboru
    fseek(inputFile, 0, SEEK_SET);
    char character;
    int currentRow = 0, currentCol = 0;
    unsigned int charPos = 0;
    //zatímco je na vstupu nějaký znak
    while ((character = fgetc(inputFile)) != EOF)
    {
        //pokud je to konec řádku
        if (character == '\n')
        {
            //vložení terminátoru na konec
            table->cell[currentRow][currentCol][charPos] = '\0';
            charPos = 0;
            currentCol = 0;
            currentRow++;
        }
        //pokud to je znak z delimu
        if (strchr(delim, character) != NULL)
        {
            //vložení terminátoru na konec
            table->cell[currentRow][currentCol][charPos] = '\0';
            charPos = 0;
            currentCol++;
        }
        //pokud je to nějaký znak, ale ne delim
        else
        {
            //pokud není na konci řádku
            if (character != '\n')
            {
                table->cell[currentRow][currentCol][charPos] = character;
                charPos++;
                if (charPos >= MINCELL)
                    if (!resizeCellOn(table, charPos + 1, currentRow, currentCol))
                        return 0;
            }
        }
    }
    return 1;
}

/* Kontrola prázdných řádků a sloupců */
void checkEmpty(array *table, int *x, int *y)
{
    for (int i = 0; i < table->rowCount; i++)
    {
        for (int j = 0; j < table->colCount; j++)
        {
            //pokud buňka není prázdná, tak se uloží do proměnných notEmptyXxx aktuální i nebo j
            if (strlen(table->cell[i][j]) != 0)
            {
                if (*x < i)
                    *x = i;
                if (*y < j)
                    *y = j;
            }
        }
    }
}

/* Vypsání tabulky */
void printTable(array *table, char delim)
{
    //kontrola prázdných sloupců respektive řádků
    int notEmptyRow = 0, notEmptyCol = 0;
    checkEmpty(table, &notEmptyRow, &notEmptyCol);

    //pro všechny řádky
    for (int i = 0; i <= notEmptyRow; i++)
    {
        //pro všechny sloupce
        for (int j = 0; j <= notEmptyCol; j++)
        {
            //pokud je to poslední sloupec
            for (unsigned int c = 0; c < strlen(table->cell[i][j]); c++)
            {
                //pokud je nějaký znak z buňky " tak vypíšu " na začátku
                if (strchr(table->cell[i][j], '"') && c == 0)
                    printf("\"");
                //pokud je momentální znak " tak vypíšu \ a znak
                if (table->cell[i][j][c] == '"')
                    printf("\\%c", table->cell[i][j][c]);
                //jinak jen znak
                else
                    printf("%c", table->cell[i][j][c]);
                //pokud je nějaký znak z buňky " tak vypíšu " na konci
                if (strchr(table->cell[i][j], '"') && c == strlen(table->cell[i][j]) - 1)
                    printf("\"");
            }
            //pokud je nová buňka
            if ((notEmptyCol) != j)
                printf("%c", delim);
        }
        printf("\n");
    }
    return;
}

/* Uložení tabulky do souboru */
void tableToFile(array *table, char delim, FILE *inputFile)
{
    //kontrola prázdných sloupců respektive řádků
    int notEmptyRow = 0, notEmptyCol = 0;
    checkEmpty(table, &notEmptyRow, &notEmptyCol);

    //pro všechny řádky
    for (int i = 0; i <= notEmptyRow; i++)
    {
        //pro všechny sloupce
        for (int j = 0; j <= notEmptyCol; j++)
        {
            //pokud je to poslední sloupec
            for (unsigned int c = 0; c < strlen(table->cell[i][j]); c++)
            {
                //pokud je nějaký znak z buňky " tak vypíšu " na začátku
                if (strchr(table->cell[i][j], '"') && c == 0)
                    fprintf(inputFile, "\"");
                //pokud je momentální znak " tak vypíšu \ a znak
                if (table->cell[i][j][c] == '"')
                    fprintf(inputFile, "\\%c", table->cell[i][j][c]);
                //jinak jen znak
                else
                    fprintf(inputFile, "%c", table->cell[i][j][c]);
                //pokud je nějaký znak z buňky " tak vypíšu " na konci
                if (strchr(table->cell[i][j], '"') && c == strlen(table->cell[i][j]) - 1)
                    fprintf(inputFile, "\"");
            }
            //pokud je nová buňka
            if ((notEmptyCol) != j)
                fprintf(inputFile, "%c", delim);
        }
        fprintf(inputFile, "\n");
    }
    return;
}

int main(int argc, const char *argv[])
{
    //kontrola argumentů
    if (checkArgs(argc, argv))
    {
        FILE *inputFile;

        //pokud se podaří otevřít soubor
        if (openFile(argc, argv, &inputFile, "r"))
        {
            array table;
            const char *delim = findDelim(argc, argv);

            //naplní tabulku daty ze souboru
            if (fileToTable(&table, inputFile, delim))
            {
                cmdArray cmds;
                selection selected;
                //rozšifrování příkazů (CMD_SEQUENCE)
                processCommands(&cmds, &selected, &table, argc, argv);
                //vypsání dat na stdout
                printTable(&table, delim[0]);

                //zavření souboru pro čtení
                closeFile(inputFile);
                //otevření souboru pro zápis
                if (openFile(argc, argv, &inputFile, "w"))
                    //uložení do souboru
                    tableToFile(&table, delim[0], inputFile);
                else
                    fprintf(stderr, "Nebylo mozne otevrit soubor pro zapis a tudiz zapsat data do souboru.");

                //destrukce pole příkazů - uvolnění paměti
                cmdDestruct(&cmds);
                //uzavření souboru pro zápis
                closeFile(inputFile);
            }
            //desktukce tabulky - uvolnění paměti
            tableDestruct(&table);
        }
        else
            //pokud se soubor nepodaří otevřít
            return 1;
    }
    return 0;
}
