#include <string.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	if(argc!=3){
		err(1,"argc");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}

	int rd;
	uint8_t byte;

	while((rd=(read(fd1,&byte,sizeof(byte))))>0){
		if(byte==0x55){
			uint8_t n;
			if(read(fd1,&n,sizeof(n))==-1){
				err(1,"read");
			}
			if(lseek(fd1,-2*sizeof(uint8_t),SEEK_CUR)==-1){
				err(1,"lseek");
			}
			uint8_t* buffer=malloc(n*sizeof(uint8_t));
			if(read(fd1,buffer,n*sizeof(uint8_t))==-1)
			{
				free(buffer);
				err(1,"read");
			}
			uint8_t checksum=0x00;
			for(uint8_t i=0;i<n-1;i++){
				checksum=checksum&buffer[i];
			}
			if(checksum==buffer[n-1]){
				if(write(fd2,buffer,n*sizeof(uint8_t))==-1){
					free(buffer);
					err(1,"write");
				}
			}
			free(buffer);
		}
	}
	if(rd==-1){
		err(1,"read");
	}
	close(fd1);
	close(fd2);

}
