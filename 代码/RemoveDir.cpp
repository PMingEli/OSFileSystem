#include "header.h"
//删除一个目录和目录下的所有文件
bool RemoveDir(const char *dirname)
{
    if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
    {
        printf("目录不合法.\n");
        return false;
    }

    //1. 检查目录是否存在
    int pos_in_directory = 0;
    int tmp_dir_ino;
    inode tmp_dir_inode;
    do
    {
        pos_in_directory++;
        for (; pos_in_directory < DIRECTORY_NUM; pos_in_directory++)
        {
            if (strcmp(currentDirectory.fileName[pos_in_directory], dirname) == 0)
            {
                break;
            }
        }
        if (pos_in_directory == DIRECTORY_NUM)
        {
            printf("没有找到该目录.\n");
            return false;
        }

        // 2. 查看inode是否是文件.
        tmp_dir_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
        fread(&tmp_dir_inode, sizeof(inode), 1, fd);
        //Directory check
    } while (tmp_dir_inode.di_mode == 1);

    //3. 权限检
     if (!((checkwre(&tmp_dir_inode,'e')||checkwre(&tmp_dir_inode,'w'))))
        {
            printf("权限不够.\n");
            return false;
        }
    //4. 开始删除
    if (tmp_dir_inode.icount > 0)
    {
        tmp_dir_inode.icount--;
        fseek(fd, INODE_START + tmp_dir_inode.i_ino * INODE_SIZE, SEEK_SET);
        fwrite(&tmp_dir_inode, sizeof(inode), 1, fd);
        //更新目录
        int pos_directory_inode = currentDirectory.inodeID[0]; //"."
        inode tmp_directory_inode;
        fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
        fread(&tmp_directory_inode, sizeof(inode), 1, fd);
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
        return true;
    }
    directory tmp_dir;
    fseek(fd, DATA_START + tmp_dir_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fread(&tmp_dir, sizeof(directory), 1, fd);

    //查找所有的子文件目录，并删除.
    inode tmp_sub_inode;
    char tmp_sub_filename[FILE_NAME_LENGTH];
    memset(tmp_sub_filename, 0, FILE_NAME_LENGTH);
    for (int i = 2; i < DIRECTORY_NUM; i++)
    {
        if (strlen(tmp_dir.fileName[i]) > 0)
        {
            strcpy(tmp_sub_filename, tmp_dir.fileName[i]);
            fseek(fd, INODE_START + tmp_dir.inodeID[i] * INODE_SIZE, SEEK_SET);
            fread(&tmp_sub_inode, sizeof(inode), 1, fd);
            directory tmp_swp;
            tmp_swp = currentDirectory;
            currentDirectory = tmp_dir;
            tmp_dir = tmp_swp;
            if (tmp_sub_inode.di_mode == 1)
            {
                DeleteFile(tmp_sub_filename);
            }
            else if (tmp_sub_inode.di_mode == 0)
            {
                RemoveDir(tmp_sub_filename);
            }
            tmp_swp = currentDirectory;
            currentDirectory = tmp_dir;
            tmp_dir = tmp_swp;
        }
    }

    //5.将inode赋为0.
    int tmp_fill[sizeof(inode)];
    memset(tmp_fill, 0, sizeof(inode));
    fseek(fd, INODE_START + tmp_dir_ino * INODE_SIZE, SEEK_SET);
    fwrite(&tmp_fill, sizeof(inode), 1, fd);
    //6. 更新映射
    inode_bitmap[tmp_dir_ino] = 0;
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(&inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    for (int i = 0; i < (tmp_dir_inode.di_size / BLOCK_SIZE + 1); i++)
    {
        recycle_block(tmp_dir_inode.di_addr[i]);
    }

    //7. 更新目录
    int pos_directory_inode = currentDirectory.inodeID[0]; //"."
    inode tmp_directory_inode;
    fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
    fread(&tmp_directory_inode, sizeof(inode), 1, fd);
    memset(currentDirectory.fileName[pos_in_directory], 0, FILE_NAME_LENGTH);
    currentDirectory.inodeID[pos_in_directory] = -1;
    fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * INODE_SIZE, SEEK_SET);
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

    //8 更新超级块
    superBlock.s_num_finode++;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);

    return true;
};
