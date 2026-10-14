/*
 * 四则运算练习系统 —— 结构体/枚举版
 *
 * 本文件是 D:\ruanjian\QQ\main 那个 Linux 二进制的"功能等价复刻"，
 * 但按 2026-10-08 课堂笔记（结构体 / 枚举 / 共用体 / 动态分配 / typedef）
 * 用 C 的自定义数据类型重新实现，便于交作业、展示今天所学。
 *
 * 功能与原程序一致：
 *   1. 练习模式 —— 无限出题，即时判对错；输入 EOF(Ctrl+Z) 结束
 *   2. 测试模式 —— 固定 10 题，每题 10 分，最后给出百分制得分
 * 每种模式可选：加法 / 减法 / 乘法 / 除法 / 取余 / 混合
 * 数字范围可选：10以内 / 100以内 / 1000以内
 *
 * 编译：gcc main.c -o main -lm
 * 运行：./main
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* =========================================================
 * 1. 枚举(enum) —— 对应笔记"枚举"一节
 *    用有意义的名字代替魔法数字 0/1/2...
 * ========================================================= */

/* 运算符类型：加 / 减 / 乘 / 除 / 取余 / 混合 */
typedef enum {
    OP_ADD = 0,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_MIXED
} OpType;

/* 数字范围档位：10以内 / 100以内 / 1000以内 */
typedef enum {
    RANGE_10 = 0,
    RANGE_100,
    RANGE_1000
} RangeLevel;

/* 主菜单选择 */
typedef enum {
    MENU_PRACTICE = 1,
    MENU_TEST = 2
} MenuChoice;

/* =========================================================
 * 2. 结构体(struct) —— 对应笔记"结构体"一节
 *    把相关的数据打包成一个复合类型
 * ========================================================= */

/* 一道题目：两个操作数 + 正确答案 + 运算符类型 */
typedef struct {
    int a;
    int b;
    int answer;
    OpType op;
} Question;

/* 运算符的配置：类型 + 符号 + 中文名（取代原程序零散的 op_symbols/op_names） */
typedef struct {
    OpType      op;
    char        symbol;
    const char *name;
} OpConfig;

/* 范围的配置：档位 + 上限 + 中文名（取代原程序的 ranges 数组） */
typedef struct {
    RangeLevel  level;
    int         max;
    const char *name;
} RangeConfig;

/* =========================================================
 * 3. 配置表（结构体数组）—— 相当于原程序的全局数组，但更整齐
 * ========================================================= */
static const OpConfig op_table[] = {
    {OP_ADD,   '+', "加"},
    {OP_SUB,   '-', "减"},
    {OP_MUL,   '*', "乘"},
    {OP_DIV,   '/', "除"},
    {OP_MOD,   '%', "取余"},
};
static const RangeConfig range_table[] = {
    {RANGE_10,   10,   "10以内"},
    {RANGE_100,  100,  "100以内"},
    {RANGE_1000, 1000, "1000以内"},
};
#define OP_COUNT   5   /* 具体运算符个数（不含"混合"） */
#define RANGE_COUNT 3  /* 范围档位数 */

/* =========================================================
 * 4. typedef + 共用体(union) —— 对应笔记"共用体/类型别名"一节
 *    说明：本程序逻辑不需要共用体（每道题的字段都要同时用），
 *    但为了满足课堂知识点展示，下面单独写一个演示函数，
 *    演示 union 多个成员共用同一段内存、节省空间的特性。
 * ========================================================= */
typedef union {
    int   as_int;     /* 整数答案 */
    float as_float;   /* 浮点答案（预留，扩展用） */
    char  as_text[20];/* 文本答案（预留，扩展用） */
} AnswerValue;

/* 演示共用体：同一块内存按不同类型解释，最后写的内容会"覆盖"前面的 */
static void demo_union(void)
{
#ifdef DEMO_UNION
    AnswerValue v;
    v.as_int = 10;
    printf("[union演示] 写入 int=10，读出 int=%d\n", v.as_int);
    v.as_float = 1.23f;
    printf("[union演示] 再写入 float=1.23，此时 int 视图变成 %d（内存被复用）\n", v.as_int);
#endif
}

/* =========================================================
 * 工具函数
 * ========================================================= */

/* 读取一行整数：成功返回 1 并把值写入 *out，遇到 EOF 或空行返回 0 */
static int checkInput(int *out)
{
    char buf[128];
    for (;;) {
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            return 0;                 /* EOF */
        char *p = strchr(buf, '\n');
        if (p) *p = '\0';
        if (strcspn(buf, " \t\r") == 0)
            return 0;                 /* 纯空白 / 空行 */
        if (sscanf(buf, "%d", out) == 1)
            return 1;
        printf("输入无效，请输入一个整数：");
    }
}

