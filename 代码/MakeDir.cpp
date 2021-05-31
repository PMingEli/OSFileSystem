#include"header.h"
//创建新目录，新目录包含. ..
bool MakeDir(const char* dirname)
{
    //参数检测
    if (dirname == NULL || strlen(dirname) > FILE_NAME_LENGTH)
    {
        printf("不合法的目录名.\n");
        return false;
    }
    
    //  1. 检查inode是否用光了。
    if (superBlock.s_num_fblock <= 0 || superBlock.s_num_finode <= 0)
    {
        printf("没有足够空间创建目录了.\n");
        return false;
    }
    int new_ino = 0;
    unsigned int new_block_addr = 0;
    for (; new_ino < INODE_NUM; new_ino++)
    {
        if (inode_bitmap[new_ino] == 0)
        {
            break;
        }
    }
    find_free_block(new_block_addr);
    if (new_block_addr == -1) return false;
    if (new_ino == INODE_NUM || new_block_addr == BLOCK_NUM)
    {
        printf("File creation error: No valid spaces.\n");
        return false;
    }
    
    //2. 检查目录名在当前目录是否有重名.
    for (int i = 0; i < DIRECTORY_NUM; i++)
    {
        if (strcmp(currentDirectory.fileName[i], dirname) == 0)
        {
            inode* tmp_file_inode = new inode;
            int tmp_file_ino = currentDirectory.inodeID[i];
            fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
            fread(tmp_file_inode, sizeof(inode), 1, fd);
            if (tmp_file_inode->di_mode == 1) continue;
            else {
                printf("File creation error: Directory name '%s' has been used.\n", currentDirectory.fileName[i]);
                return false;
            }
        }
    }
    
    //3. 检查当前目录项是否太多了.
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
    
    //4. 创建新inode.
    inode idir_tmp;
    idir_tmp.i_ino = new_ino;
    idir_tmp.di_number = 1;
    idir_tmp.di_mode = 0;                    //0 代表目录
    idir_tmp.di_size = sizeof(directory);
    memset(idir_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
    idir_tmp.di_addr[0] = new_block_addr;
    idir_tmp.di_uid = userID;
    idir_tmp.di_grp = users.groupID[userID];
    time_t t = time(0);
    strftime(idir_tmp.time, sizeof(idir_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
    idir_tmp.time[64] = 0;
    idir_tmp.icount = 0;
    idir_tmp.permission = MAX_PERMISSION;
    fseek(fd, INODE_START + new_ino * INODE_SIZE, SEEK_SET);
    fwrite(&idir_tmp, sizeof(inode), 1, fd);
    
    //5. 创建目录文件.
    directory tmp_dir;
    memset(tmp_dir.fileName, 0, sizeof(char) * DIRECTORY_NUM * FILE_NAME_LENGTH);
    memset(tmp_dir.inodeID, -1, sizeof(unsigned int) * DIRECTORY_NUM);
    strcpy(tmp_dir.fileName[0], ".");
    tmp_dir.inodeID[0] = new_ino;
    strcpy(tmp_dir.fileName[1], "..");
    tmp_dir.inodeID[1] = currentDirectory.inodeID[0];
    fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
    fwrite(&tmp_dir, sizeof(directory), 1, fd);
    
    //6.  更新映射表.
    inode_bitmap[new_ino] = 1;
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    
    //7. 更新目录.
    int pos_directory_inode = 0;
    pos_directory_inode = currentDirectory.inodeID[0]; //"."
    inode tmp_directory_inode;
    fseek(fd, INODE_START + pos_directory_inode * INODE_SIZE, SEEK_SET);
    fread(&tmp_directory_inode, sizeof(inode), 1, fd);
    for (int i = 2; i < DIRECTORY_NUM; i++)
    {
        if (strlen(currentDirectory.fileName[i]) == 0)
        {
            strcat(currentDirectory.fileName[i], dirname);
            currentDirectory.inodeID[i] = new_ino;
            break;
        }
    }
    fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
    fwrite(&currentDirectory, sizeof(directory), 1, fd);
    directory tmp_directory = currentDirectory;
    int tmp_pos_directory_inode = pos_directory_inode;
    while (true)
    {
        tmp_directory_inode.di_number++;
        fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
        fwrite(&tmp_directory_inode, sizeof(inode), 1, fd);
        if (tmp_directory.inodeID[1] == tmp_directory.inodeID[0])
        {
            break;
        }
        tmp_pos_directory_inode = tmp_directory.inodeID[1];        //".."
        fseek(fd, INODE_START + tmp_pos_directory_inode * INODE_SIZE, SEEK_SET);
        fread(&tmp_directory_inode, sizeof(inode), 1, fd);
        fseek(fd, DATA_START + tmp_directory_inode.di_addr[0] * BLOCK_SIZE, SEEK_SET);
        fread(&tmp_directory, sizeof(directory), 1, fd);
    }
    
    // 8. 更新超级块.
    superBlock.s_num_finode--;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);
    
    return true;
};
