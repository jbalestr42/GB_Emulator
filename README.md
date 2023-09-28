Blargg's test coverage:

| Category            | Test Name                  | Pass/Fail           |
|---------------------|----------------------------|---------------------|
| CPU Instructions    | 01 - Special               | :white_check_mark:	 |
| CPU Instructions    | 02 - Interrupts            | :white_check_mark:	 |
| CPU Instructions    | 03 - Op SP, HL             | :white_check_mark:	 |
| CPU Instructions    | 04 - Op r,imm              | :white_check_mark:	 |
| CPU Instructions    | 05 - Op rp                 | :white_check_mark:	 |
| CPU Instructions    | 06 - LD r,r                | :white_check_mark:	 |
| CPU Instructions    | 07 - JR,JP,Call,RET,RST    | :white_check_mark:	 |
| CPU Instructions    | 08 - Misc Instrs           | :white_check_mark:	 |
| CPU Instructions    | 09 - Op r,r                | :white_check_mark:	 |
| CPU Instructions    | 10 - Bits Ops              | :white_check_mark:	 |
| CPU Instructions    | 11 - Op a,(hl)             | :white_check_mark:	 |
| Instructions Timing | Instr Timing               | :white_check_mark:	 |

[Acid2 test](https://github.com/mattcurrie/dmg-acid2):

| Reference image                                      | Pass/Fail          |
|------------------------------------------------------|--------------------|
| ![Demo](tests/dmg-acid2-ref.png)                     | :white_check_mark: |

[Mooneye test suite](https://github.com/Gekkio/mooneye-test-suite):

| Category         | Test Name         | Pass/Fail          |
|------------------|-------------------|--------------------|
| CPU              | DAA               | :white_check_mark: |
| Bits             | MEM OAM           | :white_check_mark: |
| Bits             | Reg F             | :white_check_mark: |
| Bits             | unused_hwio-GS    | :x:                |
| OAM DMA          | Basic             | :white_check_mark: |
| OAM DMA          | reg_read          | :x:                |
| Timer            | Div Write         | :white_check_mark: |
| Timer            | Rapid toggle      | :x:                |
| Timer            | Tim00             | :white_check_mark: |
| Timer            | Tim00 Div trigger | :x:                |
| Timer            | Tim01             | :white_check_mark: |
| Timer            | Tim01 Div trigger | :x:                |
| Timer            | Tim10             | :white_check_mark: |
| Timer            | Tim10 Div trigger | :x:                |
| Timer            | Tim11             | :white_check_mark: |
| Timer            | Tim11 Div trigger | :x:                |
| Timer            | Tima reload       | :x:                |
| Timer            | Tima write reloa. | :x:                |
| Timer            | Tma write reload. | :x:                |
| MBC 1            | bits_bank_1       | :white_check_mark: |
| MBC 1            | bits_bank_2       | :white_check_mark: |
| MBC 1            | bits_mode         | :white_check_mark: |
| MBC 1            | bits_ramg         | :white_check_mark: |
| MBC 1            | multicart_rom_8Mb | :x:                |
| MBC 1            | ram_64kb          | :white_check_mark: |
| MBC 1            | ram_256kb         | :white_check_mark: |
| MBC 1            | rom_512kb         | :white_check_mark: |
| MBC 1            | rom_1Mb           | :white_check_mark: |
| MBC 1            | rom_2Mb           | :white_check_mark: |
| MBC 1            | rom_4Mb           | :white_check_mark: |
| MBC 1            | rom_8Mb           | :white_check_mark: |
| MBC 1            | rom_16Mb          | :white_check_mark: |
| MBC 2            | bits_ramg         | :white_check_mark: |
| MBC 2            | bits_romb         | :white_check_mark: |
| MBC 2            | bits_unused       | :white_check_mark: |
| MBC 2            | ram               | :white_check_mark: |
| MBC 2            | rom_512kb         | :white_check_mark: |
| MBC 2            | rom_1Mb           | :white_check_mark: |
| MBC 2            | rom_2Mb           | :white_check_mark: |