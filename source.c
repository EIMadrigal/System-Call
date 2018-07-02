#include <linux/init.h>   //����ģ���ʼ�����������Ķ���
#include <linux/module.h> //��������ģ��ʱ��Ҫ�ĺ����ͷ��Ŷ���
#include <linux/fs.h>
#include <linux/sched.h>  //������for_each_process
#include <asm/uaccess.h>
#include <linux/init_task.h>
#include <linux/unistd.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");   //ģ���������

#define for_each_process(p) \
    for(p = &init_task;(p = next_task(p)) != &init_task;)

/*ģ���ʼ������*/
static int ModuleRen_init(void)
{
    struct task_struct* p = NULL;
    struct file* fp = NULL;  //���ļ�ָ��
    struct file* fw = NULL;  //д�ļ�ָ��

    mm_segment_t fs;
    fs = get_fs();
    set_fs(KERNEL_DS); //���ڴ����������ں�����

    loff_t pos = 0;  //���ļ���ʼλ��
    loff_t pos2 = 0; //д�ļ���ʼλ��
    char cpuinfo[1000];
    char osinfo[200];
    char remind[] = "System information is as follows(print by system call):\n";
    char cpu_remind[] = "\nCpu information:\n";
    char os_remind[] = "\nOperating system information:\n";

    printk("%s\n",remind);

    /*CPU��Ϣ*/
    fp = filp_open("/proc/cpuinfo",O_RDONLY,0);  //ֻ����ʽ��cpu��Ϣ
    fw = filp_open("/home/renjingtao/Module/des.txt",O_RDWR,0);  //�ɶ���д��ʽ��д�ļ�
    vfs_read(fp,cpuinfo,sizeof(cpuinfo),&pos);

    printk("%s\n%s\n\n",cpu_remind,cpuinfo);
    vfs_write(fw,cpu_remind,sizeof(cpu_remind),&pos2);
    vfs_write(fw,cpuinfo,sizeof(cpuinfo),&pos2);

    filp_close(fp,NULL);
    set_fs(fs);  //���ڴ���ָ���ԭ��

    /*�ں˰汾��Ϣ*/
    pos = 0;  //��ָ���0
    fs = get_fs();
    set_fs(KERNEL_DS);

    fp = filp_open("/proc/version",O_RDONLY,0);

    vfs_read(fp,osinfo,sizeof(osinfo),&pos);

    printk("%s\n%s\n\n",os_remind,osinfo);

    vfs_write(fw,os_remind,sizeof(os_remind),&pos2);
    vfs_write(fw,osinfo,sizeof(osinfo),&pos2);

    filp_close(fp,NULL);
    set_fs(fs);

    /*������Ϣ*/
    printk("Process information:\n");
    printk("%-20s%-10s%-15s%-15s%-10s\n","name","pid","time(userM)","time(kernelM)","state");
    for_each_process(p)
    {
        printk("%-20s%-10d%-15d%-15d%-5d\n",p->comm,p->pid,(p->utime)/60,(p->stime)/60,p->state);
    }

    filp_close(fw,NULL);

    printk("PRINT OVER!\n");

    return 0;  //��ʼ���ɹ�
}

/*ģ��������*/
static void ModuleRen_exit(void)
{
    printk("Good Bye!\n");
}


module_init(ModuleRen_init);  //ע���ʼ������
module_exit(ModuleRen_exit);  //ע��������

