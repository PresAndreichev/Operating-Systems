#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	if(argc!=3){
		err(1,"args");
	}
	int N=atoi(argv[1]);
	int D=atoi(argv[2]);

	int pf1[2];
	int pf2[2];

	if(pipe(pf1)==-1){
		err(1,"pipe");
	}
	if(pipe(pf2)==-1){
		err(1,"pipe");
	}
	int done=0;
	pid_t child=fork();
	if(child==-1){
		err(1,"child");
	}
	if(child==0){
		close(pf1[1]);
		close(pf2[0]);
		for(int i=0;i<N;i++){
			if(read(pf1[0],&done,sizeof(done))==-1){
				err(1,"read");
			}
			dprintf(1,"dont\n");
			if(write(pf2[1],&done,sizeof(done))==-1){
				err(1,"write");
			}

		}
		close(pf1[0]);
		close(pf2[1]);		
	}
	else{
		close(pf1[0]);
		close(pf2[1]);
		for(int i=0;i<N;i++){
			dprintf(1,"ding");
			if(write(pf1[1],&done,sizeof(done))==-1){
				err(1,"write");
			}
			if(read(pf2[0],&done,sizeof(done))==-1){
				err(1,"read");
			}
			sleep(D);
		}
		close(pf1[1]);
		close(pf2[0]);
	}

}
