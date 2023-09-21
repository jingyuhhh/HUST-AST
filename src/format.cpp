#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getToken.h"

extern FILE *fp;
extern char token_text[20];
FILE *fp1 = fopen("D:\\Project\\data_structure\\src\\output.c", "a");

// 输出到output.c
void Output(char *res)
{
    if (fp1 == NULL)
    {
        printf("Cannot open file strike any key exit!");
        getchar();
        exit(1);
    }
    fprintf(fp1, "%s", res);
}

// 格式化
void format()
{
    int tabs = 0; // 控制缩进
    int flag = 0; // 用于标记无大括号但需要缩进的情况
    token *root, *tail, *p;
    while (1)
    {
        root = readline();
        if (root == NULL)
        {
            break;
        }
        tail = root;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        if ((root == tail) && (strcmp(tail->str, "}") == 0))
        {
            tabs--;
        }
        if ((root != tail) && (strcmp(root->str, "}") == 0))
        {
            tabs--;
        }
        for (int i = 0; i < tabs; i++)
        { // 输出缩进
            printf("    ");
            Output("    ");
        }
        if (flag == 1)
        {
            tabs--;
            flag = 0;
        }
        p = root;
        while (p != NULL)
        {
            printf("%s ", p->str);
            Output(p->str);
            Output(" ");
            p = p->next;
        }
        printf("\n");
        Output("\n");
        if (strcmp(tail->str, "{") == 0)
        {
            tabs++;
        }
        p = root;
        while (p != NULL)
        {
            if ((strcmp(p->str, "if") == 0) || (strcmp(p->str, "for") == 0) ||
                (strcmp(p->str, "while") == 0))
            {
                if (strcmp(tail->str, "{") != 0 &&
                    strcmp(tail->str, ";") != 0)
                {
                    tabs++;
                    flag = 1;
                }
            }
            p = p->next;
        }
    }
    fclose(fp1);
}

// 读取一行
token *readline()
{
    int word = getToken(fp);
    if (word == -1)
    {
        return NULL;
    }

    token *root = NULL, *tail = NULL;
    root = (token *)malloc(sizeof(token));
    char *token_text1 = (char *)malloc(25 * sizeof(char));
    strcpy(token_text1, token_text);
    root->str = token_text1;
    root->next = NULL;
    tail = root;

    char c;
    while ((c = fgetc(fp)) != '\n')
    {
        ungetc(c, fp);
        getToken(fp);
        tail->next = (token *)malloc(sizeof(token));
        tail = tail->next;
        token_text1 = (char *)malloc(25 * sizeof(char));
        strcpy(token_text1, token_text);
        tail->str = token_text1;
        tail->next = NULL;
    }
    return root;
}
