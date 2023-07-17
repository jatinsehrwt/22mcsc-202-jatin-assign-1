#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void createFile(const char *fileName, mode_t mode)
{
    int fd = open(fileName, O_WRONLY | O_CREAT | O_EXCL, mode);
    if (fd == -1)
    {
        perror("Error creating file");
        exit(1);
    }
    else
    {
        printf("File Created Successfully !\n");
    }
    close(fd);
}

void createNamedPipe(const char *pipeName, mode_t mode)
{
    int pipeFd = mkfifo(pipeName, mode);
    if (pipeFd == -1)
    {
        perror("Error creating named pipe");
        exit(1);
    }
    else
    {
        printf("Pipe Created Successfully !");
    }
}

void createNamedPipePurpose(const char *pipeName, mode_t mode, int purpose)
{
    int pipeFd = mkfifo(pipeName, mode);
    if (pipeFd == -1)
    {
        perror("Error creating named pipe");
        exit(1);
    }
    else
    {
        if (purpose == 0) // Read Case
        {
            int fd = open(pipeName, O_RDONLY);
            if (fd == -1)
            {
                perror("Error creating named pipe");
                exit(1);
            }

            char buff[4096];
            ssize_t bytesRead = read(fd, buff, sizeof(buff));
            if (bytesRead == -1)
            {
                perror("Error reading from named pipe");
                exit(1);
            }

            printf("Data read from named pipe: %.*s\n", (int)bytesRead, buff);
            close(fd);
        }

        else if (purpose == 1) // Write Case
        {
            int fd = open(pipeName, O_WRONLY);
            if (fd == -1)
            {
                perror("Error creating named pipe");
                exit(1);
            }

            char buff[4096];
            printf("Enter the message to write to the named pipe: ");
            fgets(buff, sizeof(buff), stdin);

            // Write data to the named pipe
            ssize_t bytesWritten = write(fd, buff, strlen(buff));
            if (bytesWritten == -1)
            {
                perror("Error writing to named pipe");
                exit(1);
            }

            printf("Data written to named pipe: %s\n", buff);
            close(fd);
        }
    }
}
