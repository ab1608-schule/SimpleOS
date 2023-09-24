#include <stdint.h>

#include <kernel/memory.h>

#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05) // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR

typedef struct __attribute__((packed)) {
  uint16_t limit_low;  // Lower 16 bits of the limit
  uint16_t base_low;   // Lower 16 bits of the base
  uint8_t base_middle; // Next 8 bits of the base
  uint8_t access;      // Access flags
  uint8_t granularity; // Granularity and high 4 bits of the limit
  uint8_t base_high;   // Last 8 bits of the base
} gdt_entry_t;

typedef struct __attribute__((packed)) {
  uint16_t limit; // The upper 16 bits of all selector limits
  uint32_t base;  // The address of the first gdt_entry struct
} gdt_ptr_t;

gdt_entry_t gdt[5];
gdt_ptr_t gdtp;

extern void _gdt_flush(void);

void gdt_setentry(unsigned int i, uint32_t base, uint32_t limit, uint16_t flag) {
  gdt_entry_t* entry = &gdt[i];

  entry->limit_low = limit & 0xFFFF;
  entry->base_low = base & 0xFFFF;
  entry->base_middle = (base >> 16) & 0xFF;
  entry->access = flag;
  entry->granularity = (limit >> 16) & 0x0F;
  entry->granularity |= (flag >> 8) & 0xF0;
  entry->base_high = (base >> 24) & 0xFF;
}


void gdt_install() {
  gdtp.limit = (sizeof(uint64_t) * 5) - 1;
  gdtp.base = (uint32_t)&gdt;

  gdt_setentry(0, 0, 0, 0);
  gdt_setentry(1, 0, 0xfffff, GDT_CODE_PL0);
  gdt_setentry(2, 0, 0xfffff, GDT_DATA_PL0);
  gdt_setentry(3, 0, 0xfffff, GDT_CODE_PL3);
  gdt_setentry(4, 0, 0xfffff, GDT_DATA_PL3);

  _gdt_flush(); // Defined in boot.s
}

void memory_init(void) {
  gdt_install();
}