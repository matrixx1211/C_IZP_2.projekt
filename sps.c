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

//Definice struktury
typedef struct
{
    int rowCount;
    int colCount;
    int cellCount;
    char **cell;
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
char const *findDelim(int argCount, char const *argValue[])
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
void arrayConstruct(array *data)
{
    /* TODO: inicializace 2D pole */
    data->cell = NULL;
    data->rowCount = 0;
    data->colCount = 0;
    data->cellCount = 0;
}

/* Zvětšení pole o danou velikost */
char *resizeArrayBy(array *data, int by)
{
    char *reallocPointer = realloc(data->cell, by * data->cellCount);
    if (reallocPointer == NULL)
    {
        fprintf(stderr, "Realokace se neprovedla");
    }
    else
        data->cell[data->cellCount];
}

void mat_alloc(array *data)
{
    data->cell = malloc(data->colCount * data->rowCount * sizeof(char *));
    if (data->cell == NULL)
    {
        printf("Malloc fail!");
        return;
    }
    for (int i = 0; i < (data->colCount * data->rowCount); i++)
    {
        data->cell[i] = malloc(sizeof(char));
        if (data->cell[i] == NULL)
        {
            printf("Malloc fail!");
            //mat_clear(tab, i);
            return;
        }
    }
}

/* Rozdělí data ze souboru do 2D pole */
int fileToArray(array *data, FILE *inputFile, char const *delim)
{
    char character;
    int cols = 0;
    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            data->rowCount++;
            cols++;
            data->colCount = cols;
            cols = 0;
        }
        for (unsigned i = 0; i < strlen(delim); i++)
        {
            if (delim[i] == character)
            {
                cols++;
            }
        }
    }
    printf("Pocet sloupcu: %d\n", data->colCount);
    printf("Pocet radku: %d\n", data->rowCount);

    mat_alloc(data);

    printf("Alokace se provedla\n");

    fseek(inputFile, 0, SEEK_SET);

    int currentCell = 0;
    int stringLength = 1;
    int isDelim = 0;

    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            data->cell[currentCell][stringLength - 1] = '\0';
            currentCell++;
            stringLength = 1;
            isDelim = 1;
        }
        for (unsigned i = 0; i < strlen(delim); i++)
        {
            if (delim[i] == character)
            {
                data->cell[currentCell][stringLength - 1] = '\0';
                currentCell++;
                stringLength = 1;
                isDelim = 1;
                break;
            }
            else
            {
                isDelim = 0;
            }
        }

        if (!isDelim)
        {
            if (character != '\n')
            {

                data->cell[currentCell][stringLength - 1] = character;
                stringLength++;
                char *p = realloc(data->cell[currentCell], stringLength * sizeof(char));
                if (p == NULL)
                {
                    printf("Realloc fail!");
                    free(p);
                    free(data->cell);
                    return 0;
                }
                data->cell[currentCell] = p;
            }
        }
    }
    for (int i = 0; i < currentCell; i++)
    {

        printf("%s ", data->cell[i]);
    }
    /* //bude číst soubor
    //ptát se jestli znak není z delimu
    char character;
    int pos = 0;
    //alokace 1. buňky
    data->cell = malloc(sizeof(char*));
    char *reallocP;
    while ((character = fgetc(inputFile)) != EOF)
    {
        //počet sloupců
        for (int i = 0; i < strlen(delim); i++)
        {
            if (character == delim[i])
            {
                data->colCount++;
                //zvětšení pole o jeden ukazatel na string
                //data->cell = resizeArrayBy(data, 1);
                //uložení do pole
                
                break;
            }
        }

        //počet řádků
        if (character == '\n')
        {
            data->rowCount++;
        }
        //načtu řádek, projdu řádek
    }


    if (data->colCount % data->rowCount == 0)
    {
        data->colCount = data->colCount / data->rowCount + 1;
        printf("Pocet sloupcu %d, pocet radku %d.\n", data->colCount, data->rowCount);
        return 1;
    }
    else
    {
        fprintf(stderr, "Pocet sloupcu je ruzny.");
        return 0;
    } */
}

/* Destruktor pro 2D pole */
void arrayDestruct()
{
    /* TODO: inicializace 2D pole */
}

int main(int argc, char const *argv[])
{
    //kontrola argumentů
    if (checkArgs(argc, argv))
    {
        FILE *inputFile;

        //pokud se podaří otevřít soubor
        if (openFile(argc, argv, &inputFile))
        {
            char const *delim;
            array data;

            delim = findDelim(argc, argv);
            /* TODO: Rozdělení dat ze souboru do pole (tabulky) req */
            //printf("delim: %s\n", delim);
            arrayConstruct(&data);
            if (fileToArray(&data, inputFile, delim))
            {

                /* TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req */

                /* TODO: free all všeho nad čím se provedl malloc nebo realloc*/
            }
            else
                return 1;

            //uzavření souboru po načtení dat do pole
            closeFile(inputFile);
        }
        else
            //pokud se soubor nepodaří otevřít
            return 1;
    }
    return 0;
}
