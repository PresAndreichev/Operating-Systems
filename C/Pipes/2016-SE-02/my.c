#include<string.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<stdint.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>


int main(int argc,char*argv[]){
	char buffer[1024];
	int rd;

	while((rd=read(1,buffer,sizeof(buffer)))>0){
		buffer[rd-1]='\0';
		if(strcmp(buffer,"exit")==0){
			exit(0);
		}
		const pid_t child=fork();
		if(child==-1){
			err(1,"fork");
		}
		if(child==0){
			if(execlp(buffer,buffer,(char*)NULL)==-1){
				err(1,"execlp");
			}

		}
		int status;
		if(wait(&status)==-1){
			err(1,"wait");
		}
		if(!WIFEXITED(status)){
			errx(1,"not exited");
		}
		if(WEXITSTATUS(status)!=0){
			errx(1,"wrong exit status");
		}

	}
	if(rd==-1){
		err(1,"read");
	}

}

