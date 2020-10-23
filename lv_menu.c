#include "lvgl/lvgl.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"


MenuItem * get_menu_item(Menu_DB * DataBase,int * coord){
    int size_cmp = coord[0]*sizeof(int);
    int iter = 0;
    while (memcmp(DataBase->CoordBase[iter],coord,size_cmp) != 0 ){
        iter++;
    }
    MenuItem * Item;
    Item = DataBase->MenuBase[iter];
    return(Item);
}

int * create_coord_prev_menu(int * coord){
        int iter = 0;
        int * prev_coord;
        prev_coord = (int *)malloc((coord[0]-1)*sizeof(int));
        while (iter != (coord[0]-1)){
            prev_coord[iter] = coord[iter];
            iter++;
        }
        prev_coord[0]-=1;
        return(prev_coord);
}

void init_menu_item(Menu_DB * DataBase,int * coord){
    DataBase->DB_size++;
    int byte_size_to_copy = coord[0]*sizeof(int);
    DataBase->CoordBase = (int **)realloc(DataBase->CoordBase,DataBase->DB_size*sizeof(int*));
    DataBase->CoordBase[DataBase->DB_size-1] = (int *)malloc(coord[0]*sizeof(int));
    DataBase->MenuBase = (MenuItem **)realloc(DataBase->MenuBase,DataBase->DB_size*sizeof(MenuItem*));
    DataBase->MenuBase[DataBase->DB_size-1] = (MenuItem *)malloc(sizeof(MenuItem));
    memcpy(DataBase->CoordBase[DataBase->DB_size-1],coord,byte_size_to_copy);
}

int * create_coord_next_menu(int level,void * coord,int btn_number){
    int * new_coord;
    int mem_size = level*sizeof(int);
    new_coord = (int *)malloc((level+1)*sizeof(int));
    if (level == 1){
        new_coord[0] = 2;
        new_coord[1] = btn_number+1;
    }
    else{
        int * recoord = (int *) coord;
        memcpy(new_coord,recoord,mem_size);
        new_coord[0]++;
        new_coord[level] = btn_number+1;
    }
    return(new_coord);
}

int phys_btn_pressed(lv_key_t * key){
    int int_key;
    if ((*key)>0){
        if (*key & HW_KEY_CODE_UP){
            int_key = 1;
            return(int_key);
        }
        if (*key & HW_KEY_CODE_DOWN){
            int_key = 2;
            return(int_key);
        }
        if (*key & HW_KEY_CODE_MENU){
            int_key = 3;
            return(int_key);
        }
        if (*key & HW_KEY_CODE_ZOMM){
            int_key = 4;
            return(int_key);
        }
    }
}

void standart_menu_cb(lv_obj_t * triger_btn,lv_event_t event){
    BtnItem * btn_attr = lv_obj_get_ext_attr(triger_btn);
    MENU * menu = btn_attr->menu;
    if (event == HW_EVENT_KEY_PRESSED){
        int key;
        lv_key_t * key_ptr = (lv_key_t *)lv_event_get_data();
        key = phys_btn_pressed(key_ptr);
        switch (key){
            case 1:
                lv_group_focus_next(menu->maingroup);
            case 2:
                lv_group_focus_prev(menu->maingroup);
            case 3:{
                MenuItem * menu_to_create = get_menu_item(btn_attr->menu->DataBase,btn_attr->menu_item_coord);
                if (btn_attr->submenu_available == 1)
                    make_and_show_menu(triger_btn,menu,menu_to_create,btn_attr->menu_item_coord);
            }
            case 4:{
                if (menu->curent_level != 1){
                    rewrite_group(menu->maingroup,menu->visible_menu_list[menu->curent_level-2]);
                    lv_obj_del(menu->visible_menu_list[menu->curent_level-1]);
                    menu->visible_menu_list = (lv_obj_t **)realloc(menu->visible_menu_list,(menu->curent_level-1)*sizeof(lv_obj_t *));
                    menu->curent_level += -1;
                }
                else {
                    menu->curent_level = 0;
                    lv_obj_del(menu->visible_menu_list[0]);
                    menu->visible_menu_list = (lv_obj_t **)realloc(menu->visible_menu_list,0*sizeof(lv_obj_t *));
                    lv_group_add_obj(menu->maingroup,menu->first_btn);
                }
            }
        }
    }
}

void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu){
    lv_group_remove_all_objs(maingroup);
    lv_obj_t * btn = lv_list_get_next_btn(previous_menu,NULL);
    while (btn != NULL){
        lv_group_add_obj(maingroup,btn);
        btn = lv_list_get_next_btn(previous_menu,btn);
    }
}

int * get_coord(MENU * mainmenu,int i){
    int * new_coord;
    int mem_size = mainmenu->DataBase->CoordBase[i-1][0]*sizeof(int);
    new_coord = (int *)malloc(mem_size);
    memcpy(new_coord,mainmenu->DataBase->CoordBase[i-1],mem_size);
    return(new_coord);
}

void set_menu_labels(MENU * mainmenu,int * coord,char ** labels){
    init_menu_item(mainmenu->DataBase,coord);
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->menu_labels = rewrite_labels(labels);
}

void set_menu_size(MENU*mainmenu,int * coord,int height,int weight){
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->size_of_menu.size_x = weight;
    one_menu->size_of_menu.size_y = height;
}

