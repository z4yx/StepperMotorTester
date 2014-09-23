#ifndef MENU_H__
#define MENU_H__

#define MAX_MENU_DEPTH 4

enum MenuItemType
{
    ItemNormal,
    ItemSubEntry,
    ItemClickable,
    ItemSelectNumber,
    ItemSelectString
};

struct MenuItem
{
    const uint8_t *str;
    enum MenuItemType type;
    union{
        struct Menu* next;

    }param;
};

struct Menu
{
    int size;
    struct MenuItem* items;
};

#endif