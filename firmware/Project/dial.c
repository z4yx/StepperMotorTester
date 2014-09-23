#include "common.h"
#include "dial.h"
#include "systick.h"
#include "rotary_encoder.h"
#include "hardware_conf.h"

static uint16_t last_value;

void Dial_Init(void)
{
    RotaryEnc_Config(DIAL_TIM, DIAL_PORT, DIAL_PINS, DIAL_PULSES*2);
    RotaryEnc_ClearCounter(DIAL_TIM);
	last_value = 0;
}

#ifdef DIAL_INVERSE
#define VAL_INC() Dial_EventHandler(DIAL_EVENT_DOWN)
#define VAL_DEC() Dial_EventHandler(DIAL_EVENT_UP)
#else
#define VAL_DEC() Dial_EventHandler(DIAL_EVENT_DOWN)
#define VAL_INC() Dial_EventHandler(DIAL_EVENT_UP)
#endif

void Dial_Task(void)
{
	uint16_t cnt = RotaryEnc_GetCounter(DIAL_TIM) >> 1;
	if(last_value != cnt){
		if(last_value == 0 && cnt == 19)
			VAL_DEC();
		else if(last_value == 19 && cnt == 0)
			VAL_INC();
		else if(last_value < cnt)
			VAL_INC();
		else
			VAL_DEC();
	}
	last_value = cnt;
}