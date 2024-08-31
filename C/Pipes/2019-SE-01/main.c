#include<time.h>
#include<stdlib.h>
#include<err.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<stdio.h>

int main(int argc,char* argv[]){
	if(argc<3){
		err(1,"args");
	}
	time_t seconds=atoi(argv[1]);
	char* Q=argv[2];
	char** args=argv+2;
	int log=open("run.log",O_WRONLY | O_CREAT | O_TRUNC,0644);
	int condition=0;

	while(condition<2){
		pid_t child=fork();
		if(child==-1){
			err(1,"fork");
		}
		if(child==0){
			close(log);
			if(execvp(Q,args)==-1){
				err(1,"exe");
			}
		}
		time_t time_start=time(NULL);
		int status;
		if(wait(&status)==-1){
			err(1,"wait");
		}
		time_t time_end=time(NULL);
		int exit_code;
		if(WIFEXITED(status)){
			exit_code=WEXITSTATUS(status);
		}
		else{
			exit_code=129;
		}
		if(dprintf(log,"%ld %ld %d", time_start, time_end, exit_code)==-1){
			err(1,"write");
		}
		if((exit_code!=0)||(time_end-time_start<seconds)){
			condition++;
		}
		else{
			condition=0;
		}
	}
	close(log);
}
