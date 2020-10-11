#ifndef MY_PROGRAMMS_H_INCLUDED
#define MY_PROGRAMMS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
typedef struct submenu_t {
    lv_obj_t * Lmenu;
    lv_obj_t * first_btns[2];// ��� �������� ����� ����������� 2 ������ � �������
    lv_group_t * maingroup;// �������� �� ������ ������� ��������� �������
    int number_of_menu_levels;
} MENU;

void submenu(MENU * mainmenu);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // MY_PROGRAMMS_H_INCLUDED
