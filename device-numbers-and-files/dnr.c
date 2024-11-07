#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/atomic.h>
#include <linux/fs.h>

#include <asm/errno.h> 


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MCLEANS");
MODULE_DESCRIPTION("A KERNEL MODULE TO CONNECT DEVICE TO A MODULE");

#define DEVICE_NAME "devnr"

static struct class *cls;
static int driver_major_number;

enum {
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1
};

/* Used to prevent multiple access to the device */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static int dnr_open(struct inode* device_file, struct file* instance){

    /* if we try to open and the device is already in use we return a busy error */
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)){
        return -EBUSY; 
    }
    
    try_module_get(THIS_MODULE); 

    pr_info("[dnr] - Device /dev/%s successfully opened\n", DEVICE_NAME);

    return 0;
}

static int dnr_close(struct inode* device_file, struct file* instance){

    /* We're now ready for our next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    /* Decrement the usage count, or else once you opened the file, you will 
     * never get rid of the module. 
     */ 
    module_put(THIS_MODULE); 

    pr_info("[dnr] - Device /dev/%s closed\n", DEVICE_NAME);

    return 0;
}

/** Create a file operation objects that hold pointers to functions defined by the driver 
 *  that perform various operations by the device. Each field of the structure co-responds to the
 *  address of some function defined by the driver to handle a requested operation
 */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dnr_open,
    .release = dnr_close

};


static int __init dnr_initialize(void){

    pr_info("[dnr] - Initializing DNR kernel module\n");

    /* Register device numbern Zero major number means the Kernel will dynamically allocate a major number for us */
    driver_major_number = register_chrdev(0, "nrldev", &fops);

    if(driver_major_number < 0){
        pr_err("[dnr] - Char device registraction failed with %d\n", driver_major_number);
    }

    pr_info("[dnr] - Initialized  with major number : %d\n", driver_major_number);

    /* Create a class based on the device name*/
    cls = class_create(DEVICE_NAME);

    /* Create a device file */
    device_create(cls, NULL, MKDEV(driver_major_number, 0), NULL, DEVICE_NAME);

    pr_info("[dnr] - Device created at /dev/%s\n", DEVICE_NAME);
    
    
    return 0;
}


static void __exit dnr_deinitilize(void){

    device_destroy(cls, MKDEV(driver_major_number, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(driver_major_number, DEVICE_NAME); 

    pr_info("De-Initialized DNR kernel module\n");

}


module_init(dnr_initialize);
module_exit(dnr_deinitilize);