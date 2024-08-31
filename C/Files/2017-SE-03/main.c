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

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"stat");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc!=4){
		err(1,"args");
	}

	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}
	if(fd3==-1){
		err(1,"open");
	}
	size_t size=get_file_size(fd1);
	if(size%sizeof(struct data)!=0){
		err(1,"size");
	}

	size/=sizeof(struct data);

	int rd;
	uint8_t byte;
	

	while((rd=(read(fd2,&byte,sizeof(byte))))>0){
		if(write(fd3,&byte,sizeof(byte))==-1){
			err(1,"write");
		}
	}

	if(rd==-1){
		err(1,"read");
	}

	close(fd2);
	
	struct data entry;

	for(size_t i=0;i<size;i++){
		if(read(fd1,&entry,sizeof(struct data))==-1){
			err(1,"read");
		}
		if(lseek(fd3,entry.offset,SEEK_SET)==-1){
			err(1,"lseek");
		}
		if(write(fd3,&entry.new_byte,sizeof(entry.new_byte))==-1){
			err(1,"write");
		}
	}

}
