
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define DEVICE0 "/dev/adxl_x"
#define DEVICE1 "/dev/adxl_y"
#define DEVICE2 "/dev/adxl_z"


int read_buf;  //Global variable

int main(){
int i,fd;
char ch;
printf("Select corresponding axis accordingly\nx=open and read,accelerometer x axis\ny=open and read,accelerometer y axis\nz=open and read,accelerometer z axis\nEnter command:");
scanf("%c",&ch);
switch(ch){
	case 'x':
		fd=open(DEVICE0,O_RDWR);
		read(fd,&read_buf,sizeof(read_buf));
		printf("ADXL_Xaxis=%d\n",read_buf);
		close(fd);
		break;
	case 'y':
		fd=open(DEVICE1,O_RDWR);
		read(fd,&read_buf,sizeof(read_buf));
		printf("ADXL_Yaxis=%d\n",read_buf);
		close(fd);
		break;
	case 'z':
		fd=open(DEVICE2,O_RDWR);
		read(fd,&read_buf,sizeof(read_buf));
		printf("ADXL_Zaxis=%d\n",read_buf);
		close(fd);
		break;
	default:
		printf("Command not recognized\n");
		break;
}
return 0;
}
