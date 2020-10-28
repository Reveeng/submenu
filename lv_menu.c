#include "lvgl/lvgl.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"

//принимает на вход базу данных с меню и координаты меню
//возвращает структуру, которая характеризует подменю
MenuItem * get_menu_item(MENU * menu,int * coord){
    int size_cmp = coord[0]*sizeof(int);
    int iter = 0;
    while (memcmp(menu->CoordBase[iter],coord,size_cmp) != 0 ){
        iter++;
    }
    MenuItem * Item;
    Item = menu->MenuBase[iter];
    return(Item);
}
//задает функцию коллбэка для нужной кнопки
//принимает координаты подменю, которое содержит кнопку, номер кнопки и саму функцию коллбэка
//функция записывается в специальный массив, в котором содержатся все функциюю для кнопок данного подменю
void set_cb_to_btn(MENU * menu,int * coord,int btn_number,lv_event_cb_t callback){
    MenuItem * Item = get_menu_item(menu,coord);
    if (btn_number ==  1)
        Item->list_of_callbacks = (lv_event_cb_t *)malloc(sizeof(lv_event_cb_t));
    else
        Item->list_of_callbacks = (lv_event_cb_t *)realloc(Item->list_of_callbacks,btn_number*sizeof(lv_event_cb_t));
    Item->list_of_callbacks[btn_number-1] = callback;
}
//функция создает массив с координатами предыдущего меню
int * create_coord_prev_menu(int * coord){
    int * prev_coord;
    if (coord[0] == 2){
        prev_coord = (int *)malloc(2*sizeof(int));
        prev_coord[0] = 1;
        prev_coord[0] = 0;

    }
    else{
        int buf_size = (coord[0]-1)*sizeof(int);
        prev_coord = (int *)malloc(buf_size);
        memcpy(prev_coord,coord,buf_size);
        prev_coord[0]-=1;
    }
    return(prev_coord);
}

//функция создает массив с координатами меню, которое будет создано по нажатию кнопки
int * create_coord_next_menu(int * coord,int btn_number){
    int * new_coord;
    int level = coord[0];
    new_coord = (int *)malloc((level+1)*sizeof(int));
    if (level == 1){
        new_coord[0] = 2;
        new_coord[1] = btn_number+1;
    }
    else{
        int mem_size = level*sizeof(int);
        memcpy(new_coord,coord,mem_size);
        new_coord[0]++;
        new_coord[level] = btn_number+1;
    }
    return(new_coord);
}

