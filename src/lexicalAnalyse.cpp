#include "lexicalAnalyse.h"
#include "getToken.h"

extern char token_text[20];
extern int cnt_lines;
extern FILE* fp;

int lexicalAnalyse() {
    int type;
    if (fp == NULL) {
        printf("fail to open file\n");
        exit(0);
    }
    printf("\n");
    printf("  WordType");
    printf("\tWord值\n");
    do {
        type = getToken(fp);
        if (type != ERROR_TOKEN) {
            switch (type) {
                case IDENT:
                    printf("%14s", "IDENT");
                    break;
                case INT_CONST:
                    printf("%14s", "INT_CONST");
                    break;
                case FLOAT_CONST:
                    printf("%14s", "FLOAT_CONST");
                    break;
                case CHAR_CONST:
                    printf("%14s", "CHAR_CONST");
                    break;
                case STRING_CONST:
                    printf("%14s", "STRING_CONST");
                    break;
                case KEYWORD:
                    printf("%14s", "KEYWORD");
                    break;
                case INT:
                    printf("%14s", "INT");
                    break;
                case DOUBLE:
                    printf("%14s", "DOUBLE");
                    break;
                case FLOAT:
                    printf("%14s", "FLOAT");
                    break;
                case CHAR:
                    printf("%14s", "CHAR");
                    break;
                case SHORT:
                    printf("%14s", "SHORT");
                    break;
                case LONG:
                    printf("%14s", "LONG");
                    break;
                case IF:
                    printf("%14s", "IF");
                    break;
                case ELSE:
                    printf("%14s", "ELSE");
                    break;
                case DO:
                    printf("%14s", "DO");
                    break;
                case WHILE:
                    printf("%14s", "WHILE");
                    break;
                case FOR:
                    printf("%14s", "FOR");
                    break;
                case STRUCT:
                    printf("%14s", "STRUCT");
                    break;
                case BREAK:
                    printf("%14s", "BREAK");
                    break;
                case SWITCH:
                    printf("%14s", "SWITCH");
                    break;
                case CASE:
                    printf("%14s", "CASE");
                    break;
                case TYPEDEF:
                    printf("%14s", "TYPEDEF");
                    break;
                case RETURN:
                    printf("%14s", "RETURN");
                    break;
                case CONTINUE:
                    printf("%14s", "CONTINUE");
                    break;
                case VOID:
                    printf("%14s", "VOID");
                    break;
                case EQ:
                    printf("%14s", "EQ");
                    break;
                case NEQ:
                    printf("%14s", "NEQ");
                    break;
                case ASSIGN:
                    printf("%14s", "ASSIGN");
                    break;
                case LP:
                    printf("%14s", "LP");
                    break;
                case RP:
                    printf("%14s", "RP");
                    break;
                case LB:
                    printf("%14s", "LB");
                    break;
                case RB:
                    printf("%14s", "RB");
                    break;
                case LM:
                    printf("%14s", "LM");
                    break;
                case RM:
                    printf("%14s", "RM");
                    break;
                case SEMI:
                    printf("%14s", "SEMI");
                    break;
                case COMMA:
                    printf("%14s", "COMMA");
                    break;
                case TIMES:
                    printf("%14s", "TIMES");
                    break;
                case DIVIDE:
                    printf("%14s", "DIVIDE");
                    break;
                case ANNO:
                    printf("%14s", "ANNO");
                    break;
                case PLUS:
                    printf("%14s", "PLUS");
                    break;
                case PLUSPLUS:
                    printf("%14s", "PLUSPLUS");
                    break;
                case MINUS:
                    printf("%14s", "MINUS");
                    break;
                case MINUSMINUS:
                    printf("%14s", "MINUSMINUS");
                    break;
                case MORE:
                    printf("%14s", "MORE");
                    break;
                case MOREEQUAL:
                    printf("%14s", "MOREEQUAL");
                    break;
                case LESS:
                    printf("%14s", "LESS");
                    break;
                case LESSEQUAL:
                    printf("%14s", "LESSEQUAL");
                    break;
                case INCLUDE:
                    printf("%14s", "INCLUDE");
                    break;
                case MACRO:
                    printf("%14s", "MACRO");
                    break;
                case ARRAY:
                    printf("%14s", "ARRAY");
                    break;
            }
            printf("\t%s\n", token_text);
        } else {
            printf("\tError in line %d\n", cnt_lines);
            break;
        }
    } while (type != -1);

    return 0;
}