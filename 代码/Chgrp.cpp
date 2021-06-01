#include "header.h"
//改变文件所属组.
void Chgrp(char *filename)
{
    printf("0=文件，1=目录，请选择:");
    int tt;
    scanf("%d", &tt);
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("不合法.\n");
        return;
    }
    int pos_in_directory = -1;
    inode *tmp_file_inode = new inode;
    do
    {
        pos_in_directory++;
        for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
        {
            if (strcmp(currentDirectory.fileName[pos_in_directory], filename) == 0)
            {
                break;
            }
        }
        if (pos_in_directory == DIRECTORY_NUM)
        {
            printf("Not found.\n");
            return;
        }
        int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(tmp_file_inode, sizeof(inode), 1, fd);
    } while (tmp_file_inode->di_mode == tt);

    if (userID == tmp_file_inode->di_uid)
    {
        if (!(tmp_file_inode->permission & OWN_E))
        {
            printf("权限不够.\n");
            return;
        }
    }
    else if (users.groupID[userID] == tmp_file_inode->di_grp)
    {
        if (!(tmp_file_inode->permission & GRP_E))
        {
            printf("权限不够.\n");
            return;
        }
    }
    else
    {
        if (!(tmp_file_inode->permission & ELSE_E))
        {
            printf("权限不够.\n");
            return;
        }
    }

    printf("请输入组号:");
    int ttt, i;
    scanf("%d", &ttt);
    for (i = 0; i < ACCOUNT_NUM; i++)
    {
        if (users.groupID[i] == ttt)
            break;
    }
    if (i == ACCOUNT_NUM)
    {
        printf("用户错误!\n");
        return;
    }
    tmp_file_inode->di_grp = ttt;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return;
}
