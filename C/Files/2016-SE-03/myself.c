#include<fcntl.h>
#include<sys/stat.h>
#include<err.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc!=2){
		errx(1,"Need 1 arg");
	}


	int fd=open(argv[0],O_RDONLY);
	if(fd1==-1){
		err(1,"open");
	}
	size_t size=get_file_size/sizeof(uint32_t);
	
	size_t size1=size/2;
	size_t size2=size1;

	if(size%2==1){
		size1++;
	}

	char temp_left[]="/tmp/tmp_left_XXXXXX";
	char temp_right[]="/tmp/tmp_right_XXXXXX";

	int fd1;
	int fd2;

	uint32_t* buffer=malloc(size1*sizeof(uin32_t));

	for(size_t i=0;i<size;i++){
		if(read(fd,&buufer[i],sizeof(buffer[i]))==-1){
			free(buffer);
			err(1,"read");
		}
	}

	for(size_t i=0;i<size2;i++){
		
}
