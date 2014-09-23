#include "common.h"
#include "menu.h"
#include "chinese_gbk.h"


void Motor_ItemStartStop(struct MenuItem* item)
{
    LOG_DBG("called");
    item->str = STR_STOP;
}

