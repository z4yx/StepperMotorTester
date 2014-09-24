#include "params.h"
#include "rotary_encoder.h"
#include "common.h"
#include "hardware_conf.h"
#include "systick.h"

static SysTick_t last_time;

void Measure_Init(void)
{
    RotaryEnc_Config(RE_TIM, RE_PORT, RE_PINS, RE_PULSES*2);
    RotaryEnc_ClearCounter(RE_TIM);
	last_time = 0;
	RotaryEnc_TIM_SetInterrupt(RE_TIM, TIM2_IRQn);
}

void Measure_Interrupt(void)
{
    if (SET != TIM_GetITStatus(RE_TIM, TIM_IT_Update))
    	return;
    TIM_ClearITPendingBit(RE_TIM, TIM_IT_Update);

	SysTick_t now = GetSystemTick();
	int delta = now - last_time;
	// LOG_DBG("Measure_Interrupt, delta: %d",delta);
	if(delta > 0){
		int velocity = 1000*60/delta;
		MeasureVelocity.val = velocity;
		Menu_NeedUpdate();
	}
	last_time = now;
}

