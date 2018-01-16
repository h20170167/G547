#include <linux/init.h>
#include <linux/module.h>

int Assignment1_init(void)
{
	printk(KERN_ALERT "HEY WORLD! WHAT'S UP?\n\n");
	return 0;
}
void Assignment1_exit(void)
{
	printk(KERN_ALERT "Bye Everyone\n\n");	
}
module_init(Assignment1_init);
module_exit(Assignment1_exit);
