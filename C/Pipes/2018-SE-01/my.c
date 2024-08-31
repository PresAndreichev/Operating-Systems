#include<fcntl.h>
#include<unistd.h>
#include<err.h>
#include<string.h>
#include<stdint.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>


int main(int argc,char*argv[]){
	if(argc!=2){
		errx(1,"Invalid arguments: %s",argv[1]);
	}

	const char* dir_name=argv[1];
	int pf[2];
	if(pipe(pf)==-1){
		err(1,"pipe");
	}

	pid_t child=fork();
	if(child==-1){
		err(1,"child");
	}
	if(child==0){
		close(pf[0]);
		if(dup2(pf[1],1)==-1){
			err(7,"could not dup2");
		}
		close(pf[1]);

		if(execlp("find","find",argv[1],"-mindepth","1","-type","f","-printf","%T@ %f\n",(char*)NULL)==-1){
			err(4,"execlp");
		}

	}
	close(pf[1]);
	int status;
	if(wait(&status)==-1){
		err(1,"child not return");
	}
	if(!WIFEXITED(status)){
		errx(1,"child not terminate proper");
	}
	if(WEXITSTATUS(status)){
		errx(1,"Child process exit status not 0");
	
	}

	int pf2[2];
	if(pipe(pf2)==-1){
		err(2,"Could not pipe");
	}

	if((child=fork())==-1){
		err(3,"could not fork");
	}
	if(child==0){
		close(pf2[0]);
		if(dup2(pf[0],0)==-1||dup2(pf2[1],1)==-1){
			err(7,"not dup2");
		}
		close(pf[0]);
		close(pf[1]);
		if(execlp("sort","sort","-nr",(char*)NULL)==-1){
			err(4,"could not execute");
		}

	}
	close(pf2[1]);
	close(pf[0]);

	int pf3[2];
	if(pipe(pf3)==-1){
		err(2,"could not pipe");
	}

	if((child=fork())==-1){
		err(3,"count not fork");
	}

	if(child==0){
		close(pf3[0]);
		if(dup2(pf2[0],0)==-1||dup2(pf3[1],1)==-1){
			err(7,"could not dup2");
		}
		close(pf2[0]);
		close(pf3[1]);
		
		if(execlp("head","head","-n1",(char*)NULL)==-1){
			err(4,"could not execute head");
		}
	}
	
	int status1;
	if(wait(&status)==-1){
		err(1,"dont wait");
	}
	if(!WIFEXITED(status)){
		errx(2,"blabla");
	}
	if(WEXITSTATUS(status)){
		errx(3,"blabla");
	}


	if(dup2(pf3[0],0)==-1){
		err(7,"Could not dup2");
	}

	if(execlp("cut","cut","-f2","-d"," ",(char*)NULL)==-1){
		err(4,"coult not execute cut");
	}
}
