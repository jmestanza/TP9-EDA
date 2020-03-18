#include "stdafx.h"
#include "generic_event.h"

ostream &operator<<(ostream &stream, generic_event* ev) {
	ev->show();
	return stream;
}

eventType move_pressed::getEventValue()
{
	return TOUCHED_MOVEMENT;
}

void move_pressed::setKeyValue(char key_t)
{
	key = key_t;
}

char move_pressed::getKeyValue()
{
	return key;
}

move_pressed::move_pressed(Worm *worm_src)
{
	worm_p = worm_src;
}

eventType move_released::getEventValue()
{
	return LEAVE_MOVEMENT;
}

void move_released::setKeyValue(char key_t)
{
	key = key_t;
}

char move_released::getKeyValue()
{
	return key;
}

move_released::move_released(Worm * worm_src)
{
	worm_p = worm_src;
}


eventType jump_pressed::getEventValue()
{
	return TOUCHED_JUMP;
}

void jump_pressed::setKeyValue(char key_t)
{
	key = key_t;
}

char jump_pressed::getKeyValue()
{
	return key;
}

jump_pressed::jump_pressed(Worm * worm_src)
{
	worm_p = worm_src;
}

eventType jump_released::getEventValue()
{
	return LEAVE_JUMP;
}

void jump_released::setKeyValue(char key_t)
{
	key = key_t;
}

char jump_released::getKeyValue()
{
	return key;
}

jump_released::jump_released(Worm * worm_src)
{
	worm_p = worm_src;
}

eventType refresh::getEventValue()
{
	return REFRESH;
}

refresh::refresh(Worm * worm_src, viewer *view_src)
{
	worm_p = worm_src;
	view_p = view_src;
}
