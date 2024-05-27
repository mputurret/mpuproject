/*
    작성자: 김나영
    C 프로그램의 기본 구조 소개
*/

#include <stdio.h>

void print_number(int);
int main(void)
{
    int num;
    num = 1;
    print_number(num);

    num = 3;
    print_number(num);

    return 0;
}

void print_number(int n)
{
    printf("%d\n",n);
}