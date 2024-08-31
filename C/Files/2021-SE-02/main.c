#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc,char*argv[]){
	if(argc!=3){
		err(1,"argc");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_WRONLY|O_TRUNC|O_CREAT,0644);

	if(fd1==-1){
		err(1,"read");
	}
	if(fd2==-1){
		err(1,"read");
	}

	int rd;
	uint16_t byte;

	while((rd=read(fd1,&byte,sizeof(byte)))>0){
		uint16_t check=0x8000;
		uint8_t result=0x00;
		uint8_t transform=0x80;

		while(check!=0){
			if((byte&check)==check){
				result=transform|result;
			}
			check=check>>2;
			transform=transform>>1;
		}
		uint8_t left=result&0xF0;
		uint8_t right=result&0x0F;
		left=left>>4;
		right=right<<4;
		result=left|right;
		if(write(fd2,&result,sizeof(result))==-1){
			err(1,"write");
		}
	}
	if(rd==-1){
		err(1,"read");
	}
	close(fd1);
	close(fd2);


}
