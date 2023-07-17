#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void readFromFile(const char *fileName, size_t bytesToRead, off_t offset)
{
    int fd = open(fileName, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file for reading");
        exit(1);
    }

    if (lseek(fd, offset, SEEK_SET) == -1)
    {
        perror("Error seeking file");
        exit(1);
    }

    char *buffer = (char *)malloc(bytesToRead);
    ssize_t bytesRead = read(fd, buffer, bytesToRead);
    if (bytesRead == -1)
    {
        perror("Error reading from file");
        exit(1);
    }

    write(STDOUT_FILENO, buffer, bytesRead);

    close(fd);
    free(buffer);
}
