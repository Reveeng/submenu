#include "lvgl/lvgl.h"
#include "C:\Users\≈гор\Desktop\работа\submenu\submenu/my_programms.h"
#include <string.h>


// в функции main вставить MENU * mainmenu = (MENU *)malloc(sizeof(MENU)); и free(mainmenu); после цикла while(1)
typedef struct{
    lv_btn_ext_t btn;
    int btn_number;
}btn_with_number;


//построение подменю
static void submenu_build(lv_obj_t * pressed_btn, lv_event_t event);
//построение 1 меню
static void menu_build(lv_obj_t * pressed_btn, lv_event_t event);
//добавление номера к кнопке
void set_extr_data(lv_obj_t * btn,int number);

/**вызывает submenu_build со значением count = 1 и третье меню*/
void connector(lv_obj_t * pressed_btn, lv_event_t event){
    if (event == LV_EVENT_CANCEL){
        lv_event_send(pressed_btn,LV_EVENT_CLICKED,NULL);
}}

/**—бор второго меню, точно так же как первого*/
static void submenu_build(lv_obj_t * pressed_btn, lv_event_t event) /**if return 1 - delete*/
{
    MENU * mainmenu = lv_obj_get_user_data(pressed_btn);
    btn_with_number * ext = lv_obj_get_ext_attr(pressed_btn);
    static int count = 0;
    lv_obj_t * list_btn;
    switch (event){
        case LV_EVENT_CLICKED:{
            if (count == 0){
                count = 1;
                lv_group_remove_all_objs(mainmenu->maingroup);
                mainmenu->second_menu = lv_list_create(lv_scr_act(), NULL);
                lv_obj_set_size(mainmenu->second_menu,100,0);
                lv_obj_align(mainmenu->second_menu,pressed_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
                for (int i = 1; i<=mainmenu->number_of_btn_in_sec_menu[ext->btn_number];i++){
                    list_btn = lv_list_add_btn(mainmenu->second_menu, NULL, mainmenu->second_menu_labels[ext->btn_number][i-1]);
                    lv_obj_set_size(list_btn,50,50);
                    lv_obj_set_event_cb(list_btn,connector);
                    lv_group_add_obj(mainmenu->maingroup,list_btn);
                }
                lv_cont_set_fit(mainmenu->second_menu,LV_FIT_TIGHT);
            }
            else {
                lv_obj_del(mainmenu->second_menu);
                lv_group_remove_all_objs(mainmenu->maingroup);
                count = 0;
                //итераци€ по всем кнопкам из прошлго меню
                lv_obj_t * child = lv_obj_get_child(mainmenu->first_menu,NULL);
                while (child){
                    lv_group_add_obj(mainmenu->maingroup,child);
                    child = lv_obj_get_child(mainmenu->first_menu,child);
                }
            }

        break;
        }
        case LV_EVENT_CANCEL:{
            lv_event_send(pressed_btn,LV_EVENT_CLICKED,NULL);
        break;
        }

    }
}

/**ѕостроение второго меню, записываю старую группу и затем удал€ю все объекты из группы и записываю новые*/
static void menu_build(lv_obj_t * pressed_btn, lv_event_t event)
{
    MENU * mainmenu = lv_obj_get_user_data(pressed_btn);
    btn_with_number * ext = lv_obj_get_ext_attr(pressed_btn);
    static uint8_t count = 0;
    lv_obj_t * list_btn;
    if (event == LV_EVENT_CLICKED){
        if (count == 0){
                mainmenu->first_menu = lv_list_create(lv_scr_act(), NULL);
                lv_obj_set_size(mainmenu->first_menu,100,0);
                lv_obj_align(mainmenu->first_menu,pressed_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
                count = 1;
                lv_group_remove_all_objs(mainmenu->maingroup);
                for (int i=1;i<=mainmenu->number_of_btn_in_first_menu[(ext->btn_number)-1];i++){
                    list_btn = lv_list_add_btn(mainmenu->first_menu, NULL, mainmenu->first_menu_labels[(ext->btn_number)-1][i-1]);
                    if (ext->btn_number == 1){
                        set_extr_data(list_btn,i-1);
                    }
                    else if (ext->btn_number == 2){
                        set_extr_data(list_btn,i+1);
                    }
                    lv_obj_set_size(list_btn,50,50);
                    lv_obj_set_user_data(list_btn,mainmenu);
                    lv_obj_set_event_cb(list_btn,submenu_build);
                    lv_group_add_obj(mainmenu->maingroup,list_btn);
                }
                lv_cont_set_fit(mainmenu->first_menu,LV_FIT_TIGHT);
                }
        /**при нажатии esc новые объекты удал€ютс€ из группы и записываютс€ старые, меню удал€етс€*/
        else if(count == 1){
                count = 0;
                lv_group_remove_all_objs(mainmenu->maingroup);
                for (int i = 1;i<=2;i++){
                    lv_group_add_obj(mainmenu->maingroup,mainmenu->first_btns[i-1]);
                }
                lv_obj_del(mainmenu->first_menu);
            }
    }
}

void set_extr_data(lv_obj_t * btn,int number){
    lv_obj_allocate_ext_attr(btn, sizeof(btn_with_number));
    btn_with_number * ext = lv_obj_get_ext_attr(btn);
    ext->btn_number = number;
}

void menu_init(MENU * mainmenu){
    char * labels_for_first_menu[5] =  {"video","image","download","upload",""};
    char * labels_for_second_menu[13] = {"1","2","3","4","5","6",
                    "7","8","9","10","11","12",""};
    int j = 0;
    for (int iter1 = 0; strcmp("",labels_for_first_menu[iter1]) != 0; iter1++){
        mainmenu->first_menu_labels[j][iter1%2] = labels_for_first_menu[iter1];
        if ((iter1+1)%2 == 0){j++;}
    }
    int k = 0;
    for (int iter2 = 0;strcmp("",labels_for_second_menu[iter2]) != 0;iter2++){
        mainmenu->second_menu_labels[k][iter2%3] = labels_for_second_menu[iter2];
        if ((iter2+1)%3 == 0){k++;}
    }
    for (int i = 0; i<=1;i++){
        mainmenu->number_of_btn_in_first_menu[i] = 2;}
    for (int i = 0; i<=17; i++){
        mainmenu->number_of_btn_in_sec_menu[i] = 3;}
}

/**создаю первые 2 кнопки и записываю их в группу*/
void submenu(MENU * mainmenu)
{
    static int menu_init_count = 0;
    if (menu_init_count == 0){
        menu_init(mainmenu); //инициализаци€ меню
        menu_init_count = 1;
    }
    lv_group_t * main_group = lv_group_create();
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_header_height(win,40);
    lv_obj_t * setting_btn = lv_win_add_btn_right(win, LV_SYMBOL_SETTINGS);
    lv_obj_t * video_btn = lv_win_add_btn_right(win, LV_SYMBOL_VIDEO);
    lv_cont_set_fit(video_btn,LV_FIT_TIGHT);
    set_extr_data(setting_btn,1);
    set_extr_data(video_btn,2);
    lv_obj_t * setting_label = lv_label_create(setting_btn,NULL);
    lv_obj_t * video_label = lv_label_create(video_btn,NULL);
    lv_label_set_text(setting_label,"seting");
    lv_label_set_text(video_label,"video");
    lv_obj_set_event_cb(setting_btn,menu_build);
    lv_obj_set_event_cb(video_btn,menu_build);
    lv_obj_set_user_data(setting_btn,mainmenu);
    lv_obj_set_user_data(video_btn,mainmenu);
    lv_group_add_obj(main_group,setting_btn);
    lv_group_add_obj(main_group,video_btn);
    mainmenu->maingroup = main_group;
    mainmenu->first_btns[0] = setting_btn;
    mainmenu->first_btns[1] = video_btn;
    /**нужно помен€ть строку и вписать сюда устройство ввода*/
    /**lv_indev_set_group(indev,main_group);*/

}
