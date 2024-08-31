#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int get_file_size(int);

int get_file_size(int fd){
	struct stat st;
	
	if (fstat(fd, &st) == -1 ){
		err(1, "stat");
	}
	return st.st_size;
}

int main(int argc, char* argv[]){
	if (argc != 2){
		err(1, "args");
	}

	int fd=open(argv[1],O_RDONLY);
	if (fd == -1){
		err(1, "open");
	}
	int size = get_file_size(fd);

	uint8_t* data= (uint8_t*)malloc(size*sizeof(uint8_t));

	for(int i=0;i<size;i++){
		if(read(fd,&data[i],sizeof(uint8_t))==-1){
			free(data);
			err(1,"read");
		}
	}
	close(fd);
	
	for(int i=0;i<size;i++){
		for(int j=i+1;j<size;j++){
			if(data[i]<data[j]){
				uint8_t temp=data[i];
				data[i]=data[j];
				data[j]=temp;
			}
		}
	}
	fd=open(argv[1], O_WRONLY | O_TRUNC);
	if(fd==-1){
		free(data);
		err(1,"open");
	}
	if(write(fd,&data,size*sizeof(uint8_t))==-1){
		free(data);
		err(1,"write");
	}
	free(data);
}

