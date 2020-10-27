#include "lvgl/lvgl.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"

//��������� �� ���� ���� ������ � ���� � ���������� ����
//���������� ���������, ������� ������������� �������
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

//������ ������� �������� ��� ������ ������
//��������� ���������� �������, ������� �������� ������, ����� ������ � ���� ������� ��������
//������� ������������ � ����������� ������, � ������� ���������� ��� �������� ��� ������ ������� �������
void set_cb_to_btn(MENU * menu,int * coord,int btn_number,lv_event_cb_t callback){
    MenuItem * Item = get_menu_item(menu,coord);
    if (btn_number ==  1)
        Item->list_of_callbacks = (lv_event_cb_t *)malloc(sizeof(lv_event_cb_t));
    else
        Item->list_of_callbacks = (lv_event_cb_t *)realloc(Item->list_of_callbacks,btn_number*sizeof(lv_event_cb_t));
    Item->list_of_callbacks[btn_number-1] = callback;
}
//������� ������� ������ � ������������ ����������� ����
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

//������� ������� ������ � ������������ ����, ������� ����� ������� �� ������� ������
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
//������� ������������ �������� ��� ������ ����
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
            case 4:{
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
}
//�������������� �������� ������ ��� �������� �������� ����, �� ���� ����������� ������
void rewrite_group(lv_group_t * maingroup,lv_obj_t * previous_menu){
    lv_group_remove_all_objs(maingroup);
    lv_obj_t * btn = lv_list_get_next_btn(previous_menu,NULL);
    while (btn != NULL){
        lv_group_add_obj(maingroup,btn);
        btn = lv_list_get_next_btn(previous_menu,btn);
    }
}

//�������, ������� ������ �������� ������ ������� �������. ��� ������� ������� ����� �������� ������ ��� ��� �������
//���������� �������� �� �������, �� �� �����������
//������� ������ MenuItem � ��� ���������� � ���� ����, ������� ��� ������� ���������� ����� init_menu �� �� lv_menu_create
void set_menu_labels(MENU * menu,int * coord,char ** labels){
    menu->DB_size++;//����� ������ �������� � ���� ������
    int byte_size_to_copy;
    if (coord[0] == 1)
        byte_size_to_copy = 2*sizeof(int);
    else
        byte_size_to_copy = coord[0]*sizeof(int);//������ ������ ��� ����������� ����
    menu->CoordBase = (int **)realloc(menu->CoordBase,menu->DB_size*sizeof(int*));//������� ���.������ ��� ������� ���������
    menu->CoordBase[menu->DB_size-1] = (int *)malloc(coord[0]*sizeof(int));//������� ������ ��� ������ � ������������
    menu->MenuBase = (MenuItem **)realloc(menu->MenuBase,menu->DB_size*sizeof(MenuItem*));//������� ���. ������ ��� ������� MenuItem`��
    menu->MenuBase[menu->DB_size-1] = (MenuItem *)malloc(sizeof(MenuItem));//������� ������ ��� ���������
    menu->MenuBase[menu->DB_size-1]->menu_labels = rewrite_labels(labels);
    menu->MenuBase[menu->DB_size-1]->list_of_callbacks=NULL;//����� ������� ��������� ��� ������� ������� ��������, �� ������ ���� �������� �� ����� ������
    memcpy(menu->CoordBase[menu->DB_size-1],coord,byte_size_to_copy);//������� ����������
}
//function that set sizes for one submenu
//coord - ptr to array of coordinates
void set_menu_size(MENU * menu,int * coord,int height,int weight){
    MenuItem * one_menu = get_menu_item(menu,coord);
    one_menu->size_of_menu.size_x = weight;
    one_menu->size_of_menu.size_y = height;
}
//function that set sizes for all submenu
void set_size_to_all_menu(MENU * menu,int height,int weight){
    int iter = 0;
    while (iter!=(menu->DB_size)){
        menu->MenuBase[iter]->size_of_menu.size_x = weight;
        menu->MenuBase[iter]->size_of_menu.size_y = height;
        iter++;
    }
}
//function that set align for all submenu
void set_align_to_all_menu(MENU * menu,lv_own_align_t align){
    int iter = 0;
    while (iter!=(menu->DB_size)){
        menu->MenuBase[iter]->align = align;
        iter++;
    }
}

