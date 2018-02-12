#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <linux/uaccess.h>

#define SLEEP _IOW(0,0,unsigned long)

static dev_t timer;//for device number
static struct cdev timer_dev;//for character device structure
static struct class *timer_cls;// for device class

static int timer_open(struct inode *i, struct file *f)//system call
{
    printk(KERN_INFO "timer : open()\n");
    return 0;
}
 
static int timer_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "timer : close()\n");
    return 0;
}
 
static ssize_t timer_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 unsigned int val_timer = inb(0x42);
 put_user (val_timer , buf);
 val_timer = inb (0x42);
 put_user (val_timer , buf+1);
    printk(KERN_INFO "timer : read()\n");
    return 0;
}

void delay1(void)
{
int x;
do
{
x=inb(0x42);
x+=inb(0x42)<<8;
}while(x>0 && x<1193);
}
void sleep(int arg)
{
for(;arg!=0;arg--)
{
outb_p(1193%256,0x42);// for lower byte
outb_p(1193/256,0x42);// for upper byte
delay1();
}
}

static long timer_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "IOCTL()");
	switch(cmd)
{
case SLEEP:
sleep(arg);
}
    return 0;
}

static struct file_operations fops =
{
  .owner    = THIS_MODULE,
  .open     = timer_open,
  .release  = timer_close,
  .read     = timer_read,
  .unlocked_ioctl=timer_ioctl
};

static int __init timer_init(void) 
{
    printk(KERN_INFO "8254 driver registered");
     
    
    if (alloc_chrdev_region(&timer, 0, 1, "8254_PIT") < 0)//for <major, minor>
    {
        return -1;
    }
     
    
    if ((timer_cls = class_create(THIS_MODULE, "chardrv")) == NULL)//create device node in /dev directory
    {
        unregister_chrdev_region(timer, 1);
        return -1;
    }
    if (device_create(timer_cls, NULL, timer, NULL, "timer_device") == NULL)
    {
        class_destroy(timer_cls);
        unregister_chrdev_region(timer, 1);
        return -1;
    }
     
    // Link fops and cdev to device node

    cdev_init(&timer_dev, &fops);
    if (cdev_add(&timer_dev, timer, 1) == -1)
    {
        device_destroy(timer_cls, timer);
        class_destroy(timer_cls);
        unregister_chrdev_region(timer, 1);
        return -1;
    }
outb(0xB0,0x43);
    return 0;
}


 
/*static ssize_t timer_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "timer : write()\n");
    return len;
}*/

 


static void __exit timer_exit(void) 
{
    cdev_del(&timer_dev);
    device_destroy(timer_cls,timer);
    class_destroy(timer_cls);
    unregister_chrdev_region(timer,1);
    printk(KERN_INFO "timer driver unregistered\n\n");
}
  
module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIPIN VERMA");
MODULE_DESCRIPTION("TIMER Character Driver");
