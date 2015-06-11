#include<stdlib.h>
int main(void)
{
   int ret ;
　　ret=system(“lsusb |awk '{if($4>"001:")print $4}'”);
    printf("the ret is %d\n");
　
}
