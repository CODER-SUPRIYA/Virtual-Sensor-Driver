#include <linux/module.h>
#include <linux/kernel.h>

static int __init vsensor_init(void){
pr_info("vsensor: loaded\n");
return 0;

}
static void __exit vsensor_exit(void){
pr_info("vsensor:unloaded\n");

}
module_init(vsensor_init);
module_exit(vsensor_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Supriya");
MODULE_DESCRIPTION("Virtual Sensor Driver");

