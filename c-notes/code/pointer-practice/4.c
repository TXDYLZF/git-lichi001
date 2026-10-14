#include<stdio.h>

int my_strcmp(const char *a,const char *b);
int main(){
  printf("%d",my_strcmp("1","1"));
}
int my_strcmp(const char *a,const char *b){
    while(*a && *a == *b)
    {a++;
    b++;}
    return(unsigned char)*a - (unsigned char)*b;
}