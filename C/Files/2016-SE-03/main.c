#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}

int main(int argc, char* argv[]){
	if(argc !=2){
		err(1,"args");
	}

	int fd=open(argv[1],O_RDONLY);
	if (fd==-1){
		err(1, "open");
	}

	size_t size=get_file_size(fd)/sizeof(uint32_t);

	size_t size1=size/2;
	size_t size2=size1;


	if(size%2==1){
		size1++;
	}
	char temp_left[]="/tmp/temp_left";
	char temp_right[]="tmp/temp_right";
	int fd1;
	int fd2;

	uint32_t* buffer=malloc(size1*sizeof(uint32_t));

	for(size_t i=0;i<size2;i++){
		if(read(fd,&buffer[i],sizeof(buffer[i]))==-1){
			free(buffer);
			err(1,"read");
		}
	}

	for(size_t i=0;i<size2;i++){
		for(size_t j=i+1;j<size2;j++){
			if(buffer[i]<buffer[j]){
				uint32_t temp=buffer[i];
				buffer[i]=buffer[j];
				buffer[j]=temp;
			}
		}

	}

	fd2=mkstemp(temp_right);
	if(fd2==-1){
		free(buffer);
		err(1,"temp");
	}
	if(write(fd2,buffer,size2*sizeof(uint32_t))==-1){
		free(buffer);
		err(1,"write");
	}
	free(buffer);
	close(fd);
	if(lseek(fd1,0,SEEK_SET)==-1){
		err(1,"lseek");
	}
	if(lseek(fd2,0,SEEK_SET)==-1){
		err(1,"lseek");
	}

	fd=open(argv[1],O_WRONLY | O_TRUNC);
	if(fd==-1){
		err(1,"Open");
	}

	uint32_t l;
	uint32_t r;
	if(read(fd1,&l,sizeof(l))==-1){
		err(1,"read");
	}
	if(read(fd2,&r,sizeof(r))==-1){
		err(1,"read");
	}
	size_t i=0;
	size_t j=0;

	while(i<size1 && j<size2){
		if(l>r){
			if(write(fd,&l,sizeof(l))==-1){
				err(1,"write");
			}
			i++;
			if(i==size1){
				continue;
			}
			if(read(fd1,&l,sizeof(l))==-1){
				err(1,"read");
			}
		}
		else{
			if(write(fd,&r,sizeof(r))==-1){
				err(1,"write");
			}
			if(j==size2){
				continue;
			}
			if(read(fd2,&r,sizeof(r))==-1){
				err(1,"read");
			}
		}
	}
	while(i<size1){
		if(write(fd,&l,sizeof(l))==-1){
			err(1,"write");
		}
		i++;
		if(i==size1){
			continue;
		}
		if(read(fd1,&l,sizeof(l))==-1){
			err(1,"read");
		}
	}
	while(j<size2){
		if(write(fd,&r,sizeof(r))==-1){
			err(1,"write");
		}
		j++;
		if(j==size2){
			continue;
		}
		if(read(fd2,&r,sizeof(r))==-1){
			err(1,"read");
		}
	}
	close(fd);
	close(fd1);
	close(fd2);

	if(unlink(temp_left)==-1){
		err(1,"del");
	}
	if(unlink(temp_right)==-1){
		err(1,"del");
	}
}
