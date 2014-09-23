#include "common.h"
#include "display.h"
#include "12864.h"
#include "hardware_conf.h"
#include <string.h>

static bool dirty[DISPLAY_LINES];
static uint8_t disp_buffer[DISPLAY_LINES][DISPLAY_CHARS];

void Display_Init(void)
{
    memset(disp_buffer, ' ', sizeof(disp_buffer));
    LCD12864_Clr_Scr();
}

void Display_Refresh()
{
    for (int i = 0; i < DISPLAY_LINES; ++i)
    {
        if(dirty[i]){
            LOG_DBG("update line %d", i);
            LCD12864_Write_String_Length(DISPLAY_LINE_START+i, 0, disp_buffer[i], DISPLAY_CHARS);
            dirty[i] = false;
        }
    }
}

void Display_WriteLine(uint8_t line, uint8_t* str)
{
    if(1<=line && line<=DISPLAY_LINES){
        int l = line - 1;
        bool zero_occur = false;
        uint8_t p[DISPLAY_CHARS];

        for (int i = 0; i < DISPLAY_CHARS; ++i)
        {
            if(zero_occur){
                p[i] = ' ';
            }
            else{
                p[i] = str[i];
                if(!str[i]){
                    p[i] = ' ';
                    zero_occur = true;
                }
            }
        }
        if(memcmp(p, disp_buffer[l], DISPLAY_CHARS) != 0){
            memcpy(disp_buffer[l], p, DISPLAY_CHARS);
            dirty[l] = true;
        }
    }
}

void Display_Task(void)
{
    Display_Refresh();
}