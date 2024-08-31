#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        errx(1, "Need 1 arg - file name");
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        err(1, "opening %s", argv[1]);
    }

    uint32_t byte_count[256] = {0};  // Array to count each byte occurrence
    uint8_t byte;
    ssize_t read_size;

    // Read the file byte by byte and count occurrences of each byte
    while ((read_size = read(fd, &byte, sizeof(byte))) > 0) {
        byte_count[byte]++;
    }

    if (read_size == -1) {
        close(fd);
        err(1, "Error reading file");
    }

    // Reset file offset to the beginning
    if (lseek(fd, 0, SEEK_SET) == -1) {
        close(fd);
        err(2, "lseek");
    }

    // Write the sorted bytes back to the file
    for (int i = 0; i < 256; i++) {
        while (byte_count[i] > 0) {
            if (write(fd, &i, sizeof(uint8_t)) == -1) {
                close(fd);
                err(3, "Error writing to file");
            }
            byte_count[i]--;
        }
    }

    // Truncate the file to the new size (in case the new size is smaller)
    if (ftruncate(fd, lseek(fd, 0, SEEK_CUR)) == -1) {
        close(fd);
        err(4, "ftruncate");
    }

    close(fd);
    return 0;
}

