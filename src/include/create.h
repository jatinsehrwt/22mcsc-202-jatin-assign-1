#ifndef CREATE_H
#define CREATE_H

void createFile(const char *fileName, mode_t mode);
void createNamedPipe(const char *pipeName, mode_t mode);
void createNamedPipePurpose(const char *pipeName, mode_t mode, int purpose);

#endif