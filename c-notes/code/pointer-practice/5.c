#include<stdio.h>
void reverse_str(char *s);
int main(){

    char s[] = "abc";   /* 必须是可写数组，不能传字面量 "abc"（只读区，写它段错误） */
    reverse_str(s);     /* void 函数必须单独成句，不能塞进 printf */
    printf("%s\n", s);  /* 打印反转后的结果 */
    return 0;
}

void reverse_str(char *s) {
    char *p = s, *q = s;
    while (*q) q++;   // q 走到 '\0'
    q--;              // 回退一格到最后一个有效字符
    while (p < q) {   // 双指针从两端往中间收
        char t = *p; *p++ = *q; *q-- = t;
    }
}
