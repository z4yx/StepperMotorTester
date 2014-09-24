#include "menu.h"
#include "params.h"
#include "chinese_gbk.h"
#include "motor.h"

#define ITEM_ARRAY_SIZE(x) (sizeof(x)/sizeof(struct MenuItem))


static struct MenuItem driver_menu_item[] =
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
static struct Menu driver_menu = {.size = ITEM_ARRAY_SIZE(driver_menu_item), .items = driver_menu_item};

static struct MenuItem measure_menu_item[] =
{
    {
        .str = STR_SHOW_VELOCITY,
        .type= ItemShowNumber,
        .param.number = &MeasureVelocity
    },
};
static struct Menu measure_menu = {.size = ITEM_ARRAY_SIZE(measure_menu_item), .items = measure_menu_item};

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
struct Menu main_menu = {.size = ITEM_ARRAY_SIZE(main_menu_item), .items = main_menu_item};
