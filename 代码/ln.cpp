#include "header.h"
//链接
bool ln(string v, string route)
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
        printf("文件名错误.\n");
        return false;
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
        return false;
    }
    int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
    fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
    fread(tmp_file_inode, sizeof(inode), 1, fd);
    //权限检测
    if (!checkwre(tmp_file_inode, 'e'))
    {
        cout << "没有权限！！" << endl;
        return false;
    }
    //取得绝对地址
    OpenMutipleDir(www);
    char absolute[1024];
    memset(absolute, 0, 1024);
    int path_pos = 0;
    vector<string> direct;
    direct = split(route, "/");
    if (strcmp(direct[0].c_str(), ab_dir[0]) == 0)
    {
        for (int i = 0; i < direct.size(); i++)
        {
            strcat(absolute, direct[i].c_str());
            strcat(absolute, "/");
        }
        strcat(absolute, "#");
    }
    else if (strcmp(direct[0].c_str(), "..") == 0)
    {
        int i = 0;
        while (direct[i] == "..")
        {
            i++;
        }
        for (int j = 0; j < dir_pointer - i; j++)
        {
            strcat(absolute, ab_dir[j]);
            strcat(absolute, "/");
        }
        for (; i < direct.size(); i++)
        {
            strcat(absolute, direct[i].c_str());
            strcat(absolute, "/");
        }
        strcat(absolute, "#");
    }
    else if (strcmp(direct[0].c_str(), ".") == 0)
    {
        for (int j = 0; j < dir_pointer; j++)
        {
            strcat(absolute, ab_dir[j]);
            strcat(absolute, "/");
        }
        for (int i = 1; i < direct.size(); i++)
        {
            strcat(absolute, direct[i].c_str());
            strcat(absolute, "/");
        }
        strcat(absolute, "#");
    }
    else
    {
        for (int j = 0; j < dir_pointer; j++)
        {
            strcat(absolute, ab_dir[j]);
            strcat(absolute, "/");
        }
        for (int i = 0; i < direct.size(); i++)
        {
            strcat(absolute, direct[i].c_str());
            strcat(absolute, "/");
        }
        strcat(absolute, "#");
    }
    char dirname[14];
    int pos_dir = 0;
    bool root = false;
    inode dir_inode;
    directory cur_dir;
    int i;
    for (i = 0; i < 4; i++)
    {
        dirname[i] = absolute[i];
    }
    if (strcmp(ab_dir[0], dirname) != 0)
    {
        printf("路径错误!\n");
        return false;
    }
    dirname[i] = '/';
    i++;
    dirname[i] = 0;
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
    pos_in_directory = -1;
    //int i=0;
    pos_in_directory++;
    for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
    {
        if (strcmp(cur_dir.fileName[pos_in_directory], filename) == 0)
        {
            cout << "cp：" << filename << "已存在" << endl;
            return false;
        }
    }
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
