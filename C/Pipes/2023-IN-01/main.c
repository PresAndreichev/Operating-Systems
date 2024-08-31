#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc,char* argv[]){
	if(argc!=3){
		err(1,"args");
	}
	int NC=atoi(argv[1]);
	int WC=atoi(argv[2]);
	const char* words[]={"tic ", "tac ", "toe\n"};

	int pf[14][2];
	for(int i=0;i<2*NC;i++){
		if(pipe(pf[i])==-1){
			err(1,"pipe");
		}
	}
	pid_t child[8];
	child[0]=1;
	for(int i=1;i<=NC;i++){
		child[i]=fork();
		if(child[i]==-1){
			err(1,"fork");
		}
		if(child[i]==0){
			for(int j=0;j<=NC;j++){
				if(i==j){
					continue;
				}
				child[j]=-1;
			}
			break;
		}
	}
	int done=0;

	for(int i=0;i<WC;i++){
		int cnt=i%(NC+1);
		if(child[cnt]==-1){
			continue;
		}
		else if (child[cnt]==0){
			if(read(pf[2*cnt-2][0],&done,sizeof(done))==-1){
				err(1,"read");
			}
			dprintf(1,"%s",words[i%3]);
			if(write(pf[2*cnt-1][1],&done,sizeof(done))==-1){
				err(1,"write");
			}

		}
		else{
			if(cnt==0){
				dprintf(1,"%s",words[i%3]);
			}
			else{
				if(write(pf[2*cnt-2][1],&done,sizeof(done))==-1){
					err(1,"write");
				}
				if(read(pf[2*cnt-1][0],&done,sizeof(done))==-1){
					err(1,"read");
				}
			}
		}


	}
}
