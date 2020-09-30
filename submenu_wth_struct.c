#include "lvgl/lvgl.h"

struct labels_for_third_menu{
    char * first_btn_third_menu_labels[3];
    int first_btn_count,second_btn_count,
    third_btn_count,fourth_btn_count,
    fifth_btn_count,six_btn_count;
    char * second_btn_third_menu_labels[3];
    char * third_btn_third_menu_labels[3];
    char * fourth_btn_third_menu_labels[3];
    char * fifth_btn_third_menu_labels[3];
    char * six_btn_third_menu_labels[3];
};

struct labels_for_second_menu{
    char * first_btn_second_menu_labels[2];
    int first_btn_count,second_btn_count = 2;
    char * second_btn_second_menu_labels[2];
};

struct labels_for_first_menu{
    char * first_menu_labels[2];
    int count = 2;
};


typedef struct submenu_t {

    struct labels_for_first_menu FLABEL;
    struct labels_for_second_menu SLABEL;
    struct labels_for_third_menu TLABEL;
    lv_obj_t * first_menu;
    lv_obj_t * second_menu;
    lv_obj_t * first_menu_list_btn;
    lv_obj_t * second_menu_list_btn;
    } MENU;



static void submenu_build(lv_obj_t * pressed_btn, char ** newbtns,int number_of_buttons);
static void menu_build(lv_obj_t * pressed_btn, int menu_order);

/**вызывает submenu_build со значением count = 1 и третье меню*/
void close_third_list(lv_obj_t * pressed_btn, lv_event_t event){
    if (event == LV_EVENT_KEY){
        int key = *(int *)lv_event_get_data();
        if (key == 27){
        submenu_build(pressed_btn,NULL,NULL);
    }}
}

