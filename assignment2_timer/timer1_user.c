#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define SLEEP _IOW(0,0,unsigned long)
int main()
{
    int fd;
    int ret_val;
    int x,y;
    char buffer[2];
    int z;
unsigned long m;
    fd=open("/dev/timer_device",0);
    if(fd<0)
    {
        printf("Timer Driver Open Failed\n");
        exit(-1);
    }
    printf("Enter 1-Read Timer 2-Sleep Mode\n");
    scanf("%d",&z);
    if(z==1)
    {
        ret_val=read(fd,buffer,2);
        if(ret_val>=0)
{
	x=(unsigned char)buffer[1];
	y=(unsigned char)buffer[0];
	x<<=8;
        printf("Timer Value: %d\n ",x+y);
}
        else
        printf("Timer Driver Read Failed\n");
    }
    else if(z==2)
    {
	printf("\nEnter sleep duration");
	scanf("%lu",&m);
        ret_val=ioctl(fd,SLEEP,m);
        if(ret_val>=0)
        printf("Timer sleep done\n");
        else
        printf("Timer Driver IOCTL Failed\n");
    }
    close(fd);
    return 0;
}
