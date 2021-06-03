#include "header.h"
bool Copy(string v, string route, inode *&currentInode)
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
    currentInode = OpenFile(filename);
    int block_num = currentInode->di_size / BLOCK_SIZE + 1;
    //读取文件
    char stack[BLOCK_SIZE];
    char str[100000];
    int cnt = 0;
    if (block_num <= NADDR - 2)
    {
        for (int i = 0; i < block_num; i++)
        {
            if (currentInode->di_addr[i] == -1)
                break;
            fseek(fd, DATA_START + currentInode->di_addr[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                {
                    str[cnt] = 0;
                    break;
                }
                str[cnt++] = stack[j];
            }
        }
    }
    else if (block_num > NADDR - 2)
    {
        for (int i = 0; i < NADDR - 2; i++)
        {
            fseek(fd, DATA_START + currentInode->di_addr[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                {
                    str[cnt] = 0;
                    break;
                }
                str[cnt++] = stack[j];
            }
        }
        unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = {0};
        fseek(fd, DATA_START + currentInode->di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
        fread(f1, sizeof(f1), 1, fd);
        for (int i = 0; i < block_num - (NADDR - 2); i++)
        {
            fseek(fd, DATA_START + f1[i] * BLOCK_SIZE, SEEK_SET);
            fread(stack, sizeof(stack), 1, fd);
            for (int j = 0; j < BLOCK_SIZE; j++)
            {
                if (stack[j] == '\0')
                {
                    str[cnt] = 0;
                    break;
                }
                str[cnt++] = stack[j];
            }
        }
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
    } while (tmp_file_inode->di_mode == 0);

    if (!checkwre(tmp_file_inode, 'e') && checkwre(tmp_file_inode, 'w'))
    {
        printf("权限不够.\n");
        return false;
    }
    //取绝对地址
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
    //cout << absolute << endl;
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
        cout << absolute << endl;
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
                cout << absolute << endl;
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
            unsigned int new_ino = 0;
            find_free_block(new_ino);
            inode ifile_tmp;
            ifile_tmp.i_ino = new_ino;
            ifile_tmp.icount = 0;
            ifile_tmp.di_uid = tmp_file_inode->di_uid;
            ifile_tmp.di_grp = tmp_file_inode->di_grp;
            ifile_tmp.di_mode = tmp_file_inode->di_mode;
            memset(ifile_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
            ifile_tmp.di_size = 0;
            ifile_tmp.permission = tmp_file_inode->permission;
            time_t t = time(0);
            strftime(ifile_tmp.time, sizeof(ifile_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
            cur_dir.inodeID[i] = new_ino;
            Write(ifile_tmp, str);
            //Update bitmaps
            inode_bitmap[new_ino] = 1;
            fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
            fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
            superBlock.s_num_finode--;
            fseek(fd, BLOCK_SIZE, SEEK_SET);
            fwrite(&superBlock, sizeof(filsys), 1, fd);
            break;
        }
    }
    if (i == DIRECTORY_NUM)
    {
        printf("No value iterms!\n");
        return false;
    }
    fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&cur_dir, sizeof(directory), 1, fd);
    dir_inode.di_number++;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return true;
}