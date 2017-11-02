#include <iostream>
#include <cstdio>
#include <windows.h>
#include "hitachi_lcd.h"
#include <string>
#include <iomanip>
#include <sstream>

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
using namespace std;

//unsigned int char2hex(char n);
int main()
{
	//FT_HANDLE lcdHandle;
	//unsigned char info = 0x00;
	//DWORD sizeSent = 0;
	//bool found = false;
	hitachi_lcd lcd;
	
	if (!lcd.lcdInitOk())
	{
		cout << "LCD INITED OK!" << endl;
		lcd << (const unsigned char*)"Hola abcdefghijklmnopqrst";
		lcd.lcdMoveCursorRight();
		lcd.lcdMoveCursorLeft();
		lcd.lcdMoveCursorUp(); 
		lcd.lcdMoveCursorDown();
		
		//lcd.lcdClearToEOL();
		//char c;
		//while ((c = getchar()) != 'q') {
		//	//for (int i = 0x41; i < 0x41 + 15; i++) {
		//	if (c != '\n') {

		//		int aux = int(c);
		//		if (lcd.write_byte_to_ir(lcdHandle, aux) == FT_OK) {
		//			cout << "LCD written!!!" << endl;
		//		}
		//		else {
		//			printf("Error writing to the LCD\n");
		//		}
		//	}

		//		}

		//}
	}

	return 0;
}

/*
unsigned int char2hex(char n) {

	int num = int(n);

	std::stringstream stream;
	stream << std::hex << num;
	std::string result(stream.str());

	cout << "EL NUMERO EN HEXA ES : " << result << endl;
	
	cout << "DEVUELVO" << stoi(&result[0]) << endl;

	return stoi(&result[0]);
}
*/

/*
FT_HANDLE lcdHandle;
unsigned char info = 0x00;
DWORD sizeSent = 0;
bool found = false;
hitachi_lcd lcd;
for (int i = 1; (i < 10) && !found; i++)
{
if (FT_Open(i, &lcdHandle) == FT_OK)	//Examples in FTDI guid use 0. But practice shows 1 is usually the case.
{
found = true;
lcd.init_4_bit_mode(lcdHandle);
UCHAR Mask = 0xFF;	//Selects all FTDI pins.
UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
if (FT_SetBitMode(lcdHandle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
{
char c;
while ((c = getchar()) != 'q') {
//for (int i = 0x41; i < 0x41 + 15; i++) {
if (c != '\n') {

if (c == 'c') {
lcd.clr_display(lcdHandle);
}
else {
int aux = int(c);
if (lcd.write_byte_to_ir(lcdHandle, aux) == FT_OK) {
cout << "LCD written!!!" << endl;
}
else {
printf("Error writing to the LCD\n");
}
}

}

//	}

}
}
else printf("Couldn't configure LCD\n");
FT_Close(lcdHandle);
}
else
printf("Couldn't open USB %d\n", i);
}
return 0;
*/