//функция стандартного коллбэка для кнопки меню
void standart_menu_cb(lv_obj_t * triger_btn,lv_event_t event){
    BtnItem * btn_attr = lv_obj_get_ext_attr(triger_btn);
    MENU * menu = btn_attr->menu;
    #if USE_ENCODER == 0
    if (event == LV_EVENT_CLICKED){
        //если доступно подменю для данной кнопки, оно создается
        if (btn_attr->submenu_available == 1){
            MenuItem * menu_to_create = get_menu_item(menu,btn_attr->menu_item_coord);
            make_and_show_menu(triger_btn,menu,btn_attr->menu_item_coord);
        }
    //если не доступно, проверяется наличие заданной функции коллбэка, при наличии функция вызывается
        else{
            if (menu->vis_menu->curent_level != 1){
                int * prev_coord;
                prev_coord = create_coord_prev_menu(btn_attr->menu_item_coord);
                MenuItem * menu_cont_btn = get_menu_item(menu,prev_coord);
                if (menu_cont_btn->list_of_callbacks != NULL){
                    lv_event_cb_t callback = menu_cont_btn->list_of_callbacks[btn_attr->btn_number];
                    callback(triger_btn,event);
                }
            }
        }
    }
    if (event == LV_EVENT_LONG_PRESSED){
        if (menu->vis_menu->curent_level != 1){
                rewrite_group(menu->maingroup,menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-2]);
                lv_obj_del(menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-1]);
                menu->vis_menu->visible_menu_list = (lv_obj_t **)realloc(menu->vis_menu->visible_menu_list,(menu->vis_menu->curent_level-1)*sizeof(lv_obj_t *));
                menu->vis_menu->curent_level += -1;

            }
        else {
            menu->vis_menu->curent_level = 0;
            lv_group_remove_all_objs(menu->maingroup);
            lv_obj_del(menu->vis_menu->visible_menu_list[0]);
            free(menu->vis_menu->visible_menu_list);
            free(menu->vis_menu);
            lv_group_add_obj(menu->maingroup,menu->vis_menu->first_btn);
        }
    }
    #else
        if (event == HW_EVENT_KEY_PRESSED){
        int key;
        lv_key_t * key = (lv_key_t *)lv_event_get_data();
        if ((*key)>0{
            if (*key & HW_KEY_CODE_MENU){
                if (btn_attr->submenu_available == 1){
                    make_and_show_menu(triger_btn,menu,btn_attr->menu_item_coord);
                }
                else{
                    if (menu->curent_level != 1){
                        int * prev_coord;
                        prev_coord = create_coord_prev_menu(btn_attr->menu_item_coord);
                        MenuItem * menu_cont_btn = get_menu_item(menu->DataBase,prev_coord);
                        if (menu_cont_btn->list_of_callbacks != NULL){
                            lv_event_cb_t callback = menu_cont_btn->list_of_callbacks[btn_attr->btn_number];
                            callback(triger_btn,event);
                        }
                    }
                }
            }
            if (*key & HW_KEY_CODE_ZOOM){
                if (menu->vis_menu->curent_level != 1){
                    rewrite_group(menu->maingroup,menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-2]);
                    lv_obj_del(menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-1]);
                    menu->vis_menu->visible_menu_list = (lv_obj_t **)realloc(menu->vis_menu->visible_menu_list,(menu->vis_menu->curent_level-1)*sizeof(lv_obj_t *));
                    menu->vis_menu->curent_level += -1;
                }
                else {
                    menu->vis_menu->curent_level = 0;
                    lv_group_remove_all_objs(menu->maingroup);
                    lv_obj_del(menu->vis_menu->visible_menu_list[0]);
                    free(menu->vis_menu->visible_menu_list);
                    free(menu->vis_menu);
                    lv_group_add_obj(menu->maingroup,menu->vis_menu->first_btn);
                }
            }
        }
    }
    #endif
}
//перезаписывает основную группу при удалении верхнего меню, на меня предыдущего уровня
void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu){
    lv_group_remove_all_objs(maingroup);
    lv_obj_t * btn = lv_list_get_next_btn(previous_menu,NULL);
    while (btn != NULL){
        lv_group_add_obj(maingroup,btn);
        btn = lv_list_get_next_btn(previous_menu,btn);
    }
}
//временно не используется
int * get_coord(MENU * mainmenu,int i){
    int * new_coord;
    int mem_size = mainmenu->CoordBase[i-1][0]*sizeof(int);
    new_coord = (int *)malloc(mem_size);
    memcpy(new_coord,mainmenu->CoordBase[i-1],mem_size);
    return(new_coord);
}

//функция задающая размеры конкретного подменю, находящегося по нужным координатам
void set_menu_size(MENU * menu,int * coord,int height,int weight){
    MenuItem * one_menu = get_menu_item(menu,coord);
    one_menu->size_of_menu.size_x = weight;
    one_menu->size_of_menu.size_y = height;
}


