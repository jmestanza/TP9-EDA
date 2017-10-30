#include "hitachi_lcd.h"



hitachi_lcd::hitachi_lcd()
{
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
	write_byte_to_dr(h, 0x01);
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
	// ACA DEBERIA HABER UN CLEAR DISPLAY;
}

hitachi_lcd::~hitachi_lcd()
{
}
