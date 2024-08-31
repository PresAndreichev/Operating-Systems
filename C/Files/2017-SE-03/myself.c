#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}

	return st.st_size;
}

struct triple{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));


int main(int argc,char* argv[]){
	
	if(argc!=4){
		errx(1,"wrong args");
	}

	int fd1=open(argv[1],O_RDONLY);
	if(fd1==-1){
		err(1,"open");
	}

	int fd2=open(argv[2],O_RDONLY);

	if(fd2==-1){
		close(fd1);
		err(1,"open");
	}

	size_t size=get_file_size(fd1);
	if(size%sizeof(struct triple)!=0){
		errx(1,"wrong size for patch.bin");
	}

	size_t count=size/sizeof(struct triple);
	int fd3=open(argv[3],O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(fd3==-1){
		err(1,"open");


	}
	for(size_t i=0;i<count;i++){
		struct triple das;
		if(read(fd1,&das,sizeof(das))==-1){
			err(1,"wrong read");
		}

		uint8_t* buffer=malloc(sizeof(uint8_t)*das.offset);
		if(read(fd2,&buffer,sizeof(buffer)-1)==-1){
			free(buffer);
			err(1,"read");
		}
		if(write(fd3,&buffer,sizeof(buffer))==-1){
			free(buffer);
			err(1,"write");
		}
		if(write(fd3,&das.new,sizeof(das.new))==-1){
			free(buffer);
			err(1,"write");
		}
		
		uint8_t byte;
		if(read(fd2,&byte,sizeof(byte))==-1){
			free(buffer);
			err(1,"read");
		}
		if(byte!=das.original){
			free(buffer);
			err(1,"wrong patch");
		}
		free(buffer);
	}
	close(fd1);
	close(fd2);
	close(fd3);

}
