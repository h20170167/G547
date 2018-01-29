#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include<asm/uaccess.h>

#define ADC_SET_CHANNEL _IOW(0,0,unsigned long)

static dev_t adc;
static struct cdev c_dev;
static struct class *cls;

static int channel_no;
static int ret_status;

static int my_open(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Open()\n");
    return 0;
}
static int my_close(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Close()");
    return 0;
}
static ssize_t my_read(struct file *f,char __user *buffer,size_t length,loff_t *offset)
{
    char return_buffer[2];
    get_random_bytes(return_buffer,2);
    printk(KERN_INFO "Read()");
    put_user(return_buffer[0],buffer);
    put_user(return_buffer[1],buffer+1);
    return 0;
}
static long my_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "IOCTL()");
    switch(cmd)
    {
        case ADC_SET_CHANNEL:
        channel_no=arg;
    }
    printk(KERN_INFO "ADC Channel is %d\n",channel_no);
    return 0;
}
static struct file_operations fops=
{
    .owner=THIS_MODULE,
    .open=my_open,
    .release=my_close,
    .read=my_read,
    .unlocked_ioctl=my_ioctl
};
static int __init my_init(void)
{
    if(alloc_chrdev_region(&adc,0,1,"ADCDRIVER")<0)
    {
        ret_status=-1;
        goto EXIT_POINT;
    }
    cls=class_create(THIS_MODULE,"adcclass");
    if(cls==NULL)
    {
        unregister_chrdev_region(adc,1);
        ret_status=-1;
        goto EXIT_POINT;
    }
    if(device_create(cls,NULL,adc,NULL,"adcdriver")==NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(adc,1);
        ret_status=-1;
        goto EXIT_POINT;
    }
    cdev_init(&c_dev,&fops);
    if(cdev_add(&c_dev,adc,1)<0)
    {
        device_destroy(cls,adc);
        class_destroy(cls);
        unregister_chrdev_region(adc,1);
        ret_status=-1;
        goto EXIT_POINT;
    }
    printk(KERN_INFO "ADC driver registered! MAJOR: %d MINOR: %d\n",MAJOR(adc),MINOR(adc));
    ret_status=0;
    channel_no=0;/*Channel set to 0 by default*/
    EXIT_POINT:
    return ret_status;
}
static void __exit my_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cls,adc);
    class_destroy(cls);
    unregister_chrdev_region(adc,1);
    printk(KERN_INFO "ADC driver unregisterd!\n");
}
module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vipin Verma");
MODULE_DESCRIPTION("ADC Driver");
