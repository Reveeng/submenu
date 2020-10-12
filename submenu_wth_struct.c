#include "lvgl/lvgl.h"

#include <string.h>
#include <malloc.h>
#include "..//..//submenu/my_programms.h"
#define _STR_ITER(labels,iter) for(;strcmp(labels[iter],"")!=0;iter++)


void standart_menu_cb(lv_obj_t * btn,lv_event_t event){
    int * keyptr = (int *)lv_event_get_data();
    int key = * keyptr;
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    MENU * mainmenu = lv_obj_get_user_data(btn);
    if (event == LV_EVENT_CLICKED){
        if (Item->next_menu_labels == NULL){
            lv_obj_set_state(btn,LV_STATE_PRESSED);
        }
        else {
            lv_menu_create(btn,Item->next_menu_labels,mainmenu,mainmenu->align);
        }
    }
    else if (key == 27){
        if (Item->menu_level != 1){
            rewrite_group(mainmenu->maingroup,mainmenu->Lmenu[Item->menu_level-1]);
            remove_menu(mainmenu->maingroup,mainmenu->Lmenu[Item->menu_level]);
            mainmenu->Lmenu = (lv_obj_t **)realloc(mainmenu->Lmenu,(Item->menu_level-1)*sizeof(lv_obj_t *));
            mainmenu->number_of_menu_levels += -1;
        }
        else {
            remove_menu(mainmenu->maingroup,mainmenu->Lmenu[Item->menu_level]);
            mainmenu->Lmenu = (lv_obj_t **)realloc(mainmenu->Lmenu,(Item->menu_level-1)*sizeof(lv_obj_t *));
            lv_group_add_obj(mainmenu->maingroup,mainmenu->first_btns[0]);
            lv_group_add_obj(mainmenu->maingroup,mainmenu->first_btns[1]);
        }
    }
}

void remove_menu(lv_group_t * maingroup,lv_obj_t * menu){
    lv_group_remove_all_objs(maingroup);
    lv_obj_del(menu);
}

void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu){
    lv_obj_t * btn = lv_list_get_next_btn(previous_menu,NULL);
    while (btn != NULL){
        lv_group_add_obj(maingroup,btn);
        btn = lv_list_get_next_btn(previous_menu,btn);
    }
}

lv_obj_t * get_btn_in_index(lv_obj_t * menu, int index){
    lv_obj_t * btn = lv_list_get_next_btn(menu,NULL);
    for (int i = 0; i!=index;i++){
        btn = lv_list_get_next_btn(menu,btn);
    }
    return (btn);
}

void set_submenu_to_btn(MENU * mainmenu,int menu_level, int number_btn,char ** labels){
    lv_obj_t * menu = mainmenu->Lmenu[menu_level-1];
    lv_obj_t * btn = get_btn_in_index(menu,number_btn-1);
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    int iter = 0;
    _STR_ITER(labels,iter){
        Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char));
        Item->next_menu_labels[iter] = labels[iter];
    }
    Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char));
    Item->next_menu_labels[iter] = "";
}


void create_and_add_new_menu(MENU * mainmenu,lv_obj_t * triger_btn){
    MenuItem * Item = lv_obj_get_ext_attr(triger_btn);
    if (Item == NULL){
        mainmenu->Lmenu = (lv_obj_t **)realloc(mainmenu->Lmenu,sizeof(lv_obj_t *));
        mainmenu->Lmenu[0] = lv_list_create(lv_scr_act(),NULL);
    }
    else if (Item->menu_level >= mainmenu->number_of_menu_levels){
        mainmenu->Lmenu = (lv_obj_t **)realloc(mainmenu->Lmenu,(Item->menu_level+1)*sizeof(lv_obj_t *));
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

void init_menu(MENU * mainmenu,lv_own_align_t align){
    static int count = 0;
    if (count == 0 ){
        mainmenu->number_of_menu_levels = 0;
        mainmenu->Lmenu = NULL;
        count++;
        mainmenu->align = align;
    }
}

void set_null_ptr(lv_obj_t * btn){
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    Item->next_menu_labels = NULL;
}

void set_align_for_menu(lv_obj_t * trigger_btn,lv_obj_t * menu,lv_own_align_t align){
    switch(align){
        case LEFT:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_LEFT_MID,0,0);
        }
        case RIGHT:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_RIGHT_MID,0,0);
        }
        case BOTTOM:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        }
    }
}

void lv_menu_create(lv_obj_t * triger_btn,char ** labels,MENU * mainmenu,lv_own_align_t align){
    init_menu(mainmenu,align);
    int iter = 0;
    lv_group_remove_all_objs(mainmenu->maingroup);
    lv_obj_t * newbtns;
    mainmenu->number_of_menu_levels++;
    set_align_for_menu(triger_btn,mainmenu->Lmenu[mainmenu->number_of_menu_levels-1],mainmenu->align);
    _STR_ITER(labels,iter){
        create_and_add_new_menu(mainmenu,triger_btn);
        newbtns = lv_list_add_btn(mainmenu->Lmenu[mainmenu->number_of_menu_levels-1],NULL,labels[iter]);
        lv_obj_allocate_ext_attr(newbtns, sizeof(MenuItem));
        btn_set_menu_level(newbtns,mainmenu->number_of_menu_levels);
        lv_obj_set_user_data(newbtns,mainmenu);
        set_null_ptr(newbtns);
        lv_obj_set_event_cb(newbtns,standart_menu_cb);
        lv_group_add_obj(mainmenu->maingroup,newbtns);
    }
}