/**—бор второго меню, точно так же как первого*/
static void submenu_build(lv_obj_t * pressed_btn, char ** newbtns,int number_of_buttons) /**if return 1 - delete*/
{
    static lv_obj_t * menu_list;
    static lv_obj_t * list_btn;
    static int count = 0;
    static lv_group_t * current_group;
    static lv_obj_t * old_group[3];
    /**выполн€етс€ при нажатии кнопки esc*/
    if (count == 1)
    {
        lv_obj_del(menu_list);
        lv_group_remove_all_objs(current_group);
        count = 0;
        for (int i = 0;i<=2;i++){
            lv_group_add_obj(current_group,old_group[i]);
        }
    }
    else
    {
        current_group = lv_obj_get_group(pressed_btn);
        int iter = 0;
        lv_obj_t ** i;
        _LV_LL_READ(current_group->obj_ll,i){
            old_group[iter] = *i;
            iter++;}
        lv_group_remove_all_objs(current_group);
        count = 1;
        menu_list = lv_list_create(lv_scr_act(), NULL);
        lv_obj_set_size(menu_list,100,0);
        lv_obj_align(menu_list,pressed_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
        lv_obj_t * list_btn;
        for (int i = 1;i<=number_of_buttons;i++)
        {
            list_btn = lv_list_add_btn(menu_list, LV_SYMBOL_FILE, newbtns[i-1]);
            lv_group_add_obj(current_group,list_btn);
            lv_obj_set_event_cb(list_btn,close_third_list);
        }
        lv_cont_set_fit(menu_list,LV_FIT_TIGHT);
    }
}

/**также смотрю на то кака€ кнопка нажата и передаю в функцию сбора второго меню нужные мне параметры*/
static void second_connector(lv_obj_t * pressed_btn, lv_event_t event){
    int menu_order = * (int *)lv_obj_get_user_data(pressed_btn);
    static char * first[3] = {"one","two","three"};
    static char * second[3] = {"uno","dos","tres"};
    static char * third[3] = {"odin","dwa","tree"};
    static char * fourth[3] = {"ain","zwai","drai"};
    static char * fifth[3] = {"1","2","3"};
    static char * six[3] = {"4","5","6"};
    if (event == LV_EVENT_CLICKED){
        if (menu_order == 1){
            submenu_build(pressed_btn,first,3);
        }
        else if (menu_order == 2){
            submenu_build(pressed_btn,second,3);
        }
        else if (menu_order == 3){
            submenu_build(pressed_btn,third,3);
        }
        else if (menu_order == 4){
            submenu_build(pressed_btn,fourth,3);
        }
        else if (menu_order == 5){
            submenu_build(pressed_btn,fifth,3);
        }
        else if (menu_order == 6){
            submenu_build(pressed_btn,six,3);
        }

    }
    /**эесли нажата кнопка esc, то вызываетс€ menu_build со значением count=1*/
    if (event == LV_EVENT_KEY){
        int key = *(int *) lv_event_get_data();
        if (key == 27){
            menu_build(pressed_btn,NULL);
        }
    }


}
/**ѕостроение второго меню, записываю старую группу и затем удал€ю все объекты из группы и записываю новые*/
static void menu_build(whatever_t * menu_to_build)
{
    int FMC = 3;
    int SMC = 3;
    char * first_menu[3] = {"video","audio","image"};
    char * second_menu[3] = {"download","upload","other"};
    static lv_obj_t * old_group[2];
    static uint8_t count = 0;
    static lv_obj_t * menu_list;
    static lv_obj_t * list_btn;
    static lv_group_t * cur_group;
    static int menu_count[6] = {1,2,3,4,5,6};
    if (count == 0){
            cur_group = lv_obj_get_group(pressed_btn);
            menu_list = lv_list_create(lv_scr_act(), NULL);
            lv_obj_set_size(menu_list,100,0);
            lv_obj_align(menu_list,pressed_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
            count = 1;
            /**запись объектов из группы*/
            int iter = 0;
            lv_obj_t ** i;
            _LV_LL_READ(cur_group->obj_ll,i){
                old_group[iter] = *i;
                iter++;
            }/**закончил запись*/
            lv_group_remove_all_objs(cur_group);
            if (menu_order == 1){
                    for (int i=1;i<=FMC;i++){
                        list_btn = lv_list_add_btn(menu_list, NULL, first_menu[i-1]);
                        lv_obj_set_size(list_btn,50,50);
                        lv_obj_set_user_data(list_btn,&menu_count[i-1]);
                        lv_obj_set_event_cb(list_btn,second_connector);
                        lv_group_add_obj(cur_group,list_btn);
                    }}
            else if (menu_order == 2){
                    for (int i=1;i<=SMC;i++){
                        list_btn = lv_list_add_btn(menu_list, NULL, second_menu[i-1]);
                        lv_obj_set_size(list_btn,50,50);
                        lv_obj_set_user_data(list_btn,&menu_count[i+2]);
                        lv_obj_set_event_cb(list_btn,second_connector);
                        lv_group_add_obj(cur_group,list_btn);
            }}
            lv_cont_set_fit(menu_list,LV_FIT_TIGHT);
            }
    /**при нажатии esc новые объекты удал€ютс€ из группы и записываютс€ старые, меню удал€етс€*/
    else if(count == 1){
            count = 0;
            lv_group_remove_all_objs(cur_group);
            for (int i = 0;i<=1;i++){
                lv_group_add_obj(cur_group,old_group[i]);
            }
            lv_obj_del(menu_list);
        }
    }
void setting_pressed(lv_obj_t * pressed_btn, lv_event_t event){
    MENU * mainmenu = lv_obj_get_user_data(pressed_btn);
    if (event == LV_EVENT_CLICKED){
    }
}
void video_pressed(lv_obj_t * pressed_btn, lv_event_t event){
    MENU * mainmenu = lv_obj_get_user_data(pressed_btn);
    if (event == LV_EVENT_CLICKED){
    }
}

/**создаю первые 2 кнопки и записываю их в группу*/
void submenu(MENU * mainmenu)
{
    mainmenu.FLABEL.first_menu_labels = {"setting","video"};
    mainmenu.FLABEL.count = 2;
    mainmenu.SLABEL.first_btn_second_menu_labels = {"video","image"};
    mainmenu.SLABEL.second_btn_second_menu_labels = {"download","upload"};
    mainmenu.SLABEL.first_btn_count = 2;
    mainmenu.SLABEL.second_btn_count = 2;
    mainmenu.TLABEL.first_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.second_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.third_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.fourth_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.fifth_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.six_btn_third_menu_labels = {"1","2","3"};
    mainmenu.TLABEL.first_btn_count = 3;
    mainmenu.TLABEL.second_btn_count = 3;
    mainmenu.TLABEL.third_btn_count = 3;
    mainmenu.TLABEL.fourth_btn_count = 3;
    mainmenu.TLABEL.fifth_btn_count = 3;
    mainmenu.TLABEL.six_btn_count = 3;

    lv_group_t * main_group = lv_group_create();
    lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_header_height(win,40);
    lv_obj_t * setting_btn = lv_win_add_btn_right(win, LV_SYMBOL_SETTINGS);
    lv_obj_t * video_btn = lv_win_add_btn_right(win, LV_SYMBOL_VIDEO);
    lv_cont_set_fit(video_btn,LV_FIT_TIGHT);
    lv_obj_set_event_cb(setting_btn,setting_pressed);
    lv_obj_set_event_cb(video_btn,video_pressed);
    lv_obj_set_user_data(setting_btn,mainmenu);
    lv_obj_set_user_data(video_btn,mainmenu);
    lv_group_add_obj(main_group,setting_btn);
    lv_group_add_obj(main_group,video_btn);
    /**нужно помен€ть строку и вписать сюда устройство ввода*/
    /**lv_indev_set_group(indev,main_group);*/

}
