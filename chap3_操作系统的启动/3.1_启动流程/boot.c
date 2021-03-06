// date:17.4.10
// author: linyang <942510346@qq.com>

typedef void (*init_func)(void);

#define UFCON0	((volatile unsigned int *)(0x50000020))

void helloworld(void)
{
	const char *p = "3.1_helloworld\n";
	while(*p) {
		*UFCON0 = *p++;
	};
}

static init_func init[] = {
	helloworld,
	0,
};

void plat_boot(void)
{
	int i;
	for(i = 0; init[i]; i++) {
		init[i]();
	}
	while(1);
}


/*
kolya@asus ~/src/leeos/chap3_操作系统的启动/3.1_启动流程 $ make
arm-none-eabi-gcc -O2 -g -c init.s
arm-none-eabi-gcc -O2 -g -c start.s
arm-none-eabi-gcc -O2 -g -c boot.c
arm-none-eabi-gcc -O2 -g -c abnormal.s
arm-none-eabi-gcc -static -nostartfiles -nostdlib -Tleeos.lds -Ttext 30000000  init.o start.o boot.o abnormal.o -o leeos.elf -lgcc
arm-none-eabi-objcopy -O binary leeos.elf leeos.bin
kolya@asus ~/src/leeos/chap3_操作系统的启动/3.1_启动流程 $ ../../skyeye_1.2.6rc1_linux/skyeye

**************************** WARNING **********************************
If you want to run ELF image, you should use -e option to indicate
your elf-format image filename. Or you only want to run binary image,
you need to set the filename of the image and its entry in skyeye.conf.
***********************************************************************

Your elf file is little endian.
arch: arm
cpu info: armv4, arm920t, 41009200, ff00fff0, 2 
mach info: name s3c2410x, mach_init addr 0x425800
uart_mod:0, desc_in:, desc_out:, converter:
SKYEYE: use arm920t mmu ops
Loaded RAM   ./leeos.bin
start addr is set to 0x30000000 by exec file.
3.1_helloworld
*/