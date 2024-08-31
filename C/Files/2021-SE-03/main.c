#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

uint32_t get_file_size(int);

uint32_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc!=3){
		err(1,"argc");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_WRONLY,O_TRUNC,O_CREAT,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}
	
	uint32_t size=get_file_size(fd1)/sizeof(uint16_t);

	if(dprintf(fd2,"arrN=%u",size)==-1){
		err(1,"dprintf");
	}

	uint16_t byte;

	for(uint32_t i=0;i<size-1;i++){
		if(read(fd1,&byte,sizeof(byte))==-1){
			err(1,"read");
		}
		uint16_t left=byte&0xFF00;
		uint16_t right=byte&0x00FF;
		left=left>>8;
		right=right<<8;
		byte=left|right;

		if(dprintf(fd2,"0x%x, ", byte)==-1){
			err(1,"write");
		}

	}
	
	if(read(fd1,&byte,sizeof(byte))==-1){
		err(1,"read");
	}
	if(dprintf(fd2,"0x%x ", byte)==-1){
		err(1,"dprintf");
	}



	close(fd1);
	close(fd2);
}
