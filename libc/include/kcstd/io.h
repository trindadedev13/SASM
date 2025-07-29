#ifndef __KCSTD_IO_H__
#define __KCSTD_IO_H__

#include "kcstd/string.h"
#include "kcstd/syscalls.h"
#include "kcstd/types.h"

typedef struct {
  string filename;
  int fd;
} file;

extern file std_files[];

#define stdin (&std_files[0])
#define stdout (&std_files[1])
#define stderr (&std_files[2])

typedef char file_open_mode;

#define FILE_MODE_READ 0
#define FILE_MODE_WRITE 1
#define PERM_RWR_R_R 0644
#define AT_FDCWD -100

#define O_RDONLY 0
#define O_CREAT 0100   // 0x40
#define O_WRONLY 0001  // 0x1
#define O_TRUNC 01000  // 0x200

#define SEEK_SET 0
#define SEEK_END 2

// Writes a content(buffer) to a file
ssize_t write(int fd, const void* buf, size_t len);
ssize_t read(int fd, void* dest, size_t count);

// Finishes the program
void exit(int exit_code);

// Writes a char in stdout.
size_t put_char(char c);
// Writes a string in stdout.
size_t put_str(string s);

// Writes a int in stdout.
void print_int(int n);
// Writes a formatted string in stdout.
void printf(const string fmt, ...);

// File
file* file_open(string filename, file_open_mode mode);
size_t file_write(const file* f, const void* buf, size_t len);
size_t file_read(const file* f, void* dest, size_t count);
int64_t file_seek(const file* f, int64_t offset, int whence);
void file_close(file* fl);

#endif