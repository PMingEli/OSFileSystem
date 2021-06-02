#include "header.h"
//删除文件
bool DeleteFile(string route)
{
    char *filename;
    vector<string> direct;
    if (route.find('/') != route.npos)
    {
        strcpy(filename, route.c_str());
    }
    else
    {
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
    }

    //文件名合法性检测
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("Error: Illegal file name.\n");
        return false;
    }

    //1.检测文件名是否存在当前目录
    int pos_in_directory = -1, tmp_file_ino;
    inode tmp_file_inode;
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
            printf("Delete error: File not found.\n");
            return false;
        }

        //2.看inode是否为文件还是目录
        tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(&tmp_file_inode, sizeof(inode), 1, fd);
        //检测目录
    } while (tmp_file_inode.di_mode == 0);

    //权限检测

    if (userID == tmp_file_inode.di_uid)
    {
        if (!(tmp_file_inode.permission & OWN_E))
        {
            printf("不好意思，你没有权限删除.\n");
            return -1;
        }
    }
    else if (users.groupID[userID] == tmp_file_inode.di_grp)
    {
        if (!(tmp_file_inode.permission & GRP_E))
        {
            printf("不好意思，没有权限删除.\n");
            return -1;
        }
    }
    else
    {
        if (!(tmp_file_inode.permission & ELSE_E))
        {
            printf("不好意思，没有权限删除.\n");
            return -1;
        }
    }

    //3.开始删除，inode赋值为0
    if (tmp_file_inode.icount > 0)
    {
        tmp_file_inode.icount--;
        fseek(fd, INODE_START + tmp_file_inode.i_ino * INODE_SIZE, SEEK_SET);
        fwrite(&tmp_file_inode, sizeof(inode), 1, fd);
        //更新目录
        int pos_directory_inode = currentDirectory.inodeID[0]; //"."
        inode tmp_directory_inode;
        fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
        fread(&tmp_directory_inode, sizeof(inode), 1, fd);
        memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
        currentDirectory.inodeID[pos_in_directory] = -1;
        fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
        fwrite(&currentDirectory, sizeof(directory), 1, fd);
        //更新相关信息
        directory tmp_directory = currentDirectory;
        int tmp_pos_directory_inode = pos_directory_inode;
        while (true)
        {
            tmp_directory_inode.di_number--;
            fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
            fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
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
        return true;
    }
    int tmp_fill[sizeof(inode)];
    memset(tmp_fill, 0, sizeof(inode));
    fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
    fwrite(&tmp_fill, sizeof(inode), 1, fd);

    //4.更新映射
    inode_bitmap[tmp_file_ino] = 0;
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    for (int i = 0; i < NADDR - 2; i++)
    {
        if (tmp_file_inode.di_addr[i] != -1)
            recycle_block(tmp_file_inode.di_addr[i]);
        else
            break;
    }
    if (tmp_file_inode.di_addr[NADDR - 2] != -1)
    {
        unsigned int f1[128];
        fseek(fd, DATA_START + tmp_file_inode.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
        fread(f1, sizeof(f1), 1, fd);
        for (int k = 0; k < 128; k++)
        {
            recycle_block(f1[k]);
        }
        recycle_block(tmp_file_inode.di_addr[NADDR - 2]);
    }

    // 5. 更新目录
    int pos_directory_inode = currentDirectory.inodeID[0]; //"."
    inode tmp_directory_inode;
    fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
    fread(&tmp_directory_inode, sizeof(inode), 1, fd);

    //更新目录项
    memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
    currentDirectory.inodeID[pos_in_directory] = -1;
    fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&currentDirectory, sizeof(directory), 1, fd);
    directory tmp_directory = currentDirectory;
    int tmp_pos_directory_inode = pos_directory_inode;
    while (true)
    {
        tmp_directory_inode.di_number--;
        fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
        fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
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

    //6.更新超级块
    superBlock.s_num_finode++;
    superBlock.special_free++;
    superBlock.s_num_fblock++;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);

    return true;
}