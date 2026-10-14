#include <stdio.h>
#include <stdlib.h>   /* qsort */

/* ---- 几个普通函数：返回 int，收两个 int ---- */
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

/* 回调示例：把“运算”当参数传进来 */
void apply(int a, int b, int (*f)(int, int)) {
    printf("apply 结果 = %d\n", f(a, b));
}

/* qsort 的比较函数：必须长成 int(*)(const void*, const void*) */
int cmp_int(const void *x, const void *y) {
    int a = *(const int *)x;
    int b = *(const int *)y;
    return a - b;   /* <0 前者小，0 相等，>0 前者大 */
}

int main(void) {
    /* 1) 函数名本身就是函数指针，能直接赋给函数指针变量 */
    int (*op)(int, int) = add;        /* op 指向 add */
    printf("op = add -> %d\n", op(3, 2));   /* 5 */

    op = sub;                         /* 随时换指向 */
    printf("op = sub -> %d\n", op(3, 2));   /* 1 */

    /* 2) 函数指针数组（命令分发表 / 状态机常用） */
    int (*table[3])(int, int) = {add, sub, mul};
    const char *name[3] = {"add", "sub", "mul"};
    for (int i = 0; i < 3; i++)
        printf("%s(4,2) = %d\n", name[i], table[i](4, 2));

    /* 3) 回调：把函数当参数传 */
    apply(10, 3, mul);                /* 通过函数指针调用 mul */

    /* 4) 标准库回调经典：qsort 用自定义比较函数排序 */
    int arr[] = {5, 2, 9, 1, 7};
    qsort(arr, 5, sizeof(int), cmp_int);
    printf("qsort 升序: ");
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
