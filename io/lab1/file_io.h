#ifndef FILE_IO_H
#define FILE_IO_H
struct file *file_open(const char *path, int flags, int rights);
void file_close(struct file *file);
int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
int file_sync(struct file *file);
extern int error_num;
#endif