//функция которою можно вызвать если нужно поменять, то с какой стороны будет находится меню от нажатой кнопки, сделана для того,чтобы для каждого меню можно было задать собственную ориентацию
void set_menu_align(MENU * menu,int * coord, lv_own_align_t align){
    MenuItem * one_menu = get_menu_item(menu,coord);
    one_menu->align = align;
}
//функция проверяющая доступность меню по выбранным координатам
int check_submenu(MENU * menu,int * coord){
    int available = 0;
    int iter = 0;
    int size_cmp = coord[0]*sizeof(int);
    while (iter!=(menu->DB_size) && available != 1){
        if (memcmp(menu->CoordBase[iter],coord,size_cmp) == 0)
            available = 1;
        iter++;
    }
    return(available);
}
//функция задаёт дополнительные атрибуты для кнопки, чтобы упростить навигацию в меню
void set_extr_attr(lv_obj_t * btn,int iter,MENU * menu,int * coord){
    BtnItem * btn_attr = lv_obj_get_ext_attr(btn);
    btn_attr->menu = menu;
    btn_attr->btn_number = iter;
    btn_attr->menu_item_coord = create_coord_next_menu(coord,iter);
//здесь проверяется существует ли меню в базе данных с таким координатами, если да, то 1, если нет, то 0
    btn_attr->submenu_available = check_submenu(menu,btn_attr->menu_item_coord);
}
//вспомогательная функция которая просто помогает скопировать строки из одной переменной в другую
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
//вызывается если нужно сделать меню прозрачным
void set_menu_transparent(MENU * menu){
    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_bg_opa(&opa_style,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    lv_style_set_bg_opa(&opa_style,LV_STATE_FOCUSED,LV_OPA_10);
    lv_style_set_border_color(&opa_style,LV_STATE_DEFAULT,LV_COLOR_BLUE);
    lv_style_set_bg_color(&opa_style,LV_STATE_FOCUSED,LV_COLOR_BLUE);
    lv_style_set_outline_opa(&opa_style,LV_STATE_FOCUSED,LV_OPA_TRANSP);
    lv_style_set_border_opa(&opa_style,LV_STATE_DEFAULT,LV_OPA_10);
    menu->opa_style = &opa_style;
    menu->btn_style = &opa_style;
}

void set_style_to_menu(MENU * menu,lv_style_t * style){
    menu->opa_style = style;
}

void set_style_to_btn(MENU * menu,lv_style_t * style){
    menu->btn_style = style;
}

//функция которой задаю где должно распологатьтся меню, для простоты ввел свой тип lv_own_align_t, который просто показывает с какой стороны это делать
//triger_btn - указатель на нажатую кнопку
//list_obj - указатель на list который хранит в себе все кнопки
//align - это просто перечисляемое, надо выбрать одно из 3(LEFT,RIGHT,BOTTOM)
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
//function that create list_obj and all btn on it, then add list_obj to array of visible menu
//menu - ptr to struct that contain information about menu(size,align,labels)
//coord - ptr to array that indicate where menu should be. For example {3,1,1} tell us that menu should be in 3 level, on the first level before we click on btn 1, and on the second level we click on btn 1
//coord - need for navigate on DataBase and tell function`s where we need to build menu
void make_and_show_menu(lv_obj_t * triger_btn,MENU * menu, int * coord){
    int iter = 0;
    lv_obj_t * btn;
    lv_group_remove_all_objs(menu->maingroup);
    menu->vis_menu->curent_level++;
    //нужно переписать, чтобы 1 меню было по координатам {1,0}
    lv_obj_t * list_obj = lv_list_create(lv_scr_act(),NULL);//create list obj
    MenuItem * Item = get_menu_item(menu,coord);
    while (strcmp(Item->menu_labels[iter],"")!=0){
        btn = lv_list_add_btn(list_obj,NULL,Item->menu_labels[iter]);
        if (menu->btn_style != NULL)
            lv_obj_add_style(btn,LV_OBJ_PART_MAIN,menu->btn_style);//задаю прозрачное кнопки если нужно
        lv_group_add_obj(menu->maingroup,btn);
        lv_obj_allocate_ext_attr(btn,sizeof(BtnItem));
        set_extr_attr(btn,iter,menu,coord);//дописываются нужные мне атриубуты в кнопку
        lv_obj_set_event_cb(btn,standart_menu_cb);
        iter++;
    }
    lv_obj_set_size(list_obj,Item->size_of_menu.size_x,Item->size_of_menu.size_y);
    set_align_to_menu(triger_btn,list_obj,Item->align);
    if (menu->opa_style != NULL)
        lv_obj_add_style(list_obj,LV_OBJ_PART_MAIN,menu->opa_style);//задаю прозрачный БГ для list_obj
    menu->vis_menu->visible_menu_list = (lv_obj_t **)realloc(menu->vis_menu->visible_menu_list,(menu->vis_menu->curent_level)*sizeof(lv_obj_t *));
    menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-1] = list_obj;
}

