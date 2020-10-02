#ifndef MY_PROGRAMMS_H_INCLUDED
#define MY_PROGRAMMS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
typedef struct submenu_t {
    char * first_menu_labels[2][2];//������ �������� ������ ��� ������� ����
    char * second_menu_labels[4][3];//������ �������� ������ ��� ������� ����
    int number_of_btn_in_first_menu[2];//������ ����������� ������ � 1 ����
    int number_of_btn_in_sec_menu[3];//������ ����������� ������ ��  ����
    lv_obj_t * first_menu;//��������� �� ������ ����
    lv_obj_t * second_menu;//�������� �� ������ ����
    lv_obj_t * first_btns[2];// ��� �������� ����� ����������� 2 ������ � �������
    lv_group_t * maingroup;// �������� �� ������ ������� ��������� �������
} MENU;

void submenu(MENU * mainmenu);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // MY_PROGRAMMS_H_INCLUDED
