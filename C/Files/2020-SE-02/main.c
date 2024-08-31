#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[]){
	if(argc!=4){
		err(1,"args");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_WRONLY|O_TRUNC|O_CREAT,0644);
	if(fd1==-1){
		err(1,"read");
	}
	if(fd2==-1){
		err(1,"read");
	}
	if(fd3==-1){
		err(1,"read");
	}
	int rd;

	uint8_t byte;
	uint16_t data;
	while((rd=read(fd1,&byte,sizeof(byte)))>0){
		uint8_t check=0x80;
		while(check!=0){
			if(read(fd2,&data,sizeof(data))==-1){
				err(1,"read");
			}
			if((check&byte)==check){
				if(write(fd3,&data,sizeof(data))==-1){
					err(1,"write");
				}
			}
			check=check>>1;
		}

	}
	if(rd==-1){
		err(1,"read");
	}
}

