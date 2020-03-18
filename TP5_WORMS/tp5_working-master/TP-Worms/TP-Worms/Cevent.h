#pragma once

enum event{PRESS_MOVE,RELEASE_MOVE,PRESS_JUMP,RELEASE_JUMP,REFRESH,QUIT,NO_EVENT};
class Cevent
{
public:
	int eventType;
	char ud;

};

