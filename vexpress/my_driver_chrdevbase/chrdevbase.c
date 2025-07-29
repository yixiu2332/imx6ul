#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init chrdevbase_init(void)
{
    reutrn 0;
}
static void __exit chrdevbase_exit(void)
{

}

/*
 * 模块入口与出口
 */
modele_init(chrdevbase_init);
modele_exit(chrdevbase_exit);