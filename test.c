#include <malloc.h>
#include <stdio.h>

typedef struct {
    int size_x;
    int size_y;
} MySize;

typedef struct {
    MySize ** sizes;
    char ** labels;
}something;




void main(void){
    MySize * A;
    A = (MySize *)malloc(2*sizeof(MySize));
    A[0].size_x = 2;
    A[0].size_y = 3;
    A[1].size_x = 4;
    A[1].size_y = 5;
    printf("%d",A[0].size_x);
    free(A);
}
