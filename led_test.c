#include "lvgl/lvgl.h"

typedef struct{
    lv_btn_ext_t btn;
    lv_obj_t * led_up;
    lv_obj_t * led_down;
    lv_obj_t * led_menu;
    lv_obj_t * led_zoom;
}led_test;

void check_on_off(lv_obj_t * led,int * on_of){
    if ((*on_off) == 0){
        lv_led_on(led);
        *on_of = 1;
    }
    else{
        lv_led_off(led);
        *on_of = 0;
    }
}


void event_cb(lv_obj_t * btn,lv_event_t event){
    static int on_off[4] = {0,0,0,0};
    if (event == HW_EVENT_KEY_PRESSED){
        led_test * attr = lv_obj_get_ext_attr(btn);
        lv_key_t * key = (lv_key_t *)lv_event_get_data();
        if ((*key)>0){
            if(*key & HW_KEY_CODE_UP)
                check_on_off(attr->led_up,&(on_off[0]));
            if(*key & HW_KEY_CODE_DOWN)
                check_on_off(attr->led_down,&(on_off[1]));
            if(*key & HW_KEY_CODE_ZOOM)
                check_on_off(attr->led_zoom,&(on_off[2]));
            if(*key & HW_KEY_CODE_MENU)
                check_on_off(attr->led_menu,&(on_off[3]));
        }
    }
}

void btn_led_test(void){
    /**********************
     *  CREATE OBJECTS
     **********************/
    lv_obj_t * led_btn1 = lv_led_create(lv_scr_act(),NULL);
    lv_obj_t * led_btn2 = lv_led_create(lv_scr_act(),NULL);
    lv_obj_t * led_btn3 = lv_led_create(lv_scr_act(),NULL);
    lv_obj_t * led_btn4 = lv_led_create(lv_scr_act(),NULL);
    lv_obj_t * label1 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_t * label2 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_t * label3 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_t * label4 = lv_label_create(lv_scr_act(),NULL);
    lv_obj_t * test_btn = lv_btn_create(lv_scr_act(),NULL);
    /**********************
     *  SET LED LABELS
     **********************/
    lv_label_set_text(label1,"up btn");
    lv_label_set_text(label2,"down btn");
    lv_label_set_text(label3,"zoom btn");
    lv_label_set_text(label4,"menu btn");
    /**********************
     *  SET LED POSITION
     **********************/
    lv_obj_set_pos(led_btn1,100,50);
    lv_obj_align_mid(led_btn2,led_btn1,LV_ALIGN_OUT_RIGHT_MID,200,0);
    lv_obj_align_mid(led_btn3,led_btn1,LV_ALIGN_OUT_BOTTOM_MID,0,100);
    lv_obj_align_mid(led_btn4,led_btn2,LV_ALIGN_OUT_BOTTOM_MID,0,100);
    lv_obj_align_mid(label1,led_btn1,LV_ALIGN_OUT_TOP_MID,0,-10);
    lv_obj_align_mid(label2,led_btn2,LV_ALIGN_OUT_TOP_MID,0,-10);
    lv_obj_align_mid(label3,led_btn3,LV_ALIGN_OUT_TOP_MID,0,-10);
    lv_obj_align_mid(label4,led_btn4,LV_ALIGN_OUT_TOP_MID,0,-10);
    lv_led_off(led_btn1);
    lv_led_off(led_btn2);
    lv_led_off(led_btn3);
    lv_led_off(led_btn4);
    lv_obj_align_mid(test_btn,led_btn1,LV_ALIGN_OUT_RIGHT_MID,90,60);
    lv_obj_allocate_ext_attr(test_btn,sizeof(led_test));
    led_test * attr = lv_obj_get_ext_attr(test_btn);
    attr->led_up = led_btn1;
    attr->led_down = led_btn2;
    attr->led_zoom = led_btn3;
    attr->led_menu = led_btn4;
    lv_obj_set_event_cb(test_btn,event_cb);
    /**********************
     *  CREATE GROUP
     **********************/
     lv_group_t * group = lv_group_create();
     lv_group_add_obj(group,test_btn);

}
