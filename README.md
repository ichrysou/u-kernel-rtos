# u-kernel-rtos

Portable (to be proven in action), preemptive, priority based, Real Time Operating System. Implemented Services:

- Task Management
- Time Management
- Intertask Communication (mailboxes, message-queues)
- Synchronization (semaphores)
- Memory Management (Dummy imlementation)
- Interrupt Handling

## How to cross-compile and run the example application:
### Prerequisites:
GNU ARM Embedded compiler must be installed. For instruction on how to install see [here](https://launchpad.net/gcc-arm-embedded)

### Steps:
1. Navigate via the command line to the Makefile directory:
```sh
$ cd ./u-kernel-rtos/GCC/uKernel/uKernel/Debug
```
2. Initiate build process:
```sh
$ make
```
3. Load the file ./bin/uKernel.hex to your device. (E.g. for MBED LPC1768 copy paste the file to the MBED drive)
4. Have fun!
