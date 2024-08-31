#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc,char* argv[]){
	if(argc!=2){
		err(1,"args");
	}
	int pf[2];
	if(pipe(pf)==-1){
		err(1,"pipe");
	}
	const pid_t child=fork();
	if(child==-1){
		err(1,"fork");
	}
	if(child==0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("cat","cat",argv[1],NULL)==-1){
			err(1,"execlp");
		}
	}
	close(pf[1]);

	dup2(pf[0],0);
	if(execlp("sort","sort",NULL)==-1){
		err(1,"execlp");
	}
}

