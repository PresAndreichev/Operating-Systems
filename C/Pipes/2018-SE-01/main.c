#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <stdint.h>

int main(int argc,char* argv[]){
	if(argc !=2){
		err(1,"args");
	}

	int pf1[2];
	if(pipe(pf1)==-1){
		err(1,"pipe");
	}
	pid_t child1=fork();
	if(child1==-1){
		err(1,"fork");
	}
	if(child1==0){
		close(pf1[0]);
		dup2(pf1[1],1);
		if(execlp("find","find",argv[1],"-type","f","-printf","%TY-%Tm-%Td %TH:%TM %p\n",NULL)==-1){
			err(1,"exe");
		}
	}
	close(pf1[1]);
	int pf2[2];
	if(pipe(pf2)==-1){
		err(1,"pipe");
	}
	pid_t child2=fork();
	if(child2==-1){
		err(1,"fork");
	}
	if(child2==0){
		close(pf2[0]);
		dup2(pf1[0],0);
		dup2(pf2[1],1);
		if(execlp("sort","sort","-r",NULL)==-1){
			err(1,"exe");
		}
	
	}
	close(pf1[0]);
	close(pf2[1]);

	int pf3[2];
	if(pipe(pf3)==-1){
		err(1,"pipe");
	}
	pid_t child3=fork();
	if(child3==-1){
		err(1,"pipe");
	}
	if(child3==0){
		close(pf3[0]);
		dup2(pf2[0],0);
		dup2(pf3[1],1);
		
		if(execlp("head","head","-1",NULL)==-1){
			err(1,"exe");
		}
	}
	close(pf2[0]);
	close(pf3[1]);
	dup2(pf3[0],0);
	if(execlp("cut","cut","-d"," ","-f","3",NULL)==-1){
		err(1,"exe");
	}
}
