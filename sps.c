/*                            IZP projekt č.2                                */
/*                            Marek Bitomský                                 */
/*                                 2020                                      */
/*                                                                           */
/*                                                                           */
/*     Cílem projektu je vytvořit tabulkový procesor na příkazové řádce.     */

//Hlavičkové soubory
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 5
#define MINARGS 3
#define DELIMPOS 1
#define MINCELL 100

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
int checkArgs(int argCount, char const *argValue[])
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
        }

        if (strchr(delim, character) != NULL)
        {
            table->cellCount++;
        }
    }
    table->cellCount += table->rowCount;
    table->colCount = table->cellCount / table->rowCount;
    return;
}

/* Inicializace tabulky */
void initTable(array *table, FILE *inputFile, const char *delim)
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

/* Rozdělí data ze souboru do 2D pole */
void fileToTable(array *table, FILE *inputFile, const char *delim)
{
    /* FIXME: checkovat délku a případně zvětšovat  */
    fseek(inputFile, 0, SEEK_SET);
    char character;
    int currentRow = 0, currentCol = 0, charPos = 0;
    while ((character = fgetc(inputFile)) != EOF)
    {
        //printf("%d, %d, %d: ", currentCol, currentRow, charPos);
        //printf("%c\n", character);
        //printf("%d, %d, %d\n", table->colCount, table->rowCount, table->cellCount);
        if (strchr(delim, character) != NULL)
        {
            //je delim
            charPos = 0;
            currentCol++;
        }
        else
        {
            //neni delim
            if (character != '\n')
            {
                table->cell[currentRow][currentCol][charPos] = character;
                charPos++;
            }
        }

        if (character == '\n')
        {
            charPos = 0;
            currentCol = 0;
            currentRow++;
        }
    }
    return;
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

/* Změní velikost řádku o zadaný počet */
void resizeRowBy(array *table, int by)
{
}

/* Změní velikost sloupce o zadaný počet */
void resizeColBy(array *table, int by)
{
}

/* Změní velikost buňky o zadaný počet */
void resizeCellBy(array *table, int by)
{
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
            initTable(&table, inputFile, delim);
            //TODO: naplnění tabulky daty ze souboru
            fileToTable(&table, inputFile, delim);
            //TODO: vypsání dat
            printTable(&table, delim[0]);
            //TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req
            //TODO: free all všeho nad čím se provedl malloc nebo realloc

            //uzavření souboru po načtení dat do pole
            closeFile(inputFile);

            //TODO: odstranění věcí v tabulce
            tableDestruct(&table);
        }
        else
            //pokud se soubor nepodaří otevřít
            return 1;
    }
    return 0;
}
