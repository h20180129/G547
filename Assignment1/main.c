#include<linux/init.h>
#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include <linux/types.h>
#include <linux/random.h>
#include <linux/uaccess.h>


static dev_t ADXL;// variable for device number
static struct cdev cdev_x;// variable for the character device structure
static struct cdev cdev_y;
static struct cdev cdev_z;
static struct class *cls; // variable for the device class
dev_t x_axis,y_axis,z_axis;
uint16_t randno;
static int    numberOpens = 0;

/*****************************************************************************
STEP 4 as discussed in the lecture, 
my_close(), my_open(), my_read(), my_write() functions are defined here
these functions will be called for close, open, read and write system calls respectively. 
*****************************************************************************/


static int adxl_xopen(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "ADXL: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}


static int adxl_xclose(struct inode *i,struct file *f)
{
printk(KERN_INFO "ADXL: Device successfully closed\n");
return 0;
}


static ssize_t adxl_xread(struct file *f,char __user *buf,size_t len,loff_t *off)
{
uint16_t n;
get_random_bytes(&n, sizeof(n));
randno=n&0x03FF;//Mask out the most significant bit after calling get_random_bytes.(1023d==0x03FFh)
copy_to_user(buf, &randno, sizeof(n));// copy_to_user has the format ( * to, *from, size)
printk(KERN_INFO "x axis:reading()\n");
printk(KERN_INFO "Value=%d\n",randno);
return 0;
}

//################################################################################################
static int adxl_yopen(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "ADXL: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}


static int adxl_yclose(struct inode *i,struct file *f)
{
printk(KERN_INFO "ADXL: Device successfully closed\n");
return 0;
}

static ssize_t adxl_yread(struct file *f,char __user *buf,size_t len,loff_t *off)
{
uint16_t n;
get_random_bytes(&n, sizeof(n));
randno=n&0x03FF;//Mask out the most significant bit after calling get_random_bytes.(1023d==0x03FFh)
copy_to_user(buf, &randno, sizeof(n));// copy_to_user has the format ( * to, *from, size)
printk(KERN_INFO "y axis:reading()\n");
printk(KERN_INFO "Value=%d\n",randno);
return 0;
}

//################################################################################################

static int adxl_zopen(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "ADXL: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}


static int adxl_zclose(struct inode *i,struct file *f)
{
printk(KERN_INFO "ADXL: Device successfully closed\n");
return 0;
}

static ssize_t adxl_zread(struct file *f,char __user *buf,size_t len,loff_t *off)
{
uint16_t n;
get_random_bytes(&n, sizeof(n));
randno=n&0x03FF;//Mask out the most significant bit after calling get_random_bytes.(1023d==0x03FFh)
copy_to_user(buf, &randno, sizeof(n));// copy_to_user has the format ( * to, *from, size)
printk(KERN_INFO "z axis:reading()\n");
printk(KERN_INFO "Value=%d\n",randno);
return 0;
}

//################################################################################################
//################################################################################################


static struct file_operations adxl_xoprs =
{
.owner=THIS_MODULE,
.open=adxl_xopen,
.release=adxl_xclose,
.read=adxl_xread,
};
static struct file_operations adxl_yoprs =
{
.owner=THIS_MODULE,
.open=adxl_yopen,
.release=adxl_yclose,
.read=adxl_yread,
};
static struct file_operations adxl_zoprs =
{
.owner=THIS_MODULE,
.open=adxl_zopen,
.release=adxl_zclose,
.read=adxl_zread,
};

//################################################################################################
//################################################################################################


//########## INITIALIZATION FUNCTION ##################
// STEP 1,2 & 3 are to be executed in this function ### 
static __init int main_init(void)
{
    printk(KERN_INFO "Namaste: ADXL driver registered\n\n");
	// STEP 1 : reserve <major, minor>
    if((alloc_chrdev_region(&ADXL,0,3,"ADXL"))<0)
        {
            return -1;
        }
    x_axis=MKDEV(MAJOR(ADXL), MINOR(ADXL));
    y_axis=MKDEV(MAJOR(ADXL), MINOR(ADXL)+1);
    z_axis=MKDEV(MAJOR(ADXL), MINOR(ADXL)+2);
	// STEP 2 : dynamically create device node in /dev directory using device_create
	// STEP 3 : Link fops and cdev to device node
    if((cls=class_create(THIS_MODULE,"adxl"))==NULL)
        {
            unregister_chrdev_region(ADXL,3);
            return -1;
        }
    cdev_init(&cdev_x,&adxl_xoprs);
    device_create(cls,NULL,x_axis,NULL,"adxl_x");
    if(cdev_add(&cdev_x,x_axis,1)<0)
        {
            device_destroy(cls, x_axis);
            class_destroy(cls);
            unregister_chrdev_region(ADXL, 3);
            return -1;
        }
    cdev_init(&cdev_y,&adxl_yoprs);
    device_create(cls,NULL,y_axis,NULL,"adxl_y");
    if(cdev_add(&cdev_y,y_axis,1)<0)
        {
            cdev_del(&cdev_x);
            device_destroy(cls, y_axis);
            device_destroy(cls, x_axis);
            class_destroy(cls);
            unregister_chrdev_region(ADXL, 3);
            return -1;
        }
    cdev_init(&cdev_z,&adxl_zoprs);
    device_create(cls,NULL,z_axis,NULL,"adxl_z");
    if(cdev_add(&cdev_z,z_axis,1)<0)
        {
            cdev_del(&cdev_y);
            cdev_del(&cdev_x);
            device_destroy(cls, z_axis);
            device_destroy(cls, y_axis);
            device_destroy(cls, x_axis);
            class_destroy(cls);
            unregister_chrdev_region(ADXL, 3);
            return -1;
        }
    return 0;

}


static __exit void main_exit(void)
{
cdev_del(&cdev_x);
cdev_del(&cdev_y);
cdev_del(&cdev_z);
device_destroy(cls,x_axis);
device_destroy(cls,y_axis);
device_destroy(cls,z_axis);
class_destroy(cls);
unregister_chrdev_region(ADXL,3);
printk(KERN_INFO "Bye: ADXL driver unregistered\n\n");
}

module_init(main_init);
module_exit(main_exit);


MODULE_AUTHOR("h20180129");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("A simple Linux char driver for ADXL");

