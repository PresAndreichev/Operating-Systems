#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

uint8_t get_size(int);

uint8_t get_size(int fd){
	uint8_t meta_data_8;
	uint16_t meta_data_16;
	uint32_t meta_data_32;
	uint64_t meta_data_64;
	uint8_t size;
	if(read(fd,&meta_data_16,sizeof(uint16_t))==-1){
		err(1,"read");
	}
	if(read(fd,&size,sizeof(uint8_t))==-1){
		err(1,"read");
	}
	if(read(fd,&meta_data_8,sizeof(uint8_t))==-1){
		err(1,"read");
	}
	if(read(fd,&meta_data_32,sizeof(uint32_t))==-1){
		err(1,"read");
	}
	if(read(fd,&meta_data_64,sizeof(uint64_t))==-1){
		err(1,"read");
	}
	return size;
}

typedef struct{
	uint16_t offset;
	uint16_t len;
}interval;

typedef struct{
	interval post;
	interval pre;
	interval in;
	interval suf;
}group;

int main(int argc,char* argv[]){
	if(argc!=7){
		err(1,"argc");
	}
	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_RDONLY);
	int fd4=open(argv[4],O_RDONLY);
	int fd5=open(argv[5],O_RDONLY);
	int fd6=open(argv[6],O_WRONLY|O_CREAT|O_TRUNC,0644);

	if(fd1==-1){
		err(1,"open");
	}

	if(fd2==-1){
		err(1,"open");
	}
	if(fd3==-1){
		err(1,"open");
	}
	if(fd4==-1){
		err(1,"open");
	}
	if(fd5==-1){
		err(1,"open");
	}
	if(fd6==-1){
		err(1,"open");
	}

	uint8_t size1=get_size(fd1);
	uint8_t size2=get_size(fd2);
	uint8_t size3=get_size(fd3);
	uint8_t size4=get_size(fd4);
	uint8_t size5=get_size(fd5);

	uint32_t* post_data=malloc(size2*sizeof(uint32_t));
	uint8_t* pre_data=malloc(size3*sizeof(uint8_t));
	uint16_t* in_data=malloc(size4*sizeof(uint16_t));
	uint64_t* suf_data=malloc(size5*sizeof(uint64_t));

	for(uint8_t i=0;i<size2;i++){
		if(read(fd2,&post_data[i],sizeof(post_data[i]))==-1){
			free(post_data);
			free(pre_data);
			free(in_data);
			free(suf_data);
			err(1,"read");
		}
	}
	close(fd2);
	for(uint8_t i=0;i<size3;i++){
		if(read(fd3,&pre_data[i],sizeof(pre_data[i]))==-1){
			free(post_data);
			free(pre_data);
			free(in_data);
			free(suf_data);
			err(1,"read");
		}
	}
	close(fd3);
	for(uint8_t i=0;i<size4;i++){
		if(read(fd4,&in_data[i],sizeof(in_data[i]))==-1){
			free(post_data);
			free(pre_data);
			free(in_data);
			free(suf_data);
			err(1,"read");
		}
	}
	close(fd4);
	for(uint8_t i=0;i<size5;i++){
		if(read(fd5,&suf_data[i],sizeof(suf_data[i]))==-1){
			free(post_data);
			free(pre_data);
			free(in_data);
			free(suf_data);
			err(1,"read");
		}
		
	}
	close(fd5);
	group entry;
	for(uint8_t j=0;j<size1;j++){
		if(read(fd1,&entry,sizeof(entry))==-1){
			free(post_data);
			free(pre_data);
			free(in_data);
			free(suf_data);
			err(1,"read");
		}
		for(uint16_t i=entry.post.offset;i<entry.post.offset+entry.post.len;i++){
			if(write(fd6,&post_data[i],sizeof(post_data[i]))==-1){
				free(post_data);
				free(pre_data);
				free(in_data);
				free(suf_data);
				err(1,"write");
			}
		}

		for(uint16_t i=entry.pre.offset;i<entry.pre.offset+entry.pre.len;i++){
			if(write(fd6,&pre_data[i],sizeof(pre_data[i]))==-1){
				free(post_data);
				free(pre_data);
				free(in_data);
				free(suf_data);
				err(1,"write");
			}
		}
		for(uint16_t i=entry.in.offset;i<entry.in.offset+entry.in.len;i++){
			if(write(fd6,&in_data[i],sizeof(in_data[i]))==-1){
				free(post_data);
				free(pre_data);
				free(in_data);
				free(suf_data);
				err(1,"write");
			}
		}
		for(uint16_t i=entry.suf.offset;i<entry.suf.offset+entry.suf.len;i++){
			if(write(fd6,&suf_data[i],sizeof(suf_data[i]))==-1){
				free(post_data);
				free(pre_data);
				free(in_data);
				free(suf_data);
				err(1,"write");
			}
		}
	}
	close(fd1);
	close(fd2);
	free(post_data);
	free(pre_data);
	free(in_data);
	free(suf_data);
}
