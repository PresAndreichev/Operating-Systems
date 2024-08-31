#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stdio.h>

struct data{
	uint16_t offset;
	uint8_t old_byte;
	uint8_t new_byte;
};

uint16_t get_file_size(int);

uint16_t get_file_size(int fd){
	struct stat st;
	if (fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc!=4){
		err(1,"args");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_WRONLY|O_TRUNC|O_CREAT,0644);

	if(fd1==-1 || fd2==-1 || fd3==-1){
		err(1,"open");
	}
	uint16_t size1=get_file_size(fd1);
	uint16_t size2=get_file_size(fd2);
	
	if(size1!=size2){
		err(1,"size");
	}

	for(uint16_t i=0;i<size1;i++){
		uint8_t old_byte,new_byte;

		if(read(fd1,&old_byte,sizeof(old_byte))==-1){
			err(1,"read");
		}
		if(read(fd2,&new_byte,sizeof(new_byte))==-1){
			err(1,"read");
		}
		if(old_byte==new_byte){
			continue;
		}
		struct data entry;
		entry.offset=i;
		entry.old_byte=old_byte;
		entry.new_byte=new_byte;
		if(write(fd3,&entry,sizeof(entry))==-1){
			err(1,"write");
		}

	}

}
