#include "header.h"
//显示当前目录下的文件信息
void List()
{
    printf("\n     name\tuser\tgroup\tinodeID\tIcount\tsize\tpermission\ttime\n");
    for (int i = 2; i < DIRECTORY_NUM; i++)
    {
        if (strlen(currentDirectory.fileName[i]) > 0)
        {
            inode tmp_inode;
            fseek(fd, INODE_START + currentDirectory.inodeID[i] * INODE_SIZE, SEEK_SET);
            fread(&tmp_inode, sizeof(inode), 1, fd);

            const char *tmp_type = tmp_inode.di_mode == 0 ? "d" : "-";
            const char *tmp_user = users.userName[tmp_inode.di_uid];
            const int tmp_grpID = tmp_inode.di_grp;

            printf("%10s\t%s\t%d\t%d\t%d\t%u\t%s", currentDirectory.fileName[i], tmp_user, tmp_grpID, tmp_inode.i_ino, tmp_inode.icount, tmp_inode.di_size, tmp_type);
            for (int x = 8; x > 0; x--)
            {
                if (tmp_inode.permission & (1 << x))
                {
                    if ((x + 1) % 3 == 0)
                        printf("r");
                    else if ((x + 1) % 3 == 2)
                        printf("w");
                    else
                        printf("x");
                }
                else
                    printf("-");
            }
            if (tmp_inode.permission & 1)
                printf("x\t");
            else
                printf("-\t");
            printf("%s\n", tmp_inode.time);
        }
    }

    printf("\n\n");
}
