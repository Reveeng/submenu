#include "lvgl/lvgl.h"

#include <string.h>
#include <malloc.h>
#include "A:\lv_sim_codeblocks_win-master\submenu\lv_menu.h"

MenuItem * get_menu_item(MenuItem * prev_menu,int level,int * coord,int iter){
    if (level != 1 ){
        MenuItem * next_menu = get_menu_item(prev_menu->submenus[coord[iter]-1],level-1,coord,iter+1);
        return(next_menu);
    }
    else{
        return(prev_menu);
    }
}

void init_menu_item(MenuItem * prev_menu,int level,int * coord){
    MenuItem * menu_item_before_initializing_menu = get_menu_item(prev_menu,level-1,coord,1);
    if (coord[level-1] == 1){
        menu_item_before_initializing_menu->submenus = (MenuItem **)malloc(sizeof(MenuItem *));
    }
    else{
        menu_item_before_initializing_menu->submenus = (MenuItem **)realloc(coord[level-1]*sizeof(MenuItem *));
    }
}

void set_menu_labels(MENU * mainmenu,int * coord,char ** labels){
    init_menu_item(mainmenu->first_menu,coord[0],coord);
    MenuItem * one_menu = get_menu_item(mainmenu->first_menu,coord[0],coord,1);
    one_menu->submenus = NULL;
    one_menu->menu_labels = (char **)malloc(sizeof(char *));
    int iter = 0;
    while (strcmp(labels[iter],"")!=0){
        one_menu->menu_labels = (char **)realloc(one_menu->menu_labels,(iter+1)*sizeof(char *))
        one_menu->menu_labels[iter] = (char *)realloc(one_menu->menu_labels[iter],30*sizeof(char));
        one_menu->menu_labels[iter] = labels[iter];
    }
}

void set_menu_size(MENU*mainmenu,int * coord,int height,int weight){
    MenuItem * one_menu = get_menu_item(mainmenu->first_menu,coord[0],coord,1);
    one_menu->size_of_menu.size_x = weight;
    one_menu->size_of_menu.size_y = height;
}
//TODO
/**void set_menu_size_recoursive(MenuItem * menu,int height,int weight){
    if (menu->submenus != NULL){

    }

}

void set_size_to_all_menu(MENU * mainmenu,int height,int weight){

}*/


