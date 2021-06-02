#include "header.h"
//输出文件信息
void PrintFile(inode &ifile)
{
    if (&ifile == NULL)
    {
        return;
    }
    //权限检测
    if (userID == ifile.di_uid)
    {
        if (!(ifile.permission & OWN_R))
        {
            printf("没有读取权限.\n");
            return;
        }
    }
    else if (users.groupID[userID] == ifile.di_grp)
    {
        if (!(ifile.permission & GRP_R))
        {
            printf("没有读取权限.\n");
            return;
        }
    }
    else
    {
        if (!(ifile.permission & ELSE_R))
        {
            printf("没有读取权限.\n");
            return;
        }
    }
    int block_num = ifile.di_size / BLOCK_SIZE + 1;
    int print_line_num = 0; //16 bytes 每一行.
    //从块中读取文件
    char stack[BLOCK_SIZE];
    if (block_num <= NADDR - 2)
    {
        for (int i = 0; i < block_num; i++)
        {
            fseek(fd, DATA_START + ifile.di_addr[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                    break;
                if (j % 16 == 0)
                {
                    printf("\n");
                    printf("%d\t", ++print_line_num);
                }
                printf("%c", stack[j]);
            }
        }
    }
    else if (block_num > NADDR - 2)
    {
        for (int i = 0; i < NADDR - 2; i++)
        {
            fseek(fd, DATA_START + ifile.di_addr[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                    break;
                if (j % 16 == 0)
                {
                    printf("\n");
                    printf("%d\t", ++print_line_num);
                }
                printf("%c", stack[j]);
            }
        }
        unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = {0};
        fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
        fread(f1, sizeof(f1), 1, fd);
        for (int i = 0; i < block_num - (NADDR - 2); i++)
        {
            fseek(fd, DATA_START + f1[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                    break;
                if (j % 16 == 0)
                {
                    printf("\n");
                    printf("%d\t", ++print_line_num);
                }
                printf("%c", stack[j]);
            }
        }
    }
    printf("\n");
};
