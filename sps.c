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

int openFile(int lastArg, char const *fileName[], FILE *inputFile)
{

    inputFile = fopen(fileName[lastArg - 1], "r");
    if (inputFile == NULL)
    {
        fprintf(stderr, "Chyba pri otevirani souboru.\n");
        return 0;
    }
    else
    {
        //debug
        printf("Otevrel se soubor: %s.\n", fileName[lastArg - 1]);
        return 1;
    }
}

/* Uzavře soubor */
void closeFile(FILE *inputFile)
{
    //debug
    printf("Soubor uzavren.\n");
    fclose(inputFile);
}

int main(int argc, char const *argv[])
{
    //zpracovávat se bude v případě, že rozsah argumentu je 3 až 5 včetně
    if (argc >= 3 && argc <= 5)
    {
        //deklarace souboru
        FILE *inputFile;

        if (openFile(argc, argv, inputFile))
        {
            /* TODO: Nalezení delimu opt */

            /* TODO: Rozdělení dat ze souboru do pole (tabulky) req */

            //uzavření souboru po načtení dat do pole
            closeFile(inputFile);

            /* TODO:  Rozšifrování CMD_SEQUENCE (příkazy oddělené ;) req */
        }
        else
            //uzavření souboru v případě, že vračí openFile NULL
            closeFile(inputFile);
    }
    return 0;
}
