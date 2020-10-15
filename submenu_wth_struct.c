#include "lvgl/lvgl.h"

#include <string.h>
#include <malloc.h>
#include "C:\submenu\my_programms.h"

//стандартная функция колбэка для кнопки из меню, если нужно задать другую, то нужно вызвать (ещё не написана функция)
void standart_menu_cb(lv_obj_t * btn,lv_event_t event){
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
    else if (event == LV_EVENT_KEY){
        int key = *(int *)lv_event_get_data();
        if (key == 27){
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
}
//очищает группу и удаляет указатель на меню из массива указателей
void remove_menu(lv_group_t * maingroup,lv_obj_t * menu){
    lv_group_remove_all_objs(maingroup);
    lv_obj_del(menu);
}
//перезаписывает группу при закрытии меню
void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu){
    lv_obj_t * btn = lv_list_get_next_btn(previous_menu,NULL);
    while (btn != NULL){
        lv_group_add_obj(maingroup,btn);
        btn = lv_list_get_next_btn(previous_menu,btn);
    }
}
//позволяет получить кнопку находящуюся по нужному индексу, для пользователя индекс кнопок начинается с 1
lv_obj_t * get_btn_in_index(lv_obj_t * menu, int index){
    lv_obj_t * btn = lv_list_get_next_btn(menu,NULL);
    for (int i = 0; i!=index;i++){
        btn = lv_list_get_next_btn(menu,btn);
    }
    return (btn);
}
//задает подменю для нужно кнопки, принимает структуру меню на вход, уровень меню(начинается с 1) и номер кнопки по порядоку сверху (начинается с 1)
void set_submenu_to_btn(MENU * mainmenu,int menu_level, int number_btn,char ** labels){
    lv_obj_t * btn = get_btn_in_index(mainmenu->Lmenu[menu_level-1],number_btn-1);
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    int iter = 0;
    while(strcmp(labels[iter],"")!=0){
        Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char*));
        Item->next_menu_labels[iter] = (char*)realloc(Item->next_menu_labels,30*sizeof(char));
        Item->next_menu_labels[iter] = labels[iter];
        iter++;
    }
    Item->next_menu_labels = (char**)realloc(Item->next_menu_labels,(iter+1)*sizeof(char*));
    Item->next_menu_labels[iter] = (char*)realloc(Item->next_menu_labels,sizeof(char));
    Item->next_menu_labels[iter] = "";
}

void set_user_menu_size(MENU * mainmenu,int menu_level,int height, int width){
    if (menu_level == -1){
       mainmenu->sizes_of_menus = (MySize *)realloc(mainmenu->sizes_of_menus,10*sizeof(MySize));
       for (int iter = 0;iter!=10;iter++){
            mainmenu->sizes_of_menus[iter].size_x = width;
            mainmenu->sizes_of_menus[iter].size_y = height;
       }
    }
    else {
        mainmenu->sizes_of_menus = (MySize *)realloc(mainmenu->sizes_of_menus,menu_level*sizeof(MySize));
        mainmenu->sizes_of_menus[menu_level].size_x = width;
        mainmenu->sizes_of_menus[menu_level].size_y = height;
    }
}

//создает меню
void create_and_add_new_menu(MENU * mainmenu,lv_obj_t * triger_btn){
    MenuItem * Item = lv_obj_get_ext_attr(triger_btn);
    if (Item->menu_level == -1){
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
//приписывает кнопке уровень меню на котором она находится
void btn_set_menu_level_and_nullptr(lv_obj_t * btn, int menu_level){
    MenuItem * Item = lv_obj_get_ext_attr(btn);
    Item->menu_level = menu_level;
    Item->next_menu_labels = NULL;
}
//костыль, чтобы все хорошо работало, делает преинициализацию меню, простo записывает нулевые указатели и нули куда нужно
void init_menu(MENU * mainmenu,lv_own_align_t align,lv_obj_t * triger_btn){
    static int count = 0;
    if (count == 0 ){
        lv_obj_allocate_ext_attr(triger_btn, sizeof(MenuItem));
        MenuItem * Item = lv_obj_get_ext_attr(triger_btn);
        Item->menu_level = -1;
        mainmenu->number_of_menu_levels = 0;
        mainmenu->Lmenu = NULL;
        mainmenu->sizes_of_menus=NULL;
        count++;
        mainmenu->align = align;
    }
}

void set_menu_size(lv_obj_t * menu, MySize * Size){
    lv_obj_set_size(menu,Size.size_x,Size.size_y);
}

//позволяет задавать то с какой стороны будет строится меню(пока что используется сразу для всех уровней)
void set_align_for_menu(lv_obj_t * trigger_btn,lv_obj_t * menu,lv_own_align_t align){
    switch(align){
        case LEFT:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
        break;
        }
        case RIGHT:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_RIGHT_TOP,0,0);
        break;
        }
        case BOTTOM:{
        lv_obj_align(menu,trigger_btn,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        break;
        }
    }
}
//обработчик события сбора меню, вызывае по порядку все функции, которые нужны для его создания
void lv_menu_create(lv_obj_t * triger_btn,char ** labels,MENU * mainmenu,lv_own_align_t align){
//    init_menu(mainmenu,align,triger_btn);
    int iter = 0;
    lv_group_remove_all_objs(mainmenu->maingroup);
    lv_obj_t * newbtns;
    mainmenu->number_of_menu_levels++;
    int level = mainmenu->number_of_menu_levels;
    create_and_add_new_menu(mainmenu,triger_btn);
    set_menu_size(mainmenu->Lmenu[level-1],mainmenu->sizes_of_menus[level-1]);
    set_align_for_menu(triger_btn,mainmenu->Lmenu[level-1],mainmenu->align);
    while(strcmp(labels[iter],"")!=0){
        newbtns = lv_list_add_btn(mainmenu->Lmenu[level-1],NULL,labels[iter]);
        lv_obj_allocate_ext_attr(newbtns, sizeof(MenuItem));
        btn_set_menu_level_and_nullptr(newbtns,level);
        lv_obj_set_user_data(newbtns,mainmenu);
        lv_obj_set_event_cb(newbtns,standart_menu_cb);
        lv_group_add_obj(mainmenu->maingroup,newbtns);
        iter++;
    }
}
