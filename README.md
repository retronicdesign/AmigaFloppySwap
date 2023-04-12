# Amiga Floppy Swap
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
![image](https://user-images.githubusercontent.com/18539931/231581277-1eed8321-dfd0-4dcd-9db9-6d271217da2a.png)
![image](https://user-images.githubusercontent.com/18539931/231581348-d27fab04-f03f-4aac-bed5-ddacad86fa6f.png)
### Internal DF0: "signal extractor" PCB
![image](https://user-images.githubusercontent.com/18539931/231581458-66220dfb-2083-49b2-8f01-d19089d4dd1e.png)
![image](https://user-images.githubusercontent.com/18539931/231581515-ed4e5da0-7c8b-4ccf-a66c-06147e8ddd4a.png)
* Don't forget to add 2 wires between the two boards!

This will feed signal MTRON and READY to and from the main circuit thus enabling the magic with internal drive!
