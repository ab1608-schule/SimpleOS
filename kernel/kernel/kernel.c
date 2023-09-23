#include <stdio.h>

#include <kernel/memory.h>
#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	terminal_enablecursor(14, 15);
	printf("SimpleOS");
	memory_init();
}