#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>
size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}

struct entry{
	uint32_t uid;
	uint32_t _;
	uint32_t __;
	uint32_t start_time;
	uint32_t end_time;
};

struct session{
	uint32_t uid;
	uint32_t time;
};

int main(int argc,char* argv[]){
	if(argc!=2){
		err(1,"args");
	}

	int fd=open(argv[0],O_RDONLY);
	if(fd==-1){
		err(1,"open");
	}

	size_t size=get_file_size(fd)/sizeof(struct entry);

	struct entry user;
	struct session sessions[16384];
	for(size_t i=0;i<size;i++){
		if(read(fd,&user,sizeof(user))==-1){
			err(1,"read");
		}
		sessions[i].uid=user.uid;
		sessions[i].time=user.end_time-user.start_time;
	}

	close(fd);

	uint32_t avg_time=0;
	for(size_t i=0;i<size;i++){
		avg_time+=sessions[i].time;
	}
	avg_time=ceil(avg_time/size);
	
	uint32_t dispersion=0;
	for(size_t i=0;i<size;i++){
		dispersion+=(sessions[i].time-avg_time)*(sessions[i].time-avg_time);
	}
	dispersion=ceil(dispersion/size);

	struct session longest[2048];
	size_t cnt=0;
	for(size_t i=0;i<size;i++){
		if(sessions[i].time>=dispersion){
			int exists=0;
			for(size_t j=0;j<cnt;j++){
				if(sessions[i].uid==longest[j].uid){
					longest[j].time=sessions[i].time;}
				exists=1;
				break;
			}
			if(exists==0){
				longest[cnt].uid=sessions[i].uid;
				longest[cnt].time=sessions[i].time;
				cnt++;
			}
		}
	}
	for(size_t i=0;i<cnt;i++){
		dprintf(1,"%u %u\n",longest[i].uid,longest[i].time);
	}
}
