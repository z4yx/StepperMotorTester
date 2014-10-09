#include "params.h"
#include "rotary_encoder.h"
#include "common.h"
#include "hardware_conf.h"
#include "systick.h"

static SysTick_t last_time;
static int re_interrupt_cnt, last_clear_value;

void Measure_Init(void)
{
    RotaryEnc_Config(RE_TIM, RE_PORT, RE_PINS, RE_PULSES*2);
    RotaryEnc_ClearCounter(RE_TIM);
	last_time = 0;
	re_interrupt_cnt = last_clear_value = 0;
	RotaryEnc_TIM_SetInterrupt(RE_TIM, TIM2_IRQn);
}

int get_total_pulses(void)
{
	return re_interrupt_cnt * RE_PULSES*2
		+ RotaryEnc_GetCounter(RE_TIM);
}

void Measure_ClearAngle(struct MenuItem *item)
{
	last_clear_value = get_total_pulses();
	MeasureAngle.val = 0;
	Menu_NeedUpdate();
}

void Measure_Task(void)
{
	int cur = get_total_pulses() - last_clear_value;
	cur %= RE_PULSES*2;
	cur = cur*360/(RE_PULSES*2);
	if(cur != MeasureAngle.val){
		MeasureAngle.val = cur;
		Menu_NeedUpdate();
	}
}

void Measure_Interrupt(void)
{
    if (SET != TIM_GetITStatus(RE_TIM, TIM_IT_Update))
    	return;
    TIM_ClearITPendingBit(RE_TIM, TIM_IT_Update);

    re_interrupt_cnt++;

	SysTick_t now = GetSystemTick();
	int delta = now - last_time;
	// LOG_DBG("Measure_Interrupt, delta: %d",delta);
	if(delta > 0){
		int velocity = (1000*60+delta-1)/delta;
		MeasureVelocity.val = velocity;
		Menu_NeedUpdate();
	}
	last_time = now;
}

