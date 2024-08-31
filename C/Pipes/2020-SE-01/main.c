#foo.c

#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sdtio.h>
#include <sys/stat.h>

int main(int argc,char* argv[]){
	if(argc!=2){
		errx(1,"ivanlid Arguments.%s",argv[1]);
	}

	const char* pipe_name="./pipe.txt";
	const char* file_name=argv[1];

	if(mkfifo(pipe_name)==-1){
		err(1,"mkfifo");
	}
	const pid_t child=fork();
	
	if(child==-1){
		err(1,"fork");
	}
	if(child==0){
		int fd;
		if((fd=open(pipe_name,O_WRONLY))==-1){
			err(8,"count not open %s",pipe_name);
		}

		if(dup2(fd,1)==-1){
			close(fd);
			err(3,"cound not dup2");
		}

		if(execlp("cat","cat",file_name,(char*)NULL))==-1){
			close(fd);
			err(3,"couldt not exec");
		}
	}
	int status;
	if(wait(&status)==-1){
		err(4,"cound not wait for status");
	}
	if(!WIFEXITED(status)){
		errx(5,"child did something wrong");
	}
	if(WEXITSTATUS(status)){
		errx(6,"child process finished with not status 0");
	}
	exit 0;
}



#bar.c
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>


int main(int argc,char* argv[]){
	if(argc!=2){
		errx(1,"bad args %s", argv[1]);
	}
	const char* pipe_name="./pipe.txt";
	const char* command=argv[1];

	if(mkfifo(pipe_name,0666)==-1){
		err(3,"coulnt not mkkiffo");
	}

	const pid_t child=fork();
	if(child==-1){
		err(2,"fork");
	}

	if(child==0){
		int fd;
		if((fd=open(pipe_name,O_RDONLY))==-1){
			err(8,"couldnt not open %s", pipe_name);
		}
		if(dup2(fd[1],1)==-1){
			close(fd);
			err(3,"could not dup2");
		}
		if(execlp(command,command,(char*)NULL)==-1){
			close(fd);
			err(3,"could not exec");
		}
	}
	int status;
	if(wait(&status)==-1){
		err(4,"child not finished");
	}
	if(!WIFEXITED(status)){
		errx(5,"child not terminate properly");
	}
	If(WEXITSTATUS(status)!=0){
		errx(6,"not finish with status 0");

	}
	if(unlink(pipe_name)==-1){
		err(7, "couldnt unlink");
	}
	exit(0);


}
