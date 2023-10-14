# Register Decoding

## Introduction

Most instructions manipulate or use registers. For instance, adding two numbers may require up to _three_ registers (two sources and a destination). This implies some mechanism to control which registers are to be used, is necessary.

Essentially, there are two options:
1. Embed the registers to work with in the opcode itself.
2. Specify the registers to work with _separately_ from the opcode.

### Embedding working registers in the opcode

The first approach decides which registers to work on by having a specific opcode for each (legal) combination of registers. For example, the opcodes 0x01, 0x02, 0x03 and 0x04 may all represent negation operations, but on different registers. Their respective assembly equivalents may, for instance, be `neg r0, r0`, `neg r0, r1`, `neg r1, r0` and `neg r1, r1`.

This approach bloats the number of instructions by having to include an exponentially growing number of instructions as the number of registers grow, if all register combinations are allowed. However, if the control unit uses an EEPROM that is programmed by a script (which it will in this case), it reduces hardware complexity.

### Specifying working registers outside the opcode

The other option is to have a general instruction for each operation (for example a generic `neg` instruction), and specify some registers as "arguments" to that instruction. The decoder (EEPROM) then simply interprets `neg r0, r1` as a negation instruction. In fact, the register "arguments" are never seen to the EEPROM.

Instead, the EEPROM now has three general purpose register-related outputs:
- enable the source A register, whatever it is
- enable the source B register, whatever it is
- load the destination register, whatever it is

The extra hardware uses these three control lines together with the 9 bits (3 each for source A, source B and destination) that specify which specific registers to work with, to determine which enables and loads to activate.

This method requires significantly fewer opcodes than the first. As a matter of fact, an _arbitrary_ number of registers can be added without influencing the the rest of the decoding logic (EEPROM) and instruction count (we are, however, limited by the 16 bits that make up each instruction in this architecture).

### The issue with the first approach

As described, the first option reduces the hardware complexity of the control unit. However, since we will be using EEPROMs to do the decoding logic, we are limited by the number of address pins available. Most acceptably priced EEPROMs have 13 address lines.

If we were to use the first approach (and allow any register combination), we would need
- 3 bits for the T state counter
- 5 bits that actually specify which operation to do (the first bits of the opcode)
- 9 bits (3x3) that specify the registers to work with.

This totals to 17 address bits. The issue is that EEPROMs this large are usually very expensive.

Going with the second approach, we only need the first 3 + 5 = 8 bits, fitting well into the standard 13-bit EEPROMs. The remaining 9 bits are decoded in hardware. This decoding logic need not be very complicated (see the `.circ` file).
