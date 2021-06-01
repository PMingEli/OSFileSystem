#include "header.h"
//链接
bool ln(char *filename)
{
    printf("0=文件，1=目录，请选择:");
    int tt;
    scanf("%d", &tt);
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("文件名错误.\n");
        return false;
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
            printf("没有找到.\n");
            return false;
        }
        int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(tmp_file_inode, sizeof(inode), 1, fd);
    } while (tmp_file_inode->di_mode == tt);
    //权限检测
    if (userID == tmp_file_inode->di_uid)
    {
        if (!(tmp_file_inode->permission & OWN_E))
        {
            printf("没有权限.\n");
            return false;
        }
    }
    else if (users.groupID[userID] == tmp_file_inode->di_grp)
    {
        if (!(tmp_file_inode->permission & GRP_E))
        {
            printf("没有权限.\n");
            return false;
        }
    }
    else
    {
        if (!(tmp_file_inode->permission & ELSE_E))
        {
            printf("没有权限.\n");
            return false;
        }
    }
    //取得绝对地址
    char absolute[1024];
    int path_pos = 0;
    printf("请输入绝对地址以'#'结束:");
    scanf("%s", absolute);
    char dirname[14];
    int pos_dir = 0;
    bool root = false;
    inode dir_inode;
    directory cur_dir;
    int i;
    for (i = 0; i < 5; i++)
    {
        dirname[i] = absolute[i];
    }
    dirname[i] = 0;
    if (strcmp("root/", dirname) != 0)
    {
        printf("路径错误!\n");
        return false;
    }
    fseek(fd, INODE_START, SEEK_SET);
    fread(&dir_inode, sizeof(inode), 1, fd);
    for (int i = 5; absolute[i] != '#'; i++)
    {
        if (absolute[i] == '/')
        {
            dirname[pos_dir++] = 0;
            pos_dir = 0;
            fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
            fread(&cur_dir, sizeof(directory), 1, fd);
            int i;
            for (i = 0; i < DIRECTORY_NUM; i++)
            {
                if (strcmp(cur_dir.fileName[i], dirname) == 0)
                {
                    fseek(fd, INODE_START + cur_dir.inodeID[i] * INODE_SIZE, SEEK_SET);
                    fread(&dir_inode, sizeof(inode), 1, fd);
                    if (dir_inode.di_mode == 0)
                        break;
                }
            }
            if (i == DIRECTORY_NUM)
            {
                printf("路径错误!\n");
                return false;
            }
        }
        else
            dirname[pos_dir++] = absolute[i];
    }
    //更新当前目录
    fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fread(&cur_dir, sizeof(directory), 1, fd);
    for (i = 0; i < DIRECTORY_NUM; i++)
    {
        if (strlen(cur_dir.fileName[i]) == 0)
        {
            strcat(cur_dir.fileName[i], filename);
            cur_dir.inodeID[i] = tmp_file_inode->i_ino;
            break;
        }
    }
    if (i == DIRECTORY_NUM)
    {
        printf("没有目录项!\n");
        return false;
    }
    fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&cur_dir, sizeof(directory), 1, fd);
    dir_inode.di_number++;
    tmp_file_inode->icount++;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return true;
}
