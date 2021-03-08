#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getToken.h"

extern FILE* fp;
extern char token_text[20];

void format() {
    int tabs = 0;  //控制缩进
    int flag = 0;  //用于标记无大括号但需要缩进的情况
    token *root, *tail, *p;
    while (1) {
        root = readline();
        if (root == NULL) {
            break;
        }
        tail = root;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        if ((root == tail) && (strcmp(tail->str, "}") == 0)) {
            tabs--;
        }
        if ((root != tail) && (strcmp(root->str, "}") == 0)) {
            tabs--;
        }
        for (int i = 0; i < tabs; i++) {  //输出缩进
            printf("    ");
        }
        if (flag == 1) {
            tabs--;
            flag = 0;
        }
        p = root;
        while (p != NULL) {
            printf("%s ", p->str);
            p = p->next;
        }
        printf("\n");
        if (strcmp(tail->str, "{") == 0) {
            tabs++;
        }
        p = root;
        while (p != NULL) {
            if ((strcmp(p->str, "if") == 0) || (strcmp(p->str, "for") == 0) ||
                (strcmp(p->str, "while") == 0)) {
                if (strcmp(tail->str, "{") != 0 &&
                    strcmp(tail->str, ";") != 0) {
                    tabs++;
                    flag = 1;
                }
            }
            p = p->next;
        }
    }
}

token* readline() {
    int word = getToken(fp);
    if (word == -1) {
        return NULL;
    }

    token *root = NULL, *tail = NULL;
    root = (token*)malloc(sizeof(token));
    char* token_text1 = (char*)malloc(25 * sizeof(char));
    strcpy(token_text1, token_text);
    root->str = token_text1;
    root->next = NULL;
    tail = root;

    char c;
    while ((c = fgetc(fp)) != '\n') {
        ungetc(c, fp);
        getToken(fp);
        tail->next = (token*)malloc(sizeof(token));
        tail = tail->next;
        token_text1 = (char*)malloc(25 * sizeof(char));
        strcpy(token_text1, token_text);
        tail->str = token_text1;
        tail->next = NULL;
    }
    return root;
}
