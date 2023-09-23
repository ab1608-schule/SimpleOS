#include <stdint.h>

#include <kernel/memory.h>

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09

#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80

#define GDT_FLAG_4K_GRAN 0x800
#define GDT_FLAG_32_BIT  0x400

struct gdt_ptr {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

uint64_t gdt[5];
struct gdt_ptr gp;

// Defined in boot.s
extern void gdt_flush();

void gdt_setentry(int i, unsigned int base, unsigned int limit, int flags) {
  gdt[i] = limit & 0xffffLL;
  gdt[i] |= (base & 0xffffffLL) << 16;
  gdt[i] |= (flags & 0xffLL) << 40;
  gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
  gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
  gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

void gdt_install() {
  gp.limit = (sizeof(uint64_t) * 3) - 1;
  gp.base = &gdt;

  gdt_setentry(0, 0, 0, 0);
  gdt_setentry(1, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
    GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);
  gdt_setentry(2, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
    GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);
  gdt_setentry(3, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
    GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
  gdt_setentry(4, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
    GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

  _gdt_flush();
}
		

void memory_init(void) {
  gdt_install();
}