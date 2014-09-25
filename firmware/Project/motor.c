/*
 * =====================================================================================
 *
 *       Filename:  motor.c
 *
 *    Description:  步进电机底层控制模块
 *
 *        Version:
 *        Created:
 *       Revision:
 *       Compiler:
 *
 *         Author:  zhangyuxiang
 *   Organization:
 *
 * =====================================================================================
 */

#include "common.h"
#include "motor.h"
#include "hardware_conf.h"
#include "chinese_gbk.h"
#include "params.h"

//电机剩余步进数,-1表示无限
static volatile int Motor_PendingSteps;
//下一个时钟输出的电平
static uint8_t Motor_OutputLevel;

void Motor_SetMicrostepping(uint8_t microstep)
{
    int bin = __builtin_ctz(microstep);
    if (microstep == 16)
        bin |= 3; //refer to A4988 datasheet
    LOG_DBG("Microstep %d, MS: %d", microstep, bin);
    GPIO_WriteBit(Motor_Port, Motor_MS1, (bin & 1) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(Motor_Port, Motor_MS2, (bin & 2) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(Motor_Port, Motor_MS3, (bin & 4) ? Bit_SET : Bit_RESET);
}

void Motor_SetDirection(int8_t dir)
{
    GPIO_WriteBit(Motor_Port, Motor_DIR, dir > 0 ? Bit_SET : Bit_RESET);
}

void Motor_SetEnabled(uint8_t en)
{
    GPIO_WriteBit(Motor_Port, Motor_EN, en ? Bit_RESET : Bit_SET);
}

static void Motor_TIM_Config(uint32_t freq, TIM_TypeDef *tim)
{
    uint16_t period, prescaler;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    TIM_Cmd(tim, DISABLE);

    Timer_16bit_Calc(freq, &period, &prescaler);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;

    TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);

    // TIM_ARRPreloadConfig(tim, ENABLE);

    TIM_SetCounter(tim, 0);

    TIM_Cmd(tim, ENABLE);

}

static void Motor_TIM_SetInterrupt(void)
{
#define Motor_IRQn Motor_TIM##_IRQn
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   // 指定响应优先级别
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //指定中断源
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(Motor_TIM, TIM_IT_Update, ENABLE);
}

void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = Motor_EN|Motor_MS1|Motor_MS2|Motor_MS3|Motor_DIR|Motor_STEP;

    RCC_TIMClockCmd(Motor_TIM, ENABLE);
    RCC_GPIOClockCmd(Motor_Port, ENABLE);

    GPIO_Init(Motor_Port, &GPIO_InitStructure);

    Motor_SetEnabled(false);

    Motor_TIM_SetInterrupt();
}

void Motor_Interrupt(TIM_TypeDef *tim)
{
    if (SET == TIM_GetITStatus(tim, TIM_IT_Update)) {
        TIM_ClearITPendingBit(tim, TIM_IT_Update);

        if (Motor_PendingSteps) {

            GPIO_WriteBit(Motor_Port, Motor_STEP, Motor_OutputLevel);
            Motor_OutputLevel ^= 1;

            if (Motor_PendingSteps > 0) {
                Motor_PendingSteps --;
                if (!Motor_PendingSteps) {
                    GPIO_ResetBits(Motor_Port, Motor_STEP);
                    TIM_Cmd(tim, DISABLE);
                    // Move_Axis_Eneded(i);
                }
            }
        }

    }
}

//强行停止电机运行,不会触发运行完成事件
void Motor_Stop()
{
    Motor_SetEnabled(false);
    GPIO_ResetBits(Motor_Port, Motor_STEP);
    TIM_Cmd(Motor_TIM, DISABLE);
    Motor_PendingSteps = 0;
}

void Motor_Start(int steps, uint32_t freq)
{
    Motor_SetEnabled(true);
    GPIO_ResetBits(Motor_Port, Motor_STEP);

    Motor_OutputLevel = 1;
    Motor_PendingSteps = steps;

    if (Motor_PendingSteps > 0) {
        //double steps and frequency for rising and falling edge
        Motor_PendingSteps <<= 1;
    }
    Motor_TIM_Config((freq << 1), Motor_TIM);
}

static void calc_freq(int velocity, int *freq, int *ms)
{
    if(velocity <= 25)
        *ms = 16;
    else if(velocity <= 60)
        *ms = 8;
    else if(velocity <= 200)
        *ms = 4;
    *freq = velocity * *ms * Motor_PULSES;
    *freq /= 60; //convert to r/sec
}

void Motor_ItemStartStop(struct MenuItem *item)
{
    if (Motor_PendingSteps) {
        Motor_Stop();
        item->str = STR_START;
    } else {
        int ms, freq;
        Motor_SetDirection(0);
        calc_freq(MotorVelocity.val, &freq, &ms);
        LOG_DBG("f: %dHz, micro: %d", freq, ms);
        Motor_SetMicrostepping(ms);
        Motor_Start(-1, freq);
        item->str = STR_STOP;
    }
}

