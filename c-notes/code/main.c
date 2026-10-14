/*
 * 四则运算练习系统
 *
 * 功能：
 *   1. 练习模式 —— 无限出题，即时判对错；输入 EOF(Ctrl+Z / Ctrl+D) 结束
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

/* 数字范围（三档）：下标 0/1/2 对应 10 / 100 / 1000 */
static int ranges[] = {10, 100, 1000};
static const char *range_names[] = {"10以内", "100以内", "1000以内"};

/* 运算符符号与名称，下标 0..4 对应 + - * / % */
static const char op_symbols[] = "+-*/%";
static const char *op_names[] = {"加", "减", "乘", "除", "取余"};

/* 当前选中的范围下标，由 selectRange 设置 */
static int current_range;

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

/* 选择数字范围，返回 0..2 */
static int selectRange(void)
{
    int i, choice;
    printf("\n请选择数字范围：\n");
    for (i = 0; i < 3; i++)
        printf("%d. %s\n", i + 1, range_names[i]);
    for (;;) {
        printf("请选择(1-%d)：", 3);
        if (checkInput(&choice) == 0) {        /* EOF 直接退出 */
            printf("\n再见！\n");
            exit(0);
        }
        if (choice < 1 || choice > 3) {
            printf("选择无效，请重新输入。\n");
            continue;
        }
        return choice - 1;
    }
}

/* 生成一道题的两个操作数 a、b */
static void getNumbers(int *a, int *b, int op)
{
    int r = ranges[current_range];
    if (op == 3) {                          /* 除法：保证 a 能被 b 整除 */
        *b = rand() % (int)sqrt((double)r) + 1;
        *a = *b * (rand() % (r / *b) + 1);
    } else {
        *a = rand() % r + 1;
        *b = rand() % r + 1;
    }
}

/* 计算正确答案 */
static int calc(int a, int b, int op)
{
    switch (op) {
        case 0: return a + b;
        case 1: return a - b;
        case 2: return a * b;
        case 3: return a / b;
        case 4: return a % b;
    }
    return 0;
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

/* 练习模式：op 为 0..4 指定运算符，5 表示混合 */
static void practice(int op)
{
    current_range = selectRange();
    const char *title = (op == 5) ? "混合" : op_names[op];
    printf("\n=== %s %s ===\n", title, range_names[current_range]);

    int a, b, ans;
    for (;;) {
        int real = (op == 5) ? rand() % 5 : op;
        getNumbers(&a, &b, real);
        printf("%d %c %d = (", a, op_symbols[real], b);
        if (checkInput(&ans) == 0)
            break;                      /* EOF 退出练习 */
        int correct = calc(a, b, real);
        if (ans == correct)
            printf("正确！\n");
        else
            printf("错误，正确答案是 %d\n", correct);
    }
}

/* 测试模式：固定 10 题，每题 10 分 */
static void test(int op)
{
    current_range = selectRange();
    const char *title = (op == 5) ? "混合" : op_names[op];
    printf("\n=== %s %s ===\n", title, range_names[current_range]);

    int score = 0, total = 10, i, a, b, ans;
    for (i = 0; i < total; i++) {
        int real = (op == 5) ? rand() % 5 : op;
        getNumbers(&a, &b, real);
        printf("%d %c %d = (%d) ", a, op_symbols[real], b, i + 1);
        if (checkInput(&ans) == 0)
            break;
        int correct = calc(a, b, real);
        if (ans == correct) {
            printf("正确\n");
            score += 10;
        } else {
            printf("错误，正确答案是 %d\n", correct);
        }
    }
    printf("得分：%d/%d\n", score, total * 10);
}

int main(void)
{
    srand((unsigned)time(NULL));

    printf("=== 四则运算练习系统 ===\n");
    int choice, op;
    for (;;) {
        printf("1. 练习模式\n2. 测试模式\n请选择：");
        if (checkInput(&choice) == 0) {
            printf("\n再见！\n");
            return 0;
        }
        if (choice == 1) {
            for (;;) {
                showPracticeMenu();
                if (checkInput(&op) != 1)
                    break;              /* EOF 回主菜单 */
                if (op < 1 || op > 6) {
                    printf("选择无效。\n");
                    continue;
                }
                practice(op - 1);
            }
        } else if (choice == 2) {
            for (;;) {
                showTestMenu();
                if (checkInput(&op) != 1)
                    break;
                if (op < 1 || op > 6) {
                    printf("选择无效。\n");
                    continue;
                }
                test(op - 1);
            }
        } else {
            printf("选择无效，请输入 1 或 2。\n");
        }
    }
    return 0;
}
