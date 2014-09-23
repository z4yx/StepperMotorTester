#ifndef HARDWARE_CONF_H_
#define HARDWARE_CONF_H_

#define LCD12864_PORT GPIOA
// #define LCD12864_PSB  GPIO_Pin_9
#define LCD12864_SID  GPIO_Pin_3
#define LCD12864_CS   GPIO_Pin_2
#define LCD12864_SCK  GPIO_Pin_4

#define DISPLAY_LINES 4
#define DISPLAY_LINE_START 1
#define DISPLAY_CHARS 16

#define DIAL_TIM   TIM3
#define DIAL_PORT  GPIOA
#define DIAL_PINS  (GPIO_Pin_6|GPIO_Pin_7)

#define RE_TIM     TIM5
#define RE_PORT    GPIOA
#define RE_PINS    (GPIO_Pin_0|GPIO_Pin_1)

#define Btn_Left_Pin GPIO_Pin_9
#define Btn_Left_Port GPIOB
#define Btn_Down_Pin GPIO_Pin_13
#define Btn_Down_Port GPIOC
#define Btn_Right_Pin GPIO_Pin_14
#define Btn_Right_Port GPIOC
#define Btn_Up_Pin GPIO_Pin_15
#define Btn_Up_Port GPIOC
#define Btn_Center_Pin GPIO_Pin_5
#define Btn_Center_Port GPIOA

//键盘最小触发间隔时间(ms)
#define KEYBOARD_MIN_TOGGLE_PERIOD 50
//键盘开始自动重复时间(ms)
#define KEYBOARD_START_REPEAT_TIME 900
//键盘自动重复间隔(ms)
#define KEYBOARD_REPEAT_PERIOD 250

#endif