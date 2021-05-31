#include"header.h"
//显示绝对目录.
void Ab_dir() {
    for (int i = 0; i < dir_pointer; i++)
        printf("%s/", ab_dir[i]);
    printf("\n");
}
