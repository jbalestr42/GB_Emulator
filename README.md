[Blargg's test coverage](https://github.com/retrio/gb-test-roms):

| Category            | Test Name                  | Pass/Fail           |
|---------------------|----------------------------|---------------------|
| CPU Instructions    | All                        | :white_check_mark:	 |
| Instructions Timing | Instr Timing               | :white_check_mark:	 |
| Memory Timing       | All                        | :white_check_mark:	 |
| Memory Timing 2     | All                        | :white_check_mark:	 |

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
| Timer            | Rapid toggle      | :white_check_mark: |
| Timer            | Tim00             | :white_check_mark: |
| Timer            | Tim00 Div trigger | :white_check_mark: |
| Timer            | Tim01             | :white_check_mark: |
| Timer            | Tim01 Div trigger | :white_check_mark: |
| Timer            | Tim10             | :white_check_mark: |
| Timer            | Tim10 Div trigger | :white_check_mark: |
| Timer            | Tim11             | :white_check_mark: |
| Timer            | Tim11 Div trigger | :white_check_mark: |
| Timer            | Tima reload       | :white_check_mark: |
| Timer            | Tima write reloa. | :white_check_mark: |
| Timer            | Tma write reload. | :white_check_mark: |
| MBC 1            | all but multicart | :white_check_mark: |
| MBC 2            | all               | :white_check_mark: |
| MBC 5            | all               | :white_check_mark: |