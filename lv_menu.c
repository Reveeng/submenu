#include "lvgl/lvgl.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"

//принимает на вход базу данных с меню и координаты меню
//возвращает структуру, которая характеризует подменю
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
//задает функцию коллбэка для нужной кнопки
//принимает координаты подменю, которое содержит кнопку, номер кнопки и саму функцию коллбэка
//функция записывается в специальный массив, в котором содержатся все функциюю для кнопок данного подменю
void set_cb_to_btn(MENU * mainmenu,int * coord,int btn_number,lv_event_cb_t callback){
    MenuItem * Item = get_menu_item(mainmenu->DataBase,coord);
    if (btn_number ==  1)
        Item->list_of_callbacks = (lv_event_cb_t *)malloc(sizeof(lv_event_cb_t));
    else
        Item->list_of_callbacks = (lv_event_cb_t *)realloc(Item->list_of_callbacks,btn_number*sizeof(lv_event_cb_t));
    Item->list_of_callbacks[btn_number-1] = callback;
}
//функция создает массив с координатами предыдущего меню
int * create_coord_prev_menu(int * coord){
        int buf_size = (coord[0]-1)*sizeof(int);
        int * prev_coord;
        prev_coord = (int *)malloc(buf_size);
        memcpy(prev_coord,coord,buf_size);
        prev_coord[0]-=1;
        return(prev_coord);
}
//инициализирует MenuItem в базе данных, т.к. для записи используются динамические массивы
void init_menu_item(Menu_DB * DataBase,int * coord){
    DataBase->DB_size++;//храню размер массивов в базе данных
    int byte_size_to_copy = coord[0]*sizeof(int);//размер буфера для копирования байт
    DataBase->CoordBase = (int **)realloc(DataBase->CoordBase,DataBase->DB_size*sizeof(int*));//выделяю доп.память для массива координат
    DataBase->CoordBase[DataBase->DB_size-1] = (int *)malloc(coord[0]*sizeof(int));//выделяю память под массив с координатами
    DataBase->MenuBase = (MenuItem **)realloc(DataBase->MenuBase,DataBase->DB_size*sizeof(MenuItem*));//выделяю доп. память для массива MenuItem`ов
    DataBase->MenuBase[DataBase->DB_size-1] = (MenuItem *)malloc(sizeof(MenuItem));//выделяю память под структуру
    DataBase->MenuBase[DataBase->DB_size-1]->list_of_callbacks=NULL;//задаю нулевой указатель для массива функция коллбэка, на случай если коллбэки не будут заданы
    memcpy(DataBase->CoordBase[DataBase->DB_size-1],coord,byte_size_to_copy);//копирую координаты
}
//функция создает массив с координатами меню, которое будет создано по нажатию кнопки
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
//функция стандартного коллбэка для кнопки меню
void standart_menu_cb(lv_obj_t * triger_btn,lv_event_t event){
    BtnItem * btn_attr = lv_obj_get_ext_attr(triger_btn);
    MENU * menu = btn_attr->menu;
    if (event == LV_EVENT_CLICKED){
        //если доступно подменю для данной кнопки, оно создается
        if (btn_attr->submenu_available == 1){
            MenuItem * menu_to_create = get_menu_item(btn_attr->menu->DataBase,btn_attr->menu_item_coord);
            make_and_show_menu(triger_btn,menu,menu_to_create,btn_attr->menu_item_coord);
        }
    //если не доступно, проверяется наличие заданной функции коллбэка, при наличии функция вызывается
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
    if (event == LV_EVENT_LONG_PRESSED){
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
    int mem_size = mainmenu->DataBase->CoordBase[i-1][0]*sizeof(int);
    new_coord = (int *)malloc(mem_size);
    memcpy(new_coord,mainmenu->DataBase->CoordBase[i-1],mem_size);
    return(new_coord);
}
//функция, которая задает названия кнопок нужного подменю. Для каждого подменю нужно вызывать каждый раз эту функцию
//желательно вызывать по порядку, но не обязательно
//функция задает MenuItem и его координаты в базе меню, поэтому эта функция вызывается после init_menu но до lv_menu_create
void set_menu_labels(MENU * mainmenu,int * coord,char ** labels){
    init_menu_item(mainmenu->DataBase,coord);
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->menu_labels = rewrite_labels(labels);
}
//функция задающая размеры конкретного подменю, находящегося по нужным координатам
void set_menu_size(MENU*mainmenu,int * coord,int height,int weight){
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->size_of_menu.size_x = weight;
    one_menu->size_of_menu.size_y = height;
}
//функция задающая размеры для всех подменю
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
//функция задающая ориентацию для каждого подменю
void set_align_to_all_menu(MENU * mainmenu,lv_own_align_t align){
    Menu_DB * DB = mainmenu->DataBase;
    int iter = 0;
    while (iter!=(DB->DB_size)){
        DB->MenuBase[iter]->align = align;
        iter++;
    }
    mainmenu->first_menu->align = align;
}

//функция которою можно вызвать если нужно поменять, то с какой стороны будет находится меню от нажатой кнопки, сделана для того,чтобы для каждого меню можно было задать собственную ориентацию
void set_menu_align(MENU * mainmenu,int * coord, lv_own_align_t align){
    MenuItem * one_menu = get_menu_item(mainmenu->DataBase,coord);
    one_menu->align = align;
}
//функция проверяющая доступность меню по выбранным координатам
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
//функция задаёт дополнительные атрибуты для кнопки, чтобы упростить навигацию в меню
void set_extr_attr(lv_obj_t * btn,int iter,MENU * menu,void * coord){
    BtnItem * btn_attr = lv_obj_get_ext_attr(btn);
    btn_attr->menu = menu;
    btn_attr->btn_number = iter;
//дальше создается массив координат, которые соответствуют меню,которое будет создаваться по нажатию кнопки
    if (menu->curent_level == 1){
        btn_attr->menu_item_coord = create_coord_next_menu(1,coord,iter);
    }
    else {
        int * fcoord = (int *)coord;
        btn_attr->menu_item_coord = create_coord_next_menu(*fcoord,coord,iter);
    }
//здесь проверяется существует ли меню в базе данных с таким координатами, если да, то 1, если нет, то 0
    btn_attr->submenu_available = check_submenu(menu->DataBase,btn_attr->menu_item_coord);
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
void set_menu_transparent(MENU * mainmenu){
    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_bg_opa(&opa_style,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    mainmenu->opa_style = &opa_style;
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
void make_and_show_menu(lv_obj_t * triger_btn,MENU * mainmenu,MenuItem * menu, void * coord){
    int iter = 0;
    lv_obj_t * btn;
    lv_group_remove_all_objs(mainmenu->maingroup);
    mainmenu->curent_level++;
    lv_obj_t * list_obj = lv_list_create(lv_scr_act(),NULL);//create list obj
    while (strcmp(menu->menu_labels[iter],"")!=0){
        btn = lv_list_add_btn(list_obj,NULL,menu->menu_labels[iter]);
        if (mainmenu->opa_style != NULL)
            lv_obj_add_style(btn,LV_OBJ_PART_MAIN,mainmenu->opa_style);//задаю прозрачное кнопки если нужно
        lv_group_add_obj(mainmenu->maingroup,btn);
        lv_obj_allocate_ext_attr(btn,sizeof(BtnItem));
        set_extr_attr(btn,iter,mainmenu,coord);//дописываются нужные мне атриубуты в кнопку
        lv_obj_set_event_cb(btn,standart_menu_cb);
        iter++;
    }
    lv_obj_set_size(list_obj,menu->size_of_menu.size_x,menu->size_of_menu.size_y);
    set_align_to_menu(triger_btn,list_obj,menu->align);
    if (mainmenu->opa_style != NULL)
        lv_obj_add_style(list_obj,LV_OBJ_PART_MAIN,mainmenu->opa_style);//задаю прозрачный БГ для list_obj
    mainmenu->visible_menu_list = (lv_obj_t **)realloc(mainmenu->visible_menu_list,(mainmenu->curent_level)*sizeof(lv_obj_t *));
    mainmenu->visible_menu_list[mainmenu->curent_level-1] = list_obj;
}
//initialize menu struct for better working,need to call it first, before other function
//mainmenu - ptr to struct that contain all information about menu
//triger_btn - ptr to btn object smth like that
void init_menu(MENU * mainmenu,lv_obj_t * triger_btn){
    mainmenu->maingroup = lv_group_create();
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
//create first menu, need to call after initialize Menu Data Base
//mainmenu - ptr to struct that contain all information about all menu`s
//triger_btn - ptr to btn object smth like that
//labels - array that contain labels for first menu
void lv_menu_create(MENU * mainmenu,lv_obj_t * triger_btn,char ** labels){
    int coord = 1;
    mainmenu->first_menu->menu_labels = rewrite_labels(labels);
    make_and_show_menu(triger_btn,mainmenu,mainmenu->first_menu,coord);
}
void close_all_menu(MENU * mainmenu){
     int iter = 0;
     while (iter != mainmenu->curent_level){
        lv_obj_del(mainmenu->visible_menu_list[mainmenu->curent_level-1]);
        mainmenu->curent_level += -1;
     }
}

