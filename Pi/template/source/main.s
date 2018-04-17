/******************************************************************************
*   main.s
*    by Alex Chadwick
*
*   A sample assembly code implementation of the ok02 operating system, that
*   simply turns the OK LED on and off repeatedly.
*   Changes since OK01 are marked with NEW.
*
*       -- RPI2 version
******************************************************************************/


// .section is a directive to our assembler telling it to place this code first.
// .globl is a directive to our assembler, that tells it to export this symbol
// to the elf file. Convention dictates that the symbol _start is used for the
// entry point, so this all has the net effect of setting the entry point here.
// Ultimately, this is useless as the elf itself is not used in the final
// result, and so the entry point really doesn't matter, but it aids clarity,
// allows simulators to run the elf, and also stops us getting a linker warning
// about having no entry point.

.section .init
.globl _start
_start:


// This command loads the physical address of the GPIO region into r0.

ldr r0,=0x3f200000
// We need to set up the GPIO peripheral. This actulally means that because the GPIO
// pins can serve different purposes, but we are interested in them acting as input or
// output. For making the LED flash we need to set up the 47th pin to act as an ouput.
// This is because the LED is set up to be controlled by the 47th pin. The GPIO has 54 pins
// and 24bytes to control these pins. The first 4bytes refer to the first 10 pins. 4bytes x 6 = 24bytes.
// Each pin has three bits which control how the pin functions.
//
// We want to select the 47th pin which will be in the 5th out of 6 sets of 4bytes. Registers are 0 indexed
// so we want GPFSEL4(GPIO Function Select 4) which is at 0x3F20 0010. Each pin is 3 bits and sel4 starts at 40 so
// we want to be working with 7x3=21 -> (8x3)-1=23 bits after the start of sel4. Setting as an output means Setting
// it to be 001. Therefore we shift 1 21 places left and place it in r1.

// Our register use is as follows:
// r0=0x3f200000 the address of the GPIO region.
// r1=0x00200000 a number with bits 21-23 set to 001 to put into the GPIO
//               function select to enable output to GPIO 47.
// then
// r1=0x00010000 a number with bit 15 high, so we can communicate with GPIO 47.
// r2=0x003F0000 a number that will take a noticeable duration for the processor
//               to decrement to 0, allowing us to create a delay.

mov r1,#1
lsl r1,#21


// Set the GPIO function select.

str r1,[r0,#0x10]


// Set the 15th bit of r1.

mov r1,#1
lsl r1,#15

// NEW
// Label the next line loop$ for the infinite looping

loop$:


// Set GPIO 47 to low, causing the LED to turn on.

str r1,[r0,#0x2c]

// NEW
// Now, to create a delay, we busy the processor on a pointless quest to
// decrement the number 0x3F0000 to 0!
// bne means branch if not equal to the named branch.

mov r2,#0x3F0000
wait1$:
    sub r2,#1
    cmp r2,#0
    bne wait1$

// NEW
// Set GPIO 47 to high, causing the LED to turn off.

str r1,[r0,#0x20]

// NEW
// Wait once more.

mov r2,#0x3F0000
wait2$:
    sub r2,#1
    cmp r2,#0
    bne wait2$

// Loop over this process forevermore

b loop$
