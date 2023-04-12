# AmigaDF0ExternalDriveSwap
An Amiga 1000-500-2000-3000 DF0: - External Floppy Drive Swapper
## Theory of operation
This module can swap an internal DF0: drive with an external DF1: floppy drive in order to boot from an external device, such as a Gotek Floppy Emulator.

First module fits under the MOS 8520 Even CIA.

Second module is inline with the internal DF0: drive.

A small CPU detects two fast consecutive resets (CTRL-A-A twice). This will activate or deactivate the floppy swapping.

A piezo will emit sounds when the swapping is activated or deactivated.

This device will fully support external booting from any 880K disks, while still permitting the use of internal DF0: as DF1: (hardware fix)

## Schematic
![image](https://user-images.githubusercontent.com/18539931/231537184-32b204a7-5ace-447a-bb3c-b94747fb4f77.png)
## PCB
### Even CIA (MOS 8520) "sandwich" PCB
![image](https://user-images.githubusercontent.com/18539931/231539229-0ff61863-58a4-4910-abc3-06dba2144de6.png)
![image](https://user-images.githubusercontent.com/18539931/231539365-14722886-b94c-4aea-b1d8-c456901443dc.png)
### Internal DF0: "signal extractor" PCB
![image](https://user-images.githubusercontent.com/18539931/231540679-b935bae1-7c6d-4aef-9848-f54847a5bd53.png)
![image](https://user-images.githubusercontent.com/18539931/231540756-23a5ed65-c647-450b-b778-b3e0bd93b2e5.png)
