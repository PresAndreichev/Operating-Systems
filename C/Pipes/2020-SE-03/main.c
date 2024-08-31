#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>

struct triple{
	char name[8];
	uint32_t offset;
	uint32_t len;
};

int main(int argc,char* argv[]){
	if(argc!=2){
		err(1,"argc");
	}
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1){
		err(1,"open");
	}

	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	int size=st.st_size/sizeof(struct triple);
	int pf[8][2];
	for(int i=0;i<size;i++){
		if(pipe(pf[i])==-1){
			err(1,"pipe");
		}
	}

	struct triple entry;
	for(int i=0;i<size;i++){
		if(read(fd,&entry,sizeof(entry))==-1){
			err(1,"read");
		}
		pid_t child=fork();
		if(child==-1){
			err(1,"fork");
		}
		if(child==0){
			close(pf[i][0]);
			int fd_temp=open(entry.name,O_RDONLY);
			if(fd_temp==-1){
				err(1,"open");
			}
			if(lseek(fd_temp,entry.offset,SEEK_SET)==-1){
				err(1,"lseek");
			}
			uint16_t xor=0x0000;
			uint16_t byte;
			for(uint32_t j=0;j<entry.len;j++){
				if(read(fd_temp,&byte,sizeof(byte))==-1){
					err(1,"read");
				}
				xor=xor^byte;
			}
			if(write(pf[i][1],&xor,sizeof(xor))==-1){
				err(1,"write");
			}
			close(pf[i][1]);
			exit(0);
		}
	}
	close(fd);
	uint16_t xor_sym=0x0000;
	uint16_t byte;
	for(int i=0;i<size;i++){
		close(pf[i][1]);
		if(read(pf[i][0],&byte,sizeof(byte))==-1){
			err(1,"read");
		}
		close(pf[i][0]);
		xor_sym=xor_sym*byte;
	
	}
	dprintf(1,"result: %dB\n",xor_sym);
}
