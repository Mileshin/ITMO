#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#include "file_io.h"
int error_num;

struct file *file_open(const char *path, int flags, int rights){
    struct file *filp = NULL;
    mm_segment_t oldfs;
    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        error_num = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file){
    filp_close(file, NULL);
}

int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size){
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = kernel_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size){
        mm_segment_t oldfs;
        int ret;

        oldfs = get_fs();
        set_fs(get_ds());

        ret = kernel_write(file, data, size, &offset);

        set_fs(oldfs);
        return ret;
    }

int file_sync(struct file *file) {
    vfs_fsync(file, 0);
    return 0;
}
