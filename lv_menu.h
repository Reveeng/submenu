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
    MySize size_of_menu;
    lv_own_align_t align;
    char ** menu_labels;
    lv_event_cb_t * list_of_callbacks;
}MenuItem;
//хранилище всех подменю, содержит в себе подменю и их координаты
typedef struct {
    MenuItem ** MenuBase;
    int ** CoordBase;
    int DB_size;
}Menu_DB;


typedef struct {
    Menu_DB * DataBase;
    lv_group_t * maingroup;
    int curent_level;
    lv_obj_t * first_btn;
    MenuItem * first_menu;
    lv_obj_t ** visible_menu_list;
    lv_style_t * opa_style;
}MENU;

typedef struct{
    lv_btn_ext_t btn;
    int * menu_item_coord;
    int btn_number;
    int submenu_available;
    MENU * menu;
}BtnItem;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
MenuItem * get_menu_item(Menu_DB * DataBase,int * coord);

int * create_coord_prev_menu(int * coord);

void init_menu_item(Menu_DB * DataBase,int * coord);

int * create_coord_next_menu(int level,void * coord,int btn_number);

void standart_menu_cb(lv_obj_t * triger_btn,lv_event_t event);

void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu);

void remove_menu(lv_group_t * maingroup,lv_obj_t * menu);

void set_menu_labels(MENU * mainmenu,int * coord,char ** labels);

void set_menu_size(MENU*mainmenu,int * coord,int height,int weight);

void set_menu_align(MENU * mainmenu,int * coord, lv_own_align_t align);

void set_extr_attr(lv_obj_t * btn,int iter,MENU * menu,void * coord);

void set_align_to_menu(lv_obj_t * triger_btn,lv_obj_t * list_obj,lv_own_align_t align);

void make_and_show_menu(lv_obj_t * triger_btn,MENU * mainmenu,MenuItem * menu, void * coord);

void lv_menu_create(MENU * menu,lv_obj_t * triger_btn,char ** labels);

void init_menu(MENU * mainmenu,lv_obj_t * triger_btn);

void menu_use(MENU * mainmenu);

void set_size_to_all_menu(MENU * mainmenu,int height,int weight);

void set_align_to_all_menu(MENU * mainmenu,lv_own_align_t align);

char ** rewrite_labels(char ** labels);

int * get_coord(MENU * mainmenu,int i);

void set_menu_transparent(MENU * mainmenu);

int phys_btn_pressed(lv_key_t * key);
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LV_MENU_H_INCLUDED
