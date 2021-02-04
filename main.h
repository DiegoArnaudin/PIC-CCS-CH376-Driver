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

#use delay(crystal=32MHz)

#define CHTX PIN_C0
#define CHRX PIN_C1

#define DBGTX PIN_C5

#use rs232(stream=PORTCH376, baud = 9600 ,parity=N,xmit=CHTX,rcv=CHRX,bits=8, timeout=30)

#use rs232(stream=PORTDEBUG, baud = 57600 ,parity=N,xmit=DBGTX,bits=8)

//#define DEBUG
#ifdef DEBUG
	#define dbg(a) fprintf(PORTDEBUG,a)
	#define dbg2(a,b) fprintf(PORTDEBUG,a,b)
#else
	#define dbg(a) 
	#define dbg2(a,b)
#endif

