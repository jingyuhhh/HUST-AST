#include <stdio.h>
#include <stdlib.h>
#include "format.h"
#include "getToken.h"
#include "lexicalAnalyse.h"
#include "syntaxAnalyse.h"

char *filename;
FILE *fp;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "%s <filename>", argv[0]);
        exit(-1);
    }
    filename = argv[1];

    int option;
    while (1)
    {
        system("clear");
        printf(" +----------------------------+\n");
        printf(" |    Choose A Function       |\n");
        printf(" |    1. Lexical Analysis     |\n");
        printf(" |    2. Syntax Analysis      |\n");
        printf(" |    3. Show Formated File   |\n");
        printf(" |    0. Exit                 |\n");
        printf(" +----------------------------+\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            fp = fopen(filename, "r");
            lexicalAnalyse();
            printf("Press any key to continue\n");
            fclose(fp);
            getchar();
            getchar();
            break;
        case 2:
            fp = fopen(filename, "r");
            syntaxAnalyse();
            printf("Press any key to continue\n");
            fclose(fp);
            getchar();
            getchar();
            break;
        case 3:
            fp = fopen(filename, "r");
            format();
            printf("Press any key to continue\n");
            fclose(fp);
            getchar();
            getchar();
            break;
        case 0:
            system("clear");
            printf("Thanks for using ;\n");
            return 0;
        default:
            printf("Unknown key, please enter again\n");
            printf("Press any key to continue\n");
            getchar();
            getchar();
            break;
        }
    }
}