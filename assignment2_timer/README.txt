This code will execute delay in milliseconds through 8254 PIT

 
Follow these steps to build the program after cloning:

#1
$ make all

this will create timer1.ko file

#2

$ sudo insmod timer1.ko

This will load the device driver into kernel.

#3

$ gcc timer1_user.c -o timer1_user

this will compile timer1_user.c

#4

$ sudo ./timer1_user


The program will ask to enter 1 or enter 2 :
if we enter 1 then
> Read current timer counter value.
if we enter 2 then
> Insert delay (in milliseconds).
Selection of this choice will ask for delay in milliseconds.
after entering value, the delay will be executed.
