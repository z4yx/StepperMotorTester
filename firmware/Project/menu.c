#include "common.h"
#include "menu.h"
#include "display.h"
#include "dial.h"
#include "keyboard.h"
#include "hardware_conf.h"
#include "rotary_encoder.h"

extern struct Menu main_menu;
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
    buf[0] = isActive ? '>' : ' ';
    if(item->type == ItemSelectNumber || item->type == ItemShowNumber){
        snprintf(buf+1, DISPLAY_CHARS, item->str, item->param.number->val);
    }
    else if(item->type == ItemSubEntry){
        snprintf(buf+1, DISPLAY_CHARS, "%-*s%c", DISPLAY_CHARS-2, item->str, item->type == ItemSubEntry ? '>' : ' ');
        if(buf[DISPLAY_CHARS-2] & 0x80){
            //avoid half Chinese character
            buf[DISPLAY_CHARS-2] = ' ';
        }
    }
    else{
        strncpy(buf+1, item->str, DISPLAY_CHARS-1);
    }
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
    item = get_current_active();
    int t;
    if(type == KEYEVENT_UP){
        switch(key) {
        case KEY_LEFT:
            if(current_level > 0){
                current_level--;
                current_top = 0;
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
            }
            break;
        case KEY_UP:
            t = cursor_stack[current_level];
            while(--t >= 0){
                if(menu_stack[current_level]->items[t].type != ItemEmpty){
                    cursor_stack[current_level] = t;
                    break;
                }
            }
            break;
        case KEY_DOWN:
            t = cursor_stack[current_level];
            while(++t < menu_stack[current_level]->size){
                if(menu_stack[current_level]->items[t].type != ItemEmpty){
                    cursor_stack[current_level] = t;
                    break;
                }
            }
            break;
        case KEY_CENTER:
            if(item->type == ItemClickable){
                item->param.clicked(item);
            }
            break;
        }
        dispMenu();
    }
}

void Dial_EventHandler(uint8_t event)
{
    struct MenuItem *item = get_current_active();
    if(item->type == ItemSelectNumber){
        struct ItemNumberModel *num = item->param.number;
        if(event == DIAL_EVENT_UP && num->val < num->max){
            num->val++;
        }
        if(event == DIAL_EVENT_DOWN && num->val > num->min){
            num->val--;
        }
        dispMenu();
    }

}

void Menu_Task(void)
{
}