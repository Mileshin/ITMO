#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h> /* put_user */
#include <linux/uaccess.h>
#include <linux/limits.h>
#include <linux/string.h>

#include "file_io.h"

#define ERROR_PARSING printk(KERN_ERR "VAR2: Command parsing error.\n"); return;

#define strtolong(str_num,number,base){              \
    char *endptr;                                    \
    number = simple_strtol(str_num,&endptr,base);    \
    if (endptr == NULL){                             \
        ERROR_PARSING                                \
    }                                                \
}

static dev_t first;
static struct cdev c_dev;
static struct class *cl;

static struct file *workf = NULL;
static char filename[PATH_MAX];
static ssize_t len_filename;

static char data[PATH_MAX + 8];
static ssize_t len_data = sizeof(data);

static int my_open(struct inode *i, struct file *f){
  printk(KERN_INFO "VAR2: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f){
  printk(KERN_INFO "VAR2: close()\n");
  return 0;
}


static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off){
    size_t n;

    if (workf != NULL) {
        vfs_llseek(workf, 0, SEEK_SET);
        n = kernel_read( workf, data, PATH_MAX, &workf->f_pos );
        vfs_llseek(workf, 0, SEEK_END);
        if( n < 0) {
            printk( "VAR2: kernel_read failed\n" );
            return -EIO;
        }
    } else {
        printk(KERN_ERR "VAR2: File not opened.\n");
    }
    len_data = n;

    /* If position is behind the end of a file we have nothing to read */
    if (*off >= len_data)
        return 0;
    /* If a user tries to read more than we have, read only as many bytes as we have */
    if( *off + len > len_data )
        len = len_data - *off;
    if (copy_to_user(buf, data + *off, len))
        return -EFAULT;
    /* Move reading position */
    *off += len;
    return len;
}

void pars_cmd(void){
    char *sep = " \t";  // I think that they were created by Satan.
    char *saveptr;
    char cmd[PATH_MAX + 8];
    strncpy(cmd,data,len_data-1);

    saveptr = cmd;                           // __________________________________________________
    char *token = strsep(&saveptr,sep);       // |    open        |   close   |   first number    |
    char *token1 = strsep(&saveptr,sep);      // |    namefile    |   NULL    |   arithmetic sign |
    char *token2 = strsep(&saveptr,sep);      // |    NULL        |   NULL    |   second number   |
    char *token3 = strsep(&saveptr,sep);      // |    NULL        |   NULL    |   NULL            |
                                              // --------------------------------------------------
    // close command //
    if ((strncmp(token,"close",strlen(token)-1)) == 0 && token1 == NULL){
        if (workf != NULL){
            file_close(workf);                                                          // close file
            workf = NULL;
            printk(KERN_INFO "VAR2: File %s closed.\n",filename);
        } else {
            printk(KERN_ERR "VAR2: Unable to close file. File not opened.\n");
        }
    } else
    // open command
    if ((strncmp(token,"open",strlen(token)-1)) == 0
            && token1 != NULL && token2 == NULL){
        if (workf == NULL){
            len_filename = strlen(token1);                                          // get filename
            strncpy(filename,token1,len_filename);
            workf = file_open(filename,O_CREAT|O_RDWR,0644);                        // open file
            if(workf == NULL) {
                printk(KERN_ERR "VAR2: Unable to open file. Error number %i.\n"
                       ,error_num);
            } else {
                printk(KERN_INFO "VAR2: File %s opened.\n",filename);
            }
        } else {
             printk(KERN_ERR "VAR2: Unable to open file. File already opened.\n");
        }
    } else
        // calculator //
        if(token3 == NULL) {
            long int first, second, ans;
            char sign;
            if (token1 != NULL)                          // get rid of gaps
                strcat(token,token1);
            if (token2 != NULL)
                strcat(token,token2);

            int off = strspn(token,"0123456789");
            token1 = token + off;                        // sing
            sign = *token1;

            token2 = token1+1;                           // second number
            off = strspn(token2,"0123456789");
            if (off < strlen(token2)){
                ERROR_PARSING
            }

            *token1 = '\0';                              // separate the number
            strtolong(token,first,10);
            strtolong(token2,second,10);

            switch ( sign ) {
                case '+':
                    ans = first + second;
                    break;
                case '-':
                    ans = first - second;
                    break;
                case '/':
                    ans = first / second;
                    break;
                case '*':
                    ans = first * second;
                    break;
                default:
                    ERROR_PARSING
                break;
            }
            //printk(KERN_ERR "VAR2: ans: %li",ans);
            sprintf(data, "%ld\n", ans);
            //printk(KERN_ERR "VAR2: %s",data);
            kernel_write(workf, data, strlen(data), &workf->f_pos);
        } else {
            ERROR_PARSING
        }
    return;
}


static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off){
    if (sizeof(data) <= *off)
        return 0;
    /* If a user tries to write more than we have, return an error */
    if (sizeof(data) - (size_t)*off < len)
        return -ENOSPC;
    if (copy_from_user(data + *off, buf, len))
        return -EFAULT;
    *off += len;
    len_data = len;
    pars_cmd();
    return len;
}

static struct file_operations mychdev_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  .write = my_write
};

static int __init ch_drv_init(void)
{
  printk(KERN_INFO "VAR2: Hello!\n");
  if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0)
  {
        return -1;
  }
  if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
  {
        unregister_chrdev_region(first, 1);
        return -1;
  }
  if (device_create(cl, NULL, first, NULL, "var2") == NULL)
  {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
  }
  cdev_init(&c_dev, &mychdev_fops);
  if (cdev_add(&c_dev, first, 1) == -1)
  {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
  }
  return 0;
}

static void __exit ch_drv_exit(void)
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "VAR2: Bye!!!\n");
}

module_init(ch_drv_init);
module_exit(ch_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mileshin Andrey");
MODULE_DESCRIPTION("Lab 1 kernel module.");
