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
#include <ctype.h>

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

/* Konstruktor pro výběr */
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

/* Destrukce pole příkazů - uvolnění paměti */
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

/* Konstruktor pro 2D pole */
void tableConstruct(array *table)
{
    /* TODO: inicializace 2D pole */
    table->cell = NULL;
    table->rowCount = 0;
    table->colCount = 0;
    table->cellCount = 0;
    return;
}

/* Destruktor pro 2D pole */
void tableDestruct(array *table)
{
    for (int i = 0; i < table->rowCount; i++)
    {
        for (int j = 0; j < table->colCount; j++)
        {
            if (table->cell[i][j] != NULL)
                free(table->cell[i][j]);
        }
        if (table->cell[i] != NULL)
            free(table->cell[i]);
    }
    if (table->cell != NULL)
        free(table->cell);
    return;
}

/* Alokace pole příkazů */
void initArray(cmdArray *cmds)
{
    //alokace řádků
    //printf("%d", cmds->cmdCount);
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
    //vypočítání počtu příkazů
    for (unsigned int i = 0; i <= strlen(argValue[seqPos]); i++)
        if (argValue[seqPos][i] == ';' || argValue[seqPos][i] == '\0')
            cmds->cmdCount++;
    return;
}

/* Rozdělí příkazy do buněk v poli */
void commandsToArray(cmdArray *cmds, int argCount, const char *argValue[])
{
    //Podle argumentů nastaví pozici CMD_Sequence
    int seqPos = argCount - 2;

    cmdConstruct(cmds);

    countCommands(cmds, argValue, seqPos);
    //printf("cmdCount: %d\n", cmds->cmdCount);

    initArray(cmds);

    int j = 0, character = 0;

    for (int i = 0; i < cmds->cmdCount; i++)
    {
        while (argValue[seqPos][j] != '\0')
        {
            if (argValue[seqPos][j] != ';')
            {
                cmds->cell[i][character] = argValue[seqPos][j];
                character++;
                j++;
                if (j >= MINCELL)
                    resizeArray(cmds, i, character + 1);
            }
            else
            {
                cmds->cell[i][character] = '\0';
                j++;
                i++;
                character = 0;
            }
        }
        if (argValue[seqPos][j] == '\0')
            cmds->cell[i][character] = '\0';
    }
}

/* Výpis příkazů */
void printCommands(cmdArray *cmds)
{
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        printf("%d. %s\n", i, cmds->cell[i]);
    }
}

