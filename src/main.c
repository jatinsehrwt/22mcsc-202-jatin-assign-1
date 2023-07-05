#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

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
    if (mkfifo(pipeName, mode) == -1)
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
    int pipeFd = mkfifo(pipeName, 0666);
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
        int count = strlen(buff);
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

void getStats(const char *fileName)
{
    struct stat fileStat;
    if (stat(fileName, &fileStat) == -1)
    {
        perror("Error getting file stats");
        exit(1);
    }

    printf("File Stats:\n");
    printf("Size: %lld bytes\n", (long long)fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("Inode: %ld\n", fileStat.st_ino);
    printf("Owner UID: %u\n", fileStat.st_uid);
    printf("Group GID: %u\n", fileStat.st_gid);
    printf("Last Access Time: %s", ctime(&fileStat.st_atime));
    printf("Last Modification Time: %s", ctime(&fileStat.st_mtime));
    printf("Last Status Change Time: %s", ctime(&fileStat.st_ctime));
}

void copyFileUsingPipes(const char *sourceFile, const char *destinationFile)
{
    int sourceFd = open(sourceFile, O_RDONLY);
    if (sourceFd == -1)
    {
        perror("Error opening source file");
        exit(1);
    }

    int destFd = open(destinationFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1)
    {
        perror("Error opening destination file");
        exit(1);
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(sourceFd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten == -1)
        {
            perror("Error writing to destination file");
            exit(1);
        }
    }

    if (bytesRead == -1)
    {
        perror("Error reading from source file");
        exit(1);
    }

    printf("Successfully copied contents from '%s' to '%s'.\n", sourceFile, destinationFile);

    close(sourceFd);
    close(destFd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s --operation ...\n", argv[0]);
        printf("For File: %s --file {-c -r -w -s} fileName [mode(Octal)]\n", argv[0]);
        printf("For Pipe: %s --pipe {-c -r -w -s} pipeName [mode(Octal)]\n", argv[0]);
        printf("For Copy: %s --copy sourceFile DestinationFile\n", argv[0]);
        return 1;
    }

    const char *operation = argv[1];

    if (strcmp(operation, "--file") == 0)
    {
        const char *fileCommand = argv[2];

        if (strcmp(fileCommand, "-c") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --file -c fileName [mode(in Octal)]\n", argv[0]);
                return 1;
            }
            const char *fileName = argv[3];
            mode_t mode = 0644;
            if (argc == 5)
            {
                mode = strtol(argv[4], NULL, 8);
            }
            // printf("%d", mode);
            createFile(fileName, mode);
        }
        else if (strcmp(fileCommand, "-r") == 0)
        {
            if (argc < 5)
            {
                printf("Usage: %s --file -r fileName [BytesToRead] [OffSet]\n", argv[0]);
                return 1;
            }
            const char *fileName = argv[3];
            size_t bytesToRead = atoi(argv[4]);
            off_t offset = 0;
            if (argc >= 6)
            {
                offset = atoi(argv[5]);
            }
            readFromFile(fileName, bytesToRead, offset);
        }
        else if (strcmp(fileCommand, "-w") == 0)
        {
            if (argc < 5)
            {
                printf("Usage: %s --file -w fileName [BytesToWrite] [Offset] [Whence(beg/curr/end)]\n", argv[0]);
                return 1;
            }
            const char *fileName = argv[3];
            // const char *content = argv[4];
            size_t bytesToWrite = atoi(argv[4]);
            off_t offset = 0;
            int whence;
            if (argc >= 6)
            {
                offset = atoi(argv[5]);
            }
            if (argc >= 7)
            {
                whence = atoi(argv[6]);
            }
            writeToFile(fileName, bytesToWrite, offset, whence);
        }
        else if (strcmp(fileCommand, "-s") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --file -s fileName\n", argv[0]);
                return 1;
            }
            const char *fileName = argv[3];
            getStats(fileName);
        }
        else
        {
            printf("Invalid file command\n");
            return 1;
        }
    }
    else if (strcmp(operation, "--pipe") == 0)
    {
        const char *pipeCommand = argv[2];

        if (strcmp(pipeCommand, "-c") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --pipe -c pipeName [mode(in Octal)]\n", argv[0]);
                return 1;
            }
            const char *pipeName = argv[3];
            mode_t mode = 0644;
            if (argc >= 5)
            {
                mode = strtol(argv[4], NULL, 8);
            }
            createNamedPipe(pipeName, mode);
        }
        else if (strcmp(pipeCommand, "-r") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --pipe -r pipeName [mode(in Octal)]\n", argv[0]);
                return 1;
            }
            const char *pipeName = argv[3];
            mode_t mode = 0644;
            if (argc >= 5)
            {
                mode = strtol(argv[4], NULL, 8);
            }
            createNamedPipePurpose(pipeName, mode, 0);
        }
        else if (strcmp(pipeCommand, "-w") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --pipe -w pipeName [mode(in Octal)]\n", argv[0]);
                return 1;
            }
            const char *pipeName = argv[3];
            mode_t mode = 0644;
            if (argc >= 5)
            {
                mode = strtol(argv[4], NULL, 8);
            }
            createNamedPipePurpose(pipeName, mode, 1);
        }
        else if (strcmp(pipeCommand, "-s") == 0)
        {
            if (argc < 4)
            {
                printf("Usage: %s --pipe -s pipeName\n", argv[0]);
                return 1;
            }
            const char *pipeName = argv[3];
            getStats(pipeName);
        }
        else
        {
            printf("Invalid pipe command\n");
            return 1;
        }
    }
    else if (strcmp(operation, "--copy") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: %s --copy sourceFile DestinationFile\n", argv[0]);
            return 1;
        }
        const char *sourceFile = argv[2];
        const char *destinationFile = argv[3];
        copyFileUsingPipes(sourceFile, destinationFile);
    }
    else
    {
        printf("Invalid operation\n");
        return 1;
    }

    return 0;
}
