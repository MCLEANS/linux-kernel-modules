#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MCLEANS");
MODULE_DESCRIPTION("A KERNEL MODULE TEMPLATE");


static int __init initialize(void){

    pr_info("Initialized template kernel module\n");

    return 0;
}


static void __exit deinitilize(void){

    pr_info("De-Initialized template kernel module\n");

}


module_init(initialize);
module_exit(deinitilize);