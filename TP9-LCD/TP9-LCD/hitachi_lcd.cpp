#include "hitachi_lcd.h"


hitachi_lcd::hitachi_lcd()
{
	bool found = false;
	error_log = true; // No usable LCD at start
	for (int i = 1; (i < 10) && !found; i++)
	{
		lcd_stat = FT_Open(i, &Handle);
		if (lcd_stat == FT_OK)
			found = true;
	}

	if (found) 
	{
		init_4_bit_mode(Handle);
		UCHAR Mask = 0xFF;	//Selects all FTDI pins.
		UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
		lcd_stat = FT_SetBitMode(Handle, Mask, Mode);
		if (lcd_stat == FT_OK)
		{
			error_log = false; // LCD init succesfull
			cadd = 1;
		}
		else
		{
			FT_Close(Handle);
		}
	}
}

FT_STATUS hitachi_lcd::write_nybble_to_ir(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	DWORD bytes_sent;
	UCHAR temp;
	temp = (data << 4) & 0xF0;
	temp = temp | RS_IR & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret= FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp& ~ENABLE;
	FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1);
	return ret;
}

FT_STATUS hitachi_lcd::write_nybble_to_dr(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	DWORD bytes_sent;
	UCHAR temp;
	temp = (data << 4) & 0xF0;
	temp = temp | RS_DR & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp& ~ENABLE;
	FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1);
	return ret;
}

FT_STATUS hitachi_lcd::write_byte_to_ir(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	UCHAR temp;
	temp = (data >> 4) & 0x0F;
	ret = write_nybble_to_ir(h, temp);
	temp = data & 0x0F;
	ret = write_nybble_to_ir(h, temp);

	return ret;
}

FT_STATUS hitachi_lcd::write_byte_to_dr(FT_HANDLE h, UCHAR data)
{
	FT_STATUS ret;
	UCHAR temp;
	temp = (data >> 4) & 0x0F;
	ret = write_nybble_to_dr(h, temp);
	temp = data & 0x0F;
	ret = write_nybble_to_dr(h, temp);

	return ret;
}

void hitachi_lcd::clr_display(FT_HANDLE h)
{
	write_byte_to_dr(h, LCD_CLEAR);
}


void hitachi_lcd::init_4_bit_mode(FT_HANDLE h)
{
	write_nybble_to_ir(h, 0x03);
	Sleep(4);
	write_nybble_to_ir(h, 0x03);
	Sleep(100);
	write_nybble_to_ir(h, 0x03);
	write_nybble_to_ir(h, 0x02);

	clr_display(h);
}

bool hitachi_lcd::lcdInitOk()
{
	return error_log;
}

bool hitachi_lcd::lcdClear()
{
	clr_display(Handle);
	cadd = 1;
	return true;
}

basic_lcd & hitachi_lcd::operator<<(const unsigned char c)
{
	this->write_byte_to_ir(Handle, int(c));
	return *this;
}

basic_lcd & hitachi_lcd::operator<<(const unsigned char * c)
{
	unsigned int i = 0;
	while (c[i] != NULL)
	{
		this->write_byte_to_ir(Handle, int(c[i]));
		i++;
	}
	return *this;
}

hitachi_lcd::~hitachi_lcd()
{
}
