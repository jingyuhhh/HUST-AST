#include "syntaxAnalyse.h"
#include "getToken.h"

using namespace std;

extern char token_text[20];
extern char string_num[20];
extern int cnt_lines;
int w, type;
int haveMistake = 0;
extern FILE *fp;
VDN *Vroot; // VarName链表根节点
int isVoid, hasReturn, isInRecycle = 0;

// 开始语法分析
void syntaxAnalyse()
{
    ASTTree *root = program();
    if (root == NULL || haveMistake == 1) // 若有错或root为空则输出语法错误
    {
        printf("Syntax Error!\n");
        return;
    }
    else
    {
        PreorderTranverse(root, 0); // 否则进行先序遍历
    }
}

// 程序入口
ASTTree *program()
{
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    Vroot = (VDN *)malloc(sizeof(VDN));
    Vroot->size = 0;
    Vroot->next = NULL;
    ASTTree *p = ExtDefList();
    if (p != NULL)
    {
        if (isVoid == 0 && hasReturn == 0)
        {
            printf("Error: Mising return value\n");
            exit(0);
        }
        ASTTree *root = p;
        root->type = EXTDEFLIST;
        return root;
    }
    else
    {
        haveMistake = 1;
        return NULL;
    }
}

// 形成最表层的节点
ASTTree *ExtDefList()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    if (w == -1)
    {
        return NULL;
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->type = EXTDEFLIST;
    root->l = ExtDef();
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    root->r = ExtDefList(); // 下一个节点
    return root;
}

// 判断节点是函数还是数组还是变量
ASTTree *ExtDef()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT &&
        w != FLOAT && w != VOID)
    {
        printf("Mistake in line %d\n", cnt_lines);
        printf("Error: Unexpected datatype. Expected one of INT, DOUBLE, CHAR, LONG, SHORT, FLOAT, VOID.\n");
        exit(0);
    }
    type = w;
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    if (w != IDENT && w != ARRAY)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Expected an identifier or array declaration after datatype.\n");
        exit(0);
    }
    char token_text0[20];
    strcpy(token_text0, token_text);
    ASTTree *p;
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    strcpy(token_text, token_text0);
    if (w == LP)
    {
        p = FuncDef();
    }
    else if (w == ARRAY)
    {
        p = ArrayDef();
    }
    else
    {
        p = ExtVarDef();
    }
    return p;
}

// 解析数组
ASTTree *ArrayDef()
{
    if (type == VOID)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Can't declare a void type array\n");
        exit(0);
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->type = ARRAYDEF;
    root->l = NULL;
    root->r = NULL;
    root->data.data = NULL;
    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->type = ARRAYTYPE;
    p->l = NULL;
    p->r = NULL;
    if (type == INT)
    {
        p->data.data = "int";
    }
    if (type == DOUBLE)
    {
        p->data.data = "double";
    }
    if (type == CHAR)
    {
        p->data.data = "char";
    }
    if (type == FLOAT)
    {
        p->data.data = "float";
    }
    if (type == LONG)
    {
        p->data.data = "long";
    }
    if (type == SHORT)
    {
        p->data.data = "short";
    }

    root->l = p;
    p = (ASTTree *)malloc(sizeof(ASTTree));
    p->type = ARRAYNAME;
    p->l = NULL;
    p->r = NULL;
    char *token_text3 = (char *)malloc(20 * sizeof(char));
    strcpy(token_text3, token_text);
    p->data.data = token_text3;
    root->r = p;
    ASTTree *q = (ASTTree *)malloc(sizeof(ASTTree));
    q->type = ARRAYSIZE;
    q->l = q->r = NULL;
    q->data.data = string_num;
    p->l = q;
    return root;
}

