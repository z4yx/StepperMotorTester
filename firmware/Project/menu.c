#include "common.h"
#include "menu.h"
#include "display.h"
#include "keyboard.h"
#include "hardware_conf.h"
#include "rotary_encoder.h"

#define ITEM_ARRAY_SIZE(x) (sizeof(x)/sizeof(struct MenuItem))

static struct MenuItem sub_menu_1_item[] =
{
    {
        .str = "hello",
        .type= ItemNormal,
    },
    {
        .str = "world",
        .type= ItemNormal,
    },
};

static struct Menu sub_menu_1 = {.size = ITEM_ARRAY_SIZE(sub_menu_1_item), .items = sub_menu_1_item};

static struct MenuItem main_menu_item[] =
{
    {
        .str = "abc",
        .type= ItemNormal,
    },
    {
        .str = "def",
        .type= ItemNormal,
    },
    {
        .str = "ghi",
        .type= ItemNormal,
    },
    {
        .str = "hehe",
        .type= ItemNormal,
    },
    {
        .str = "uuu",
        .type= ItemNormal,
    },
    {
        .str = "www",
        .type= ItemSubEntry,
        .param.next= &sub_menu_1,
    },
    {
        .str = "xxx",
        .type= ItemNormal,
    },
    {
        .str = "z",
        .type= ItemNormal,
    },
};

static struct Menu main_menu = {.size = ITEM_ARRAY_SIZE(main_menu_item), .items = main_menu_item};

static struct Menu* menu_stack[MAX_MENU_DEPTH];
static int cursor_stack[MAX_MENU_DEPTH];
static int current_level, current_top;

static void calc_menu_top(void)
{
    int cursor = cursor_stack[current_level];
    if(cursor < current_top)
        current_top = cursor;
    else if(cursor >= current_top + DISPLAY_LINES)
        current_top = cursor - DISPLAY_LINES + 1;
}

static void format_item_str(char* buf, struct MenuItem* item, bool isActive)
{
    int i;
    buf[0] = isActive ? '>' : ' ';
    for(i=1; i<DISPLAY_CHARS-1; i++){
        if(!item->str[i-1])
            break;
        buf[i] = item->str[i-1];
    }
    while(i<DISPLAY_CHARS-1){
        buf[i] = ' ';
        i++;
    }
    buf[DISPLAY_CHARS-1] = item->type == ItemSubEntry ? '>' : ' ';
    buf[DISPLAY_CHARS] = 0;
}

static void dispMenu(void)
{
    struct Menu *m = menu_stack[current_level];
    int cursor = cursor_stack[current_level];
 
    calc_menu_top();
    for (int i = 0; i < DISPLAY_LINES; ++i)
    {
        int j = i + current_top;
        if(j >= m->size){
            Display_WriteLine(i+1, "");
        }else{
            char buf[DISPLAY_CHARS+1];
            format_item_str(buf, &m->items[j], j==cursor);
            Display_WriteLine(i+1, buf);
        }
    }
}

static struct MenuItem* get_current_active(void)
{
    return &menu_stack[current_level]->items[cursor_stack[current_level]];
}

void Menu_Init(void)
{
    current_level = 0;
    cursor_stack[0] = 0;
    menu_stack[0] = &main_menu;
    dispMenu();
}

void KeyBoard_EventHandler(uint8_t key, uint8_t type)
{
    struct MenuItem *item;
    if(type == KEYEVENT_UP){
        item = get_current_active();
        switch(key) {
        case KEY_LEFT:
            if(current_level > 0){
                current_level--;
                current_top = 0;
                dispMenu();
            }
            break;
        case KEY_RIGHT:
            if(item->type == ItemSubEntry){
                if(!item->param.next){
                    LOG_ERR("Next is NULL");
                    break;
                }
                if(current_level == MAX_MENU_DEPTH-1){
                    LOG_ERR("Too many levels");
                    break;
                }
                current_level++;
                cursor_stack[current_level] = 0;
                menu_stack[current_level] = item->param.next;
                current_top = 0;
                dispMenu();
            }
            break;
        case KEY_UP:
            if(cursor_stack[current_level] > 0)
                cursor_stack[current_level]--;
            dispMenu();
            break;
        case KEY_DOWN:
            if(cursor_stack[current_level] < menu_stack[current_level]->size - 1)
                cursor_stack[current_level]++;
            dispMenu();
            break;
        }
    }
}


void Menu_Task(void)
{
    // char buf[32];
    // uint16_t cnt = RotaryEnc_GetCounter(DIAL_TIM);
    // sprintf(buf, "Rotary: %02d", cnt);
    // Display_WriteLine(3,buf);
    // LOG_DBG("%s", buf);
    // Delay_ms(500);
}