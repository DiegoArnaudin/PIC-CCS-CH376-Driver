#include <18LF26K22.h>

#fuses   NOWDT
#fuses   INTRC_IO
#fuses   PUT
#fuses   PROTECT
#fuses   NODEBUG
#fuses   BROWNOUT
#fuses   NOLVP
#fuses   NOCPD
#fuses   NOWRT

#use delay(internal=32MHz)

#define CHTX PIN_C0
#define CHRX PIN_C1

#define DBGTX PIN_C6
#define DBGRX PIN_C7

#define SDCARD_PIN_SELECT PIN_C2
#define SDCARD_PIN_INT PIN_C0

//#use rs232(stream=PORTCH376, baud = 9600 ,parity=N,xmit=CHTX,rcv=CHRX,bits=8, timeout=30)

#use rs232(stream=PORTDEBUG, baud = 57600, parity=N, xmit=DBGTX, rcv=DBGRX, bits=8)


#define SDCARD_SPI_HW

#ifndef SDCARD_SPI_HW
   
   #define SDCARD_PIN_SDI PIN_C4
   #define SDCARD_PIN_SDO PIN_C5
   #define SDCARD_PIN_SCL PIN_C3
   
   //#use spi(MASTER, DI=SDCARD_PIN_SDI, DO=SDCARD_PIN_SDO, CLK=SDCARD_PIN_SCL, BITS=8, LSB_FIRST, MODE=3, baud=500000)
   #use spi(MASTER, SPI1, MODE=0,/* baud=500000,*/ stream=SPI_USB, MSB_FIRST)
#endif



#define dbg(a) // fprintf(PORTDEBUG,a)
