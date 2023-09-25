#include <stdint.h>
#include <stdio.h>

#include <kernel/interrupt.h>
#include <kernel/io.h>

typedef struct __attribute__((packed)) {
	uint16_t    isr_low;
	uint16_t    kernel_cs; // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;
	uint8_t     attributes;
	uint16_t    isr_high;
} idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idt_ptr_t;

idt_entry_t idt[256];
idt_ptr_t idtr;

typedef struct {
    // Manually saved
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    uint32_t intr;
    uint32_t error;

    // Automatically saved
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} cpu_state_t;

void exception_handler(cpu_state_t* cpu) {
    if (cpu->intr <= 0x1f) {
        printf("Exception %d, kernel halted!\n", cpu->intr);

        // TODO: Output CPU state

        while(1) {
            asm volatile("cli \n" "hlt");
        }
    } else {
      if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {
        if (cpu->intr >= 0x28) {
            outb(0xa0, 0x20);
        }
        outb(0x20, 0x20);
      }
      
      // TODO: Handle interrupts
    }
}

void idt_setentry(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
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

void pic_configureirqs(void) {
  /*
   * Hardware exceptions: 0x00 - 0x1f
   * IRQs:                0x20 - 0x2f
   * Software interrupts: 0x30 - 0xff
   */

  // Master-PIC
  outb(0x20, 0x11); // Initialization
  outb(0x21, 0x20); // IRQ 0 number
  outb(0x21, 0x04); // Bitmask for mapped slaves
  outb(0x21, 0x01); // ICW 4

  // Slave-PIC
  outb(0x20, 0x11); // Initialization
  outb(0x21, 0x28); // IRQ 0 number
  outb(0x21, 0x02); // Single mode; no slaves
  outb(0x21, 0x01); // ICW 4

  // Enable all IRQs (Bitmask)
  outb(0x21, 0x0);
  outb(0xa1, 0x0);
}

void interrupt_init(void) {
  idt_init();
  pic_configureirqs();
  interrupt_enable();
}