// 解析变量
ASTTree *ExtVarDef()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    int cnt;
    if (type == VOID)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Can't declare a void type variable\n");
        exit(0);
    }

    int u;
    u = addName(token_text);
    if (u == 1)
    {
        haveMistake = 1;
        return NULL;
    }

    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->l = NULL;
    root->r = NULL;
    root->data.data = NULL;
    root->type = EXTVARDEF;

    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->l = p->r = NULL;
    p->data.data = NULL;
    p->type = EXTVARTYPE;
    p->data.type = type;

    if (type == INT)
    {
        p->data.data = "int";
    }
    if (type == DOUBLE)
    {
        p->data.data = "double";
    }
    if (type == CHAR)
    {
        p->data.data = "char";
    }
    if (type == FLOAT)
    {
        p->data.data = "float";
    }
    if (type == LONG)
    {
        p->data.data = "long";
    }
    if (type == SHORT)
    {
        p->data.data = "short";
    }

    root->l = p;
    p = (ASTTree *)malloc(sizeof(ASTTree));
    p->l = p->r = NULL;
    p->data.data = NULL;
    p->type = EXTVARLIST;
    root->r = p;

    p->l = (ASTTree *)malloc(sizeof(ASTTree));
    p->l->l = p->l->r = NULL;
    p->data.data = NULL;
    p->l->type = EXTVAR;
    char *token_text1 = (char *)malloc(25 * sizeof(char)); //@
    strcpy(token_text1, token_text);
    p->l->data.data = token_text1;

    while (1)
    {
        if (w != COMMA && w != SEMI)
        {
            if (cnt_lines > cnt)
            {
                cnt_lines--;
            }
            printf("Error in line %d\n", cnt_lines); //%
            printf("Error: Wrong external define\n");
            exit(0);
        }
        if (w == SEMI)
        {
            return root;
        }
        w = getToken(fp);
        if (w != IDENT)
        {
            printf("Error in line %d\n", cnt_lines); //%
            printf("Error: Wrong external define\n");
            exit(0);
        }

        int u;
        u = addName(token_text);
        if (u == 1)
        {
            haveMistake = 1;
            return NULL;
        }

        ASTTree *q = (ASTTree *)malloc(sizeof(ASTTree));
        q->l = q->r = NULL;
        q->data.data = NULL;
        q->type = EXTVARLIST;
        p->r = q;
        p = q;

        p->l = (ASTTree *)malloc(sizeof(ASTTree));
        p->l->l = p->l->r = NULL;
        p->l->data.data = NULL;
        p->l->type = EXTVAR;
        char *token_text1 = (char *)malloc(25 * sizeof(char));
        strcpy(token_text1, token_text);
        p->l->data.data = token_text1;
        cnt = cnt_lines;
        w = getToken(fp);

        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
    }
}

// 解析函数
ASTTree *FuncDef()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->type = FUNCDEF;
    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->data.data = NULL;
    p->type = FUNCRETURNTYPE;
    p->data.type = type;
    if (type == INT)
    {
        p->data.data = "int";
        isVoid = 0;
    }
    if (type == DOUBLE)
    {
        p->data.data = "double";
        isVoid = 0;
    }
    if (type == CHAR)
    {
        p->data.data = "char";
        isVoid = 0;
    }
    if (type == FLOAT)
    {
        p->data.data = "float";
        isVoid = 0;
    }
    if (type == LONG)
    {
        p->data.data = "long";
        isVoid = 0;
    }
    if (type == SHORT)
    {
        p->data.data = "short";
        isVoid = 0;
    }
    if (type == VOID)
    {
        p->data.data = "void";
        isVoid = 1;
    }
    p->l = NULL;
    p->r = NULL;
    root->l = p;

    // 参数
    ASTTree *q = (ASTTree *)malloc(sizeof(ASTTree));
    q->data.data = NULL;
    q->type = FUNCNAME;
    char *token_text1 = (char *)malloc(25 * sizeof(char));
    strcpy(token_text1, token_text);
    q->data.data = token_text1;

    // 创建一个新的VarName链表节点，用于保存函数名
    VDN *last = Vroot;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = (VDN *)malloc(sizeof(VDN));
    last = last->next;
    last->next = NULL;
    last->size = 0;

    root->r = q;
    q->l = FormParaList();

    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    if (w == SEMI)
    {
        // prototype declare
        root->r->r = NULL;
        root->type = FUNCCLAIM;
    }
    else if (w == LB)
    {
        q->r = CompState();
        q->r->type = FUNCBODY;
    }
    else
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Error in function declaration\n");
        exit(0);
    }
    return root;
}

