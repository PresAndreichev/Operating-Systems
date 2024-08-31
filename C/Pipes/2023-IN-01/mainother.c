#include<fcntl.h>
#include<string.h>
#include<err.h>
#include<sys/stat.h>
#include<stdint.h>
#include<sys/wait.h>
#include<stdio.h>


const char L[3][4]={"tic ","tac ", "toe\n" };

int main(int argc,char* argv[]){
	if(argc!=3){
		errx(1,"bad args");
	}
	
	int NC=atoi(argv[1]);
	int WC=atoi(argv[2]);

	if(NC<1||NC>7){
		errx(2,"children need to be 1-7");
	}
	if(WC<1||WC>35){
		errx(2,"words 1-35");
	}
	int pds[8][2];

	for(int i=0;i<NC+1;i++){
		if(pipe(pds[i])==-1){
			err(2,"creating pipe");
		}
	}

	int id=NC;
	pid_t pid=0;
	for(int i=0;i<NC;i++){
		pid=fork();
		if(pid==-1){
			err(4,"cannot fork");
		}
		if(pid==0){
			id=i;
			break;
		}
		
	}

	if(pid>0){
		for(int i=1;i<NC;i++){
			close(pds[i][0]);
			close(pds[i][1]);
		}
		close(pds[0][0]);
		close(pds[NC][0]);

	}
	else{
		for(int i=0;i<NC+1;i++){
			if(i==id){
				close(pds[i][1]);
				close(pds[i+1][0]);
				i++;
		
			}
			else{
				close(pds[i][0]);
				close(pds[i][1]);
			}
		
		}
	}

	int count=0;
	if(id==NC){
		if(write(1,L[count%3],4)==-1){
			err(2,"writing to stdout");
		}
		count++;
		if(write(pds[0][1],&count,sizeof(count))==-1){
			err(2,"writing to pipde %d", count);
		}

	}
	while(read(pds[id][0],&count,sizeof(count))){
		if(count==WC){
			break;}
		if(write(1,L[count%3],4)==-1){
			err(3,"writing to pipe");
		}
		count++;
		if(id==NC){
			if(write(pds[0][1],&count,sizeof(count))==-1)
			{
				err(3,"writing to pipe parent");
			}
			else{
				if(write(pds[id+1][1],&count,sizeof(count))==-1){
					err(3,"writing to pipe child");
				}
			}
		}
	}
	if(id==NC){
		close(pds[0][1]);
		cloes(pds[id][0]);
	}
	else{
		close(pds[id+1][1]);
		close(pds[id][0]);
	}
	return 0;
}
