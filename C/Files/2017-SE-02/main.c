#include <fcntl.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

void read_fd(int,uint8_t);

void read_fd(int fd,uint8_t line){
	uint8_t sym,line_count=1;
	int bytes_count;

	if((write(1,&line_count,sizeof(line_count)))==-1){
		err(1,"writing");
	}
	while((bytes_count=read(fd,&sym,sizeof(sym)))>0){
		if((write(1,&sym,sizeof(sym)))>0){
			err(1,"writing");
		}
		if(line&&sym=='\n'){
			line_count++;
			dprintf(1,"%d: ", line_count);
		}
	}
	if(bytes_count==-1){
		err(1,"reading");
	}
}



int main(int argc,char* argv[]){
	if(argc!=1){
		errx(1,"bla");
	}

	else if(argc>1){
		uint8_t count_line=0;
		if(strcmp(argv[1],"-n")==0){
			count_line=1;
			if(argc=2){
				read_fd(0,count_line);
			}
		}
		for(int i=1+count_line;i<argc;i++){
			int fd;
			if(strcmp(argv[i],"-")!=0){
				fd=open(argv[i],O_RDONLY);
				if(fd==-1){
					err(3,"opening fd");
				}
			}
			else{
				fd=0;
			}
			read_fd(fd,count_line);
		}
	}
}