// 构造表示函数的形式参数列表的抽象语法树
ASTTree *FormParaList()
{
    if (haveMistake == 1)
    {
        return NULL;
    }

    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    if (w == RP)
    {
        return NULL;
    }
    if (w == COMMA)
    {
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->type = FUNCFORMALPARALIST;
    root->l = FormParaDef();
    root->r = FormParaList();
    return root;
}

// 构造表示函数形式参数定义的抽象语法树
ASTTree *FormParaDef()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT &&
        w != FLOAT)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Unexpected datatype. Expected one of INT, DOUBLE, CHAR, LONG, SHORT, FLOAT, VOID.\n");
        exit(0);
    }
    type = w;
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    if (w != IDENT)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Expected an identifier or array declaration after datatype.\n");
        exit(0);
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->type = FUNCFORMALPARADEF;
    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->data.data = NULL;
    p->type = FUNCFORMALPARATYPE;
    p->data.type = type;
    if (type == INT)
    {
        p->data.data = "int";
    }
    if (type == DOUBLE)
    {
        p->data.data = "double";
    }
    if (type == CHAR)
    {
        p->data.data = "char";
    }
    if (type == FLOAT)
    {
        p->data.data = "float";
    }
    if (type == LONG)
    {
        p->data.data = "long";
    }
    if (type == SHORT)
    {
        p->data.data = "short";
    }
    if (type == VOID)
    {
        p->data.data = "void";
    }
    p->l = p->r = NULL;
    root->l = p;
    p = (ASTTree *)malloc(sizeof(ASTTree));
    p->data.data = NULL;
    p->type = FUNCFORMALPARA;

    int u;
    u = addName(token_text);
    if (u == 1)
    {
        haveMistake = 1;
        return NULL;
    }

    char *token_text1 = (char *)malloc(25 * sizeof(char));
    strcpy(token_text1, token_text);
    p->data.data = token_text1;
    p->l = p->r = NULL;
    root->r = p;
    return root;
}

// 构造复合语句的抽象语法树
ASTTree *CompState()
{
    if (haveMistake == 1)
    {
        return NULL;
    }

    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->l = NULL;
    root->r = NULL;
    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT ||
        w == FLOAT)
    {
        root->l = LocalVarDefList();
    }
    else
    {
        root->l = NULL;
    }
    root->r = StateList();
    if (w == RB)
    {
        return root;
    }
    else
    {
        printf("Error: error in compound statement\n");
        exit(0);
        haveMistake = 1;
        freeTree(root);
        return NULL;
    }
}

// 构造局部变量定义列表的抽象语法树
ASTTree *LocalVarDefList()
{
    if (haveMistake == 1)
    {
        return NULL;
    }

    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->data.data = NULL;
    root->type = LOCALVARDEFLIST;
    root->l = NULL;
    root->r = NULL;
    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->data.data = NULL;
    p->type = LOCALVARDEF;
    p->l = p->r = NULL;
    root->l = p;
    p->l = (ASTTree *)malloc(sizeof(ASTTree));
    p->l->data.data = NULL;
    p->l->type = LOCALVARTYPE;
    char *token_text1 = (char *)malloc(25 * sizeof(char));
    strcpy(token_text1, token_text);
    p->l->data.data = token_text1;
    p->l->l = p->l->r = NULL;

    w = getToken(fp);
    while (w == ANNO || w == INCLUDE)
    {
        w = getToken(fp);
    }
    ASTTree *q = (ASTTree *)malloc(sizeof(ASTTree)); // VarNameS结点
    q->data.data = NULL;
    q->l = q->r = NULL;

    p->r = q;
    q->type = LOCALVARNAMELIST;
    q->l = (ASTTree *)malloc(sizeof(ASTTree)); // LocalVarName结点
    q->l->data.data = NULL;
    q->l->type = LOCALVARNAME;
    char *token_text2 = (char *)malloc(25 * sizeof(char));
    strcpy(token_text2, token_text);
    q->l->data.data = token_text2;
    q->l->l = q->l->r = NULL;

    int u;
    u = addName(token_text);
    if (u == 1)
    {
        haveMistake = 1;
        return NULL;
    }

    while (1)
    {
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w == SEMI)
        {
            q->r = NULL;
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            break;
        }
        else if (w == COMMA)
        {
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            ASTTree *s = (ASTTree *)malloc(sizeof(ASTTree));
            s->data.data = NULL;
            q->r = s;
            q = q->r;
            q->type = LOCALVARNAMELIST;
            q->l = (ASTTree *)malloc(sizeof(ASTTree));
            q->l->data.data = NULL;
            q->l->type = LOCALVARNAME;
            char *token_text1 = (char *)malloc(25 * sizeof(char));
            strcpy(token_text1, token_text);
            q->l->data.data = token_text1;

            int u;
            u = addName(token_text);
            if (u == 1)
            {
                freeTree(root);
                haveMistake = 1;
                return NULL;
            }
        }
        else
        {
            printf("Error in line %d\n", cnt_lines); //%
            printf("Error: Wrong definition of variable\n");
            exit(0);
        }
    }

    if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT ||
        w == FLOAT)
    {
        root->r = LocalVarDefList();
    }
    else
    {
        root->r = NULL;
    }
    root->r = NULL;
    return root;
}

