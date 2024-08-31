#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

const size_t SIZE=1024;

int main(void){
	char buffer[SIZE];
	int rd;

	while((rd=read(1,buffer,sizeof(buffer)))>0){
		buffer[rd-1]='\0';
		if(strcmp(buffer,"exit")==0){
			exit(0);
		}
		const pid_t child=fork();
		if(child==-1){
			err(1,"child");
		}
		if(child==0){
			if(execlp(buffer,buffer,NULL)==-1){
				err(1,"execlp");
			}
		}
		int status;
		if(wait(&status)==-1){	
			err(1,"wait");
		}

		if(!WIFEXITED(status)){
			err(1,"not exitted");
		}
		if(WEXITSTATUS(status)!=0){
			err(1, "wrong exit status");
		}
	}
	if(rd==-1){
		err(1,"read");
	}
}
