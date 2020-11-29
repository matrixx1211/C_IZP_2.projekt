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
}

/* Zvětšení pole o danou velikost */
void resizeArray(array *data, FILE *inputFile)
{
}

/* Rozdělí data ze souboru do 2D pole */
void fileToArray(array *data, FILE *inputFile, char const *delim)
{
    //bude číst soubor
    //ptát se jestli znak není z delimu
    char character;
    while ((character = fgetc(inputFile)) != EOF)
    {
        //počet sloupců
        for (int i = 0; i < strlen(delim); i++)
        {
            if (character == delim[i])
            data->colCount++;
        }

        //počet řádků
        if (character == '\n')
            data->rowCount++;

    }
        if (data->colCount % data->rowCount == 0)
        {
        data->colCount = data->colCount/data->rowCount+1;
        printf("Pocet sloupcu %d, pocet radku %d.\n", data->colCount, data->rowCount);
        } else 
        fprintf(stderr, "Pocet sloupcu je ruzny.");
        
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
            fileToArray(&data, inputFile, delim);

            //uzavření souboru po načtení dat do pole
            closeFile(inputFile);

            /* TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req */

            /* TODO: free all všeho nad čím se provedl malloc nebo realloc*/
        }
        else
            //pokud se soubor nepodaří otevřít
            return 1;
    }
    return 0;
}