// 表达式解析函数，endsym为结束符
ASTTree *Expression(int endsym)
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    if (w == endsym)
    {
        return NULL;
    }
    int error = 0;
    stack<ASTTree *> op;
    ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
    p->data.data = NULL;
    p->type = OPERATOR;
    p->data.type = POUND;
    op.push(p);
    stack<ASTTree *> opn;
    ASTTree *t, *t1, *t2, *root;
    while (((w != endsym) || (op.top()->data.type != POUND)) && !error)
    {
        if (op.top()->data.type == RP)
        {
            if (op.size() < 3)
            {
                error++;
                break;
            }
            op.pop();
            op.pop();
        }
        if (w == IDENT)
        {
            if (checkname_exist(token_text) == 0)
            {
                haveMistake = 1;
            }
        }
        if (w == IDENT || w == INT_CONST || w == FLOAT_CONST ||
            w == CHAR_CONST || w == ARRAY || w == STRING_CONST)
        {
            p = (ASTTree *)malloc(sizeof(ASTTree));
            p->data.data = NULL;
            p->type = OPERAND;
            char *token_text1 = (char *)malloc(25 * sizeof(char));
            strcpy(token_text1, token_text);
            p->data.data = token_text1;
            opn.push(p);
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
        }
        else if (w == endsym)
        {
            ASTTree *p = (ASTTree *)malloc(sizeof(ASTTree));
            p->data.data = NULL;
            p->type = OPERATOR;
            p->data.type = POUND;
            while (op.top()->data.type != POUND)
            {
                t2 = opn.top();
                if (!t2 && (op.top()->data.type) != PLUSPLUS &&
                    (op.top()->data.type) != MINUSMINUS)
                {
                    error++;
                    break;
                }
                if (t2 != NULL)
                    opn.pop();
                if (opn.size() == 0)
                {
                    t1 = NULL;
                }
                else
                {
                    t1 = opn.top();
                }
                if (!t1 && (op.top()->data.type) != PLUSPLUS &&
                    (op.top()->data.type) != MINUSMINUS)
                {
                    error++;
                    break;
                }
                if (t1 != NULL)
                    opn.pop();
                t = op.top();
                if (!t)
                {
                    error++;
                    break;
                }
                op.pop();
                t->l = t1;
                t->r = t2;
                opn.push(t);
            }
            if (opn.size() != 1)
            {
                error++;
            }
        }
        else if (w >= EQ && w <= MINUSMINUS)
        {
            char *token_text1 = (char *)malloc(25 * sizeof(char)); //@
            switch (Precede(op.top()->data.type, w))
            {
            case '<':
                p = (ASTTree *)malloc(sizeof(ASTTree));
                p->data.data = NULL;
                p->type = OPERATOR;
                p->data.type = w;
                strcpy(token_text1, token_text);
                p->data.data = token_text1;
                op.push(p);
                w = getToken(fp);
                while (w == ANNO || w == INCLUDE)
                {
                    w = getToken(fp);
                }
                break;
            case '=':
                t = op.top();
                if (!t)
                {
                    error++;
                    op.pop();
                }
                w = getToken(fp);
                while (w == ANNO || w == INCLUDE)
                {
                    w = getToken(fp);
                }
                break;
            case '>':
                t2 = opn.top();
                if (!t2 && (op.top()->data.type) != PLUSPLUS &&
                    (op.top()->data.type) != MINUSMINUS)
                {
                    error++;
                    break;
                }
                if (t2 != NULL)
                    opn.pop();
                if (opn.size() == 0)
                {
                    t1 = NULL;
                }
                else
                {
                    t1 = opn.top();
                }
                if (!t1 && (op.top()->data.type) != PLUSPLUS &&
                    (op.top()->data.type) != MINUSMINUS)
                {
                    error++;
                    break;
                }
                if (t1 != NULL)
                    opn.pop();
                t = op.top();
                if (!t)
                {
                    error++;
                    break;
                }
                op.pop();
                t->l = t1;
                t->r = t2;
                opn.push(t);

                p = (ASTTree *)malloc(sizeof(ASTTree));
                p->data.data = NULL;
                p->type = OPERATOR;
                p->data.type = w;
                strcpy(token_text1, token_text);
                p->data.data = token_text1;
                op.push(p);

                w = getToken(fp);
                while (w == ANNO || w == INCLUDE)
                {
                    w = getToken(fp);
                }
                break;
            case '\0':
                printf("Error in line %d\n", cnt_lines);
                printf("Error: unknown operator\n");
                exit(0);
            }
        }
        else
        {
            error = 1;
        }
    };
    if ((opn.size() == 1) && (op.top()->data.type == POUND) && error == 0)
    {
        t = opn.top();
        opn.pop();
        root = (ASTTree *)malloc(sizeof(ASTTree));
        root->data.data = NULL;
        root->l = NULL;
        root->r = NULL;
        root->type = EXPRESSION;
        root->l = t;
        return root;
    }
    else
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: wrong expression\n");
        exit(0);
    }
}

