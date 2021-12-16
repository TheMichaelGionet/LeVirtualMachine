# LeVirtualMachine
64 bit RISC-V virtual machine


Special Addresses:
0x100000001 - write a byte to print the ascii character
0x100000002 - write an integer (byte to dword acceptable) (signed if you write a 64 bit integer, unsigned if you write an 8-32 bit integer)
0x10000000a - write a hex value (byte to dword acceptable)
0x100000020 - write a byte of 0 to this address to terminate the VM
0x100000030 - read a byte from this to obtain the next character from stdin (it is blocking, I could add a non-blocking version or a "get key state" method)
