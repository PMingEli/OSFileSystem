#include "header.h"

void Printsign()
{
    printf("\033[32m%s\033[37m:", userName);
    for (int i = 0; i < dir_pointer - 1; i++)
    {
        printf("\033[34m%s/", ab_dir[i]);
    }
    printf("\033[34m%s\033[37m$ ", ab_dir[dir_pointer - 1]);
}