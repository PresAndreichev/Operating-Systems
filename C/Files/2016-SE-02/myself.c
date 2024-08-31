#include<fcntl.h>
#include<string.h>
#include<err.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"bad size");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc!=4){
		errx(1,"need 4 args");
	}

	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);

	if(fd1==-1){
		err(1,"opening %s",argv[1]);
	}

	if(fd2==-1){
		close(fd1);
		err(1,"opening %s", argv[2]);
	}

	int fd3=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,0644);
	if(fd3==-1){
		close(fd1);
		close(fd2);
		err(1,"opening %s",argv[3]);
	}
	uint32_t size1=get_file_size(fd1);
	uint32_t size2=get_file_size(fd2);

	if(size1!=size2){
		close(fd1);
		close(fd2);
		close(fd3);
		errx(2,"one file got more numbers");
	}

	for(uint32_t i=0;i<size1;i++){
		uint32_t num1;
		uint32_t num2;
		if(read(fd1,&num1,sizeof(num1))==-1){
			close(fd1);
			close(fd2);
			close(fd3);
			err(1,"reading");
		}
		if(read(fd2,&num2,sizeof(num2))==-1){
			close(fd1);
			close(fd2);
			close(fd3);
			err(1,"reading");
		}

		while(num2!=0){
			if(write(fd3,&num1,sizeof(num1))==-1){
				close(fd1);
				close(fd2);
				close(fd3);
				err(1,"write");
			}
			num2--;
		}
	}
	close(fd1);
	close(fd2);
	close(fd3);
}
