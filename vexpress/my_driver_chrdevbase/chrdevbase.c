#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define BUF_SIZE 256

static const char *name = "chrdevbase"; /*device name*/
static const int major = 200;           /*device major id*/
static char write_buf[BUF_SIZE];        /*write function buffer*/
static char read_buf[BUF_SIZE];         /*read function buffer*/
static char kernel_data[] = {"this is kernel data!"}; /*The data stored in the kernel*/


static int chrdevbase_open (struct inode *inode, struct file *filp)
{
    printk("chrdevbase_open...\n");
    return 0;
}

static int chrdevbase_release (struct inode *inode, struct file *filp)
{
    printk("chrdevbase_release...\n");
    return 0;
}

static ssize_t chrdevbase_read (struct file *filp, char __user *buf, 
                size_t count, loff_t *ppos)
{
    size_t data_len = sizeof(kernel_data);
    ssize_t actual_read = 0;

    if (*ppos >= data_len) {
        return 0; // 已经读到数据末尾，返回0
    }

    // 确定本次实际可读的字节数，避免越界
    if (*ppos + count > data_len) {
        actual_read = data_len - *ppos;
    } else {
        actual_read = count;
    }

    // 将数据从内核空间拷贝到用户空间
    if (copy_to_user(buf, kernel_data + *ppos, actual_read)) {
        printk(KERN_ERR "Failed to copy data to user\n");
        return -EFAULT;
    }

    // 更新文件偏移量，这是实现分批读取的关键
    *ppos += actual_read;
    
    printk(KERN_INFO "Read %zd bytes\n", actual_read);

    return actual_read; // 返回实际读取的字节数
}

static ssize_t chrdevbase_write (struct file *filp, const char __user *buf, 
                size_t count, loff_t *ppos)
{
     ssize_t actual_write = 0;

    if (*ppos >= BUF_SIZE) {
        printk(KERN_ERR "Buffer is full, cannot write more\n");
        return -ENOSPC; // 没有空间了
    }

    // 确定本次实际可写的字节数
    if (*ppos + count > BUF_SIZE) {
        actual_write = BUF_SIZE - *ppos;
    } else {
        actual_write = count;
    }

    // 从用户空间拷贝数据到内核空间，从正确的偏移量开始
    if (copy_from_user(write_buf + *ppos, buf, actual_write)) {
        printk(KERN_ERR "Failed to copy data from user\n");
        return -EFAULT;
    }

    // 更新文件偏移量
    *ppos += actual_write;

    printk(KERN_INFO "Wrote %zd bytes. Current pos: %lld\n", actual_write, *ppos);

    return actual_write; // 返回实际写入的字节数
}
static const struct file_operations chrdevbase_fops = 
{
    .owner = THIS_MODULE,
    .open = chrdevbase_open,
    .release = chrdevbase_release,
    .read = chrdevbase_read,
    .write = chrdevbase_write,
};
static int __init chrdevbase_init(void)
{
    printk(KERN_INFO "chrdevbase module initialized\n");
    register_chrdev(major,name,&chrdevbase_fops);
    return 0;
}
static void __exit chrdevbase_exit(void)
{
    unregister_chrdev(major,name);
    printk("chrdevbase module exited\n");
}

/*
 * 模块入口与出口
 */
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);
/*
 * 其他信息
 */
MODULE_LICENSE("GPL");