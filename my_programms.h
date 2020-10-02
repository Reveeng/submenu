#ifndef MY_PROGRAMMS_H_INCLUDED
#define MY_PROGRAMMS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
typedef struct submenu_t {
    char * first_menu_labels[2][2];//массив названий кнопок для первого меню
    char * second_menu_labels[4][3];//массив названий кнопок для второго меню
    int number_of_btn_in_first_menu[2];//массив колличества кнопок в 1 меню
    int number_of_btn_in_sec_menu[3];//массив колличества кнопок во  меню
    lv_obj_t * first_menu;//указатель на первое меню
    lv_obj_t * second_menu;//указатль на второе меню
    lv_obj_t * first_btns[2];// для удобства храню изначальные 2 кнопки в массиве
    lv_group_t * maingroup;// указтель на группу которой управляет инкодер
} MENU;

void submenu(MENU * mainmenu);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // MY_PROGRAMMS_H_INCLUDED
