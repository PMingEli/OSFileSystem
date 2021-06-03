#include "header.h"
//修改文件权限
void Chmod(string v, string str)
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

    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("不合法.\n");
        return;
    }

    //1. 检查是否存在.
    int pos_in_directory = -1;
    inode *tmp_file_inode = new inode;
    //int i=0;
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
    // cout<<"pos in dir:"<<pos_in_directory<<endl;

    //2. 检查是否存在目录.
    int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
    fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
    fread(tmp_file_inode, sizeof(inode), 1, fd);
    // cout<<"tem_inode-di-mode"<<tt<<endl;
    // cout<<"loop:"<<i++<<endl;

    // printf("请输入 0&1 串给予权限\n");
    // printf("格式: rwerwerwe\n");
    // char str[10];
    // scanf("%s", &str);
    //判断权限
    if (!checkwre(tmp_file_inode, 'e'))
    {
        cout << "没有权限" << endl;
        return;
    }
    int temp = 0;
    for (int i = 0; i < 8; i++)
    {
        if (str[i] == '1')
            temp += 1 << (8 - i);
    }
    if (str[8] == '1')
        temp += 1;
    tmp_file_inode->permission = temp;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    OpenMutipleDir(www);
    return;
}
