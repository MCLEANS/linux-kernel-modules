#include <linux/module.h>
#include <linux/init.h>


/**
 * For a Kernel module there are 2 important events:
 * i) When the module is loaded into the Kernel
 * ii) When the module is removed from the Kernel
 * 
 * We Therefore need callbacks to handle this special events
 * 
 */

/**
 * @brief Runs when the module is loaded into the Kernel
 * 
 * @return int 0 if the loading was successful else a negative value
 */
int init(void){

    printk("Hello from Custom Kernel Module!\n");

    return 0;
}


void deinit(void){

    printk(" Goodbye from custom Kernel Module!\n");

}


module_init(init);
module_exit(deinit);

/**
 * To make the module compilable we have to define the License of the module
 * 
 */
MODULE_LICENSE("GPL");


