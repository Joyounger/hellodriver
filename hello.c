#include<linux/init.h>  //初始换函数
#include<linux/kernel.h>  //内核头文件
#include<linux/module.h>  //模块的头文件

MODULE_LICENSE("Xiongyao  BSD/GPL");

static  int __init hello_init(void)
{
	  printk(KERN_ALERT "Hello, world\n"); //模块运行在内核态，不能使用用户态C库函数中的printf函数，而要使用printk函数
	   //打印调试信息
	 return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_ALERT "Goodbye,Hello world\n");
	
}


module_init(hello_init);
module_exit(hello_exit);
