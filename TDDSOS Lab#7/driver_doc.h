#ifndef DRIVER_DOC_H
#define DRIVER_DOC_H

#include <linux/ioctl.h>

#define IOCTL_SET_MSG _IOW('k', 0, char *)
#define IOCTL_MSG_DELETE _IO('k', 1)

int open_file(int file_desc);
int close_file(int file_desc);
int write_to_file(int file_desc, const char* data);
int read_from_file(int file_desc, char* buffer, int size);
int ioctl_set_msg(int file_desc, const char* msg);
int ioctl_msg_delete(int file_desc);

#endif