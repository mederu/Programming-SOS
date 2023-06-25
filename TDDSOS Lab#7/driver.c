#include "driver_doc.h"
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydriver"
#define DEVICE_MAGIC 'M'

#define IOCTL_SET_MSG      _IOW(DEVICE_MAGIC, 0, char*)
#define IOCTL_MSG_DELETE   _IO(DEVICE_MAGIC, 1)

static char message[256];

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "File opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "File closed\n");
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    if (copy_from_user(message, buffer, length)) {
        return -EFAULT;
    }
    printk(KERN_INFO "Data written: %s\n", message);
    return length;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    size_t data_len = strlen(message);
    if (copy_to_user(buffer, message, data_len)) {
        return -EFAULT;
    }
    printk(KERN_INFO "Data read: %s\n", message);
    return data_len;
}

static long ioctl_set_msg(struct file *file, unsigned long ioctl_param)
{
    const char *msg = (const char *)ioctl_param;
    size_t msg_len = strlen(msg);
    
    if (msg_len >= sizeof(message))
        return -EINVAL;
    
    strncpy(message, msg, sizeof(message));
    printk(KERN_INFO "Message set: %s\n", message);
    
    return 0;
}

static long ioctl_msg_delete(struct file *file)
{
    memset(message, 0, sizeof(message));
    printk(KERN_INFO "Message deleted\n");
    return 0;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
    switch (ioctl_num) {
        case IOCTL_SET_MSG:
            return ioctl_set_msg(file, ioctl_param);
        case IOCTL_MSG_DELETE:
            return ioctl_msg_delete(file);
        default:
            return -EINVAL;
    }
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .write = device_write,
    .read = device_read,
    .unlocked_ioctl = device_ioctl,
};

static int __init driver_init(void)
{
    if (register_chrdev(0, DEVICE_NAME, &fops) < 0) {
        printk(KERN_ALERT "Failed to register driver\n");
        return -1;
    }
    printk(KERN_INFO "Driver registered\n");
    return 0;
}

static void __exit driver_exit(void)
{
    unregister_chrdev(0, DEVICE_NAME);
    printk(KERN_INFO "Driver unregistered\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Sample driver");
