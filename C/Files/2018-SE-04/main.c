#include <fcntl.h>
#include <unistd.h>
#include <sytdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

size_t get_file_size(int);

size_t get_file_size(int fd){
	struct stat st;
	if(fstat(fd,&st)==-1){
		err(1,"fstat");
	}
	return st.st_size;
}


