# hellodriver
linux hello world driver

Ubuntu下Hello World驱动实现全过程


今天是一个值得纪念的日子，我也不知道我花了多长时间才编译成功！在编写这个驱动的过程中，真的体会到驱动编程不是那么简单的～，现在有点小小的激动，真的，很不容易啊～我把整个过程在重复一遍吧！

在编写这个驱动的过程中参考了网上的很多文档，最终的结果是弄的我头都晕了，每个人写的都不一样，其实我现在还有一些概念不是很清楚。

1：到底丫的什么是内核源码树？

2:为什么要编译内核源码树？

1：内核源码树我现在的理解就是整个linux内核源代码，它是编译驱动的前提。Ubuntu系统默认情况下是没有的，

内核源码树是要自己下载的。

2：驱动最终以*.ko的形式生成，insmod的本质就是将ko文件与运行的内核进行链接的过程。类似于编译helloworld的链接过程。

链接必然需要先进行编译，以便确定所需的外部符号（EXPORT_SYMBOLS）是否存在，因为有些符号（函数或全局变量）在

内核中。在驱动中如果使用到这些符号，必须预留一个位置，insmod时进一步确定这些符号的具体位置（符号绑定）。

如果内核都没有编译过，怎么知道这些符号有没有编入内核中

Hello World驱动实现全过程

1：首先要查看自己系统是使用的内核版本

www.linuxidc.com @linux:~$  uname -r

3.2.0-34-generic-pae  //我用的是ubuntu12.04版本的，比较高

如果系统自动安装了源码。在/usr/src目录下面会有对应的使用的版本，我那个版本没有，我是自己下载的

www.linuxidc.com @linux:/usr/src$ ls

linux-headers-3.2.0-34              linux-source-3.2.0（我下载的）

linux-headers-3.2.0-34-generic-pae

如果没有源码，查看一下可以下载的源码包（不要使用超级用户使用此命令，他会提示没有这个命令）

#apt-cache  search  linux-source

www.linuxidc.com @linux:/usr/src$ apt-cache  search  linux-source

linux-source - Linux kernel source with Ubuntu patches

linux-source-3.2.0 - Linux kernel source for version 3.2.0 with Ubuntu patches

2：然后下载linux-source-3.2.0

#sudo apt-get  install  linux-source-3.2.0

下载完成之后，会在/usr/src下，文件名为linux-source-3.2.0.tar.bz2这个压缩包，解压之后就可以得到整个源代码

# sudo  tar jxvf  linux-source-3.2.0.tar.bz2

解压之后会生成一个新的目录/usr/src/linux-source-3.2.0，现在所有的源代码都在里面

3：现在开始配置内核，有三种选择选择方式1：make oldconfig  2:make  menuconfig 3:make  xconfig

我选择的是最快的配置原版的配置方法

#sudo  make  oldconfig

（如果配置的过程中出现问题，原因是因为你没有下载配置环境的那个依赖的库文件，可以自己下载）

配置完成之后就要开始对内核进行编译了

4：编译内核

#sudo  make

这个过程很漫长，建议去做做别别的事情吧～，我大概花了一个多小时，吃完晚饭回来就ok了

#sudo  make  bzImage


编译内核的结果最终出现了几个错误，但最终对那个Hello  World 没有造成影响

执行结束之后，会在当前目录下面生成一个新的文件：vmlinux

5：然后就是编译模块了

#sudo make  modules

6:安装模块

#sudo  make  modules_install

执行结束之后，会在/lib/modules下生成一个新的目录/lib/modules/3.2.0/

在随后的编译模块文件时，需要用到这个路径下的build目录。到这一不，内核就编译完成了

好了，下了很长时间的Hello  World 程序现在终于可以派上用场了

我的Hello  World在/hoem/xiongyao/下面


//hello.c

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


Makefile 文件

# Makefile2.6
ifneq ($(KERNELRELEASE),)
 #kbuild syntax. dependency relationshsip of files and target modules are listed here.
 mymodule-objs := hello.o 
 obj-m := hello.o    
else
PWD  := $(shell pwd)
KVER ?= $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) 
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif



最好关键的一步到了，所有准备就绪，进入makefile的那个目录

www.linuxidc.com @linux:~$ sudo make
make -C /lib/modules/3.2.31/build M=/home/xiongyao 
make[1]: 正在进入目录 `/usr/src/linux-source-3.2.0/linux-source-3.2.0'
  LD      /home/xiongyao/built-in.o
  CC [M]  /home/xiongyao/hello.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/xiongyao/hello.mod.o
  LD [M]  /home/xiongyao/hello.ko
make[1]:正在离开目录 `/usr/src/linux-source-3.2.0/linux-source-3.2.0'

上面证明已经成功了

然后加载模块
#sudo  insmod  ./hello.ko

本应该会在终端显示hello，world  但是终端什么也没有显示（以后在去解决）

查看加载模块

#sudo  lsmod 

里面已经生成了hello

hello  2560 0

^_^，已经加载上了 

删除模块

#sudo  rmmod  hello

那么程序的输出到底在哪里呢？在网上看到，如果不出现在终端，则会下进syslog中

#cat  /var/log/syslog  |grep  world

#Hello,world
#Goodbye,linux  world

现在所有工作全部完成了，希望做这个Hello World 是我进入linux驱动的第一步～，相信你们也能编译的！成功的编译成功的。

linux