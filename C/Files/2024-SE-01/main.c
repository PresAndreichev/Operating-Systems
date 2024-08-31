#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct Node{
	uint64_t next;
	char user_data[504];
}Node;


int main(int argc,char* argv[]){
	if(argc!=2){
		errx(1,"not enough args");
	}

	int fd=open(argv[1],O_RDWR);
	if(fd==-1){
		err(1,"%s",argv[1]);
	}
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"stat %s", argv[1]);
	}
	if(st.st_size%sizeof(Node)!=0){
		errx(1,"%s:bad file size %ld, expected multiple of %ld",argv[1],st.st_size,sizeof(Node));
	}
	char tempName[]="/tmp/tmp.XXXXXX";
	int temp=mkstemp(tempName);
	if(temp==-1){
		err(1,"temp File");
	}
	off_t currentElement=0;
	ssize_t readSize;

	do{
		if(lseek(fd,currentElement*sizeof(Node),SEEK_SET)==-1){
			err(1,"lseek");
		}
		Node node;
		readSize=read(fd,&node,sizeof(Node));
		if(readSize==-1){
			err(1,"read");
		}
		if(readSize<(ssize_t)sizeof(node)){
			errx(1,"unexpected EPF");
		}

		if(lseek(temp,currentElement*sizeof(Node),SEEK_SET)==-1){
			err(2,"lseek");
		}
		if(write(temp,&node,sizeof(Node))==-1){
			err(2,"write");
		}
		currentElement=node.next;
	}while(currentElement!=0);

	uint8_t zero=0;
	if(lseek(fd,0,SEEK_SET)==-1){
		err(3,"lseek");
	}

	for(off_t i=0;i<st.st_size;i++){
		if(write(fd,&zero,sizeof(zero))==-1){
			err(3,"write");
		}
	}
	if(lseek(fd,0,SEEK_SET)==-1){
		err(4,"lseek");
	}
	if(lseek(temp,0,SEEK_SET)==-1){
		err(5,"lseek");
	}
	uint8_t byte;

	while((readSize=read(temp,&byte,sizeof(byte)))>0){
		if(write(fd,&byte,sizeof(byte))==-1){
			err(4,"Write");
		}
	}
	if(readSize<0){
		err(5,"read");
	}
	close(fd);
	close(temp);

	if(unlink(tempName)==-1){
		warn("cant delete temp file");
	}
	return 0;

}
