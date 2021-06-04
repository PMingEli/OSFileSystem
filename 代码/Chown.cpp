#include "header.h"
//改变文件所属
void Chown(string v,string str)
{
   char currentdir[1000];
    memset(currentdir, 0, 1000);
    for (int i = 0; i < dir_pointer; i++)
    {
        strcat(currentdir, ab_dir[i]);
        strcat(currentdir, "/");
    }
    string www = currentdir;
    char filename[14];
    memset(filename, 0, 14);
    vector<string> dir;
    dir = split(v, "/");
    if (strcmp(dir[0].c_str(), ab_dir[0]) == 0)
    {
        while (dir_pointer > 1)
        {
            OpenDir("..");
        }
        for (int i = 1; i < dir.size() - 1; i++)
        {
            OpenDir(dir[i].c_str());
        }
        strcpy(filename, dir[dir.size() - 1].c_str());
    }
    else
    {
        for (int i = 0; i < dir.size() - 1; i++)
        {
            OpenDir(dir[i].c_str());
        }
        strcpy(filename, dir[dir.size() - 1].c_str());
    }
    //参数检测
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("参数不合法.\n");
        return;
    }
    if(str.length()>USER_NAME_LENGTH){
        printf("用户名不合法.\n");
        return;
    }
    // 1. 检查文件是否存在.
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
            printf("Not found.\n");
            return;
        }

        int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(tmp_file_inode, sizeof(inode), 1, fd);

    //检测权限
    if(!((checkwre(tmp_file_inode,'w')&&checkwre(tmp_file_inode,'e')))){
        cout<<"没有权限！！！"<<endl;
        return;
    }

    
    int i;
    
    for (i = 0; i < ACCOUNT_NUM; i++)
    {
        if (strcmp(users.userName[i], str.c_str()) == 0)
            break;
    }
    if (i == ACCOUNT_NUM)
    {
        printf("不合法用户!\n");
        return;
    }
    tmp_file_inode->di_uid = i;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return;
}
