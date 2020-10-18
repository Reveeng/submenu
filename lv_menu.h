#ifndef LV_MENU_H_INCLUDED
#define LV_MENU_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"

enum{
    LEFT,
    RIGHT,
    BOTTOM,
};
typedef uint8_t lv_own_align_t;

typedef struct {
    int size_x;
    int size_y;
} MySize;

typedef struct {
    lv_obj_t * menu;
    MySize size_of_menu;
    lv_own_align_t align;
    char ** menu_labels;
    MenuItem ** submenus;
}MenuItem;

typedef struct {
    MenuItem * first_menu;
    lv_group_t * maingroup;
    int curent_level;
}MENU;


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LV_MENU_H_INCLUDED
