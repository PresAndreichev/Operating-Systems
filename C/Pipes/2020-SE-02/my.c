#include<fcntrl.h>
#include<stdlib.h>
#include<err.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/wait.h>

int main(int argc,char* argv[]){
	if(argc!=3){
		errx(1,"2 args needed");
	}

	int pf[2];
	if(pipe(pf)==-1){
		err(1,"pipe");
	}
	pid_t child=fork();
	if(child==-1){
		err(2,"fork");
	}
	if(child==0){
		close(pf[0]);

		if(dup2(pf[1],1)==-1){
			err(1,"dup2 went wrong");
		}
		if(execlp("cat","cat",argv[1],NULL)==-1){
			err(4,"exec");
		}


	}
	close(pf[1]);
	int fd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(fd==-1){
		err(5,"open");
	}
	uint8_t xor_byte=0x20;
	uint8_t byte;
	uint8_t escape=0x7D;
	while((byte=read(pf[0],&byte,sizeof(byte)))>0){
		if(byte==escape){
			uint8_t next_byte;
			if(read(pf[0],&next_byte,sizeof(next_byte))==-1)
			{
				err(6,"read");
			}
			uint8_t new=next_byte^xor_byte;
			if(new==0x00||new==0xFF||new==0x55||new==0x7D){
				if(write(fd,&new,sizeof(new))==-1){
					err(7,"write");
				}
				else{
					if(write(fd,&next_byte,sizeof(next_byte))==-1){
						err(8,"write");
					}
				}
			}
		}
		else if(byte==0x55){
			continue;
		}
		else{
			if(write(fd,&byte,sizeof(byte))==-1){
				err(10,"write");
			}
		}

	}
	if(byte==-1){
		err(11,"read");
	}

	close(fd);
	exit(0);
}
