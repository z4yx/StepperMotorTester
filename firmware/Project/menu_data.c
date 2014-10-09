#include "menu.h"
#include "params.h"
#include "chinese_gbk.h"
#include "motor.h"
#include "measure.h"

#define ITEM_ARRAY_SIZE(x) (sizeof(x)/sizeof(struct MenuItem))
#define DECLARE_MENU(name,item_array) \
struct Menu name = {.size = ITEM_ARRAY_SIZE(item_array), .items = item_array};

static struct MenuItem fixed_velocity_menu_item[] =
{
    {
        .str = STR_SET_VELOCITY,
        .type= ItemSelectNumber,
        .param.number = &MotorVelocity
    },
    {
        .str = "",
        .type= ItemEmpty,
    },
    {
        .str = STR_START,
        .type= ItemClickable,
        .param.clicked = Motor_ItemStartStop
    },
};
DECLARE_MENU(fixed_velocity_menu, fixed_velocity_menu_item);

static struct MenuItem fixed_freq_menu_item[] =
{
    {
        .str = STR_SET_FREQ,
        .type= ItemSelectNumber,
        .param.number = &MotorFreq
    },
    {
        .str = STR_SET_MS,
        .type= ItemSelectString,
        .param.strings = &MotorMicroStep
    },
    {
        .str = "",
        .type= ItemEmpty,
    },
    {
        .str = STR_START,
        .type= ItemClickable,
        .param.clicked = Motor_ItemStartStopFreq
    },
};
DECLARE_MENU(fixed_freq_menu, fixed_freq_menu_item);

static struct MenuItem fixed_step_menu_item[] =
{
    {
        .str = STR_SET_STEPS,
        .type= ItemSelectNumber,
        .param.number = &MotorSteps
    },
    {
        .str = STR_SET_MS,
        .type= ItemSelectString,
        .param.strings = &MotorMicroStep
    },
    {
        .str = "",
        .type= ItemEmpty,
    },
    {
        .str = STR_RUN,
        .type= ItemClickable,
        .param.clicked = Motor_ItemRun
    },
};
DECLARE_MENU(fixed_step_menu, fixed_step_menu_item);

static struct MenuItem driver_menu_item[] =
{
    {
        .str = STR_FIXED_VELOCITY,
        .type= ItemSubEntry,
        .param.next = &fixed_velocity_menu
    },
    {
        .str = STR_FIXED_FREQ,
        .type= ItemSubEntry,
        .param.next = &fixed_freq_menu
    },
    {
        .str = STR_FIXED_STEPS,
        .type= ItemSubEntry,
        .param.next = &fixed_step_menu
    },
};
DECLARE_MENU(driver_menu, driver_menu_item);

static struct MenuItem measure_menu_item[] =
{
    {
        .str = STR_SHOW_VELOCITY,
        .type= ItemShowNumber,
        .param.number = &MeasureVelocity
    },
    {
        .str = STR_SHOW_ANGLE,
        .type= ItemShowNumber,
        .param.number = &MeasureAngle
    },
    {
        .str = "",
        .type= ItemEmpty,
    },
    {
        .str = STR_CLEAR,
        .type= ItemClickable,
        .param.clicked = Measure_ClearAngle
    },
};
DECLARE_MENU(measure_menu, measure_menu_item);

static struct MenuItem main_menu_item[] =
{
    {
        .str = STR_MOTOR_DRIVE,
        .type= ItemSubEntry,
        .param.next=&driver_menu
    },
    {
        .str = STR_MOTOR_MEASURE,
        .type= ItemSubEntry,
        .param.next= &measure_menu
    },
};
DECLARE_MENU(main_menu, main_menu_item);
