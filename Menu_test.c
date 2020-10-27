#include "lvgl/lvgl.h"
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"

void event_handler(lv_obj_t * btn,lv_event_t event){
    if (event == LV_EVENT_CLICKED){
        lv_menu_create(btn);
    }
}

void my_event_cb2(lv_obj_t * triger_btn,lv_event_t  event){
    lv_obj_t * label = lv_label_create(lv_scr_act(),NULL);
    lv_label_set_text(label,"Hello");
    lv_obj_set_pos(label,100,100);
}
void my_event_cb3(lv_obj_t * triger_btn,lv_event_t  event){
    lv_obj_t * label = lv_label_create(lv_scr_act(),NULL);
    lv_label_set_text(label,"World");
    lv_obj_set_pos(label,100,150);
}

void using_menu(void){
    lv_obj_t * test_btn = lv_btn_create(lv_scr_act(),NULL);
    MENU * menu = bind_menu_to_obj(test_btn);
    lv_own_align_t align = RIGHT;
    int C_0[2] = {1,0}; int C_1[2] = {2,1}; int C_2[2] = {2,2};
    char * F_menu[3] = {"test1","test2",""};
    char * S_menu_1[3] = {"1 test","2 test",""};
    char * S_menu_2[3] = {"1 test 1","1 test 2",""};
    set_menu_labels(menu,C_0,F_menu);
    set_menu_labels(menu,C_1,S_menu_1);
    set_menu_labels(menu,C_2,S_menu_2);
    set_align_to_all_menu(menu,align);
    set_size_to_all_menu(menu,100,100);
    set_menu_transparent(menu);
    set_cb_to_btn(menu,C_1,1,my_event_cb2);
    set_cb_to_btn(menu,C_1,2,my_event_cb3);
    lv_obj_set_event_cb(test_btn,event_handler);
}
