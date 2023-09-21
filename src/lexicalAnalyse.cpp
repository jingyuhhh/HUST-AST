#include "lexicalAnalyse.h"
#include "getToken.h"

extern char token_text[20];
extern int cnt_lines;
extern FILE *fp;

const char *token_names[] = {
    "ERROR_TOKEN",
    "IDENT",
    "INT_CONST",
    "FLOAT_CONST",
    "CHAR_CONST",
    "STRING_CONST",
    "KEYWORD",
    "INT",
    "DOUBLE",
    "FLOAT",
    "CHAR",
    "SHORT",
    "LONG",
    "IF",
    "ELSE",
    "DO",
    "WHILE",
    "FOR",
    "STRUCT",
    "BREAK",
    "SWITCH",
    "CASE",
    "TYPEDEF",
    "RETURN",
    "CONTINUE",
    "VOID",
    "EQ",
    "NEQ",
    "ASSIGN",
    "LP",
    "RP",
    "LB",
    "RB",
    "LM",
    "RM",
    "SEMI",
    "COMMA",
    "TIMES",
    "DIVIDE",
    "ANNO",
    "PLUS",
    "PLUSPLUS",
    "MINUS",
    "MINUSMINUS",
    "MORE",
    "MOREEQUAL",
    "LESS",
    "LESSEQUAL",
    "INCLUDE",
    "MACRO",
    "ARRAY"};

void printToken(int type, const char *value)
{
    printf("%-14s\t%s\n", token_names[type], value);
}

int lexicalAnalyse()
{
    int type;

    printf("\n");
    printf("WordType\tWordValue\n");

    do
    {
        type = getToken(fp);
        if (type != ERROR_TOKEN)
        {
            printToken(type, token_text);
        }
        else
        {
            printf("\t\x1b[31mError in line %d\x1b[0m\n", cnt_lines);
        }
    } while (type != -1);

    return 0;
}
