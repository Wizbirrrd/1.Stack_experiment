#include <iostream>
#include <math.h>

#define OK 1
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define OVERFLOW -1
#define ARGC 2
#define STACK_INIT_SIZE 50
#define SPACEINCREMENT 10

typedef int Status;
typedef char ElemType;
typedef double NumType;

Status Dot = FALSE;
Status Optrear = TRUE;
Status Numrear = FALSE;
Status DivERROR = FALSE;
Status OptERROR = FALSE;
// 设置状态变量，分别为小数点、非法连续运算符、连续数字输入、除法错误

typedef struct
{
    ElemType *base;
    ElemType *top;
    int Stacksize;
} OptStack;
typedef struct
{
    NumType *base;
    NumType *top;
    int Stacksize;
} NumStack;
// 定义操作符栈与数字栈

void InitOS(OptStack &S)
{
    S.base = (ElemType *)malloc(STACK_INIT_SIZE * sizeof(ElemType));
    if (!S.base)
    {
        exit(OVERFLOW);
    }
    S.top = S.base;
    S.Stacksize = STACK_INIT_SIZE;
}
void InitNS(NumStack &S)
{
    S.base = (NumType *)malloc(STACK_INIT_SIZE * sizeof(NumType));
    if (!S.base)
    {
        exit(OVERFLOW);
    }
    S.top = S.base;
    S.Stacksize = STACK_INIT_SIZE;
}
Status EmptyStack(OptStack S)
{
    if (S.top == S.base)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
Status OSPush(OptStack &S, ElemType e)
{
    if (S.base - S.base >= S.Stacksize)
    {
        S.base = (ElemType *)realloc(S.base, (S.Stacksize + SPACEINCREMENT) * sizeof(ElemType));
        if (!S.base)
        {
            exit(OVERFLOW);
        }
        S.top = S.base + S.Stacksize;
        S.Stacksize += SPACEINCREMENT;
    }
    *(S.top++) = e;
    return OK;
}
Status NSPush(NumStack &S, NumType e)
{
    if (S.base - S.base >= S.Stacksize)
    {
        S.base = (NumType *)realloc(S.base, (S.Stacksize + SPACEINCREMENT) * sizeof(NumType));
        if (!S.base)
        {
            exit(OVERFLOW);
        }
        S.top = S.base + S.Stacksize;
        S.Stacksize += SPACEINCREMENT;
    }
    *(S.top++) = e;
    return OK;
}
Status OSPop(OptStack &S, ElemType &e)
{
    if (S.top == S.base)
    {
        return ERROR;
    }
    e = *--S.top;
    return OK;
}
Status NSPop(NumStack &S, NumType &e)
{
    if (S.top == S.base)
    {
        return ERROR;
    }
    e = *--S.top;
    return OK;
}
// 实现栈的基本操作

NumType ConvertNUM(ElemType c)
{
    return c - '0';
}
// 将字符型变量转换成数字型以方便入栈
int PriorVal(ElemType Operator)
{
    if (Operator == '(')
    {
        return 0;
    }
    if (Operator == '+' || Operator == '-')
    {
        return 1;
    }
    if (Operator == '*' || Operator == '/')
    {
        return 2;
    }
    if (Operator == '^')
    {
        return 3;
    }
    else
    {
        return ERROR;
    }
}
// 判断操作符的优先级
NumType Calculate(NumType n1, NumType n2, ElemType Operator)
{
    switch (Operator)
    {
    case '+':
        return n2 + n1;
        break;
    case '-':
        return n2 - n1;
        break;
    case '*':
        return n2 * n1;
        break;
    case '/':
        if (n1 == 0)
        {
            DivERROR = TRUE;
            return ERROR;
        }
        return n2 / n1;
        break;
    case '^':
        return pow(n2, n1);
    default:
        OptERROR = TRUE;
        return ERROR;
    }
}
// 实现四则和幂运算

int main(int argc, char **argv)
{
    if (argc != ARGC)
    {
        printf("ERROR_01");
        return ERROR;
    } // 命令行参数检查
    OptStack OS;
    NumStack NS;
    InitOS(OS);
    InitNS(NS);
    NumType num_1;
    NumType num_2;
    ElemType RearOperator;

    char *Operation = argv[1];
    if (argc != 2)
    {
        printf("ERROR_01");
        return ERROR;
    }

    while (*Operation != '\0')
    {
        if (*Operation == '.')
        {
            if (!Dot)
            {
                Dot = TRUE;
            }
            else
            {
                printf("ERROR_02");
                return ERROR;
            }
        }
        // 进入小数点输入模式,直到再次读到运算符
        else if (*Operation >= '0' && *Operation <= '9')
        {
            if (Dot)
            {
                NSPop(NS, num_1);
                NSPush(NS, num_1 + ConvertNUM(*Operation) * pow(0.1, Dot));
                Dot++;
            }
            else if (Numrear == TRUE)
            {
                NSPop(NS, num_1);
                NSPush(NS, num_1 * 10 + ConvertNUM(*Operation));
            }
            else
            {
                NSPush(NS, ConvertNUM(*Operation));
                Numrear = TRUE;
            }

            Optrear = FALSE;
        }
        else
        {
            if (Optrear == TRUE && *Operation != '(')
            {
                printf("ERROR_02");
                return ERROR;
            }
            // 非法性检测
            if (!EmptyStack(OS))
            {
                OSPop(OS, RearOperator);
                if (*Operation == ')')
                {
                    while (RearOperator != '(')
                    {
                        NSPop(NS, num_1);
                        NSPop(NS, num_2);
                        NSPush(NS, Calculate(num_1, num_2, RearOperator));
                        if (EmptyStack(OS))
                        {
                            printf("ERROR_02");
                            return ERROR;
                        }
                        OSPop(OS, RearOperator);
                    }
                    Operation++;
                    continue;
                }
                else if (PriorVal(RearOperator) >= PriorVal(*Operation) && *Operation != '(')
                {
                    NSPop(NS, num_1);
                    NSPop(NS, num_2);
                    NSPush(NS, Calculate(num_1, num_2, RearOperator));
                    if (RearOperator == '^')
                    {
                        continue;
                    }
                    // 解决形如2*3^2+1的问题
                }
                else
                {
                    OSPush(OS, RearOperator);
                }
            }
            else if (*Operation == ')')
            {
                printf("ERROR_02");
                return ERROR;
            }
            OSPush(OS, *Operation);
            Numrear = FALSE;
            Optrear = TRUE;
            Dot = FALSE;
        }
        Operation++;
    }
    while (!EmptyStack(OS))
    {
        NSPop(NS, num_1);
        NSPop(NS, num_2);
        OSPop(OS, RearOperator);
        if (RearOperator == '(')
        {
            printf("ERROR_02");
            return ERROR;
        }
        NSPush(NS, Calculate(num_1, num_2, RearOperator));
    }
    if (OptERROR)
    {
        printf("ERROR_02");
        return ERROR;
    }
    if (DivERROR)
    {
        printf("ERROR_03");
        return ERROR;
    }
    NSPop(NS, num_1);
    printf("%s = %g\n", argv[1], num_1);
    return 0;
}