#include "header.h"
//文件复制
bool Copy(string rroute, string troute, inode *&currentInode)
{
    char currentroute[1024];
    memset(currentroute, 0, 1024);
    for (int i = 0; i < dir_pointer - 1; i++)
    {
        strcat(currentroute, ab_dir[i]);
        strcat(currentroute, "/");
    }
    char filename[14];
    vector<string> direct;
    direct = split(rroute, "/");
    strcpy(filename, direct[direct.size() - 1].c_str());
    currentInode = OpenMutipleFile(filename);
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
    if (tmp_file_inode->di_mode == 1)
    {
        //权限检测
        if (userID == tmp_file_inode->di_uid)
        {
            if (!(tmp_file_inode->permission & OWN_E))
            {
                printf("权限不够.\n");
                return false;
            }
        }
        else if (users.groupID[userID] == tmp_file_inode->di_grp)
        {
            if (!(tmp_file_inode->permission & GRP_E))
            {
                printf("权限不够.\n");
                return false;
            }
        }
        else
        {
            if (!(tmp_file_inode->permission & ELSE_E))
            {
                printf("权限不够.\n");
                return false;
            }
        }
    }
    else
    {
        printf("%s:是一个目录.\n", filename);
        return false;
    }
    OpenMutipleDir(currentroute);
    OpenMutipleDir(troute);
    CreateFile(filename);
    vector<string> tdirect;
    tdirect = split(rroute, "/");
    currentInode = OpenFile(tdirect[tdirect.size() - 1].c_str());
    Write(*currentInode, str);
    OpenMutipleDir(currentroute);
    //取绝对地址
    // char absolute[1024];
    // int path_pos = 0;
    // printf("请输入地址以'#'结尾:");
    // scanf("%s", absolute);
    // char dirname[14];
    // int pos_dir = 0;
    // bool root = false;
    // inode dir_inode;
    // directory cur_dir;
    // int i;
    // for (i = 0; i < 5; i++)
    // {
    //     dirname[i] = absolute[i];
    // }
    // dirname[i] = 0;
    // if (strcmp("root/", dirname) != 0)
    // {
    //     printf("路径错误!\n");
    //     return false;
    // }
    // fseek(fd, INODE_START, SEEK_SET);
    // fread(&dir_inode, sizeof(inode), 1, fd);
    // for (int i = 5; absolute[i] != '#'; i++)
    // {
    //     if (absolute[i] == '/')
    //     {
    //         dirname[pos_dir++] = 0;
    //         pos_dir = 0;
    //         fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    //         fread(&cur_dir, sizeof(directory), 1, fd);
    //         int i;
    //         for (i = 0; i < DIRECTORY_NUM; i++)
    //         {
    //             if (strcmp(cur_dir.fileName[i], dirname) == 0)
    //             {
    //                 fseek(fd, INODE_START + cur_dir.inodeID[i] * INODE_SIZE, SEEK_SET);
    //                 fread(&dir_inode, sizeof(inode), 1, fd);
    //                 if (dir_inode.di_mode == 0)
    //                     break;
    //             }
    //         }
    //         if (i == DIRECTORY_NUM)
    //         {
    //             printf("路径错误!\n");
    //             return false;
    //         }
    //     }
    //     else
    //         dirname[pos_dir++] = absolute[i];
    // }
    // //更新当前目录
    // fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    // fread(&cur_dir, sizeof(directory), 1, fd);
    // for (i = 0; i < DIRECTORY_NUM; i++)
    // {
    //     if (strlen(cur_dir.fileName[i]) == 0)
    //     {
    //         strcat(cur_dir.fileName[i], filename);
    //         int new_ino = 0;
    //         for (; new_ino < INODE_NUM; new_ino++)
    //         {
    //             if (inode_bitmap[new_ino] == 0)
    //             {
    //                 break;
    //             }
    //         }
    //         inode ifile_tmp;
    //         ifile_tmp.i_ino = new_ino;
    //         ifile_tmp.icount = 0;
    //         ifile_tmp.di_uid = tmp_file_inode->di_uid;
    //         ifile_tmp.di_grp = tmp_file_inode->di_grp;
    //         ifile_tmp.di_mode = tmp_file_inode->di_mode;
    //         memset(ifile_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
    //         ifile_tmp.di_size = 0;
    //         ifile_tmp.permission = tmp_file_inode->permission;
    //         time_t t = time(0);
    //         strftime(ifile_tmp.time, sizeof(ifile_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
    //         cur_dir.inodeID[i] = new_ino;
    //         Write(ifile_tmp, str);
    //         //Update bitmaps
    //         inode_bitmap[new_ino] = 1;
    //         fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    //         fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    //         superBlock.s_num_finode--;
    //         fseek(fd, BLOCK_SIZE, SEEK_SET);
    //         fwrite(&superBlock, sizeof(filsys), 1, fd);
    //         break;
    //     }
    // }
    // if (i == DIRECTORY_NUM)
    // {
    //     printf("No value iterms!\n");
    //     return false;
    // }
    // fseek(fd, DATA_START + dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    // fwrite(&cur_dir, sizeof(directory), 1, fd);
    // dir_inode.di_number++;
    // fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    // fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return true;
}
