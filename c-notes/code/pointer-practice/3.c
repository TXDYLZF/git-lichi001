#include<stdio.h>
char *my_strcpy(char *dst,const char *src);
int main(){
    char buf[20];
    my_strcpy(buf,"lizhifan");
   printf("%s\n",buf);
}

char *my_strcpy(char *dst,const char *src){
    char *d = dst;
    while ((*d++ = *src++) != '\0');
    return dst;
}
