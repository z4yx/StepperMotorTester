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
    ItemSelectString
};

struct ItemNumberModel
{
    int min, max, val;
};

struct MenuItem
{
    const uint8_t *str;
    enum MenuItemType type;
    union{
        struct Menu* next;
        struct ItemNumberModel* number;
    }param;
};

struct Menu
{
    int size;
    struct MenuItem* items;
};

#endif