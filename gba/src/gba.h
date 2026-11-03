// Joshemon GBA hardware registers and helpers. Only what the game uses.
#pragma once
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;

#define REG16(a) (*(volatile u16 *)(a))
#define REG32(a) (*(volatile u32 *)(a))
#define REG_DISPCNT  REG16(0x04000000)
#define REG_VCOUNT   REG16(0x04000006)
#define REG_BG0CNT   REG16(0x04000008)
#define REG_BG1CNT   REG16(0x0400000A)
#define REG_BG2CNT   REG16(0x0400000C)
#define REG_BG3CNT   REG16(0x0400000E)
#define REG_BG0HOFS  REG16(0x04000010)
#define REG_BG0VOFS  REG16(0x04000012)
#define REG_BG1HOFS  REG16(0x04000014)
#define REG_BG1VOFS  REG16(0x04000016)
#define REG_BG2HOFS  REG16(0x04000018)
#define REG_BG2VOFS  REG16(0x0400001A)
#define REG_BG3HOFS  REG16(0x0400001C)
#define REG_BG3VOFS  REG16(0x0400001E)
#define REG_WIN0H    REG16(0x04000040)
#define REG_WIN0V    REG16(0x04000044)
#define REG_WININ    REG16(0x04000048)
#define REG_WINOUT   REG16(0x0400004A)
#define REG_BLDCNT   REG16(0x04000050)
#define REG_BLDALPHA REG16(0x04000052)
#define REG_BLDY     REG16(0x04000054)
#define REG_KEYINPUT REG16(0x04000130)

#define MODE0 0
#define BG0_ON (1 << 8)
#define BG1_ON (1 << 9)
#define BG2_ON (1 << 10)
#define BG3_ON (1 << 11)
#define OBJ_ON (1 << 12)
#define OBJ_1D (1 << 6)
#define BG_CBB(n) ((n) << 2)
#define BG_SBB(n) ((n) << 8)
#define BG_8BPP (1 << 7)
#define BG_PRIO(n) (n)

#define PAL_BG  ((volatile u16 *)0x05000000)
#define PAL_OBJ ((volatile u16 *)0x05000200)
#define VRAM    ((volatile u16 *)0x06000000)
#define OAM     ((volatile u16 *)0x07000000)
#define CHARBLOCK(n) ((volatile u16 *)(0x06000000 + (n) * 0x4000))
#define SCREENBLOCK(n) ((volatile u16 *)(0x06000000 + (n) * 0x800))
#define OBJ_TILES ((volatile u16 *)0x06010000)

enum { KEY_A = 1, KEY_B = 2, KEY_SELECT = 4, KEY_START = 8, KEY_RIGHT = 16, KEY_LEFT = 32, KEY_UP = 64, KEY_DOWN = 128, KEY_R = 256, KEY_L = 512 };

#define RGB15(r, g, b) ((r) | ((g) << 5) | ((b) << 10))

static inline void vsync(void) { while (REG_VCOUNT >= 160); while (REG_VCOUNT < 160); }
