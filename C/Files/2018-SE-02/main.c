#include <stdio.h>
#include <sys/stat.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"stat");
	}
	return st.st_size;
}

int main(int argc,char* argv[]){
	if(argc !=2){
		err(1,"args");
	}
	
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1){
		err(1,"open");
	}
	size_t size=get_file_size(fd);
	if(size%sizeof(uint32_t)!=0){
		err(1,"size");
	}

	size/=sizeof(uint32_t);
	size_t size_l=size/2;
	size_t size_r=size_l;
	if(size_r%2==1){
		size_l++;
	}

	char template_l[]="/tmp/temp_left.XXXXXX";
	char template_r[]="/tmp/temp_right.XXXXXX";

	int fd_1=mkstemp(template_l);
	int fd_2=mkstemp(template_r);

	uint32_t* buffer=malloc(size_l*sizeof(int32_t));

	if(read(fd,buffer,size_l*sizeof(uint32_t))==-1){
		free(buffer);
		err(1,"read");
	}

	for(size_t i=0;i<size_l;i++){
		for(size_t j=i+1;j<size_l;i++){
			if(buffer[i]<buffer[j]){
				uint32_t temp=buffer[i];
				buffer[i]=buffer[j];
				buffer[j]=temp;
			}
		}
	}

	if(write(fd_1,buffer,size_l*sizeof(uint32_t))==-1){
		free(buffer);
		err(1,"write");
	}
	free(buffer);

	buffer=malloc(size_r*sizeof(int32_t));

	if(read(fd,buffer,size_r*sizeof(uint32_t))==-1){
		free(buffer);
		err(1,"read");
	}

	for(size_t i=0;i<size_r;i++){
		for(size_t j=i+1;j<size_r;j++){
			if(buffer[i]<buffer[j]){
				uint32_t temp=buffer[i];
				buffer[i]=buffer[j];
				buffer[j]=temp;
			}
		}
	}
	if(write(fd_2,buffer,size_r*sizeof(uint32_t))==-1){
		free(buffer);
		err(1,"write");
	}
	free(buffer);
	
	if(lseek(fd,0,SEEK_SET)==-1){
		err(1,"lseek");
	}
	if(lseek(fd_1,0,SEEK_SET)==-1)
	{
		err(1,"lseek");
	}
	if(lseek(fd_2,0,SEEK_SET)==-1){
		err(1,"lseek");
	}
	
	uint32_t l,r;

	if(read(fd_1,&l,sizeof(l))==-1){
		err(1,"read");
	}

	if(read(fd_2,&r,sizeof(r))==-1){
		err(1,"read");
	}

	for(size_t i=0,j=0;i+j<size;){
		if((i==size_r)||(r>l)){
			if(write(fd,&r,sizeof(r))==-1){
				err(1,"write");
			}
			j++;
			if(j==size_r){
				continue;
			}
			if(read(fd_2,&r,sizeof(r))==-1){
				err(1,"read");
			}
		}
		else{
			if(write(fd,&l,sizeof(l))==-1){
				err(1,"write");
			}
			i++;
			if(i==size_l){
				continue;
			}
			if(read(fd_1,&l,sizeof(l))==-1){
				err(1,"read");
			}
		}
	}
	if(unlink(template_l)==-1){
		err(1,"unlink");
	}
	if(unlink(template_r)==-1){
		err(1,"unlink");
	}
	close(fd);
	close(fd_1);
	close(fd_2);

}
