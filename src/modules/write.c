#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void writeToFile(const char *fileName, size_t bytesToWrite, off_t offset, int whence)
{
    int fd = open(fileName, O_WRONLY), byteWrite = 0;
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(1);
    }

    if (lseek(fd, offset, whence) == -1)
    {
        perror("Error seeking file");
        exit(1);
    }

    char buff[4096];
    printf("Enter data to write to file. Press Ctrl-D to finish Writing.\n");

    while (bytesToWrite > 0 && (fgets(buff, 4096, stdin) != NULL))
    {
        size_t count = strlen(buff);
        if (bytesToWrite < count)
            // if bytes left to write are less then bytes written by the user at terminal
            count = bytesToWrite;
        if ((write(fd, buff, count)) == -1)
        {
            perror("Error while writing");
            return;
        }
        bytesToWrite -= count;
        byteWrite += count; // maintaing the count of bytes written by the user
    }

    printf("\nSuccessfully written %d bytes to file.\n", byteWrite);

    close(fd);
}