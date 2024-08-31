#include <fcntl.h>
#include <err.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc,char*argv[]){
	if(argc!=4){
		err(1,"argc");
	}

	char* word=argv[1];
	int fd1=open(argv[2],O_RDONLY);
	int fd2=open(argv[3],O_RDONLY);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}

	struct stat st;
	if(fstat(fd2,&st)==-1)
	{
		err(1,"fstat");
	}
	size_t l=0;
	size_t r=(st.st_size)/sizeof(uint32_t);

	while(l<=r){
		size_t m=l+(r-l)/2;
		char curr_word[64];
		int cnt=0;
		if(lseek(fd2,m*sizeof(uint32_t),SEEK_SET)==-1){
			err(1,"lseek");
		}
		uint32_t pos;
		if(read(fd2,&pos,sizeof(pos))==-1){
			err(1,"read");
		}

		if(lseek(fd1,pos*sizeof(char),SEEK_SET)==-1){
			err(1,"lseek");
		}
		char ch;
		if(read(fd1,&ch,sizeof(ch))==-1){
			err(1,"read");
		}
		if(ch!='\0'){
			err(1,"incorrect index");
		}

		while(cnt<64){
			if(read(fd1,&ch,sizeof(ch))==-1){
				err(1,"read");
			}
			if(ch=='\n'){
				curr_word[cnt]='\0';
				break;
			}
			curr_word[cnt]=ch;
			cnt++;
		}
		if(cnt==64){
			err(1,"incorrect format");
		}
		if(strcmp(curr_word,word)==0){
			while(ch!='\0'){
				if(read(fd1,&ch,sizeof(ch))==-1){
					err(1,"read");
				}
				dprintf(1,"%c",ch);
			}
			close(fd1);
			close(fd2);
			exit(0);
		}
		else if(strcmp(curr_word,word)<0){
			l=m+1;
		}
		else{
			r=m-1;
		}
	}
	dprintf(1,"Wordnotfound\n");
	close(fd1);
	close(fd2);

}
