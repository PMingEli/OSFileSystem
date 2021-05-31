#include "header.h"
//根据文件名打开文件.
inode *OpenFile(const char *filename)
{
    //文件名检测
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("不合法文件名.\n");
        return NULL;
    }

    //1. 查找是否存在该文件.
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
            printf("没有找到该文件.\n");
            return NULL;
        }

        // 2. 判断inode是否是目录
        int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(tmp_file_inode, sizeof(inode), 1, fd);
    } while (tmp_file_inode->di_mode == 0);

    return tmp_file_inode;
};