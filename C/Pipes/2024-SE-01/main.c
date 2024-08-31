#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/wait.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define STATUS_CRASHED 42

int main(int argc,char* argv[]){
	if(argc!=4){
		errx(1,"bad Args");
	}
	char* endptr;
	int N=strtol(argv[2],&endptr,10);
	if(*argv[2]=='\0'||*endptr!='\0'){
		errx(1,"N must be an int");
	}
	int status=0;

	int urandom=open("/dev/urandom",O_RDONLY);
	if(urandom==-1){
		err(1,"/dev/urandom");
	}

	int null=open("/dev/null",O_WRONLY);
	if(null==-1){
		err(1,"/dev/null");
	}

	char buf[UINT16_MAX];
	for(int i=0;i<N;i++){
		int out=open(argv[3],O_RDWR|O_CREAT|O_TRUNC,0644);
		if(out==-1){
			err(1,"%s",argv[1]);
		}
		uint16_t to_write;
		if(read(urandom,&to_write,sizeof(to_write))==-1){
			err(1,"read from /dev/urandom");
		}
		if(read(urandom,&buf,to_write)!=to_write){
			err(1,"read from /dev/urandom");
		}
		if(write(out,&buf,to_write)==-1){
			err(1,"write to out");
		}
		if(lseek(out,0,SEEK_SET)==-1){
			err(1,"lseek to start");
		}

		pid_t pid=fork();
		if(pid==-1){
			err(1,"fork");
		}
		if(pid==0){
			close(urandom);
			dup2(out,0);
			close(out);
			dup2(null,1);
			dup2(null,2);
			close(null);
			execl(argv[1],argv[1],(char*)NULL);
			err(1,"exec");

		}
		close(out);
		int res;
		if(wait(&res)==-1){
			err(1,"wait");
		}
		if(WIFSIGNALED(res)){
			status=STATUS_CRASHED;
			break;
		}


	}
	if(status!=STATUS_CRASHED){
		if(truncate(argv[3],0)==-1){
			err(2,"%s",argv[3]);
		}
	}
	close(urandom);
	close(null);
	return status;
}
