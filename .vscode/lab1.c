#include <stdio.h>

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int main()
{
    int a = 10;
    int b = 20;

    swap(&a,&b);

    printf("a: %d\n",a);
    printf("b: %d\n",b);
    return 0;
}