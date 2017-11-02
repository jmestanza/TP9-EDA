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
	cursorPosition ex;
	ex.row = 1;
	ex.column = 15;
	hitachi_lcd lcd;
	
	if (!lcd.lcdInitOk())
	{
		cout << "--------------------" << endl;
		cout << "| TP9 - LCD Fase 1 |" << endl;
		cout << "--------------------" << endl;
		cout << ">> LCD Hitachi - HD44780 inicializacion OK" << endl;
		cout << ">> Presione ENTER para mostrar el texto de prueba por default" << endl;
		getchar();
		lcd << "The quick brown fox";
		cout << ">> ENTER para mover el cursor a la linea de arriba" << endl;
		getchar();
		lcd.lcdMoveCursorUp();
		cout << ">> ENTER para borrar desde la posicion actual hasta el final de linea" << endl;
		getchar();
		lcd.lcdClearToEOL();
		cout << ">> ENTER para mover el cursor un lugar a la derecha" << endl;
		getchar();
		lcd.lcdMoveCursorRight();
		cout << ">> ENTER para mover el cursor a la fila de abajo" << endl;
		getchar();
		lcd.lcdMoveCursorDown();
		cout << ">> ENTER para mover el cursor un lugar a la izquierda" << endl;
		getchar();
		lcd.lcdMoveCursorLeft();
		cout << ">> ENTER para mover el cursor a la fila de arriba" << endl;
		getchar();
		lcd.lcdMoveCursorUp();
		cout << ">> Ingrese un mensaje para mostrar en el LCD y presione ENTER" << endl;
		cout << ">> ";
		string aux;
		cin >> aux;
		lcd << aux.c_str();
		cout << ">> Presione ENTER para obtener la posicion actual del cursor" << endl;
		getchar();
		ex = lcd.lcdGetCursorPosition();
		cout << "		ROW: " << ex.row << " COLUMN: " << ex.column << endl;
		cout << "Presione ENTER para limpiar todo el LCD" << endl;
		getchar();
		lcd.lcdClear();
		cout << ">> Testing succesfull. Presione ENTER para salir.-" << endl;
		getchar();

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