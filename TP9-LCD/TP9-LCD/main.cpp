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
			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(lcdHandle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
			{
				/*
				//Examplo to write 0xf0 to the display 

				//(E=0, RS=0, D4-D7=f)...
				info = 0xf0;

				//Finally executes the action "write to LCD"...
				if (FT_Write(lcdHandle, &info, 1, &sizeSent) == FT_OK)
				{
					//If success continue with the program (...)


				}*/


				lcd.init_4_bit_mode(lcdHandle);
				UCHAR bitmode;
				for (int i = 0; i < 7; i++) {
					f_stat = FT_GetBitMode(lcdHandle, &bitmode);
					if (f_stat==FT_OK){
						cout << "el bit " << i <<" tiene valor " << bitmode << endl;
					}
					else {
						cout << "hubo un bit q no se pudo ver el valor" << endl;
					}
				}
				if (lcd.write_byte_to_ir(lcdHandle,20) == FT_OK) {
					cout << "se pudo escribir en el LCD!!!" << endl;
					for (int i = 0; i < 7; i++) {
						f_stat = FT_GetBitMode(lcdHandle, &bitmode);
						if (f_stat == FT_OK) {
							cout << "el bit " << i << " tiene valor " << bitmode << endl;
						}
						else {
							cout << "hubo un bit q no se pudo ver el valor" << endl;
						}
					}
				}
				else
					printf("Error writing to the LCD\n");
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
