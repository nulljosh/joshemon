@ Joshemon cartridge start-up: header, stacks, copy .data/.iwram to RAM, clear .bss, call main.
    .section .crt0, "ax"
    .global _start
    .arm
_start:
    b       reset
    @ Header: logo area left blank (emulators boot without it); gbafix.py fills title and checksum.
    .fill   156, 1, 0
    .ascii  "JOSHEMON\0\0\0\0"      @ title, 12 bytes
    .ascii  "AJSE"                  @ game code
    .ascii  "01"                    @ maker
    .byte   0x96, 0, 0              @ fixed value, unit, device
    .fill   7, 1, 0
    .byte   0                       @ version
    .byte   0                       @ complement checksum (gbafix.py)
    .byte   0, 0

reset:
    mov     r0, #0x12               @ IRQ mode stack
    msr     cpsr_c, r0
    ldr     sp, =0x03007FA0
    mov     r0, #0x1F               @ system mode stack
    msr     cpsr_c, r0
    ldr     sp, =0x03007D00          @ leaves the IRQ stack 672 bytes for the sound interrupt

    ldr     r0, =__data_lma         @ .data and IWRAM code from ROM to RAM
    ldr     r1, =__data_start
    ldr     r2, =__data_end
1:  cmp     r1, r2
    ldrlo   r3, [r0], #4
    strlo   r3, [r1], #4
    blo     1b

    ldr     r1, =__bss_start        @ clear .bss
    ldr     r2, =__bss_end
    mov     r3, #0
2:  cmp     r1, r2
    strlo   r3, [r1], #4
    blo     2b

    ldr     r0, =main
    mov     lr, pc
    bx      r0
3:  b       3b
    .pool
