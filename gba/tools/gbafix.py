"""Fill the cartridge header complement checksum (0xBD) so the header is valid. Usage: gbafix.py rom.gba"""
import sys

rom = bytearray(open(sys.argv[1], 'rb').read())
rom[0xBD] = (-(sum(rom[0xA0:0xBD]) + 0x19)) & 0xFF
open(sys.argv[1], 'wb').write(rom)
