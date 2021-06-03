#include "header.h"
//根据路径和文件名创建文件
bool CreateFile(string route)
{
    char filename[14];
    memset(filename, 0, 14);
    vector<string> direct;
    direct = split(route, "/");
    int n = direct.size();
    if (strcmp(direct[0].c_str(), "root") == 0)
    {
        while (dir_pointer > 1)
        {
            OpenDir("..");
        }
        for (int i = 1; i < n - 1; i++)
        {
            OpenDir(direct[i].c_str());
        }
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            OpenDir(direct[i].c_str());
        }
    }

    strcpy(filename, direct[n - 1].c_str());

    //文件名合法性检测
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("不合法的文件名.\n");
        return false;
    }

    //检测是否还有空闲的block
    if (superBlock.s_num_fblock <= 0 || superBlock.s_num_finode <= 0)
    {
        printf("没有空间创建新文件了.\n");
        return false;
    }
    //若有空闲block，则找新的inode
    int new_ino = 0;
    unsigned int new_block_addr = -1;
    for (; new_ino < INODE_NUM; new_ino++)
    {
        if (inode_bitmap[new_ino] == 0)
        {
            break;
        }
    }

    //检测当前目录是否有同名文件
    for (int i = 0; i < DIRECTORY_NUM; i++)
    {
        if (strcmp(currentDirectory.fileName[i], filename) == 0)
        {
            inode *tmp_file_inode = new inode;
            int tmp_file_ino = currentDirectory.inodeID[i];
            fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
            fread(tmp_file_inode, sizeof(inode), 1, fd);
            if (tmp_file_inode->di_mode == 0)
            {
                printf("文件夹名'%s' 有重复.\n", currentDirectory.fileName[i]);
                return false;
            }
            else
            {
                printf("文件名'%s' 有重复.\n", currentDirectory.fileName[i]);
                return false;
            }
        }
    }

    //检测当前目录的文件数量是否达到限制
    int itemCounter = 0;
    for (int i = 0; i < DIRECTORY_NUM; i++)
    {
        if (strlen(currentDirectory.fileName[i]) > 0)
        {
            itemCounter++;
        }
    }
    if (itemCounter >= DIRECTORY_NUM)
    {
        printf("File creation error: Too many files or directories in current path.\n");
        return false;
    }

    //创建新的inode
    inode ifile_tmp;
    ifile_tmp.i_ino = new_ino;
    find_free_block(ifile_tmp.i_ino);
    // fseek(fd,DATA_START + new_ino * BLOCK_SIZE, SEEK_SET);

    ifile_tmp.di_number = 1;
    ifile_tmp.di_mode = 1; //1 表示文件
    ifile_tmp.di_size = 0; //新文件大小为0
    memset(ifile_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
    ifile_tmp.di_addr[0] = ifile_tmp.i_ino;
    for (unsigned int i = 0; i < BLOCK_SIZE; i++)
    {
        fseek(fd, DATA_START + ifile_tmp.di_addr[0] * BLOCK_SIZE + i, SEEK_SET);
        fputc('\0', fd);
    }
    ifile_tmp.di_uid = userID;                //当前用户id.
    ifile_tmp.di_grp = users.groupID[userID]; //当前用户的组
    ifile_tmp.permission = MAX_PERMISSION;
    ifile_tmp.icount = 0;
    time_t t = time(0);
    strftime(ifile_tmp.time, sizeof(ifile_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
    ifile_tmp.time[64];
    fseek(fd, INODE_START + new_ino * INODE_SIZE, SEEK_SET);
    fwrite(&ifile_tmp, sizeof(inode), 1, fd);

    //更新映射表
    inode_bitmap[new_ino] = 1;
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);

    //更新目录
    //查找当前目录的inode
    int pos_directory_inode = 0;
    pos_directory_inode = currentDirectory.inodeID[0]; //"."
    inode tmp_directory_inode;
    fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
    fread(&tmp_directory_inode, sizeof(inode), 1, fd);

    //加入当前目录
    for (int i = 2; i < DIRECTORY_NUM; i++)
    {
        if (strlen(currentDirectory.fileName[i]) == 0)
        {
            strcat(currentDirectory.fileName[i], filename);
            currentDirectory.inodeID[i] = new_ino;
            break;
        }
    }
    //写入文件
    fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&currentDirectory, sizeof(directory), 1, fd);

    //更新相关信息
    directory tmp_directory = currentDirectory;
    int tmp_pos_directory_inode = pos_directory_inode;
    while (true)
    {
        tmp_directory_inode.di_number++;
        fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
        fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
        //如果到了根目录就停止更新
        if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
        {
            break;
        }
        tmp_pos_directory_inode = tmp_directory.inodeID[1]; //".."
        fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
        fread(&tmp_directory_inode, sizeof(inode), 1, fd);
        fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
        fread(&tmp_directory, sizeof(directory), 1, fd);
    }

    //更新超级块
    superBlock.s_num_finode--;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);

    return true;
};