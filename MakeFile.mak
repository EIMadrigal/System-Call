ifneq ($(KERNELRELEASE),)
    obj-m := exper_4.o  #obj-mָ������ⲿģ��
else
    KERNELDIR := /lib/modules/$(shell uname -r)/build  #����һ��������ָ���ں�Ŀ¼
    PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  #������Tab����ʼ��������ں�ģ��
endif

