#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define BUF_SIZE 256
#define CHRDEVUP_MAJOR 0   /* 可选是否指定设备号，0表示动态申请*/
#define CHRDEVUP_CNT 1		/*注册设备的个数*/

static const char *name = "chrdevup"; 	/*device name*/
static char write_buf[BUF_SIZE];        /*write function buffer*/
static char read_buf[BUF_SIZE];         /*read function buffer*/
static char kernel_data[] = {"this is kernel data!"}; /*The data stored in the kernel*/

/* chrdevup设备结构体 */
struct chrdevup_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
} chrdevup;


static int chrdevup_open (struct inode *inode, struct file *filp)
{
    printk("chrdevup_open...\n");
    return 0;
}

static int chrdevup_release (struct inode *inode, struct file *filp)
{
    printk("chrdevup_release...\n");
    return 0;
}

static ssize_t chrdevup_read (struct file *filp, char __user *buf, 
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

static ssize_t chrdevup_write (struct file *filp, const char __user *buf, 
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
static const struct file_operations chrdevup_fops = 
{
    .owner = THIS_MODULE,
    .open = chrdevup_open,
    .release = chrdevup_release,
    .read = chrdevup_read,
    .write = chrdevup_write,
};
static int __init chrdevup_init(void)
{
    /* 注册字符设备驱动 */
	/* 1、创建设备号 */
	if (CHRDEVUP_MAJOR) {		/*  定义了设备号 */
		chrdevup.devid = MKDEV(CHRDEVUP_MAJOR, 0);
		register_chrdev_region(chrdevup.devid, CHRDEVUP_CNT, name);
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&chrdevup.devid, 0, CHRDEVUP_CNT, name);	/* 申请设备号 */
		chrdevup.major = MAJOR(chrdevup.devid);	/* 获取分配号的主设备号 */
		chrdevup.minor = MINOR(chrdevup.devid);	/* 获取分配号的次设备号 */
	}
	printk("chrdevup major=%d,minor=%d\n",chrdevup.major, chrdevup.minor);	
	
	/* 2、初始化cdev */
	chrdevup.cdev.owner = THIS_MODULE;
	cdev_init(&chrdevup.cdev, &chrdevup_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&chrdevup.cdev, chrdevup.devid, CHRDEVUP_CNT);

	/* 4、创建类 */
	chrdevup.class = class_create(name);
	if (IS_ERR(chrdevup.class)) {
		return PTR_ERR(chrdevup.class);
	}

	/* 5、创建设备 */
	chrdevup.device = device_create(chrdevup.class, NULL, chrdevup.devid, NULL, name);
	if (IS_ERR(chrdevup.device)) {
		return PTR_ERR(chrdevup.device);
	}
    return 0;
}
static void __exit chrdevup_exit(void)
{
    /* 注销字符设备驱动 */
	cdev_del(&chrdevup.cdev);/*  删除cdev */
	unregister_chrdev_region(chrdevup.devid, CHRDEVUP_CNT); /* 注销设备号 */

	device_destroy(chrdevup.class, chrdevup.devid);
	class_destroy(chrdevup.class);
}

/*
 * 模块入口与出口
 */
module_init(chrdevup_init);
module_exit(chrdevup_exit);
/*
 * 其他信息
 */
MODULE_LICENSE("GPL");