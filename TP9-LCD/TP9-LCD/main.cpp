#include <iostream>
#include<cstdio>
#include<windows.h>
#include "hitachi_lcd.h"

#define FTD2XX_EXPORTS
#include "ftd2xx.h"

using namespace std;
int main()
{

	
	FT_HANDLE lcdHandle;
	unsigned char info = 0x00;
	DWORD sizeSent = 0;
	bool found = false;
	hitachi_lcd lcd;
	FT_STATUS f_stat;
	for (int i = 0; (i < 10) && !found; i++)
	{
		if (FT_Open(i, &lcdHandle) == FT_OK)	//Examples in FTDI guid use 0. But practice shows 1 is usually the case.
		{
			found = true;
			lcd.init_4_bit_mode(lcdHandle);
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(lcdHandle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
			{
				for(int i = 0x41 ; i < 0x41+15 ; i ++){
					if (lcd.write_byte_to_ir(lcdHandle,i) == FT_OK) {
						cout << "se pudo escribir en el LCD!!!" << endl;
					}
					else
						printf("Error writing to the LCD\n");
				}
				
			}
			else
				printf("Couldn't configure LCD\n");

			FT_Close(lcdHandle);
		}
		else
			printf("Couldn't open USB %d\n", i);
	}

	return 0;
}
