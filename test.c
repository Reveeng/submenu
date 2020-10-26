#include <malloc.h>
#include <stdio.h>
#include <string.h>

void action1(void)
{
    printf("Action 1");
}
void action2(void)
{
    printf("Action 2");

}
void action3(void)
{
    printf("Action 3");
}

void main(void){
    int i = 3;
    void (* actions[3]());
    actions[0] = action1;
    actions[1] = action2;
    actions[2] = action3;
    *actions[0]();
}
