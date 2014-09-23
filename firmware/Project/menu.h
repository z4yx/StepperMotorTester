#ifndef MENU_H__
#define MENU_H__

#include "common.h"

#define MAX_MENU_DEPTH 4

enum MenuItemType
{
    ItemNormal,
    ItemSubEntry,
    ItemClickable,
    ItemSelectNumber,
    ItemSelectString,
    ItemShowNumber,
    ItemEmpty,
};

struct ItemNumberModel
{
    int min, max, val;
};

struct MenuItem;

typedef void (*ItemClickEventHandler_t)(struct MenuItem*);

struct MenuItem
{
    const uint8_t *str;
    enum MenuItemType type;
    union{
        struct Menu* next;
        struct ItemNumberModel* number;
        ItemClickEventHandler_t clicked;
    }param;
};

struct Menu
{
    int size;
    struct MenuItem* items;
};

#endif