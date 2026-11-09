# The Ultimate Computer, Ever. Version 1

The TUCE-V1 is a hardware Brainfuck interpreter.

---

# TUCE Registers and Flags
| Abbreviation | Full Name | Size |
| --- | --- | --- |
| PC | Program Counter | 16 Bits |
| MP | Memory Pointer | 16 Bits |
| DT | Depth Tracker | 16 Bits |
| INR | Input Register | 8 Bits |
| OUTR | Output Register | 8 Bits |
| IR | Instruction Register | 8 Bits |
| MIR | Memory Input Register | 8 Bits |
| MOR | Memory Output Register | 8 Bits |
| OWF | Output Write Flag | 1 Bit |
| MOWF | Memory Output Write Flag | 1 Bit |
| FLF | Forward Loop Flag | 1 Bit |
| BLF | Backward Loop Flag | 1 Bit |
| IDU | Increment Decrement Unit | 8 Bits |
| SC | Stage Counter | 2 Bits |

# Other Abbreviations
| Abbreviation | Full Name |
| --- | --- |
| CLK | Clock Input |
| RST | Reset |
| BF | Brainfuck |
| LM | Loop Mode |
| NM | Normal Mode |
| IO | Input / Output |

# TUCE Modes
| Mode | Description |
| --- | --- |
| Normal | TUCE will interpret instructions as normal.
| Loop | TUCE will ignore any instruction that isn't `[` or `]`

# Brainfuck Instructions
| Instruction | Function | Binary Value |
|---|---|---|
| `N` | Do nothing | 00000000 |
| `>` | Increment the MP by one | 00000001 |
| `<` | Decrement the MP by one | 00000010 |
| `+` | Increment the current cell by one | 00000100 |
| `-` | Decrement the current cell by one | 00001000 |
| `.` | Output the current cell | 00010000 |
| `,` | Input to the current cell | 00100000 |
| `[` | If the current cell is zero, move `PC` forwards to the instruction after the matching `]` | 01000000 |
| `]` | If the current cell is non-zero, move `PC` backwards to the instruction after the matching `[` | 10000000 |

- Please note that `N` is not a real BF instruction, and only exists on the TUCE because of the way instructions are decoded.
- Instructions with values other than described above will produce undefined results.

---
# TUCE Stages and Operation

- Rise refers to when `CLK` goes high.
- Fall refers to when `CLK` goes low.
- SC increments every time `CLK` goes low.

## S1 – Initialize  (Both Modes)

### Rise
- Load `IR`

### Fall
| Instruction | Operation |
| --- | --- |
| `>` | Increment `MP` |
| `<` | Decrement `MP` |
| `+` `-` `.` `[` `]` | Load `MIR` |
| `,` | Load `INR` |
---
# TUCE Stages and Operation: Normal Mode
## S2 – Memory / IO
### Rise
| Instruction | Operation |
|---|---|
| `[` | Set `FLF` if `MIR` is zero |
| `]` | Set `BLF` if `MIR` is non-zero |

### Fall
| Instruction | Operation |
|---|---|
| `+` `-` | Load `MOR` with `IDU` |
| `.` | Load `OUTR` with `MIR` |
| `,` | Load `MOR` with `INR` |
| `+` `-` `,` | Set `MOWF` |
| `.` | Set `OWF` |

## S3 – Delay

- This does nothing but waste a cycle.
- It does this so memory and IO devices have a clock cycle to use the data that was output.

---

# TUCE Stages and Operation: Loop Mode

## S2 - Instruction Checking
### Rise
- Nothing.

### Fall
| Flag | Operation |
|---|---|
| `FLF` | If `IR` is a matching `]`, decrement `DT`; otherwise, increment `DT`. |
| `BLF` | If `IR` is a matching `[`, decrement `DT`; otherwise, increment `DT`. |

## S3 - Depth Checking

### Rise
- If `DT` is equal to zero, clear loop flags.

### Fall
- Nothing.

---

# S4 – Finalize (Both Modes)

### Rise
| Mode / Flag | Operation |
| --- | --- |
| `NM` `FLF` | Increment `PC` |
| `BLF` | Decrement `PC` |
| `NM` `FLF` `BLF` | Clear `MOWF`, `OWF`, `OUTR`, `MOR`, `MIR`, `INR`, and `IR` |

### Fall
- Nothing.