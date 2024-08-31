#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <err.h>
int main (int argc, char* argv[]){
	if(argc !=3){
		err(1, "args");
	}
	int pf[2];
	if(pipe(pf)==-1){
		err(1,"Pipe");
	}
	pid_t child=fork();
	if(child==-1){
		err(1,"fork");
	}
	if(child==0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("cat","cat",argv[1],NULL)==-1){
			err(1,"exe");
		}

	}
	close(pf[1]);
	uint8_t empty_byte=0x55;
	uint8_t escape_byte=0x7d;
	int fd=open(argv[2],O_RDONLY|O_CREAT|O_TRUNC,0644);

	if(fd==-1){
		err(1,"open");
	}
	int rd;
	uint8_t byte;
	int status;

	if(wait(&status)==-1){
		err(1,"wait");
	}
	if(!WIFEXITED(status)){
		err(1,"not exited");
	}
	if(WEXITSTATUS(status)!=0){
		err(1,"not exited properly");
	}
	while((rd=read(pf[0],&byte,sizeof(byte)))>0){
		if(byte==empty_byte){
			continue;
		}
		else if (byte==escape_byte){
			if(read(pf[0],&byte,sizeof(byte))==-1){
				err(1,"read");
			}
			byte=byte^0x20;
		}
		if(write(fd,&byte,sizeof(byte))==-1){
			err(1,"write");
		}
	}	
	if(rd==-1){
		err(1,"read");
	}
	close(pf[0]);
	close(fd);
}
