#include <fcntl.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	if(argc!=3){
		err(1,"args");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_WRONLY|O_TRUNC|O_CREAT,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}

	int rd;
	uint8_t byte;
	
	while((rd=read(fd1,&byte,sizeof(byte)))>0){
		uint8_t check=0x80;
		uint16_t result=0;
		uint16_t transform_zero=0x4000;
		uint16_t transform_one=0x8000;
		while(check!=0){
			if((byte&check)==check){
					result=result|transform_one;
			}
			else{
				result=result|transform_zero;
			}
			check=check>>1;
			transform_zero=transform_zero>>2;
			transform_one=transform_one>>2;

		}
		uint16_t left=result&0xFF00;
		uint16_t right=result&0x00FF;
		left=left>>8;
		right=right<<8;
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
