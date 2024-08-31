#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<string.h>
#include<stdinth.>
#include<err.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

const int BUFF_SIZE=4096;

void echo(void);
void execute(const char* command);
void validate_argument(const char* arg, const char* message);

void validate_argument(const char* arg,const char* message){
	if(strlen(arg)>4){
		errx(5,"invalig %s" message);
	}
}

void echo(void){
	char c;
	int bytes_read;
	while((bytes_read=read(0,&c,sizeof(c)))==sizeof(c)){
		if(write(1,&c,sizeof(c))!=sizeof(c)){
			err(3,"could not write to stdout");
		}
	}

	if(bytes_read==-1){
		err(2,"couldnt read from stdin");

	}

}

void execute(const char* command){
	char buff[BUFF_SIZE],c;
	int bytes_read,index=0;

	while((bytes_read=read(0,&c,sizeof(c)))==sizeof(c)){
		if(c==' ' || c=='\n'|| c=='\t'){
			buff[index]='\0';
			index=0;
			if(strlen(buff)>0){
				validate_argument(buff,"string");
				const pid_t child=fork();
				if(child==-1){
					err(4,"could not fork");
				}
				if(child==0){
					if(execlp(command,buff,buff,(char*)NULL)==-1){
						err(5,"execlp");
					}


				}
				int status;
				if(wait(&status)==-1){

					err(6,"could not wait for child process");

				}
				if(!WIFEXITED(status)){
					errx(7,"Child didnt terminate normally");
				}
				if(WEXITSTATUS(status)!=0){
					errx(8,"child process finished with");
				}


			}
			else{
				buff[index]=c;
				index++;
			}
	}
	if(bytes_read==-1){
		err(2,"read");
	}

}


int main(int argc,char* argv[]){
	if(argc>2){
		errx(1,"invalid Arguments %s",argv[0]);
	}

	if(argc==2){
		validate_arguments(argv[1],"command");
		execute(argv[1]);
	}
	else{
		echo();
	}

}
