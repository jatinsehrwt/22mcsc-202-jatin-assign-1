#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int copyFileUsingPipes(const char* sourceFilePath, const char* destinationFilePath) {
  // Create the unnamed pipe
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("Failed to create pipe.\n");
    exit(1);
  }

  // Fork a child process
  pid_t pid = fork();
  if (pid == -1) {
    perror("Failed to fork process.\n");
    exit(1);
  }

  if (pid == 0) {
    // Child process (reading from the pipe and writing to the destination file)
    close(pipefd[1]); // Close the write end of the pipe

    // Open the destination file for writing (create if it doesn't exist)
    int destinationFile = open(destinationFilePath, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (destinationFile == -1) {
      perror("Failed to open destination file.\n");
      exit(1);
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      ssize_t bytesWritten = write(destinationFile, buffer, bytesRead);
      if (bytesWritten != bytesRead) {
        perror("Failed to write to destination file.\n");
        close(destinationFile);
        exit(1);
      }
    }

    if (bytesRead == -1) {
      perror("Failed to read from pipe.\n");
      close(destinationFile);
      exit(1);
    }

    close(destinationFile);
    close(pipefd[0]); // Close the read end of the pipe in the child process
    exit(0);
  } else {
    // Parent process (reading from the source file and writing to the pipe)
    close(pipefd[0]); // Close the read end of the pipe

    // Open the source file for reading
    int sourceFile = open(sourceFilePath, O_RDONLY);
    if (sourceFile == -1) {
      fprintf(stderr, "Failed to open source file.\n");
      exit(1);
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(sourceFile, buffer, sizeof(buffer))) > 0) {
      ssize_t bytesWritten = write(pipefd[1], buffer, bytesRead);
      if (bytesWritten != bytesRead) {
        fprintf(stderr, "Failed to write to pipe.\n");
        close(sourceFile);
        exit(1);
      }
    }

    if (bytesRead == -1) {
      fprintf(stderr, "Failed to read from source file.\n");
      close(sourceFile);
      exit(1);
    }

    close(sourceFile);
    close(pipefd[1]); // Close the write end of the pipe in the parent process

    // Wait for the child process to finish
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      printf("File copied successfully.\n");
      exit(0);
    } else {
      printf("Child process failed.\n");
      exit(1);
    }
  }
}