@ The .globl GetGpioAddress command is a message to the assembler to make
@ the label GetGpioAddress accessible to all files. This means that in our
@ main.s file we can branch to the label GetGpioAddress even though it is
@ not defined in that file.
@
@ lr is the address to branch back to when a function is finished, but this
@ does have to contain the same address after the function has finished. So
@ when GetGpioAddress is called with bl(branch link) the next instruction address
@ will be stored in lr, then when we return from GetGpioAddress lr is moved
@ back into pc.
.globl GetGpioAddress
GetGpioAddress:
ldr r0,=0x3F200000
mov pc,lr



@ One of the first things we should always think about when writing functions is
@ our inputs. What do we do if they are wrong? In this function, we have one input
@ which is a GPIO pin number, and so must be a number between 0 and 53, since
@ there are 54 pins. Each pin has 8 functions, numbered 0 to 7 and so the function
@ code must be too. We could just assume that the inputs will be correct, but this
@ is very dangerous when working with hardware, as incorrect values could cause
@ very bad side effects. Therefore, in this case, we wish to make sure the inputs
@ are in the right ranges.
@
@ To do this we need to check that r0 <= 53 and r1 <= 7. First of all, we can use
@ the comparison we've seen before to compare the value of r0 with 53. The next
@ instruction, cmpls is a normal comparison instruction that will only be run if
@ r0 was lower than or the same as 53. If that was the case, it compares r1 with
@ 7, otherwise the result of the comparison is the same as before. Finally we go
@ back to the code that ran the function if the result of the last comparison was
@ that the register was higher than the number.
@
@ The effect of this is exactly what we want. If r0 was bigger than 53, then the
@ cmpls command doesn't run, but the movhi does. If r0 is <= 53, then the cmpls
@ command does run, and so r1 is compared with 7, and then if it is higher than 7,
@ movhi is run, and the function ends, otherwise movhi does not run, and we know
@ for sure that r0 <= 53 and r1 <= 7.

.globl SetGpioFunction
SetGpioFunction:
cmp r0,#53
cmpls r1,#7
movhi pc,lr


@ These next three commands are focused on calling our first method. The push {lr}
@ command copies the value in lr onto the top of the stack, so that we can
@ retrieve it later. We must do this because when we call GetGpioAddress, we will
@ need to use lr to store the address to come back to in our function.
@
@ If we did not know anything about the GetGpioAddress function, we would have to
@ assume it changes r0,r1,r2 and r3, and would have to move our values to r4 and
@ r5 to keep them the same after it finishes. Fortunately, we do know about
@ GetGpioAddress, and we know it only changes r0 to the address, it doesn't affect
@ r1,r2 or r3. Thus, we only have to move the GPIO pin number out of r0 so it
@ doesn't get overwritten, but we know we can safely move it to r2, as
@ GetGpioAddress doesn't change r2.
@
@ Finally we use the bl instruction to run GetGpioAddress. Normally we use the
@ term 'call' for running a function, and I will from now. As discussed earlier
@ bl calls a function by updating the lr to the next instruction's address, and
@ then branching to the function.
@
@ When a function ends we say it has 'returned'. When the call to GetGpioAddress
@ returns, we now know that r0 contains the GPIO address, r1 contains the function
@ code and r2 contains the GPIO pin number.

push {lr}
mov r2,r0
bl GetGpioAddress


@ I mentioned earlier that the GPIO functions are stored in blocks of 10, so first
@ we need to determine which block of ten our pin number is in. This sounds like a
@ job we would use a division for, but divisions are very slow indeed, so it is
@ better for such small numbers to do repeated subtraction.
@
@ This simple loop code compares the pin number to 9. If it is higher than 9, it
@ subtracts 10 from the pin number, and adds 4 to the GPIO Controller address then
@ runs the check again.
@
@ The effect of this is that r2 will now contain a number from 0 to 9 which
@ represents the remainder of dividing the pin number by 10. r0 will now contain
@ the address in the GPIO controller of this pin's function settings. This would
@ be the same as GPIO Controller Address + 4 × (GPIO Pin Number ÷ 10).

functionLoop$:
cmp r2,#9
subhi r2,#10
addhi r0,#4
bhi functionLoop$


@ This code finishes off the method. The first line is actually a multiplication
@ by 3 in disguise. Multiplication is a big and slow instruction in assembly code,
@ as the circuit can take a long time to come up with the answer. It is much
@ faster sometimes to use some instructions which can get the answer quicker. In
@ this case, I know that r2 × 3 is the same as r2 × 2 + r2. It is very easy to
@ multiply a register by 2 as this is conveniently the same as shifting the binary
@ representation of the number left by one place.
@
@ One of the very useful features of the ARMv6 assembly code language is the
@ ability to shift an argument before using it. In this case, I add r2 to the
@ result of shifting the binary representation of r2 to the left by one place.
@ In assembly code, you often use tricks such as this to compute answers more
@ easily, but if you're uncomfortable with this, you could also write something
@ like mov r3,r2; add r2,r3; add r2,r3.
@
@ Now we shift the function value left by a number of places equal to r2. Most
@ instructions such as add and sub have a variant which uses a register rather
@ than a number for the amount. We perform this shift because we want to set the
@ bits that correspond to our pin number, and there are three bits per pin.
@
@ We then store the the computed function value at the address in the GPIO
@ controller. We already worked out the address in the loop, so we don't need to
@ store it at an offset like we did in OK01 and OK02.
@
@ Finally, we can return from this method call. Since we pushed lr onto the stack,
@ if we pop pc, it will copy the value that was in lr at the time we pushed it
@ into pc. This would be the same as having used mov pc,lr and so the function
@ call will return when this line is run.
@
@ The very keen may notice that this function doesn't actually work correctly.
@ Although it sets the function of the GPIO pin to the requested value, it causes
@ all the pins in the same block of 10's functions to go back to 0! This would
@ likely be quite annoying in a system which made heavy use of the GPIO pins.

add r2, r2,lsl #1
lsl r1,r2
str r1,[r0]
pop {pc}
