#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

struct header{
	uint16_t magic;
	uint16_t filetype;
	uint32_t count;
}__attribute__((packed));

struct pair{
	uint16_t pos;
	uint16_t value;
}__attribute__((packed));

int main(int argc,char* argv[]){

	if(argc!=3){
		err(1,"argc");
	}

	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_WRONLY|O_TRUNC|O_CREAT,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}
	if(fd3==-1){
		err(1,"open");
	}
	
	struct header header_list;
	struct header header_data;
	struct header header_out;

	if(read(fd1,&header_list,sizeof(header_list))==-1){
		err(1,"read");
	}
	if(read(fd2,&header_data,sizeof(header_data))==-1){
		err(1,"read");
	}

	if(header_list.magic!=0x5A4D || header_data.magic!=0x5A4D){
		err(1,"wrong magic");
	}
	if(header_list.filetype!=1||header_data.filetype!=2){
		err(1,"filetype");
	}

	header_out.magic=0x5A4D;
	header_out.filetype=3;
	header_out.count=0;

	struct pair list_entry;

	for(uint32_t i=0;i<header_list.count/2;i++){
		if(read(fd1,&list_entry,sizeof(list_entry))==-1){
			err(1,"read");
		}
		if(list_entry.value>header_out.count){
			header_out.count=list_entry.value;
		}
	}
	header_out.count++;

	if(lseek(fd1,sizeof(header_list),SEEK_SET)==-1){
		err(1,"lseek");
	}

	if(write(fd3,&header_out,sizeof(header_out))==-1){
		err(1,"write");
	}

	for(uint32_t i=0;i<header_out.count;i++){
		uint64_t empty=0;
		if(write(fd3,&empty,sizeof(uint64_t))==-1){
			err(1,"write");
		}
	}
	for(uint32_t i=0;i<header_list.count/2;i++){
		if(read(fd1,&list_entry,sizeof(list_entry))==-1){
			err(1,"read");
		}
		if(lseek(fd2,sizeof(header_data)+list_entry.pos*sizeof(uint32_t),SEEK_SET)==-1){
			err(1,"lseek");
		}

		uint32_t data32;
		if(read(fd2,&data32,sizeof(data32))==-1){
			err(1,"read");
		}
		uint64_t data64=(uint64_t)data32;



		if(write(fd3,&data64,sizeof(data64))==-1){
			err(1,"read");
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);
}