// 解析语句块
ASTTree *StateList()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    ASTTree *root = NULL;
    ASTTree *r1 = Statement();
    if (r1 == NULL)
    {
        return NULL;
    }
    else
    {
        // 创建一个新节点来保存状态
        root = (ASTTree *)malloc(sizeof(ASTTree));
        root->data.data = NULL;
        root->type = STATELIST;
        root->l = r1;
        root->r = NULL;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != RB)
        {
            root->r = StateList();
            return root;
        }
        else
        {
            return root;
        }
    }
}

// 解析单个语句
ASTTree *Statement()
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    ASTTree *root = (ASTTree *)malloc(sizeof(ASTTree));
    root->l = NULL;
    root->r = NULL;
    root->data.data = NULL;
    switch (w)
    {
    case IF:
    {
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != LP)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: Wrong IF statement\n");
            exit(0);
        }

        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        ASTTree *p1 = (ASTTree *)malloc(sizeof(ASTTree));
        p1->data.data = NULL;
        p1->type = IFPART;
        p1->l = Expression(RP);
        if (p1->l == NULL)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: ifStatementCondition部分出错\n");
            exit(0);
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w == LB)
        {
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            p1->r = StateList();
        }
        else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST ||
                 w == IDENT || w == KEYWORD || w == IF || w == WHILE ||
                 w == ELSE || w == FOR || w == DO)
        {
            p1->r = Statement();
            p1->r->r = NULL;
        }
        else
        {
            printf("Error: Error within IF\n");
            haveMistake = 1;
            return NULL;
        }
        root->l = p1;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w == ELSE)
        {
            root->type = IFELSESTATEMENT;
            ASTTree *p2 = (ASTTree *)malloc(sizeof(ASTTree));
            p2->data.data = NULL;
            p2->type = ELSEPART;
            root->r = p2;
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            if (w == LB)
            {
                w = getToken(fp);
                while (w == ANNO || w == INCLUDE)
                {
                    w = getToken(fp);
                }
                p2->r = StateList();
            }
            else if (w == INT_CONST || w == FLOAT_CONST ||
                     w == CHAR_CONST || w == IDENT || w == KEYWORD)
            {
                p2->r = Statement();
                p2->r->r = NULL;
            }
            else if (w == IF)
            {
                p2->l = Statement();
            }
            else
            {
                printf("Error: Error in ELSE\n");
                haveMistake = 1;
                return NULL;
            }
        }
        else
        {
            root->type = IFSTATEMENT;
            returnToken(w, fp);
        }
        return root;
    }
    case WHILE:
    {
        isInRecycle = 1;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != LP)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: Error in WHILE\n");
            exit(0);
            haveMistake = 1;
            return NULL;
        }

        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        ASTTree *p1 = (ASTTree *)malloc(sizeof(ASTTree));
        p1->data.data = NULL;
        p1->type = WHILEPART;
        p1->l = NULL;
        p1->r = NULL;
        p1->l = Expression(RP);
        if (p1->l == NULL)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: whileStatementCondition部分出错\n");
            exit(0);
            haveMistake = 1;
            return NULL;
        }
        ASTTree *p2 = (ASTTree *)malloc(sizeof(ASTTree));
        p2->data.data = NULL;
        p2->type = WHILEBODY;
        p2->l = NULL;
        p2->r = NULL;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w == LB)
        {
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            p2->r = StateList();
        }
        else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST ||
                 w == IDENT || w == KEYWORD)
        {
            p2->r = Statement();
            p2->r->r = NULL;
        }
        else
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: Error in WHILE\n");
            exit(0);
            haveMistake = 1;
            return NULL;
        }
        root->type = WHILESTATEMENT;
        root->l = p1;
        root->r = p2;
        isInRecycle = 0;
        return root;
    }
    case FOR:
    {
        isInRecycle = 1;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != LP)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: Error in FOR\n");
            exit(0);
            haveMistake = 1;
            return NULL;
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        ASTTree *p1 = (ASTTree *)malloc(sizeof(ASTTree));
        p1->data.data = NULL;
        p1->type = FORPART;
        ASTTree *q = (ASTTree *)malloc(sizeof(ASTTree)); // FOR part 1
        p1->l = q;
        q->type = FORPART1;
        q->data.data = NULL;
        q->l = Expression(SEMI);
        if (q->l == NULL)
        {
            q->data.data = "None";
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        q->r = (ASTTree *)malloc(sizeof(ASTTree)); // FOR part 2
        q = q->r;
        q->type = FORPART2;
        q->data.data = NULL;
        q->l = Expression(SEMI);
        if (q->l == NULL)
        {
            q->data.data = "None";
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        q->r = (ASTTree *)malloc(sizeof(ASTTree)); // FOR part 3
        q = q->r;
        q->r = NULL;
        q->type = FORPART3;
        q->data.data = NULL;
        q->l = Expression(RP);
        if (q->l == NULL)
        {
            q->data.data = "None";
        }

        ASTTree *p2 = (ASTTree *)malloc(sizeof(ASTTree)); // FOR body
        p2->l = NULL;
        p2->r = NULL;
        p2->type = FORBODY;
        p2->data.data = NULL;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w == LB)
        {
            w = getToken(fp);
            while (w == ANNO || w == INCLUDE)
            {
                w = getToken(fp);
            }
            p2->r = StateList();
        }
        else if (w == INT_CONST || w == FLOAT_CONST || w == CHAR_CONST ||
                 w == IDENT || w == KEYWORD)
        {
            p2->r = Statement();
            p2->r->r = NULL;
        }
        else
        {
            printf("Error: Error in FOR\n");
            exit(0);
        }
        root->type = FORSTATEMENT;
        root->l = p1;
        root->r = p2;
        isInRecycle = 0;
        return root;
    }
    case RETURN:
    {
        hasReturn = 1;
        if (isVoid == 1)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: There should be no return statement\n");
            exit(0);
        }
        root->type = RETURNSTATEMENT;
        root->l = NULL;
        root->r = NULL;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        root->r = Expression(SEMI);
        return root;
    }
    case DO:
    {
        isInRecycle = 1;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != LB)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: mising bracket in do-while\n");
            exit(0);
        }

        ASTTree *p1 = (ASTTree *)malloc(sizeof(ASTTree));
        p1->type = DOWHILEBODY;
        p1->l = p1->r = NULL;

        ASTTree *p2 = (ASTTree *)malloc(sizeof(ASTTree));
        p2->type = DOWHILECONDITION;
        p2->l = p2->r = NULL;

        root->l = p1;
        root->r = p2;
        root->data.data = p1->data.data = p2->data.data = NULL;

        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        p1->l = StateList();

        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != WHILE)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: missing WHILE in do-while\n");
            freeTree(root);
            exit(0);
        }
        root->type = DOWHILESTATEMENT;
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != LP)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: missing condition in do-while\n");
            freeTree(root);
            exit(0);
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        p2->l = Expression(RP);
        if (p2->l == NULL)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: missing condition in do-while\n");
            exit(0);
        }
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != SEMI)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: mising semicolon in do-while\n");
            freeTree(root);
            exit(0);
        }
        isInRecycle = 0;
        return root;
    }
    case BREAK:
    {
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != SEMI)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: mising semicolon in BREAK\n");
            exit(0);
        }
        if (isInRecycle == 0)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: unexpected BREAK\n");
            exit(0);
        }
        root->type = BREAKSTATEMENT;
        return root;
    }
    case CONTINUE:
    {
        w = getToken(fp);
        while (w == ANNO || w == INCLUDE)
        {
            w = getToken(fp);
        }
        if (w != SEMI)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: mising semicolon in continue\n");
            exit(0);
        }
        if (isInRecycle == 0)
        {
            printf("Error in line %d\n", cnt_lines);
            printf("Error: unexpected continue\n");
            exit(0);
        }
        root->type = CONTINUESTATEMENT;
        return root;
    }
    case INT_CONST:
    case FLOAT_CONST:
    case CHAR_CONST:
    case IDENT:
    case ARRAY:
        return Expression(SEMI);
    }
    return root;
}

