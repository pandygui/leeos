// date:17.4.10
// author: linyang <942510346@qq.com>

#include "storage.h"

typedef void (*init_func)(void);

#define UFCON0	((volatile unsigned int *)(0x50000020))

void helloworld(void)
{
	const char *p = "7.1_helloworld\n";
	while(*p) {
		*UFCON0 = *p++;
	};
}

static init_func init[] = {
	helloworld,
	0,
};

void test_mmu(void)
{
	const char *p = "7.1_test_mmu\n";
	while (*p) {
		*(volatile unsigned int *)0xd0000020 = *p++;
	};
}

extern void test_printk(void);

#define TIMER_BASE  (0xd1000000)
#define TCFG0   	((volatile unsigned int *)(TIMER_BASE+0x0))
#define TCFG1   	((volatile unsigned int *)(TIMER_BASE+0x4))
#define TCON    	((volatile unsigned int *)(TIMER_BASE+0x8))
#define TCONB4  	((volatile unsigned int *)(TIMER_BASE+0x3c))
void timer_init(void)
{
	*TCFG0|=0x800;
	*TCON&=(~(7<<20));
	*TCON|=(1<<22);
	*TCON|=(1<<21);

	*TCONB4=10000;

	*TCON|=(1<<20);
	*TCON&=~(1<<21);

	umask_int(14);
	enable_irq();
}

void *kmalloc(unsigned int size);

void plat_boot(void)
{
	int i;
	for(i = 0; init[i]; i++) {
		init[i]();
	}
	init_sys_mmu();
	start_mmu();
	// test_mmu();
	// test_printk();
	// timer_init();

	init_page_map();
	kmalloc_init();

	char buf[128];
	ramdisk_driver_init();
	storage[RAMDISK]->dout(storage[RAMDISK],buf,0,sizeof(buf));
	printk("7.1-ram.img's content\n");
	for(i = 0; i <  sizeof(buf); i++) {
		printk("%d ",buf[i]);
	}
	printk("\n");

	while(1);
}


