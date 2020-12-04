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

//Definice struktury
typedef struct
{
    int rowCount;
    int colCount;
    int cellCount;
    char ***cell;
} array;

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
            free(table->cell[i][j]);
        }
        free(table->cell[i]);
    }
    free(table->cell);
    return;
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
        tableDestruct(table);
        fprintf(stderr, "Chyba pri alokaci mista v pameti.");
        return;
    }
    for (int i = 0; i < table->rowCount; i++)
    {

        table->cell[i] = malloc(sizeof(char *) * table->colCount);
        if (table->cell == NULL)
        {
            tableDestruct(table);
            fprintf(stderr, "Chyba pri alokaci mista v pameti.");
            return;
        }
        for (int j = 0; j < table->colCount; j++)
        {
            //printf("i:%d, j:%d  - %d\n", i, j, table->colCount);
            //možná by stačilo za 1 dát colCount ověřit
            table->cell[i][j] = calloc(1, MINCELL);
            if (table->cell == NULL)
            {
                tableDestruct(table);
                fprintf(stderr, "Chyba pri alokaci mista v pameti.");
                return;
            }
        }
    }
    return;
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
/* void resizeColBy(array *table, int by)
{
    //char *colPointer = ;
} */

/* Změní velikost buňky o zadaný počet */
int resizeCellBy(array *table, int by, int row, int col)
{
    //printf("realokace bunky: [%d, %d] na velikost: %d\n", row, col, by);
    //printf("%d: \"%s\" <-", strlen(table->cell[row][col]) + by, table->cell[row][col]);
    //printf("%ld <-", strlen(table->cell[row][col]) + by);
    //strlen vrátí hodnotu bez \0 takže pro text "ahoj", který potřebuje velikost 5 strlen vrací 4
    //1 * ((4 + 1) + 1) = 1 * (5 + 1) = 6
    //z toho plyne že je potřeba volat, když je velikost buňky x velká 2 + 1 + 1
    char *cellPointer = realloc(table->cell[row][col], sizeof(char) * (strlen(table->cell[row][col]) + by));
    if (cellPointer == NULL)
    {
        fprintf(stderr, "Chyba pri realokaci.");
        free(cellPointer);
        tableDestruct(table);
        return 0;
    }
    table->cell[row][col] = cellPointer;
    return 1;
}

/* Rozdělí data ze souboru do 2D pole */
int fileToTable(array *table, FILE *inputFile, const char *delim)
{
    /* FIXME: checkovat délku a případně zvětšovat  */
    fseek(inputFile, 0, SEEK_SET);
    char character;
    int currentRow = 0, currentCol = 0, charPos = 0;
    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            //VERY IMPORTANT THING RIGHT HERE
            table->cell[currentRow][currentCol][charPos] = '\0';
            //printf("\n%s\n", table->cell[currentRow][currentCol]);

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
                //printf("pozice: %d, znak: %c, velikost: %d\n", charPos, character, charPos + 1);

                //na začátku velikost buňky je 1 uložím tam znak (charPos = 0)
                table->cell[currentRow][currentCol][charPos] = character;
                //přičtu pozici pro další znak (charPos = 1)
                charPos++;
                if (charPos >= MINCELL)
                    //zavolá se resize by
                    if (!resizeCellBy(table, 1, currentRow, currentCol))
                        return 0;
                //pokud je velikost buňky (charPos = 1) + 1 => 2 > 1
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

            tableConstruct(&table);
            //TODO: spočítat počet řádků a sloupců
            countRowsAndCols(&table, inputFile, delim);
            //TODO: vytvoření tabulky
            initTable(&table, inputFile);
            //TODO: naplnění tabulky daty ze souboru
            if (fileToTable(&table, inputFile, delim))
            {
                /* //test
                resizeRowBy(&table, 10);
                resizeCellBy(&table, 10, table.rowCount - 1, table.colCount - 1);
                sprintf(table.cell[table.rowCount - 1][table.colCount - 1], "%d, %d last", table.rowCount - 1, table.colCount - 1); */
                //printf("%d\n", strlen("xxx"));
                //TODO: vypsání dat
                printTable(&table, delim[0]);
                //TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req
                //TODO: free all všeho nad čím se provedl malloc nebo realloc
            }

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
