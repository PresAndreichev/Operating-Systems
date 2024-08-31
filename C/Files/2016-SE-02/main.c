#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct pair{
	uint32_t pos;
	uint32_t offset;
};

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"Stat");
	}
	return st.st_size;
}

int main(int argc, char* argv[]){
	if(argc != 4){
		err(1,"args");
	}

	int fd1=open(argv[1],O_RDONLY);
	if(fd1==-1){
		err(1,"open");
	}
	
	size_t size=get_file_size(fd1);

	if(size%sizeof(struct pair)!=0){
		err(1, "incorrect file 1 size");
	}

	size/=sizeof(struct pair);
	struct pair* data=(struct pair*)malloc(size);

	for (size_t i=0;i<size;i++){
		if(read(fd1,&data[i].pos,sizeof(uint32_t))==-1){
			free(data);
			err(1,"read");
		}
		if(read(fd1,&data[i].offset,sizeof(uint32_t))==-1){
			free(data);
			err(1,"read");
		}

	}
	close(fd1);
	
	int fd2=open(argv[2], O_RDONLY);
	if(fd2==-1){
		free(data);
		err(1,"open");
	}
	int fd3=open(argv[3],O_WRONLY|O_TRUNC|O_CREAT, 0644);
	if (fd3==-1){
		free(data);
		err(1,"open");
	}
	
	for(size_t i=0;i<size;i++){
		if(lseek(fd2,0,SEEK_SET)==-1){
			free(data);
			err(1,"lseek");
		}
		for(size_t j=0;j<data[i].offset;j++){
			uint32_t temp;
			if(read(fd2,&temp,sizeof(temp))==-1){
				free(data);
				err(1,"read");
			}
			if(write(fd3,&temp,sizeof(temp))==-1){
				free(data);
				err(1,"write");
			}
		}

	}

	free(data);
	close(fd2);
	close(fd3);
}