// 判断运算符优先级
char Precede(int c1, int c2)
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    if (c1 == PLUS || c1 == MINUS)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case RP:
        case POUND:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case ASSIGN:
            return '>';
        case TIMES:
        case DIVIDE:
        case LP:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        default:
            return '\0';
            break;
        }
    }
    else if (c1 == TIMES || c1 == DIVIDE)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case RP:
        case POUND:
        case TIMES:
        case DIVIDE:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
            return '>';
        case LP:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        default:
            return '\0';
        }
    }
    else if (c1 == LP)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        case RP:
            return '=';
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case POUND:
            return '>';
        default:
            return '\0';
        }
    }
    else if (c1 == RP)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case PLUSPLUS:
        case MINUSMINUS:
        case POUND:
            return '>';
        default:
            return '\0';
        }
    }
    else if (c1 == ASSIGN)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        case POUND:
            return '>';
        default:
            return '\0';
        }
    }
    else if (c1 == MORE || c1 == LESS || c1 == MOREEQUAL || c1 == LESSEQUAL)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        case RP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case POUND:
            return '>';
        default:
            return '\0';
        }
    }
    else if (c1 == EQ || c1 == NEQ)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        case RP:
        case EQ:
        case NEQ:
        case POUND:
            return '>';
        default:
            return '\0';
        }
    }
    else if (c1 == POUND)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case RP:
        case EQ:
        case NEQ:
        case ASSIGN:
        case PLUSPLUS:
        case MINUSMINUS:
            return '<';
        case POUND:
            return '=';
        default:
            return '\0';
        }
    }
    else if (c1 == PLUSPLUS || c1 == MINUSMINUS)
    {
        switch (c2)
        {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case LP:
        case MORE:
        case LESS:
        case MOREEQUAL:
        case LESSEQUAL:
        case EQ:
        case NEQ:
        case ASSIGN:
        case POUND:
            return '>';
        case RP:
            return '<';
        default:
            return '\0';
        }
    }
}

