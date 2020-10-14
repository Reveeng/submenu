#include "lvgl/lvgl.h"
#include "C:\submenu\my_programms.h"

void event_cb(lv_obj_t * btn, lv_event_t event){
    if(event == LV_EVENT_CLICKED){
        char * btn_label[3] = {"test1","test2",""};
        lv_own_align_t my_align = RIGHT;
        MENU * mainmenu = (MENU *)lv_obj_get_user_data(btn);
        mainmenu->maingroup = lv_group_create();
        init_menu(mainmenu,my_align,btn);
        lv_group_add_obj(mainmenu->maingroup,btn);
        set_user_menu_size(mainmenu,1,150,130);
        lv_menu_create(btn,btn_label,mainmenu,my_align);
    }
}



void using_submenu(MENU * mainmenu){
    lv_obj_t * test_btn = lv_btn_create(lv_scr_act(),NULL);
    lv_obj_set_user_data(test_btn,mainmenu);
    lv_obj_set_event_cb(test_btn,event_cb);
}
