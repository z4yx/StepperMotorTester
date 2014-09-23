#ifndef DIAL_H__
#define DIAL_H__

enum
{
	DIAL_EVENT_UP,
	DIAL_EVENT_DOWN
};

void Dial_Init(void);
void Dial_EventHandler(uint8_t event);

#endif