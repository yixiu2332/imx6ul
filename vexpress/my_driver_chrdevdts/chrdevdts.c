#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/of_address.h>

#define CHRDEVDTS_MAJOR 0   /* 可选是否指定设备号，0表示动态申请*/
#define CHRDEVDTS_CNT 1		/*注册设备的个数*/

static const char *name = "chrdevdts"; 	/*device name*/
static void __iomem *REGDATA1;
static void __iomem *REGDATA0;
/* chrdevdts设备结构体 */
struct chrdevdts_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;	/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
    struct device_node	*nd; /* 设备节点 */
} chrdevdts;


static int chrdevdts_open (struct inode *inode, struct file *filp)
{
    filp->private_data = &chrdevdts;
    return 0;
}

static int chrdevdts_release (struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t chrdevdts_read (struct file *filp, char __user *buf, 
                size_t count, loff_t *ppos)
{
    unsigned int reg_values[2]; // 在内核空间创建一个缓冲区
    size_t data_len = sizeof(reg_values); // 数据总长度为 8 字节
    int ret;
    
    // 检查用户请求的读取大小，防止越界
    if (count < data_len) {
        return -EINVAL; // 无效参数
    }
    
    // 1. 读取寄存器数据到内核缓冲区
    reg_values[0] = readl(REGDATA0);
    reg_values[1] = readl(REGDATA1);
    
    // 2. 将内核缓冲区的数据复制到用户空间
    ret = copy_to_user(buf, reg_values, data_len);
    if (ret != 0) {
        // 部分或全部复制失败
        return -EFAULT; 
    }

    // 3. 返回实际读取的字节数
    return data_len; 
}

static ssize_t chrdevdts_write (struct file *filp, const char __user *buf, 
                size_t count, loff_t *ppos)
{
    ssize_t actual_write = 0;

    return actual_write; // 返回实际写入的字节数
}
static const struct file_operations chrdevdts_fops = 
{
    .owner = THIS_MODULE,
    .open = chrdevdts_open,
    .release = chrdevdts_release,
    .read = chrdevdts_read,
    .write = chrdevdts_write,
};
static int __init chrdevdts_init(void)
{
    struct property *proper; /*节点属性*/
    const char *str;
    unsigned int regdata[2];
    int ret = 0;
    /*获取设备树节点*/
    chrdevdts.nd = of_find_node_by_path("/test-node");
    if(chrdevdts.nd == NULL){
        printk("test-node not find!\n");
		return -EINVAL;
    }
    /*读取status属性*/
    ret = of_property_read_string(chrdevdts.nd, "status", &str);
    if(ret < 0){
		printk("status read failed!\n");
	} else {
		printk("status = %s\n",str);
	}
    /* 4、获取reg属性内容 */
    ret = of_property_read_u32_array(chrdevdts.nd, "reg", regdata, 2);
    if(ret < 0) {
		printk("reg property read failed!\n");
	} else {
		printk("regdata[0]=%#X;regdata[1]=%#X\n",regdata[0],regdata[1]);
	}
    /*获取地址映射*/
	REGDATA0 = of_iomap(chrdevdts.nd, 0);
	REGDATA1 = of_iomap(chrdevdts.nd, 1);

    writel(0,REGDATA0);
    writel(1,REGDATA1);

    /* 注册字符设备驱动 */
	/* 1、创建设备号 */
	if (CHRDEVDTS_MAJOR) {		/*  定义了设备号 */
		chrdevdts.major = CHRDEVDTS_MAJOR;
        chrdevdts.devid = MKDEV(chrdevdts.major, 0);
        register_chrdev_region(chrdevdts.devid, CHRDEVDTS_CNT, name);
        chrdevdts.minor = 0; 
	} else {						/* 没有定义设备号 */
		alloc_chrdev_region(&chrdevdts.devid, 0, CHRDEVDTS_CNT, name);	/* 申请设备号 */
		chrdevdts.major = MAJOR(chrdevdts.devid);	/* 获取分配号的主设备号 */
		chrdevdts.minor = MINOR(chrdevdts.devid);	/* 获取分配号的次设备号 */
	}
	printk("chrdevdts major=%d,minor=%d\n",chrdevdts.major, chrdevdts.minor);	
	
	/* 2、初始化cdev */
	chrdevdts.cdev.owner = THIS_MODULE;
	cdev_init(&chrdevdts.cdev, &chrdevdts_fops);
	
	/* 3、添加一个cdev */
	cdev_add(&chrdevdts.cdev, chrdevdts.devid, CHRDEVDTS_CNT);

	/* 4、创建类 */
	chrdevdts.class = class_create(name);
	if (IS_ERR(chrdevdts.class)) {
		return PTR_ERR(chrdevdts.class);
	}

	/* 5、创建设备 */
	chrdevdts.device = device_create(chrdevdts.class, NULL, chrdevdts.devid, NULL, name);
	if (IS_ERR(chrdevdts.device)) {
		return PTR_ERR(chrdevdts.device);
	}
    return 0;
}
static void __exit chrdevdts_exit(void)
{
    iounmap(REGDATA0);
    iounmap(REGDATA1);
    
    /*销毁的顺序应该和注册相反*/
	device_destroy(chrdevdts.class, chrdevdts.devid);
	class_destroy(chrdevdts.class);
    /* 注销字符设备驱动 */
	cdev_del(&chrdevdts.cdev);/*  删除cdev */
	unregister_chrdev_region(chrdevdts.devid, CHRDEVDTS_CNT); /* 注销设备号 */
}

/*
 * 模块入口与出口
 */
module_init(chrdevdts_init);
module_exit(chrdevdts_exit);
/*
 * 其他信息
 */
MODULE_LICENSE("GPL");