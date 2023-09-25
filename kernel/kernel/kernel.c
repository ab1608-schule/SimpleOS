#include <stdio.h>

#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	terminal_enablecursor(14, 15);
	printf("SimpleOS\n\n");
	printf("Setting up memory... ");     memory_init();    printf("Done\n");
	printf("Setting up interrupts... "); interrupt_init(); printf("Done\n");
	asm volatile("int $0x10"); // TEST CODE - REMOVE WHEN DONE TESTING
}