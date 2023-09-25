#include <stdint.h>
#include <stdio.h>

#include <kernel/interrupt.h>

typedef struct __attribute__((packed)) {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idt_ptr_t;

idt_entry_t idt[256];
idt_ptr_t idtr;

void exception_handler(void) {
  printf("Exception encountered!\n");
  asm volatile ("cli \n"
                "hlt"); // Completely hangs the computer
}

void idt_setentry(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

extern void* isr_stub_table[];
 
void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_setentry(vector, isr_stub_table[vector], 0x8E);
    }
 
    asm volatile ("lidt %0" : : "m"(idtr)); // Load IDT
}

void interrupt_enable(void) {
  asm volatile ("sti");
}

void interrupt_disable(void) {
  asm volatile("cli");
}

void interrupt_init(void) {
  idt_init();
  interrupt_enable();
}