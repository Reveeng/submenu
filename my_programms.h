#ifndef MY_PROGRAMMS_H_INCLUDED
#define MY_PROGRAMMS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

enum{
    LEFT,
    RIGHT,
    BOTTOM,
};
typedef uint8_t lv_own_align_t;

typedef struct submenu_t {
    lv_obj_t ** Lmenu;
    lv_obj_t * first_btns[2];// для удобства храню изначальные 2 кнопки в массиве
    lv_group_t * maingroup;// указтель на группу которой управляет инкодер
    int number_of_menu_levels;
    lv_own_align_t align;
} MENU;

typedef struct{
    lv_btn_ext_t btn;
    char ** next_menu_labels;
    int menu_level;
}MenuItem;



void standart_menu_cb(lv_obj_t * btn,lv_event_t event);
void remove_menu(lv_group_t * maingroup,lv_obj_t * menu);
void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu);
lv_obj_t * get_btn_in_index(lv_obj_t * menu, int index);
void set_submenu_to_btn(MENU * mainmenu,int menu_level, int number_btn,char ** labels);
void create_and_add_new_menu(MENU * mainmenu,lv_obj_t * triger_btn);
void btn_set_menu_level(lv_obj_t * btn, int menu_level);
void init_menu(MENU * mainmenu,lv_own_align_t align);
void set_null_ptr(lv_obj_t * btn);
void lv_menu_create(lv_obj_t * triger_btn,char ** labels,MENU * mainmenu,lv_own_align_t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // MY_PROGRAMMS_H_INCLUDED