// 将token返回到文件流
void returnToken(int w, FILE *fp)
{
    int digit = strlen(token_text);
    int i;
    for (i = 0; i < digit; i++)
    {
        ungetc(token_text[digit - 1 - i], fp);
    }
}

// 显示节点类型
void showType(int type)
{
    switch (type)
    {
    case 1:
        printf("ExtDefS\n");
        break;
    case 2:
        printf("ExtDef\n");
        break;
    case 3:
        printf("ExtVarType\n");
        break;
    case 4:
        printf("ExtVarNameS\n");
        break;
    case 5:
        printf("ExtVarName\n");
        break;
    case 6:
        printf("FuncDef\n");
        break;
    case 7:
        printf("FuncRetTypr\n");
        break;
    case 8:
        printf("FuncName\n");
        break;
    case 9:
        printf("FuncParamS\n");
        break;
    case 10:
        printf("FuncPramaDef\n");
        break;
    case 11:
        printf("FuncPramaType\n");
        break;
    case 12:
        printf("FuncPramaName\n");
        break;
    case 13:
        printf("FuncBody\n");
        break;
    case 14:
        printf("LocalVarDefS\n");
        break;
    case 15:
        printf("LocalVarDef\n");
        break;
    case 16:
        printf("LocalVarType\n");
        break;
    case 17:
        printf("LocalVarNameS\n");
        break;
    case 18:
        printf("LocalVarName\n");
        break;
    case 19:
        printf("StatementS\n");
        break;
    case 20:
        printf("Operatee\n");
        break;
    case 21:
        printf("Operator\n");
        break;
    case 22:
        printf("Statement\n");
        break;
    case 23:
        printf("IF-ConditionStatement\n");
        break;
    case 24:
        printf("ELSE-StatementBody\n");
        break;
    case 25:
        printf("IF-Statement\n");
        break;
    case 26:
        printf("IF-ELSE-Statement\n");
        break;
    case 27:
        printf("WHILE-Statement\n");
        break;
    case 28:
        printf("WHILE-ConditionStatement\n");
        break;
    case 29:
        printf("WHILE-StatementBody\n");
        break;
    case 30:
        printf("FOR-Statement\n");
        break;
    case 31:
        printf("FOR-ConditionStatement\n");
        break;
    case 32:
        printf("FOR-StatementCondition1\n");
        break;
    case 33:
        printf("FOR-StatementCondition2\n");
        break;
    case 34:
        printf("FOR-StatementCondition3\n");
        break;
    case 35:
        printf("FOR-StatementBody\n");
        break;
    case 36:
        printf("RETURN-Statement\n");
        break;
    case 37:
        printf("BREAK-Statement\n");
        break;
    case 38:
        printf("DO-WHILE-Statement\n");
        break;
    case 39:
        printf("DO-WHILE-StatementBody\n");
        break;
    case 40:
        printf("DO-WHILE-Condition\n");
        break;
    case 41:
        printf("continueStatement\n");
        break;
    case 42:
        printf("FuncDeclare\n");
        break;
    case 43:
        printf("ArrDef\n");
        break;
    case 44:
        printf("ArrType\n");
        break;
    case 45:
        printf("ArrName\n");
        break;
    case 46:
        printf("ArrSize\n");
        break;
    }
}

