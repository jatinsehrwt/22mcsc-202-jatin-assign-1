#ifndef WRITE_H
#define WRITE_H

void writeToFile(const char *fileName, size_t bytesToWrite, off_t offset, int whence);

#endif