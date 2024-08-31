#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

struct entry{
	char name[8];
	uint32_t offset;
	uint32_t length;
};

int main(int argc, char* argv[]){
	if(argc!=2){
		errx(1,"bad args");
	}

	int fd=open(argv[1],O_RDONLY);
	if(fd==-1){
		err(1,"opening %s", argv[1]);
	}

	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}

	if(st.st_size%sizeof(struct entry)!=0){
		errx(1,"bad size");
	}

	uint32_t size=st.st_size/sizeof(struct entry);
	int pf[8][2];
	for(uint32_t i=0;i<size;i++)
	{
		if(pipe(pf[i])==-1){
			err(1,"pipe");
		}
	}

	struct entry entr;

	for(uint32_t i=0;i<size;i++){
		if(read(fd,&entr,sizeof(entr))==-1){
			err(1,"read");
		}
		pid_t child=fork();
		if(child==-1){
			err(1,"fork");
		}
		if(child==0){
			close(pf[i][0]);
			int fd_temp=open(entr.name,O_RDONLY);
			if(fd_temp==-1){
				err(1,"open");
			}
			if(lseek(fd_temp,entr.offset,SEEK_SET)==-1){
				err(1,"lseek");
			}
			uint16_t xor=0x0000;
			uint16_t byte;
			for(uint32_t j=0;j<entr.length;j++){
				if(read(fd_temp,&byte,sizeof(byte))==-1){
					err(1,"read");
				}
				xor=xor^byte;
			}
			if(write(pf[i][1],&xor,sizeof(byte))==-1){
				err(1,"write");
			}
			close(pf[i][1]);
			exit(0);
		}

	}
	close(fd);
	uint16_t xor_sym=0x0000;
	uint16_t byte;
	for(uint32_t i=0;i<size;i++){
		close(pf[i][1]);
		if(read(pf[i][0],&byte,sizeof(byte))==-1){
			err(1,"read");
		}
		xor_sym=xor_sym^byte;
		close(pf[i][0]);
	}
	char buffer[100];
	snprintf(buffer,sizeof(buffer),"Result %dB\n", xor_sym);
}
