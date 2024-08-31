#include<fcntl.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<stdio.h>
#include<err.h>
#include<unistd.h>


int main(void){
	int pf[2];
	if(pipe(pf)==-1){
		err(1,"pipe");
	}
	pid_t child1=fork();
	if(child1==-1){
		err(1,"fork");
	}

	if(child1==0){
		close(pf[0]);
		if(dup2(pf[1],1)==-1){
			err(1,"Dup2");
		}
		close(pf[1]);
		if(execlp("cut","cut","-d","-f","7","/etc/passwd",(char*)NULL)==-1){
			err(1,"execlp");
		}
	}
	close(pf[1]);
	int pf2[2];
	if(pipe(pf2)==-1){
		err(1,"pipe2");
	}
	pid_t child2=fork();
	if(child2==-1){
		err(1,"fork2");

	}
	if(child2==0){
		close(pf2[0]);
		if(dup2(pf[0],0)==-1){
			err(1,"dup2");
		}
		close(pf[1]);
		if(dup2(pf[1],1)==-1){
			err(1,"dup2");
		}
		close(pf[0]);
		if(execlp("sort","sort",NULL)==-1){
			err(1,"execlp");
		}
	}
	int pf3[2];
	if(pipe(pf3)==-1){
		err(1,"error pipe");
	}
	pid_t child3=fork();
	if(child3==-1){
		err(1,"child3");
	}
	if(child3==0){
		close(pf3[0]);
		dup2(pf2[0],0);
		close(pf2[0]);
		dup2(pf3[1],1);
		close(pf3[1]);
		if(execlp("uniq","uniq","-c",NULL)==-1){
			err(1,"exe");
		}


	}
	close(pf2[0]);
	close(pf3[1]);
	if(dup2(pf3[0],0)==-1){
		err(1,"dup2");
	}
	close(pf3[0]);
	if(execlp("sort","sort",NULL)==-1){
		err(1,"execlp");
	}
	for(int i=0;i<3;i++){
		wait(NULL);
	}
	return 0;


}
