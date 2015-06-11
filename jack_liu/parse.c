#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char str1[]="console=ttymxc0,115200 nosmp ip=dhcp root=/dev/mmcblk0p1 rootwait partions=0x0000000000000000,0x0000000000080000,0x0000000000100000,0x0000000000180000,0x0000000000680000,0x0000000000980000,0x0000000000a00000,0x0000000000f00000,0x0000000001200000,-";
unsigned long long int parse_u64(const char *s, char **end)
{
	unsigned long long int ret ;
	unsigned int base;
    if( strlen(s) > 2 ) {
        if( s[0] == '0' ) {
            if( (s[1] == 'x') || (s[1] == 'X') ) {
                base = 16;
                s += 2;
            } else if( (s[1] == 'b') || (s[1] == 'B') ) {
                base = 2;
                s += 2;
            }
        }
    }
	ret=strtoull(s,NULL, base );
	return ret;

}

int main()
{
	  unsigned long long int ret;
	  char p1;
	  //memcpy(p1,str1,strlen(str1));
	  char *p = (char*)p1;
	  char *tmp =NULL;
	  char sep1[] ="0x";
	  char *sep2 =",";
	  tmp=strstr(p, sep1);
		p =tmp;
	  while((tmp[0]!='-')&&(tmp!=NULL)){
	  
	  tmp=strsep(&p, sep2);	
	  if(tmp[0]!='-'){
		ret=parse_u64(tmp,NULL);
		printf("the ret is 0x%llx\n",ret);
		}

	  printf("str:%s \n",tmp);
	}
	printf("the str1 is %s",str1);
	return 0;
}