void set_size_to_all_menu(MENU * mainmenu,int height,int weight){
    Menu_DB * DB = mainmenu->DataBase;
    int iter = 0;
    while (iter!=(DB->DB_size)){
        DB->MenuBase[iter]->size_of_menu.size_x = weight;
        DB->MenuBase[iter]->size_of_menu.size_y = height;
        iter++;
    }
    mainmenu->first_menu->size_of_menu.size_x = weight;
    mainmenu->first_menu->size_of_menu.size_y = height;
}

void set_align_to_all_menu(MENU * mainmenu,lv_own_align_t align){
    Menu_DB * DB = mainmenu->DataBase;
    int iter = 0;
    while (iter!=(DB->DB_size)){
        DB->MenuBase[iter]->align = align;
        iter++;
    }
    mainmenu->first_menu->align = align;
}


void set_menu_align(MENU * mainmenu,int * coord, lv_own_align_t align){
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->align = align;
}

int check_submenu(Menu_DB * DataBase,int * coord){
    int available = 0;
    int iter = 0;
    int size_cmp = coord[0]*sizeof(int);
    while (iter!=(DataBase->DB_size) && available != 1){
        if (memcmp(DataBase->CoordBase[iter],coord,size_cmp) == 0)
            available = 1;
        iter++;
    }
    return(available);
}

void set_extr_attr(lv_obj_t * btn,int iter,MENU * menu,void * coord){
    BtnItem * btn_attr = lv_obj_get_ext_attr(btn);
    btn_attr->menu = menu;
    btn_attr->btn_number = iter;
    if (menu->curent_level == 1){
        btn_attr->menu_item_coord = create_coord_next_menu(1,coord,iter);
    }
    else {
        int * fcoord = (int *)coord;
        btn_attr->menu_item_coord = create_coord_next_menu(*fcoord,coord,iter);
    }
    btn_attr->submenu_available = check_submenu(menu->DataBase,btn_attr->menu_item_coord);
}

char ** rewrite_labels(char ** labels){
    int iter = 1;
    char ** new_labels;
    while (strcmp(labels[iter-1],"") != 0){
        iter++;
    }
    int mem_size = iter*sizeof(char **);
    new_labels = (char**)malloc(mem_size*sizeof(char*));
    memcpy(new_labels,labels,mem_size);
    return(new_labels);
}

void set_menu_transparent(MENU * mainmenu){
    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_bg_opa(&opa_style,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    mainmenu->opa_style = &opa_style;
}

void set_align_to_menu(lv_obj_t * triger_btn,lv_obj_t * list_obj,lv_own_align_t align){
    switch(align){
        case LEFT:{
        lv_obj_align(list_obj,triger_btn,LV_ALIGN_OUT_LEFT_TOP,0,0);
        break;
        }
        case RIGHT:{
        lv_obj_align(list_obj,triger_btn,LV_ALIGN_OUT_RIGHT_TOP,0,0);
        break;
        }
        case BOTTOM:{
        lv_obj_align(list_obj,triger_btn,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        break;
        }
    }
}

void make_and_show_menu(lv_obj_t * triger_btn,MENU * mainmenu,MenuItem * menu, void * coord){
    int iter = 0;
    lv_obj_t * btn;
    lv_group_remove_all_objs(mainmenu->maingroup);
    mainmenu->curent_level++;
    lv_obj_t * list_obj = lv_list_create(lv_scr_act(),NULL);
    while (strcmp(menu->menu_labels[iter],"")!=0){
        btn = lv_list_add_btn(list_obj,NULL,menu->menu_labels[iter]);
        if (mainmenu->opa_style != NULL)
            lv_obj_add_style(btn,LV_OBJ_PART_MAIN,mainmenu->opa_style);
        lv_group_add_obj(mainmenu->maingroup,btn);
        lv_obj_allocate_ext_attr(btn,sizeof(BtnItem));
        set_extr_attr(btn,iter,mainmenu,coord);
        lv_obj_set_event_cb(btn,standart_menu_cb);
        iter++;
    }
    lv_obj_set_size(list_obj,menu->size_of_menu.size_x,menu->size_of_menu.size_y);
    set_align_to_menu(triger_btn,list_obj,menu->align);
    if (mainmenu->opa_style != NULL)
        lv_obj_add_style(list_obj,LV_OBJ_PART_MAIN,mainmenu->opa_style);
    mainmenu->visible_menu_list = (lv_obj_t **)realloc(mainmenu->visible_menu_list,(mainmenu->curent_level)*sizeof(lv_obj_t *));
    mainmenu->visible_menu_list[mainmenu->curent_level-1] = list_obj;
}

void init_menu(MENU * mainmenu,lv_obj_t * triger_btn){
    mainmenu->visible_menu_list = (lv_obj_t **)malloc(sizeof(lv_obj_t *));
    mainmenu->curent_level = 0;
    mainmenu->DataBase = (Menu_DB *)malloc(sizeof(Menu_DB));
    mainmenu->DataBase->DB_size = 0;
    mainmenu->DataBase->CoordBase = (int**)malloc(sizeof(int*));
    mainmenu->DataBase->MenuBase = (MenuItem **)malloc(sizeof(MenuItem*));
    mainmenu->first_menu = (MenuItem *)malloc(sizeof(MenuItem));
    mainmenu->first_menu->menu_labels = (char **)malloc(sizeof(char *));
    mainmenu->first_btn = triger_btn;
    mainmenu->opa_style = NULL;
}

void lv_menu_create(MENU * mainmenu,lv_obj_t * triger_btn,char ** labels){
    int coord = 1;
    mainmenu->first_menu->menu_labels = rewrite_labels(labels);
    make_and_show_menu(triger_btn,mainmenu,mainmenu->first_menu,coord);
}

