#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int main(void) {
    /* 函数指针数组：下标 0~3 对应 + - * /，这就是“分发表” */
    int (*op[4])(int, int) = {add, sub, mul, divide};
    const char *sym[4] = {"+", "-", "*", "/"};

    int x = 20, y = 6;
    for (int i = 0; i < 4; i++)
        printf("%d %s %d = %d\n", x, sym[i], y, op[i](x, y));
    return 0;
}
