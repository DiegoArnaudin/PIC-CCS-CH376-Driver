#include <main.h>
#include <CH376.h>

void printbuff(char block, *buff){
	int b,i;
	for(b=0;b<4;b++){
		fprintf(PORTDEBUG,"\r\n[%04lu]",(long)(block)*64+(b*16));
		for(i=0;i<16;i++)
			fprintf(PORTDEBUG,"%x",buff[b*16+i]);
	}
}

void printbar(long block,long totalblocks){
    char bar[11] = {".........."};

    long pos = (long)(block)*10/totalblocks;

    bar[pos] = '|';

    fprintf(PORTDEBUG, "\r%s",bar);
}

char TryLoadFile(char *file){
	
	char buff[64] = {0};
	
	if( LoadFile(file) ){
		long long size = GetFileSize();
		long fileblocks = ((float)(size)/64)-(long long)(size/64) > 0 ? (long long)(size/64) + 1 :  (long long)(size/64) ;
		fprintf(PORTDEBUG,"\r\nsize: %lu bytes, blocks: %lu\r\n", size, fileblocks );

		dbg("Reading\n\r");
		long blocks = 0;

		while(ReadFile(buff)) 
		{
			/* Write Buff to flash memory */
			
			
			printbar(blocks, fileblocks);			
			blocks++;
		}
		fprintf(PORTDEBUG,"\n\r");

		fprintf(PORTDEBUG,"\r\nfileblocks:%lu, blocks readed: %lu", fileblocks, blocks);


		return (fileblocks==blocks);
	}
	else
		return 0;
}

void main()
{
	int times = 5, deviceOk=0;
	while(times-- && !deviceOk){
		if(InitDevice()) deviceOk=1;
	}
	if(!deviceOk){
		fprintf(PORTDEBUG,"\r\nNo media present");
		while(1);
	}

	dbg("Init Ok.\n\r");	
	const char *f1 = {"/DATA.BIN"};
	int TryTimes = 3, SuccessLoad = 0;
	while( !SuccessLoad && TryTimes-- ){
		SuccessLoad = TryLoadFile(f1);
		fprintf(PORTDEBUG,"\r\nTry.");
		delay_ms(2000);
	}
	
	if(!SuccessLoad)
		fprintf(PORTDEBUG,"\r\nfailure while loading file");
	else
		fprintf(PORTDEBUG,"\r\nfile load success");
	
	while(TRUE);
}
