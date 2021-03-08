#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stack>

#ifndef LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
#define LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H

typedef struct ASTTree {
    struct ASTTree* l;
    struct ASTTree* r;
    int type;
    struct data {
        int type;
        char* data;
    } data;
} ASTTree;

typedef struct VariableDefNode {
    int size;            // define num
    char* variable[20];  // names
    struct VariableDefNode* next;
} VDN;

typedef enum DataType {
    EXTDEFLIST = 1,
    EXTVARDEF,
    EXTVARTYPE,
    EXTVARLIST,          //外部变量名序列结点
    EXTVAR,              //外部变量名结点
    FUNCDEF,             //函数定义结点
    FUNCRETURNTYPE,      //函数返回值类型结点
    FUNCNAME,            //函数名结点
    FUNCFORMALPARALIST,  //函数形式参数序列结点
    FUNCFORMALPARADEF,   //函数形式参数结点10
    FUNCFORMALPARATYPE,  //函数形参类型结点
    FUNCFORMALPARA,      //函数形参名结点
    FUNCBODY,            //函数体结点
    LOCALVARDEFLIST,     //局部变量定义序列结点
    LOCALVARDEF,         //局部变量定义结点15
    LOCALVARTYPE,        //局部变量类型结点
    LOCALVARNAMELIST,    //局部变量名序列
    LOCALVARNAME,        //局部变量名
    STATELIST,           //语句序列结点
    OPERAND,             //操作数结点20
    OPERATOR,            //运算符结点
    EXPRESSION,          //表达式
    IFPART,              // if语句部分
    ELSEPART,            // else部分
    IFSTATEMENT,         // if语句25
    IFELSESTATEMENT,     // if-else语句
    WHILESTATEMENT,      // while语句结点
    WHILEPART,           // while条件语句结点
    WHILEBODY,           // while语句体
    FORSTATEMENT,        // for语句结点30
    FORPART,             // for条件语句
    FORPART1,            // for语句条件一
    FORPART2,            // for语句条件二
    FORPART3,            // for语句条件三
    FORBODY,             // for语句体35
    RETURNSTATEMENT,     // return语句
    BREAKSTATEMENT,      // break语句
    DOWHILESTATEMENT,    // do-while循环语句
    DOWHILEBODY,         // do-while语句体
    DOWHILECONDITION,    // do-while条件40
    CONTINUESTATEMENT,   // continue语句
    FUNCCLAIM,           //函数声明
    ARRAYDEF,            //数组定义
    ARRAYTYPE,           //数组类型
    ARRAYNAME,           //数组名45
    ARRAYSIZE,           //数组大小
};

void syntaxAnalyse();

void freeTree(ASTTree* root);

ASTTree* program();

ASTTree* ExtDefList();

ASTTree* ExtDef();

ASTTree* ExtVarDef();

ASTTree* FuncDef();

ASTTree* FormParaList();

ASTTree* FormParaDef();

ASTTree* CompState();

ASTTree* LocalVarDefList();

ASTTree* StateList();

ASTTree* Statement();

ASTTree* Expression(int endsym);

char Precede(int c1, int c2);

void returnToken(int w, FILE* fp);

void showType(int type);

void PreorderTranverse(ASTTree* root, int depth);

int addName(char* token_text);

int checkname_exist(char* token_text);

ASTTree* ArrayDef();

#endif  // LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
