#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "./include/create.h"
#include "./include/finfo.h"
#include "./include/pipecp.h"
#include "./include/read.h"
#include "./include/write.h"







// All the argument parsing is done in main(), it finds --operation -suboperation and other parameters.

// Test of some success and failure definition are done using /tests folder.

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s --operation -suboperation ...\n", argv[0]);
        printf("For File: %s --file {-c -r -w -s} fileName\n", argv[0]);
        printf("  with '-c', Additional Parameters: ... [mode(in Octal)]\n");
        printf("  with '-r', Additional Parameters: ... BytesToRead [OffSet]\n");
        printf("  with '-w', Additional Parameters: ... BytesToWrite [OffSet] [Whence(beg/curr/end){0, 1, 2}]\n");
        printf("For Pipe: %s --pipe {-c -r -w -s} pipeName\n", argv[0]);
        printf("  with '-c' or '-r' or '-w', Additional Parameters: ... [mode(in Octal)]\n");
        printf("For Copy: %s --copy sourceFile DestinationFile\n\n", argv[0]);
        printf("{}: Required Choice, []: Optional \n");
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
                printf("Usage: %s --file -r fileName BytesToRead [OffSet]\n", argv[0]);
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
                printf("Usage: %s --file -w fileName BytesToWrite [Offset] [Whence(beg/curr/end){0, 1, 2}]\n", argv[0]);
                return 1;
            }
            const char *fileName = argv[3];
            // const char *content = argv[4];
            size_t bytesToWrite = atoi(argv[4]);
            off_t offset = 0;
            int whence = 0;
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
