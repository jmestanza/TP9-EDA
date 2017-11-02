#include "hitachi_lcd.h"
#define HITACHI_LINE 40
#define LCD_LINE 16

hitachi_lcd::hitachi_lcd()
{
	bool found = false;
	cadd = 1;
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
	temp = (temp | RS_IR) & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret= FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp& (~ENABLE);
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
	temp = (temp | RS_DR) & ~ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(1); // 1ms
	temp = temp | ENABLE;
	ret = FT_Write(h, &temp, sizeof(temp), &bytes_sent);
	Sleep(3); // 3 ms 
	temp = temp & (~ENABLE);
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
	lcd_stat = write_byte_to_dr(h, LCD_CLEAR);
}


void hitachi_lcd::init_4_bit_mode(FT_HANDLE h)
{
	lcd_stat = write_nybble_to_ir(h, 0x03);
	Sleep(5);
	lcd_stat = write_nybble_to_ir(h, 0x03);
	Sleep(0.2);
	lcd_stat = write_nybble_to_ir(h, 0x03);
	lcd_stat = write_nybble_to_ir(h, 0x02);

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

bool hitachi_lcd::lcdClearToEOL()
{
	if (cadd%LCD_LINE == 0)
	{
		return false;
	}
	else
	{
		int aux = 0;
		if (cadd <= LCD_LINE)
		{
			for (aux = 0; aux < (LCD_LINE - cadd); aux++)
			{
				lcd_stat = write_byte_to_ir(Handle, EMPTY_CHAR);
			}
			for (int i = 0; i < aux; i++)
			{
				lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
			}
		}
		else
		{
			for (aux = 0; aux < ((2*LCD_LINE) - cadd); aux++)
			{
				lcd_stat = write_byte_to_ir(Handle, EMPTY_CHAR);
			}
			for (int i = 0; i < aux; i++)
			{
				lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
			}
		}
		return true;
	}
}

basic_lcd & hitachi_lcd::operator<<(const unsigned char c)
{
	this->write_byte_to_ir(Handle, int(c));
	if (cadd%LCD_LINE == 0)
	{
		for (int i = 0; i < (HITACHI_LINE - LCD_LINE); i++)
		{
			lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		}
		if (cadd == (2 * LCD_LINE))
		{
			cadd = 1;
		}
		else
		{
			cadd++;
		}
	}
	else
	{
		cadd++;
	}
	return *this;
}

basic_lcd & hitachi_lcd::operator<<(const unsigned char * c)
{
	unsigned int i = 0;
	while (c[i] != NULL)
	{
		this->write_byte_to_ir(Handle, int(c[i]));
		i++;
		if (cadd%LCD_LINE == 0)
		{
			for (int i = 0; i < (HITACHI_LINE - LCD_LINE); i++)
			{
				lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
			}
			if (cadd == (2 * LCD_LINE))
			{
				cadd = 1;
			}
			else
			{
				cadd++;
			}
		}
		else
		{
			cadd++;
		}
	}
	return *this;
}

bool hitachi_lcd::lcdMoveCursorUp()
{
	if (cadd <= LCD_LINE)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < HITACHI_LINE; i++)
		{
			lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
		}
		cadd -= LCD_LINE;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorDown()
{
	if (cadd >= (LCD_LINE + 1))
	{
		return false;
	}
	else
	{
		for (int i = 0; i < HITACHI_LINE; i++)
		{
			lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		}
		cadd += LCD_LINE;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorRight()
{
	if (cadd%LCD_LINE == 0)
	{
		return false;
	}
	else
	{
		lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_R));
		cadd++;
		return true;
	}
}

bool hitachi_lcd::lcdMoveCursorLeft()
{
	if ((cadd == 1) || (cadd == (LCD_LINE + 1)))
	{
		return false;
	}
	else
	{
		lcd_stat = write_byte_to_dr(Handle, (LCD_CURSOR_MOVE | CURSOR_MOVE_L));
		cadd--;
		return true;
	}
}

bool hitachi_lcd::lcdSetCursorPosition(const cursorPosition pos)
{
	if ((pos.row < 0) || (pos.row > 1) || (pos.column < 0) || (pos.column > 15))
	{
		return false;
	}
	else
	{
		if (cadd < (LCD_LINE+1))
		{
			if (pos.row == 1)
			{
				lcdMoveCursorDown();
				while (cadd != (LCD_LINE + 1))
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1 + LCD_LINE))
				{
					lcdMoveCursorRight();
				}
			}
			else
			{
				while (cadd != 1)
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1))
				{
					lcdMoveCursorRight();
				}
			}
		}
		else
		{
			if (pos.row == 0)
			{
				lcdMoveCursorUp();
				while (cadd != 1)
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1))
				{
					lcdMoveCursorRight();
				}
			}
			else
			{
				while (cadd != (LCD_LINE + 1))
				{
					lcdMoveCursorLeft();
				}
				while (cadd != (pos.column + 1 + LCD_LINE))
				{
					lcdMoveCursorRight();
				}
			}
		}
		return true;
	}
}

cursorPosition hitachi_lcd::lcdGetCursorPosition()
{
	cursorPosition aux;
	if (cadd > LCD_LINE)
	{
		aux.row = 1;
		aux.column = cadd - LCD_LINE - 1;
	}
	else
	{
		aux.row = 0;
		aux.column = cadd - 1;
	}
	return aux;
}

hitachi_lcd::~hitachi_lcd()
{
}
