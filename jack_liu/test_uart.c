#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include <time.h>
#include <unistd.h>
#include <asm-generic/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
//#include <sys/ioctl.h>
#include <fcntl.h>
int set_opt1(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 
    newtio.c_lflag = ICANON;
    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}
int main(int argc, char **argv)
{
    int fd, nwrite=0,nread=0;
    
    char recvcmd[] = {0xAA,0x0F,0xF0,0x00,0x70,0x84,0x01,0x01,0x01,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x01,0x5E};
    char sendcmd[] = {0xAA 0x0B,0xF0,0x00,0x10,0x62,0x10,0x50,0x30,0x7F,0x1C,0x08,0x00,0x38};
    char buf[100];
    memset(buf,'\0',sizeof(buf));
    if((fd=open("/dev/ttySAC2",O_RDWR|O_NOCTTY))<0)
    {
        perror("open_port error");
        return;
    }
   if(fcntl(fd, F_SETFL, 0)<0)
    {
        printf("fcntl failed!\n");
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(isatty(STDIN_FILENO)==0)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty success!\n");
    }
    printf("fd-open=%d\n",fd);
    if((i=set_opt(fd,115200,8,'N',1))<0)
    {
        perror("set_opt error");
        return;
    }
    while(1)
	{
	   nread=read(fd,buf,sizeof(buf));
	   printf("nread=%d,%s\n",nread,buf);
	  
           nwrite=write(fd,sendcmd,sizeof(sendcmd));
	   printf("nwrite=%d,%s\n",nwrite,buf);
	}

 close(fd);
return 0;
}
