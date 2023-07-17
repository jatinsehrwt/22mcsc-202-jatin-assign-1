# Files and Pipes 
A command line utility program for demonstrating various system calls for files and pipes. This program allows users to create, read, write and inspect regular file and named pipe in unix/ unix-like environment. 

## Executable

- Compiled and tested using GNU gcc compiler version 11.3.0 (Ubuntu 11.3.0-1ubuntu1~22.04.1) using zsh .
- Executable is at 
  ```bash
    ./bin/j29
  ``` 
- Automatic compilation can be done using
  ```bash
    make
  ```
- After compilation, to clean files use 
  ```bash
    make clean
  ```

## Usage

### Default Values

- For file/pipe
  1. Permissions: 0644 (-rw-r--r--)
  2. Buffer size: 4K or 4096
- For cursor
  1. Offset: 0 
  2. Whence: 0, i.e, Beginning.

### Example Scripts

```bash
# To create a file 
./j29 --file -c fileName [mode(in Octal)]

# To create a named pipe
./j29 --pipe -c pipeName [mode(in Octal)] 
# To create a named pipe with purpose(read/write)
./j29 --pipe {-r,-w} pipeName [mode(in Octal)]

# To Read the contents of a file
./j29 --file -r fileName BytesToRead [OffSet]
# To Write the contents to a file 
./j29 --file -w fileName BytesToWrite [Offset] [Whence(beg/curr/end){0, 1, 2}]

# To Get Stats of a file
./j29 --file -s fileName
# To Get Stats of a named pipe
./j29 --pipe -s pipeName

# To Copy contents of a file to another file using pipes
./j29 --copy sourceFile DestinationFile
```
- Note:
  - There will be exact matching of case and dashes
  - Dashed:      Required Commands
  - Non-dashed: Required Parameters
  - { }:        Required Choice Parameters
  - [ ]:        Optional Parameters

- Information about usage is printed for any other wrong command executed.

## Project structure

``` 
|- bin              <- Compiled binaries
|   |- j29          <- Project's main executable
|- src              <- Source files
|   |- include      <- Header files
|   |- modules      <- Modular Components 
|   |- main.c       <- Driver Code
|- tests            <- Unit tests

|- Makefile         <- Project makefile
|- README           <- Project README
```

## Authors
- Jatin Sehrawat (jatin.mcs22@cs.du.ac.in)

## Bibliography

- The Design of the UNIX Operating System, Maurice J. Bach
- Linux Manual Pages for System Calls and Functions: [man7](https://www.man7.org/linux/man-pages/index.html)

## References 

- [GitHub](https://github.com/jatinsehrwt/22mcsc202-jatin-assign-1)