#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"Fstat");
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
		errx(1,"need 4 args");
	}

	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);

	if(fd1==-1){
		err(1,"error opnening %s", argv[1]);
	}
	if(fd2==-1){
		err(1,"error opnening %s",argv[2]);
	}

	size_t size1=get_file_size(fd1);
	size_t size2=get_file_size(fd2);

	if(size1!=size2){
		close(fd1);
		close(fd2);
		errx(1,"wrong size of %s and %s",argv[1],argv[2]);
	}
	int fd3=open(argv[3],O_APPEND|O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(fd3==-1){
		err(1,"error opneing %s",argv[3]);
	}


	for(size_t i=0;i<size1;i++){
		uint8_t byte1;
		uint8_t byte2;
		if(read(fd1,&byte1,sizeof(byte1))==-1){
			err(1,"read");
		}
		if(read(fd2,&byte2,sizeof(byte2))==-1){
			err(1,"read");
		}
		if(byte1!=byte2){
			struct triple das;
			das.offset=i;
			das.original=byte1;
			das.original=byte2;
			if(write(fd3,&das,sizeof(das))==-1){
				err(1,"write");
			}
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);


}
