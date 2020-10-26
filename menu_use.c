#include "lvgl/lvgl.h"
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"


void my_event_cb(lv_obj_t * trigger_btn,lv_event_t * event){
    if (event == HW_EVENT_KEY_PRESSED){
        int key;
        lv_key_t * key_ptr = (lv_key_t *)lv_event_get_data();
        key = phys_btn_pressed(key_ptr);
        if (key == 3){
            char * btn_label[3] = {"test1","test2",""};
            char * btn_label1[3] = {"test12","test22",""};
            char * btn_label2[3] = {"test13","test23",""};
/**            char * btn_label3[3] = {"test14","test24",""};
            char * btn_label4[3] = {"test15","test25",""};*/
            lv_own_align_t align = RIGHT;
            int C_1[2] = {2,1}; int C_2[2] = {2,2}; int C_3[3] = {3,1,1}; int C_4[3] = {3,1,2};
            MENU * mainmenu = (MENU *)lv_obj_get_user_data(trigger_btn);
            init_menu(mainmenu,trigger_btn);//инициализая меню
            set_menu_labels(mainmenu,C_1,btn_label1);//задаются кнопки для нужных подменю
            set_menu_labels(mainmenu,C_2,btn_label2);
/**            set_menu_labels(mainmenu,C_3,btn_label3);
            set_menu_labels(mainmenu,C_4,btn_label4);*/
            set_size_to_all_menu(mainmenu,80,100);//задаю размер всех подменю
            set_align_to_all_menu(mainmenu,align);//задаю ориентацию
            set_menu_transparent(mainmenu);//делаю меню прозрачным
            set_cb_to_btn(mainmenu,C_1,1,my_event_cb2);//задаю пользовательские коллбэки для меню
            set_cb_to_btn(mainmenu,C_1,2,my_event_cb3);
            lv_menu_create(mainmenu,trigger_btn,btn_label);
        }
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

void menu_use(MENU * mainmenu){
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_t * test_btn = lv_btn_create(lv_scr_act(),NULL);
    lv_obj_set_user_data(test_btn,mainmenu);
    mainmenu->maingroup = lv_group_create();
    lv_group_add_obj(mainmenu->maingroup,test_btn);
    lv_obj_set_event_cb(test_btn,my_event_cb);

}
