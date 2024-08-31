#include<sys/stat.h>
#include<fcntl.h>
#include<err.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc,char*argv[]){
	if(argc!=2){
		errx(1,"One parameter only");
	}

	int pf[2];
	if(pipe(pf)==-1){
		err(1,"pipe");
	}

	pid_t child;
	child=fork();
	if(child==-1){
		err(1,"fork");
	}

	if(child==0){
		close(pf[0]);

		if(dup2(pf[1],1)==-1){
			err(1,"dup2");
		}


		close(pf[1]);

		execlp("cat","cat",argv[1],(char*)NULL);
		err(2,"execlp");
	}

	close(pf[1]);
	if(dup2(pf[0],0)==-1){
		err(1,"dup2");
	}
	close(pf[0]);

	execlp("sort","sort",(char*)NULL);
	err(2,"execlp");

	int status;
	wait(&status);

}