/* Změní velikost řádku o zadaný počet */
void resizeRowBy(array *table, int by)
{
    char ***rowPointer;
    char **colPointer;
    char *cellPointer;
    table->rowCount += by;
    //realokace ukazatelu radku
    //printf("rows: %d\n", table->rowCount);
    //printf("cols: %d\n", table->colCount);
    //TODO: rozdělit free na cell row col if (by<0)
    rowPointer = realloc(table->cell, sizeof(char *) * table->rowCount);
    if (rowPointer == NULL)
    {
        fprintf(stderr, "Realokace se nepovedla.");
        tableDestruct(table);
        return;
    }
    else
        table->cell = rowPointer;

    //alokace ukazatelu sloupcu na nových řádcích
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

        //alokace bunek
        for (int j = 0; j < table->colCount; j++)
        {
            //calloc vycisti alokovany prostor
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
    //5 (4) + 2 = 7 (6)
    //realokace sloupců
    for (int i = 0; i < (table->rowCount); i++)
    {
        colPointer = realloc(table->cell[i], table->colCount * sizeof(char *));
        if (colPointer == NULL)
        {
            fprintf(stderr, "Alokace se nepovedla.");
            tableDestruct(table);
            return;
        }
        else
            table->cell[i] = colPointer;

        //alokace bunek od poslední neinicializované až po poslední
        for (int j = table->colCount - by; j < table->colCount; j++)
        {
            //calloc vycisti alokovany prostor
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

/* FIXME: nevím jestli chci používat */
/* Změní velikost buňky o zadaný počet */
int resizeCellBy(array *table, int by, int row, int col)
{
    char *cellPointer;
    int newSize = strlen(table->cell[row][col]) + 1 + by;
    /* FIXME: otestovat s * */
    if (table->colCount == col + 1 && table->rowCount == row + 1)
        printf("%d", newSize);
    cellPointer = realloc(table->cell[row][col], sizeof(char) * (by + strlen(table->cell[row][col])));
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
    cellPointer = realloc(table->cell[row][col], sizeof(char) * newSize /* (strlen(table->cell[row][col]) + by) */);
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
    for (unsigned int j = 1; j < strlen(cmds->cell[i]); j++)
    {
        if (cmds->cell[i][j] == ',' || cmds->cell[i][j] == ']')
        {

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
        else if (cmds->cell[i][j] == '_')
        {
            number[numberCount] = 0;
            numberCount++;
        }
        else
        {
            number[numberCount] = cmds->cell[i][j];
            numberCount++;
        }
    }
}

/* Vloží řádek nad vybrané buňky */
void insertRow(array *table, selection *selected)
{
    if (selected->selRow1 > 0)
    {
        char **tmp;
        resizeRowBy(table, 1);

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
    resizeRowBy(table, 1);

    for (int i = table->rowCount - 1; i > selected->selRow1; i--)
    {
        tmp = table->cell[i];
        table->cell[i] = table->cell[i - 1];
        table->cell[i - 1] = tmp;
    }
}

/* TODO: Odstraní vybraný řádek */
/* void deleteRow(array *table, selection *selected)
{
    char **tmp;

    for (int i = table->rowCount - 1; i > selected->selRow1; i--)
    {
        tmp = table->cell[i];
        table->cell[i] = table->cell[i - 1];
        table->cell[i - 1] = tmp;
    }
    resizeRowBy(table, -1);
} */

/* Rozšifrování CMD_sequence */
void processCommands(cmdArray *cmds, selection *selected, array *table, int argCount, const char *argValue[])
{
    commandsToArray(cmds, argCount, argValue);
    selectionConstruct(selected);
    for (int i = 0; i < cmds->cmdCount; i++)
    {
        if ((cmds->cell[i][0] == '[') && (cmds->cell[i][strlen(cmds->cell[i]) - 1] == ']'))
        {
            selectionSet(cmds, selected, i);
            //TODO: přidat ještě [min], [max] a [find STR]
        }
        else
        {
            //uprava struktury
            if (strstr(cmds->cell[i], "irow") != NULL)
                insertRow(table, selected);
            if (strstr(cmds->cell[i], "arow") != NULL)
                appendRow(table, selected);
            if (strstr(cmds->cell[i], "drow") != NULL)
                deleteRow(table, selected);
            /* if (strstr(cmds->cell[i], "icol") != NULL)
                insertCol();
            if (strstr(cmds->cell[i], "acol") != NULL)
                appendCol();
            if (strstr(cmds->cell[i], "dcol") != NULL)
                deleteCol(); */

            //uprava obsahu bunek
            /* if((strstr(cmds->cell[i], "set") != NULL) && (strstr(cmds->cell[i], "[set]") == NULL)) set();
            if(strstr(cmds->cell[i], "clear") != NULL) clearCell();
            if(strstr(cmds->cell[i], "swap") != NULL) swapCells();
            if(strstr(cmds->cell[i], "sum") != NULL) sum();
            if(strstr(cmds->cell[i], "avg") != NULL) avg();
            if(strstr(cmds->cell[i], "count") != NULL) count();
            if(strstr(cmds->cell[i], "len") != NULL) len(); */

            //příkazy pro práci s dočasnými proměnnými
            /* if(strstr(cmds->cell[i], "def") != NULL) def();
            if(strstr(cmds->cell[i], "use") != NULL) use();
            if(strstr(cmds->cell[i], "inc") != NULL) inc();
            //FIXME: toto zatím nechápu implementovat jako poslední
            if(strstr(cmds->cell[i], "[set]") != NULL) tmpSet(); */
        }
    }
}

int openFile(int lastArg, char const *fileName[], FILE **inputFile)
{

    *inputFile = fopen(fileName[lastArg - 1], "r");
    if (*inputFile == NULL)
    {
        fprintf(stderr, "Chyba pri otevirani souboru.\n");
        return 0;
    }
    else
    {
        ////debug
        //printf("Otevrel se soubor: %s.\n", fileName[lastArg - 1]);
        return 1;
    }
}

/* Uzavře soubor */
void closeFile(FILE *inputFile)
{
    ////debug
    //printf("Soubor uzavren.\n");
    fclose(inputFile);
    return;
}

/* Kontroluje, zda argumenty byli zadány správně */
int checkArgs(int argCount, const char *argValue[])
{
    /* Pokud ukazatel na .txt není NULL a zároveň byl zadán DELIM, tak musí být počet argumentů 5 nebo počet argumentů musí být 3  */
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
    if (!strcmp(argValue[DELIMPOS], "-d") && (argCount == MAXARGS))
    {
        char const *delim = argValue[DELIMPOS + 1];
        return delim;
    }
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
    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            table->rowCount++;
            table->colCount = 0;
        }

        if (strchr(delim, character) != NULL)
        {
            //počet nalezených znaků delim
            table->cellCount++;
            table->colCount++;
        }
    }
    //DEBUG
    //printf("col: %d, row: %d, cell: %d\n", table->colCount, table->rowCount, table->cellCount);

    //windows nerad přidává konec řádku na konec posledního řádku
    /* FIXME: dělení 0 */
    /* if (table->colCount != 0)
        table->rowCount++; */

    //počet delimů + počet řádků = počet buněk

    table->cellCount += table->rowCount;
    table->colCount = table->cellCount / table->rowCount;
    //DEBUG
    //printf("col: %d, row: %d, cell: %d\n", table->colCount, table->rowCount, table->cellCount);
    return;
}

/* Inicializace tabulky */
void initTable(array *table, FILE *inputFile)
{
    fseek(inputFile, 0, SEEK_SET);
    //alokace počtu řádků
    table->cell = malloc(sizeof(char *) * table->rowCount);
    if (table->cell == NULL)
    {
        fprintf(stderr, "Chyba pri alokaci mista v pameti.");
        tableDestruct(table);
        return;
    }
    for (int i = 0; i < table->rowCount; i++)
    {

        table->cell[i] = malloc(sizeof(char *) * table->colCount);
        if (table->cell == NULL)
        {
            fprintf(stderr, "Chyba pri alokaci mista v pameti.");
            tableDestruct(table);
            return;
        }
        for (int j = 0; j < table->colCount; j++)
        {
            //printf("i:%d, j:%d  - %d\n", i, j, table->colCount);
            //možná by stačilo za 1 dát colCount ověřit
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

    tableConstruct(table);
    //TODO: spočítat počet řádků a sloupců
    countRowsAndCols(table, inputFile, delim);
    //TODO: vytvoření tabulky
    initTable(table, inputFile);
    /* FIXME: checkovat délku a případně zvětšovat  */
    fseek(inputFile, 0, SEEK_SET);
    char character;
    int currentRow = 0, currentCol = 0;
    unsigned int charPos = 0;
    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            //VERY IMPORTANT THING RIGHT HERE
            table->cell[currentRow][currentCol][charPos] = '\0';
            charPos = 0;
            currentCol = 0;
            currentRow++;
        }

        if (strchr(delim, character) != NULL)
        {
            //VERY IMPORTANT THING RIGHT HERE
            table->cell[currentRow][currentCol][charPos] = '\0';
            charPos = 0;
            currentCol++;
        }
        else
        {
            if (character != '\n')
            {
                table->cell[currentRow][currentCol][charPos] = character;
                //0
                charPos++;
                //1
                if (charPos >= MINCELL)
                    if (!resizeCellOn(table, charPos + 1, currentRow, currentCol))
                        return 0;
            }
        }
    }
    return 1;
}

/* Vypsání tabulky */
void printTable(array *table, char delim)
{
    for (int i = 0; i < table->rowCount; i++)
    {
        for (int j = 0; j < table->colCount; j++)
            if ((table->colCount - 1) == j)
                printf("%s", table->cell[i][j]);
            else
                printf("%s%c", table->cell[i][j], delim);
        printf("\n");
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
        if (openFile(argc, argv, &inputFile))
        {
            array table;
            const char *delim = findDelim(argc, argv);

            //TODO: naplnění tabulky daty ze souboru
            if (fileToTable(&table, inputFile, delim))
            {
                cmdArray cmds;
                selection selected;
                //resizeRowBy(&table, 10);
                /* //TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req */

                processCommands(&cmds, &selected, &table, argc, argv);
                //printf("selected: %d[%d, %d]\n", cmds.cell[selected.selRow1 - 1][selected.selCol1 - 1], selected.selRow1, selected.selCol1);
                /* //TODO: vypsání dat */
                //printf("%d, %d, %d, %d", selected.selRow1, selected.selCol1, selected.selRow2, selected.selCol2);
                //printf("%d", table.rowCount);
                printTable(&table, delim[0]);
                cmdDestruct(&cmds);
            }

            //TODO: free all všeho nad čím se provedl malloc nebo realloc
            //uzavření souboru po načtení dat do pole
            closeFile(inputFile);

            //desktukce tabulky - uvolnění paměti
            tableDestruct(&table);
        }
        else
            //pokud se soubor nepodaří otevřít
            return 1;
    }
    return 0;
}
