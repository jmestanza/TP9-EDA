#pragma once
#include "basic_lcd.h"
#include "windows.h"
#include "ftd2xx.h"
#define ENABLE 0x01
#define RS_IR 0x02
#define RS_DR 0x00


class hitachi_lcd : public basic_lcd 
{
public:
	hitachi_lcd();
	FT_STATUS write_nybble_to_ir(FT_HANDLE h, UCHAR data);
	FT_STATUS write_nybble_to_dr(FT_HANDLE h, UCHAR data);
	FT_STATUS write_byte_to_ir(FT_HANDLE h, UCHAR data);
	FT_STATUS write_byte_to_dr(FT_HANDLE h, UCHAR data);
	void init_4_bit_mode(FT_HANDLE h);
	~hitachi_lcd();
};

