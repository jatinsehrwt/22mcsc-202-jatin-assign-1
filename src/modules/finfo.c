#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

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