/* 选择数字范围，返回 RangeLevel（EOF 时直接退出程序） */
static RangeLevel selectRange(void)
{
    int i, choice;
    printf("\n请选择数字范围：\n");
    for (i = 0; i < RANGE_COUNT; i++)
        printf("%d. %s\n", i + 1, range_table[i].name);
    for (;;) {
        printf("请选择(1-%d)：", RANGE_COUNT);
        if (checkInput(&choice) == 0) {
            printf("\n再见！\n");
            exit(0);
        }
        if (choice < 1 || choice > RANGE_COUNT) {
            printf("选择无效，请重新输入。\n");
            continue;
        }
        return (RangeLevel)(choice - 1);
    }
}

/* 计算正确答案（先声明，供 getNumbers 调用） */
static int calc(int a, int b, OpType op);

/* 根据运算符 + 范围，生成一道题（写入 *q）。除法保证整除 */
static void getNumbers(Question *q, RangeLevel r)
{
    int max = range_table[r].max;
    if (q->op == OP_DIV) {
        q->b = rand() % (int)sqrt((double)max) + 1;
        q->a = q->b * (rand() % (max / q->b) + 1);
    } else {
        q->a = rand() % max + 1;
        q->b = rand() % max + 1;
    }
    q->answer = calc(q->a, q->b, q->op);
}

/* 计算正确答案 */
static int calc(int a, int b, OpType op)
{
    switch (op) {
        case OP_ADD: return a + b;
        case OP_SUB: return a - b;
        case OP_MUL: return a * b;
        case OP_DIV: return a / b;
        case OP_MOD: return a % b;
        default:     return 0;
    }
}

/* 把"混合"翻译成一个具体运算符 */
static OpType resolveOp(OpType op)
{
    return (op == OP_MIXED) ? (OpType)(rand() % OP_COUNT) : op;
}

static void showPracticeMenu(void)
{
    printf("\n=== 练习模式 ===\n");
    printf("1. 加法  2. 减法  3. 乘法\n");
    printf("4. 除法  5. 取余  6. 混合\n");
    printf("请选择运算符：");
}

static void showTestMenu(void)
{
    printf("\n=== 测试模式 ===\n");
    printf("1. 加法  2. 减法  3. 乘法\n");
    printf("4. 除法  5. 取余  6. 混合\n");
    printf("请选择运算符：");
}

/* 练习模式：无限出题，EOF 结束 */
static void practice(OpType op)
{
    RangeLevel r = selectRange();
    const char *title = (op == OP_MIXED) ? "混合" : op_table[op].name;
    printf("\n=== %s %s ===\n", title, range_table[r].name);

    Question q;
    for (;;) {
        q.op = resolveOp(op);
        getNumbers(&q, r);
        printf("%d %c %d = (", q.a, op_table[q.op].symbol, q.b);
        int ans;
        if (checkInput(&ans) == 0)
            break;                      /* EOF 退出练习 */
        if (ans == q.answer)
            printf("正确！\n");
        else
            printf("错误，正确答案是 %d\n", q.answer);
    }
}

/* 测试模式：固定 10 题，每题 10 分（用 malloc 动态分配题库） */
static void test(OpType op)
{
    RangeLevel r = selectRange();
    const char *title = (op == OP_MIXED) ? "混合" : op_table[op].name;
    printf("\n=== %s %s ===\n", title, range_table[r].name);

    const int total = 10;
    /* —— 动态分配(malloc)：对应笔记"动态分配"一节 —— */
    Question *qs = (Question *)malloc(total * sizeof(Question));
    if (qs == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    int score = 0, i, ans;
    for (i = 0; i < total; i++) {
        qs[i].op = resolveOp(op);
        getNumbers(&qs[i], r);
        printf("%d %c %d = (%d) ", qs[i].a, op_table[qs[i].op].symbol, qs[i].b, i + 1);
        if (checkInput(&ans) == 0) {
            free(qs);                   /* 用完释放，避免内存泄漏 */
            return;
        }
        if (ans == qs[i].answer) {
            printf("正确\n");
            score += 10;
        } else {
            printf("错误，正确答案是 %d\n", qs[i].answer);
        }
    }
    printf("得分：%d/%d\n", score, total * 10);

    free(qs);                           /* —— 释放(malloc 配套) —— */
}

int main(void)
{
    srand((unsigned)time(NULL));
    demo_union();                       /* 共用体演示（默认未开启，见上方 #ifdef） */

    printf("=== 四则运算练习系统 ===\n");
    int choice, op;
    for (;;) {
        printf("1. 练习模式\n2. 测试模式\n请选择：");
        if (checkInput(&choice) == 0) {
            printf("\n再见！\n");
            return 0;
        }
        if (choice == MENU_PRACTICE) {
            for (;;) {
                showPracticeMenu();
                if (checkInput(&op) != 1)
                    break;              /* EOF 回主菜单 */
                if (op < 1 || op > 6) {
                    printf("选择无效。\n");
                    continue;
                }
                practice((OpType)(op - 1));
            }
        } else if (choice == MENU_TEST) {
            for (;;) {
                showTestMenu();
                if (checkInput(&op) != 1)
                    break;
                if (op < 1 || op > 6) {
                    printf("选择无效。\n");
                    continue;
                }
                test((OpType)(op - 1));
            }
        } else {
            printf("选择无效，请输入 1 或 2。\n");
        }
    }
    return 0;
}
