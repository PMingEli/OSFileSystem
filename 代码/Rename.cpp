#include "header.h"
//对文件或目录重命名
void Rename(char *filename, char *newfilename)
{
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("参数不合法.\n");
        return;
    }
    int pos_in_directory = -1;
    inode *tmp_file_inode = new inode;
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
        return;
    }
    int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
    fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
    fread(tmp_file_inode, sizeof(inode), 1, fd);

    //检查权限
    if (!((checkwre(tmp_file_inode, 'e') || checkwre(tmp_file_inode, 'w'))))
    {
        printf("权限不够.\n");
        return;
    }
    pos_in_directory = -1;
    //int i=0;
    pos_in_directory++;
    for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
    {
        if (strcmp(currentDirectory.fileName[pos_in_directory], newfilename) == 0)
        {
            cout << "mv：文件" << newfilename << "已存在" << endl;
            return;
        }
    }
    for (int i = 0; i < DIRECTORY_NUM; i++)
    {
        if (currentDirectory.inodeID[i] == tmp_file_inode->i_ino)
        {

            strcpy(currentDirectory.fileName[i], newfilename);
            cout << currentDirectory.fileName[i]
                 << endl;
            break;
        }
    }
    fseek(fd, DATA_START + tmp_file_inode->di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&currentDirectory, sizeof(directory), 1, fd);
    cout << "success" << endl;
    return;
}
