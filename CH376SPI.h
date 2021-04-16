/*
   Author: Diego Arnaudin 

   Problems and solutions:
      - Won't open file ([FileOpen]): Format with FAT32 or FAT16, the device does not recognize NTFS format.

*/

#ifndef CH376_H
#define CH376_H

/* Configuration defines */
#define RETRY_DLY 50 
#define RETRY_TIMES 10

/* responses  */
#define ANSW_USB_INT_SUCCESS 0x14
#define ANSW_RET_SUCCESS 0x51
#define ANSW_USB_INT_DISK_READ 0x1d
/* commands */
#define CMD_CHECK_EXIST 0x06
#define CMD_RESET_ALL 0x05
#define CMD_SET_USB_MODE 0x15
#define CMD_DISK_CONNECT 0x30
#define CMD_DISK_MOUNT 0x31
#define CMD_FILE_OPEN 0x32
#define CMD_SET_FILE_NAME 0x2F
#define CMD_BYTE_READ 0x3A
#define CMD_RD_USB_DATA0 0x27
#define CMD_BYTE_RD_GO 0x3b
#define CMD_SET_BAUDRATE 
#define CMD_GET_STATUS 0x22

/* prototypes declaration */

#define CH376Xfer(x) spi_read(x)

#define WriteCH376(x)  spi_write(x)
#define ReadCH376() spi_read(0xff)


//#define StartCmd() fprintf(PORTCH376,"%s",_start)
#define SPISelect() output_low(SDCARD_PIN_SELECT);
#define SPIDeselect() output_high(SDCARD_PIN_SELECT);

char SetMode(char mode);
char ResetAll();
char CheckExists();
char DiskConnect();
char DiskMount();
char SetFileName(char*);
char FileOpen(char*);
char LoadFile(char*);
char ReadBlock(char*, char);
char ByteRead();
char GetStatus();

char InitDevice();

typedef char (*fun_ptr)();
char TryNTimes(fun_ptr func){
   char r=0x00, retrys = 0;      
   do{
      r = func();
     delay_ms(RETRY_DLY);
   }while(!r && (++retrys)<RETRY_TIMES);
   
   return r;
}

typedef char (*fun_ptrP)(char);
char TryNTimesP(fun_ptrP func,char p1){
   char r=0x00, retrys = 0;      
   do{
      r = func(p1);
     delay_ms(RETRY_DLY);
   }while(!r && (++retrys)<RETRY_TIMES);
   
   return r;
}


/* functions definitions */

char GetStatus(){
	int ret = 0;
	
	while(input(SDCARD_PIN_INT)){
			// timeout
			//printf(".");
	}/*
	printf("\n\r[GetStatus]");*/
	SPISelect();
	WriteCH376(CMD_GET_STATUS);
	ret = ReadCH376();
	SPIDeselect();
	
	return ret;
}

char SetMode(char mode){
   char ret = 0;
   
   dbg("\n\r[SetMode]");

   SPISelect();   
   WriteCH376(CMD_SET_USB_MODE);   
   WriteCH376(mode);
   
   delay_us(10);

   ret = ReadCH376();// == ANSW_RET_SUCCESS;
   
   printf("[%x]",ret);
   
   ret = ret == ANSW_RET_SUCCESS;
   
   SPIDeselect();
   
   delay_us(20);
   
   return ret;
}

char ResetAll(){
   dbg("\n\r[ResetAll]");
   
   SPISelect();
   WriteCH376(CMD_RESET_ALL);
   SPIDeselect();
   
   delay_ms(50);

   return 1;
}

char CheckExists(){
   char ret = 0;
   
  // SetInt();
   
   dbg("\n\r[CheckExists]");

   SPISelect();
   WriteCH376(CMD_CHECK_EXIST);
   
   WriteCH376(0x01);
   ret = ReadCH376();
   
   
   printf("[%x]",ret);
   
   ret = ret == 0xFE;
   
   SPIDeselect();
   
   return ret;
   
}

char DiskConnect(){
   char ret = 0;
   
   dbg("\n\r[DiskConnect]");
   SPISelect();
   WriteCH376(CMD_DISK_CONNECT);
   //ret = ReadCH376();
   
   SPIDeselect();
	
	ret = GetStatus();
   
   printf("[%x]",ret);
   
   return ret == ANSW_USB_INT_SUCCESS;
}

long long GetFileSize(){
   dbg("\n\r[GetFileSize]");
   
   SPISelect();
   WriteCH376(0x0c);
   WriteCH376(0x68);
   char a=0,b=0,c=0,d=0;
   a=ReadCH376();
   b=ReadCH376();
   c=ReadCH376();
   d=ReadCH376();
   SPIDeselect();
   
   long long n = (long long) ((long long)d<<24) | ((long long)c<<16) |((long long)b<<8) | a;
   
   return n;
}

