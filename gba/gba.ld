/* Joshemon cartridge layout: code and data in ROM, variables in IWRAM (32 KB), big buffers in EWRAM (256 KB). */
OUTPUT_ARCH(arm)
ENTRY(_start)
MEMORY {
  rom   : ORIGIN = 0x08000000, LENGTH = 32M
  iwram : ORIGIN = 0x03000000, LENGTH = 31K
  ewram : ORIGIN = 0x02000000, LENGTH = 256K
}
SECTIONS {
  .text : { KEEP(*(.crt0)) *(.text*) *(.rodata*) . = ALIGN(4); } > rom
  .data : { __data_start = .; *(.data.0qa) *(.data*) *(.iwram*) . = ALIGN(4); __data_end = .; } > iwram AT > rom
  __data_lma = LOADADDR(.data);
  .bss (NOLOAD) : { __bss_start = .; *(.bss*) *(COMMON) . = ALIGN(4); __bss_end = .; } > iwram
  .ewram (NOLOAD) : { *(.ewram*) } > ewram
  /DISCARD/ : { *(.ARM.exidx*) *(.comment) }
}
