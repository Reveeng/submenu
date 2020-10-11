#include "lvgl/lvgl.h"
#include "C:\Users\Егор\Desktop\работа\submenu\submenu\my_programms.h"
#include <string.h>
#include <malloc.h>

#define _STR_ITER(labels,iter) for(iter;strcmp(labels[iter],"")!=0;iter++)


typedef struct{
    lv_btn_ext_t btn;
    char ** next_menu_labels;
    int menu_level;
}MenuItem;

void standart_menu_cb(lv_obj_t * btn,lv_event_t event){
    int key = (int)lv_event_get_data(event);
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    MENU * mainmenu = lv_obj_get_user_data(btn);
    if (event == LV_EVENT_CLICKED){
        if (Item->next_menu_labels = NULL){
            return 0;
        }
        else {
            lv_menu_create(btn,Item->next_menu_labels,mainmenu);
        }
    }
    else if (key == 27){
        if (Item->menu_level != 1){
            lv_group_remove_all_objs(mainmenu->maingroup);
            lv_obj_del(mainmenu->Lmenu[Item->menu_level]);
            rewrite_group(mainmenu->maingroup,mainmenu->Lmenu[Item->menu_level-1]);
        }
    }
}

void rewrite_group(lv_group_t * maingroup){


}


void menuitem_set_submenu(lv_obj_t * btn,char ** labels){
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    int iter = 0;
    _STR_ITER(labels,iter){
        Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char));
        Item->next_menu_labels[iter] = labels[iter];
    }
    Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char));
    Item->next_menu_labels[iter] = "";
}

lv_obj_t * create_btn_with_label(lv_obj_t * list,char * label){
    lv_obj_t * btn;
    btn = lv_btn_create(list,NULL);
    lv_obj_t * btn_label = lv_label_create(btn,NULL);
    lv_label_set_text(btn_label,label);
    return (btn);
}

void create_and_add_new_menu(MENU * mainmenu,lv_obj_t * triger_btn){
    MenuItem * Item = lv_obj_get_ext_attr(triger_btn);
    if (Item == NULL){
        mainmenu->Lmenu = (lv_obj_t *)realloc(mainmenu->Lmenu,sizeof(lv_obj_t));
        mainmenu->Lmenu[0] = lv_list_create(lv_scr_act(),NULL);
    }
    else if (Item->menu_level >= mainmenu->number_of_menu_levels){
        mainmenu->Lmenu = (lv_obj_t *)realloc(mainmenu->Lmenu,(Item->menu_level+1)*sizeof(lv_obj_t))
        mainmenu->Lmenu[Item->menu_level] = lv_list_create(lv_scr_act(),NULL);
    }
    else if (Item->menu_level < mainmenu->number_of_menu_levels){
        mainmenu->Lmenu[Item->menu_level] = lv_list_create(lv_scr_act(),NULL);
    }
}

void btn_set_menu_level(lv_obj_t * btn, int menu_level){
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    Item->menu_level = menu_level;
}

void init_menu(MENU * mainmenu){
    static int count = 0;
    if (count == 0 ){
        mainmenu->number_of_menu_levels = 0;
        count++;
    }
}

void set_null_ptr(lv_obj_t * btn){
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    Item->next_menu_labels = NULL;
}

void lv_menu_create(lv_obj_t * triger_btn,char * labels,MENU * mainmenu){
    init_menu(mainmenu);
    int iter = 0;
    lv_group_remove_all_objs(mainmenu->maingroup);
    _STR_ITER(labels,iter){
        create_and_add_new_menu(mainmenu,triger_btn);
        lv_obj_t * btn = create_btn_with_label(menu,labels[iter]);
        lv_obj_allocate_ext_attr(btn, sizeof(MenuItem));
        btn_set_menu_level(btn,menu_level);
        lv_obj_set_user_data(mainmenu);
        set_null_ptr(btn);
        lv_obj_set_event_cb(btn,standart_menu_cb);
        lv_list_add_btn(menu,btn);
        lv_group_add_obj(mainmenu->maingroup,btn);
    }
    mainmenu->number_of_menu_levels++;
}