//������� ������� ����� ������� ���� ����� ��������, �� � ����� ������� ����� ��������� ���� �� ������� ������,
//������� ��� ����,����� ��� ������� ���� ����� ���� ������ ����������� ����������
void set_menu_align(MENU * menu,int * coord, lv_own_align_t align){
    MenuItem * one_menu = get_menu_item(menu,coord);
    one_menu->align = align;
}

//������� ����������� ����������� ���� �� ��������� �����������
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
//function sets extra attributes for the button
//btn -  ptr to button
//iter -  button number
//coord - ptr to array of coordinates of the menu created by pressing this button
void set_extr_attr(lv_obj_t * btn,int iter,MENU * menu,int * coord){
    BtnItem * btn_attr = lv_obj_get_ext_attr(btn);
    btn_attr->menu = menu;
    btn_attr->btn_number = iter;
    btn_attr->menu_item_coord = create_coord_next_menu(coord,iter);
//����� ����������� ���������� �� ���� � ���� ������ � ����� ������������, ���� ��, �� 1, ���� ���, �� 0
    btn_attr->submenu_available = check_submenu(menu,btn_attr->menu_item_coord);
}

//function that copies one variable to another
//labels -  ptr to array of string
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
//function makes menu transparent
//mainmenu - ptr to main menu structure
void set_menu_transparent(MENU * menu){
    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_bg_opa(&opa_style,LV_STATE_DEFAULT,LV_OPA_TRANSP);
    menu->opa_style = &opa_style;
}
//the function sets the alignment for the menu
//triger_btn - ptr to pressed btn
//list_obj - ptr to list which contain all btn object
//align - enumerate, need to choose from 3(LEFT,RIGHT,BOTTOM)
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
    //����� ����������, ����� 1 ���� ���� �� ����������� {1,0}
    lv_obj_t * list_obj = lv_list_create(lv_scr_act(),NULL);//create list obj
    MenuItem * Item = get_menu_item(menu,coord);
    while (strcmp(Item->menu_labels[iter],"")!=0){
        btn = lv_list_add_btn(list_obj,NULL,Item->menu_labels[iter]);
        if (menu->opa_style != NULL)
            lv_obj_add_style(btn,LV_OBJ_PART_MAIN,menu->opa_style);//����� ���������� ������ ���� �����
        lv_group_add_obj(menu->maingroup,btn);
        lv_obj_allocate_ext_attr(btn,sizeof(BtnItem));
        set_extr_attr(btn,iter,menu,coord);//������������ ������ ��� ��������� � ������
        lv_obj_set_event_cb(btn,standart_menu_cb);
        iter++;
    }
    lv_obj_set_size(list_obj,Item->size_of_menu.size_x,Item->size_of_menu.size_y);
    set_align_to_menu(triger_btn,list_obj,Item->align);
    if (menu->opa_style != NULL)
        lv_obj_add_style(list_obj,LV_OBJ_PART_MAIN,menu->opa_style);//����� ���������� �� ��� list_obj
    menu->vis_menu->visible_menu_list = (lv_obj_t **)realloc(menu->vis_menu->visible_menu_list,(menu->vis_menu->curent_level)*sizeof(lv_obj_t *));
    menu->vis_menu->visible_menu_list[menu->vis_menu->curent_level-1] = list_obj;
}

//create first menu, need to call after initialize Menu Data Base
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

//������� ���������� ��� ��������� � ������������� �������� ������ ��-�� ���� ��� �� ����� ��������/����������� ������ ������ �������
//�� ���� ��������� ��������� �� ������
//���������� ��������� �� ��������� ����, ������� ������������ ����� ���������
MENU * bind_menu_to_obj(lv_obj_t * triger_obj){
    lv_obj_allocate_ext_attr(triger_obj,sizeof(special_menu_btn));
    special_menu_btn * attr = lv_obj_get_ext_attr(triger_obj);
    attr->menu = (MENU *)malloc(sizeof(MENU));
    attr->menu->CoordBase = (int **)malloc(sizeof(int*));
    attr->menu->MenuBase = (MenuItem **)malloc(sizeof(MenuItem *));
    attr->menu->maingroup = lv_group_create();
    lv_group_add_obj(attr->menu->maingroup,triger_obj);
    attr->menu->opa_style = NULL;
    attr->menu->vis_menu = NULL;
    attr->menu->DB_size = 0;
    return(attr->menu);
}

