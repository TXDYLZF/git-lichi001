#include<stdio.h>
int my_strlen(const char *s);

int main(){
    printf("%d\n",my_strlen("hello"));
    return 0;
 }

int my_strlen(const char *s){
    const char *p = s;
    while(*p)
    p++;
    return (int)(p - s);
}