char DiskMount(){
   char ret = 0;
   dbg("\n\r[DiskMount]");
   SPISelect();
   WriteCH376(CMD_DISK_MOUNT);
   SPIDeselect();
  // ret = ReadCH376();// == ANSW_USB_INT_SUCCESS;
   ret = GetStatus();   

   printf("[%x]",ret);
   
   return ret  == ANSW_USB_INT_SUCCESS;
}

char SetFileName(char *filename){
   dbg("\n\r[SetFileName]");

   SPISelect();
   WriteCH376(CMD_SET_FILE_NAME);

   int i=0;
   do{
   WriteCH376(filename[i]);
   }while(filename[i++]!='\0');
   
   SPIDeselect();

   return 1; 
}

char FileOpen(){
   char ret = 0;
   dbg("\n\r[FileOpen]");
   SPISelect();
   WriteCH376(CMD_FILE_OPEN);
  // ret = ReadCH376();
   SPIDeselect();
   
   ret = GetStatus();
   
   printf("[%x]",ret);
   
   return ret == ANSW_USB_INT_SUCCESS;
   
}

char ByteRead(){
   dbg("\n\r[ByteRead]");
   char response = 0, retrys = 10;
   
   do{
	SPISelect();
	//delay_ms(50);
   WriteCH376(CMD_BYTE_READ);   
   WriteCH376(0x40);
   WriteCH376(0x00);
     SPIDeselect();
   //response = ReadCH376();
//	delay_ms(50);
	//printf("\n\rrraap");
   response = GetStatus();
 //  printf("|resp:%x, retrys: %d",response, retrys);
   retrys--;
   }while( ((response != ANSW_USB_INT_DISK_READ) /*|| (response != ANSW_USB_INT_SUCCESS) */ ) && retrys>0);
   
 
   
   return  (response == ANSW_USB_INT_DISK_READ) /*|| (response == ANSW_USB_INT_SUCCESS)*/;
}

char ReadBlock(char* buff, char length){   
   int i=0;
   int size=0;

   dbg("\n\r[ReadBlock]");
   SPISelect();
   do{

      WriteCH376(CMD_RD_USB_DATA0);
   
      i = size = ReadCH376();   
   }
   while(size == 0);   
      
   while(i--){
      *(buff++) = ReadCH376();
	  
   }
   SPIDeselect();
   printf("size:%d",size);
   return size;
}


char ByteRdGo(){
   dbg("\n\r[ByteRdGo]");
   
   
   char response = 0, retrys = 10;
   do{
	SPISelect();
   WriteCH376(CMD_BYTE_RD_GO);
    SPIDeselect();
   //response = ReadCH376();
   response = GetStatus();
   retrys--;
   }while( ( (response != ANSW_USB_INT_SUCCESS) /*|| (response != ANSW_USB_INT_DISK_READ)*/  ) && retrys>0);
  
   return (response == ANSW_USB_INT_SUCCESS) /*|| (response == ANSW_USB_INT_DISK_READ) */ ;
   
}

char LoadFile(char *filename){
   int size = 0;
   dbg("\n\r(LoadFile)");
   
   SetFileName(filename);
   
   return FileOpen();
}

char ReadFile( char *buff ){
   int size = 0;
   
   dbg("\n\r[Read Block]");
   
   if (!ByteRead()) { printf("\n\rabrime la puertaa"); return 0;}
   
   ReadBlock(buff,64);   

   return ByteRdGo();
}

void InitSPI(){
   //setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_CLK_DIV_4);
   #ifdef SDCARD_SPI_HW
   setup_spi( spi_master | spi_l_to_h /*| spi_clk_div_16*/ | SPI_XMIT_L_TO_H);
   //#define CH376Xfer(x)    spi_read(x)
   output_float(SDCARD_PIN_INT);
   
   #endif
   
  #ifndef SDCARD_SPI_HW
     #if defined(SDCARD_PIN_SCL)
      output_drive(SDCARD_PIN_SCL);
     #endif
     #if defined(SDCARD_PIN_SDO)
      output_drive(SDCARD_PIN_SDO);
     #endif
     #if defined(SDCARD_PIN_SDI)
      output_float(SDCARD_PIN_SDI);
     #endif
	 
	 spi_init(SPI_USB, 500000);
   #endif
  
   output_drive(SDCARD_PIN_SELECT);
   output_high(SDCARD_PIN_SELECT);
   
   delay_ms(100);
}



char InitDevice(){
   InitSPI();
   
   return (
      ResetAll() &&
      TryNTimes(&CheckExists) &&
      TryNTimesP(&SetMode, 0x05) &&
      TryNTimesP(&SetMode, 0x06) &&
      TryNTimes(&DiskConnect) &&
      TryNTimes(&DiskMount) 
   );
}               

#endif /* CH376_H */