// 前序遍历抽象语法树并输出
void PreorderTranverse(ASTTree *root, int depth)
{
    if (root == nullptr)
        printf("  ");
    else
    {
        int i;
        for (i = 0; i < depth; i++)
        {
            printf("  ");
        }
        showType(root->type);
        if (root->data.data != NULL)
        {
            for (i = 0; i < depth; i++)
            {
                printf("  ");
            }
            printf("%s\n", root->data.data);
        }
        PreorderTranverse(root->l, depth + 1);
        PreorderTranverse(root->r, depth + 1);
    }
}

// 添加名称到变量名列表中
int addName(char *token_text)
{
    if (haveMistake == 1)
    {
        return NULL;
    }
    int i, flag = 0;
    VDN *p = Vroot;
    while (p->next != NULL)
    {
        p = p->next; // 找到最后一个节点
    }

    for (i = 0; i < (p->size); i++)
    {
        if (strcmp(token_text, p->variable[i]) == 0) // 如果变量名已经存在
        {
            flag = 1;
            break;
        }
    }
    if (flag == 1) // 如果变量名已经存在
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Duplicate variable define\n");
        exit(0);
        haveMistake = 1;
        return 1;
    }
    char *savename = (char *)malloc(25 * sizeof(char));
    strcpy(savename, token_text);
    p->variable[p->size] = savename;
    p->size++;
    return flag;
}

// 检查变量名是否已在列表中声明
int checkname_exist(char *token_text)
{
    if (haveMistake == 1)
    {
        return NULL;
    }

    int i;
    int flag = 0;
    VDN *p = Vroot;
    while (p->next != NULL)
    {
        p = p->next;
    }
    for (i = 0; i < (p->size); i++)
    {
        if (strcmp(token_text, p->variable[i]) == 0)
        {
            flag = 1;
            break;
        }
    }
    for (i = 0; i < (Vroot->size); i++)
    {
        if (strcmp(token_text, Vroot->variable[i]) == 0)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("Error in line %d\n", cnt_lines);
        printf("Error: Using undeclared variable\n");
        exit(0);
        haveMistake = 1;
    }
    return flag;
}

// 释放抽象语法树
void freeTree(ASTTree *root)
{
    if (root)
    {
        freeTree(root->l);
        freeTree(root->r);
        free(root);
    }
}