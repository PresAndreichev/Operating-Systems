#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>

struct header{
	uint32_t magic;
	uint8_t header_ver;
	uint8_t data_ver;
	uint16_t count;
	uint32_t _;
	uint32_t __;
}__attribute__((packed));

struct old_ver{
	uint16_t offset;
	uint8_t org_byte;
	uint8_t new_byte;
}__attribute__((packed));

struct new_ver{
	uint32_t offset;
	uint16_t org_byte;
	uint16_t new_byte;
}__attribute__((packed));

int main(int argc,char* argv[]){
	if(argc!=4){
		err(1,"argc");
	}

	int fd1=open(argv[1],O_RDONLY);
	int fd2=open(argv[2],O_RDONLY);
	int fd3=open(argv[3],O_WRONLY|O_CREAT|O_TRUNC,0644);

	if(fd1==-1){
		err(1,"open");
	}
	if(fd2==-1){
		err(1,"open");
	}
	if(fd3==-1){
		err(1,"open");
	}

	struct header header;
	if(read(fd1,&header,sizeof(header))==-1){
		err(1,"read");
	}
	if(header.magic!=0xEFBEADDE){
		err(1,"magic");
	}
	if(header.header_ver!=0x01){
		err(1,"header_version");
	}

	uint8_t byte;
	int rd=0;
	
	while((rd=read(fd2,&byte,sizeof(byte)))>0){
		if(write(fd3,&byte,sizeof(byte))==-1){
			err(1,"write");
		}
	}
	if(rd==-1){
		err(1,"read");
	}

	if(header.data_ver==0x00){
		for(uint16_t i=0;i<header.count;i++){
			struct old_ver entry;
			uint8_t ver_byte;
			if(read(fd1,&entry,sizeof(entry))==-1){
				err(1,"read");
			}
			if(lseek(fd2,entry.offset*sizeof(ver_byte),SEEK_SET)==-1){
				err(1,"lseek");
			}
			if(read(fd2,&ver_byte,sizeof(ver_byte))==-1){
				err(1,"read");
			}
			if(ver_byte!=entry.org_byte){
				err(1,"no mtach");
			}
			if(lseek(fd3,entry.offset*sizeof(ver_byte),SEEK_SET)==-1){
				err(1,"lseek");
			}
			if(write(fd3,&entry.new_byte,sizeof(entry.new_byte))==-1){
				err(1,"write");
			}


		
		}
	}
		else if (header.data_ver==0x01){
			for(uint16_t i=0;i<header.count;i++){
				struct new_ver entry;
				uint16_t ver_byte;
				if(read(fd1,&entry,sizeof(entry))==-1){
					err(1,"read");
				}
				if(lseek(fd2,entry.offset*sizeof(ver_byte),SEEK_SET)==-1){
					err(1,"lseek");
				}
				if(read(fd2,&ver_byte,sizeof(ver_byte))==-1){
					err(1,"read");
				}
				if(ver_byte!=entry.org_byte){
					err(1,"no match");
				}
				if(lseek(fd3,entry.offset*sizeof(ver_byte),SEEK_SET)==-1){
					err(1,"lseek");
				}
				if(write(fd3,&entry.new_byte,sizeof(entry.new_byte))==-1){
					err(1,"write");
				}
			}
		}
	close(fd1);
	close(fd2);
	close(fd3);

}

