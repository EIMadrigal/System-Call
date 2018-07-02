#include <linux/init.h>   //包含模块初始化和清理函数的定义
#include <linux/module.h> //包含加载模块时需要的函数和符号定义
#include <linux/fs.h>
#include <linux/sched.h>  //包括宏for_each_process
#include <asm/uaccess.h>
#include <linux/init_task.h>
#include <linux/unistd.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");   //模块许可声明

#define for_each_process(p) \
    for(p = &init_task;(p = next_task(p)) != &init_task;)

/*模块初始化函数*/
static int ModuleRen_init(void)
{
    struct task_struct* p = NULL;
    struct file* fp = NULL;  //读文件指针
    struct file* fw = NULL;  //写文件指针

    mm_segment_t fs;
    fs = get_fs();
    set_fs(KERNEL_DS); //将内存检查扩大至内核区域

    loff_t pos = 0;  //读文件起始位置
    loff_t pos2 = 0; //写文件起始位置
    char cpuinfo[1000];
    char osinfo[200];
    char remind[] = "System information is as follows(print by system call):\n";
    char cpu_remind[] = "\nCpu information:\n";
    char os_remind[] = "\nOperating system information:\n";

    printk("%s\n",remind);

    /*CPU信息*/
    fp = filp_open("/proc/cpuinfo",O_RDONLY,0);  //只读方式打开cpu信息
    fw = filp_open("/home/renjingtao/Module/des.txt",O_RDWR,0);  //可读可写方式打开写文件
    vfs_read(fp,cpuinfo,sizeof(cpuinfo),&pos);

    printk("%s\n%s\n\n",cpu_remind,cpuinfo);
    vfs_write(fw,cpu_remind,sizeof(cpu_remind),&pos2);
    vfs_write(fw,cpuinfo,sizeof(cpuinfo),&pos2);

    filp_close(fp,NULL);
    set_fs(fs);  //将内存检查恢复至原来

    /*内核版本信息*/
    pos = 0;  //读指针归0
    fs = get_fs();
    set_fs(KERNEL_DS);

    fp = filp_open("/proc/version",O_RDONLY,0);

    vfs_read(fp,osinfo,sizeof(osinfo),&pos);

    printk("%s\n%s\n\n",os_remind,osinfo);

    vfs_write(fw,os_remind,sizeof(os_remind),&pos2);
    vfs_write(fw,osinfo,sizeof(osinfo),&pos2);

    filp_close(fp,NULL);
    set_fs(fs);

    /*进程信息*/
    printk("Process information:\n");
    printk("%-20s%-10s%-15s%-15s%-10s\n","name","pid","time(userM)","time(kernelM)","state");
    for_each_process(p)
    {
        printk("%-20s%-10d%-15d%-15d%-5d\n",p->comm,p->pid,(p->utime)/60,(p->stime)/60,p->state);
    }

    filp_close(fw,NULL);

    printk("PRINT OVER!\n");

    return 0;  //初始化成功
}

/*模块清理函数*/
static void ModuleRen_exit(void)
{
    printk("Good Bye!\n");
}


module_init(ModuleRen_init);  //注册初始化函数
module_exit(ModuleRen_exit);  //注册清理函数

