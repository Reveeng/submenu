#include "lvgl/lvgl.h"
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"


void my_event_cb(lv_obj_t * trigger_btn,lv_event_t * event){
    if (event == LV_EVENT_CLICKED){
        int test;
        char * btn_label[3] = {"test1","test2",""};
        char * btn_label1[3] = {"test12","test22",""};
        char * btn_label2[3] = {"test13","test23",""};
        char * btn_label3[3] = {"test14","test24",""};
        char * btn_label4[3] = {"test15","test25",""};
        lv_own_align_t align = RIGHT;
        int C_1[2] = {2,1}; int C_2[2] = {2,2}; int C_3[3] = {3,1,1}; int C_4[3] = {3,1,2};
        int * massive;
        MENU * mainmenu = (MENU *)lv_obj_get_user_data(trigger_btn);
        init_menu(mainmenu,trigger_btn);
        set_menu_labels(mainmenu,C_1,btn_label1);
        set_menu_labels(mainmenu,C_2,btn_label2);
        set_menu_labels(mainmenu,C_3,btn_label3);
        set_menu_labels(mainmenu,C_4,btn_label4);
        set_size_to_all_menu(mainmenu,80,100);
        set_align_to_all_menu(mainmenu,align);
        set_menu_transparent(mainmenu);
        lv_menu_create(mainmenu,trigger_btn,btn_label);
    }
}

void my_event_cb2(lv_obj_t * triger_btn,lv_event_t  event){
    if (event == LV_EVENT_CLICKED){
        MENU * mainmenu = (MENU *)lv_obj_get_user_data(triger_btn);
        lv_group_focus_next(mainmenu->maingroup);
    }

}


void menu_use(MENU * mainmenu){
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_t * test_btn = lv_btn_create(lv_scr_act(),NULL);
    lv_obj_set_user_data(test_btn,mainmenu);
    lv_obj_set_event_cb(test_btn,my_event_cb);
    lv_obj_t * test_btn2 = lv_btn_create(lv_scr_act(),NULL);
    lv_obj_set_user_data(test_btn2,mainmenu);
    lv_obj_set_event_cb(test_btn2,my_event_cb2);
    lv_obj_set_pos(test_btn2,100,100);
    lv_obj_add_style(test_btn2,LV_OBJ_PART_MAIN,&style);
}