//функция вызывается в коллбэке кнопки по нажатию на которую должно вызываться меню
//mainmenu - ptr to struct that contain all information about all menu`s
//triger_btn - ptr to btn object smth like that
//labels - array that contain labels for first menu
void lv_menu_create(lv_obj_t * triger_btn){
    special_menu_btn * attr = lv_obj_get_ext_attr(triger_btn);
    MENU * menu = attr->menu;
    menu->vis_menu = (visible_menu *)malloc(sizeof(visible_menu));
    menu->vis_menu->curent_level = 0;
    menu->vis_menu->visible_menu_list = (lv_obj_t **)malloc(sizeof(lv_obj_t *));
    menu->vis_menu->first_btn = triger_btn;
    int coord[2] = {1,0};
    make_and_show_menu(triger_btn,menu,coord);
}
//функция которая позволяет закрыть все открытые меню
void close_all_menu(MENU * mainmenu){
     int iter = 0;
     while (iter != mainmenu->vis_menu->curent_level){
        lv_obj_del(mainmenu->vis_menu->visible_menu_list[mainmenu->vis_menu->curent_level-1]);
        mainmenu->vis_menu->curent_level += -1;
     }
     free(mainmenu->vis_menu->visible_menu_list);
     free(mainmenu->vis_menu);
}
//задаем размеры всем подменю
void set_size_to_all_menu(MENU * menu,int height,int weight){
    int iter = 0;
    while (iter!=(menu->DB_size)){
        menu->MenuBase[iter]->size_of_menu.size_x = weight;
        menu->MenuBase[iter]->size_of_menu.size_y = height;
        iter++;
    }
}

//функция задающая ориентацию для каждого подменю
void set_align_to_all_menu(MENU * menu,lv_own_align_t align){
    int iter = 0;
    while (iter!=(menu->DB_size)){
        menu->MenuBase[iter]->align = align;
        iter++;
    }
}

//Нужно вызывать после привязки меню к кнопке,заполняет данные структуры названиями кнопок и координатами меню
//То есть заранее заполняем массивы координат и меню
void set_menu_labels(MENU * menu,int * coord,char ** labels){
    menu->DB_size++;//храню размер массивов в базе данных
    int byte_size_to_copy;
    if (coord[0] == 1)
        byte_size_to_copy = 2*sizeof(int);
    else
        byte_size_to_copy = coord[0]*sizeof(int);//размер буфера для копирования байт
    menu->CoordBase = (int **)realloc(menu->CoordBase,menu->DB_size*sizeof(int*));//выделяю доп.память для массива координат
    menu->CoordBase[menu->DB_size-1] = (int *)malloc(coord[0]*sizeof(int));//выделяю память под массив с координатами
    menu->MenuBase = (MenuItem **)realloc(menu->MenuBase,menu->DB_size*sizeof(MenuItem*));//выделяю доп. память для массива MenuItem`ов
    menu->MenuBase[menu->DB_size-1] = (MenuItem *)malloc(sizeof(MenuItem));//выделяю память под структуру
    menu->MenuBase[menu->DB_size-1]->menu_labels = rewrite_labels(labels);
    menu->MenuBase[menu->DB_size-1]->list_of_callbacks=NULL;//задаю нулевой указатель для массива функция коллбэка, на случай если коллбэки не будут заданы
    memcpy(menu->CoordBase[menu->DB_size-1],coord,byte_size_to_copy);//копирую координаты
}
//функция дописывает мою структуру в доолнительные атрибуты кнопки из-за чего мне не нужно выделять/освобождать память самому заранее
//На вход принимает указатель на кнопку
//Возвращает указатель на структуру меню, которую впоследствии можно заполнить
MENU * bind_menu_to_obj(lv_obj_t * triger_obj){
    lv_obj_allocate_ext_attr(triger_obj,sizeof(special_menu_btn));
    special_menu_btn * attr = lv_obj_get_ext_attr(triger_obj);
    attr->menu = (MENU *)malloc(sizeof(MENU));
    attr->menu->CoordBase = (int **)malloc(sizeof(int*));
    attr->menu->MenuBase = (MenuItem **)malloc(sizeof(MenuItem *));
    attr->menu->maingroup = lv_group_create();
    lv_group_add_obj(attr->menu->maingroup,triger_obj);
    attr->menu->opa_style = NULL;
    attr->menu->btn_style = NULL;
    attr->menu->vis_menu = NULL;
    attr->menu->DB_size = 0;
    return(attr->menu);
}
