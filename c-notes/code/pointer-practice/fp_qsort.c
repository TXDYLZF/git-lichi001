#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    char name[16];
    int score;
};

/* 比较函数 1：按分数升序（回调交给 qsort 调） */
int cmp_score(const void *a, const void *b) {
    const struct student *p = a;
    const struct student *q = b;
    return p->score - q->score;   /* void* 先强转回具体类型再比 */
}

/* 比较函数 2：按姓名字典序 */
int cmp_name(const void *a, const void *b) {
    const struct student *p = a;
    const struct student *q = b;
    return strcmp(p->name, q->name);
}

void show(struct student *s, int n) {
    for (int i = 0; i < n; i++)
        printf("%-8s %d\n", s[i].name, s[i].score);
}

int main(void) {
    struct student cls[] = {
        {"Li", 88}, {"Wang", 72}, {"Zhang", 95}, {"Zhao", 60}
    };
    int n = sizeof(cls) / sizeof(cls[0]);

    qsort(cls, n, sizeof(struct student), cmp_score);
    printf("按分数升序:\n");
    show(cls, n);

    qsort(cls, n, sizeof(struct student), cmp_name);
    printf("按姓名字典序:\n");
    show(cls, n);

    return 0;
}
