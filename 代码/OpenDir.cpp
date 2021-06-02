#include "header.h"
//打开一个目录
bool OpenDir(const char *dirname)
{
    //参数检测
    if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
    {
        printf("不合法名称.\n");
        return false;
    }
    //1. 检查是否存在该目录.
    int pos_in_directory = 0;
    inode tmp_dir_inode;
    int tmp_dir_ino;
    for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
    {
        if (strcmp(currentDirectory.fileName[pos_in_directory], dirname) == 0)
        {
            break;
        }
    }
    if (pos_in_directory == DIRECTORY_NUM)
    {
        printf("未找到目录\n");
        return false;
    }

    //2. 查找inode，查看是否为目录.
    tmp_dir_ino = currentDirectory.inodeID[pos_in_directory];
    fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
    fread(&tmp_dir_inode, sizeof(inode), 1, fd);
    if (tmp_dir_inode.di_mode == 0)
    {
        if (userID == tmp_dir_inode.di_uid)
        {
            if (tmp_dir_inode.permission & OWN_E != OWN_E)
            {
                printf("权限不够.\n");
                return NULL;
            }
        }
        else if (users.groupID[userID] == tmp_dir_inode.di_grp)
        {
            if (tmp_dir_inode.permission & GRP_E != GRP_E)
            {
                printf("权限不够.\n");
                return false;
            }
        }
        else
        {
            if (tmp_dir_inode.permission & ELSE_E != ELSE_E)
            {
                printf("权限不够.\n");
                return false;
            }
        }
        //3. 更新当前目录.
        directory new_current_dir;
        fseek(fd, DATA_START + tmp_dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
        fread(&new_current_dir, sizeof(directory), 1, fd);
        currentDirectory = new_current_dir;
        if (dirname[0] == '.' && dirname[1] == 0)
        {
            dir_pointer;
        }
        else if (dirname[0] == '.' && dirname[1] == '.' && dirname[2] == 0)
        {
            if (dir_pointer > 1)
            {
                dir_pointer--;
            }
        }
        else
        {
            for (int i = 0; i < 14; i++)
            {
                ab_dir[dir_pointer][i] = dirname[i];
            }
            dir_pointer++;
        }
        return true;
    }
    else
    {
        printf("cd: %s: 不是目录\n", dirname);
    }
}
