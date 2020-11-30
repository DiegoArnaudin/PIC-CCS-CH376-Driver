# PIC CCS CH376 Library
 CH376 Library for Microchip PIC with CCS C Compiler
 
It uses UART to communicate with CH376 USB Host
Actually it only opens a file for reading.

Usage:


	- InitDevice()
		Will try to initialize CH376 device and returns 0 if it fails.

	- LoadFile(char* filename)
		Specify null terminated char array as file name in parameter.
		Internally uses SetFileName(char*) and FileOpen() functions.
		Returns 0 if it fails.

	- GetFileSize() 
		returns a 32 bit wide integer with file size information.	

	- ReadFile(char* buff)
		Reads 64-byte chunks and stores in buff.
		If the end of the file is reached, it returns 0.