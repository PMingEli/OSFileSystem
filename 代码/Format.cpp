#include"header.h"
//初始化文件系统
bool Format()
{
    //在当前目录新建一个文件作为文件卷
    FILE* fd = fopen("data", "wb+");
    if (fd == NULL)
    {
        printf("初始化文件系统失败!\n");
        return false;
    }
    
    //初始化超级块
    filsys superBlock;
    superBlock.s_num_inode = INODE_NUM;
    superBlock.s_num_block = BLOCK_NUM + 3 + 64; //3代表，0空闲块、1超级块、2Inode位示图表,64块存inode
    superBlock.s_size_inode = INODE_SIZE;
    superBlock.s_size_block = BLOCK_SIZE;
    superBlock.s_num_fblock = BLOCK_NUM - 2;
    superBlock.s_num_finode = INODE_NUM - 2;
    superBlock.special_stack[0] = 48;
    for (int i = 1; i < 50; i++)
    {
        superBlock.special_stack[i] = 49 - i;
    }
    superBlock.special_free = 47;
    //写超级块进入文件
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fread(&superBlock, sizeof(filsys), 1, fd);
    
    //初始化位示图
    unsigned short inode_bitmap[INODE_NUM];
    
    memset(inode_bitmap, 0, INODE_NUM);
    inode_bitmap[0] = 1;
    inode_bitmap[1] = 1;
    //写位示进入文件
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fwrite(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    
    //成组连接
    unsigned int stack[51];
    for (int i = 0; i < BLOCK_NUM / 50; i++)
    {
        memset(stack, 0, sizeof(stack));
        for (unsigned int j = 0; j < 50; j++)
        {
            stack[j] = (49 + i * 50) - j;
        }
        stack[0] = 49 + (i + 1) * 50;
        stack[50] = 49;
        fseek(fd, DATA_START + (49 + i * 50)*BLOCK_SIZE, SEEK_SET);
        fwrite(stack, sizeof(unsigned int) * 51, 1, fd);
    }
    memset(stack, 0, sizeof(stack));
    for (int i = 0; i < 12; i++)
    {
        stack[i] = 511 - i;
    }
    stack[0] = 0;
    stack[50] = 11;
    fseek(fd, DATA_START + 511 * BLOCK_SIZE, SEEK_SET);
    fwrite(stack, sizeof(unsigned int) * 51, 1, fd);
    
    fseek(fd, DATA_START + 49 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 99 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 149 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 199 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 249 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 299 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 349 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 399 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 449 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 499 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    fseek(fd, DATA_START + 511 * BLOCK_SIZE, SEEK_SET);
    fread(stack, sizeof(unsigned int) * 51, 1, fd);
    
    
    //创建根目录
    inode iroot_tmp;
    iroot_tmp.i_ino = 0;   //根目录用掉0号块
    iroot_tmp.di_number = 2;//目录下目录以及文件数量
    iroot_tmp.di_mode = 0;                    //0 =目录
    iroot_tmp.di_size = 0;                    //目录大小为0
    memset(iroot_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
    iroot_tmp.di_addr[0] = 0;
    iroot_tmp.permission = MAX_OWNER_PERMISSION;
    iroot_tmp.di_grp = 1;
    iroot_tmp.di_uid = 0;                    //Root user id.
    iroot_tmp.icount = 0;
    time_t t = time(0);
    strftime(iroot_tmp.time, sizeof(iroot_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
    iroot_tmp.time[64] = 0;
    fseek(fd, INODE_START, SEEK_SET);
    fwrite(&iroot_tmp, sizeof(inode), 1, fd);
    
    //直接创建文件
    directory droot_tmp;
    memset(droot_tmp.fileName, 0, sizeof(char) * DIRECTORY_NUM * FILE_NAME_LENGTH);
    memset(droot_tmp.inodeID, -1, sizeof(unsigned int) * DIRECTORY_NUM);
    strcpy(droot_tmp.fileName[0], ".");
    droot_tmp.inodeID[0] = 0;
    strcpy(droot_tmp.fileName[1], "..");
    droot_tmp.inodeID[1] = 0;
    //A sub directory for accounting files
    strcpy(droot_tmp.fileName[2], "pw");
    droot_tmp.inodeID[2] = 1;
    
    //写入
    fseek(fd, DATA_START, SEEK_SET);
    fwrite(&droot_tmp, sizeof(directory), 1, fd);
    
    //创建用户文件
    //先创建 inode
    inode iaccouting_tmp;
    iaccouting_tmp.i_ino = 1;
    iaccouting_tmp.di_number = 1;
    iaccouting_tmp.di_mode = 1;                    //1 代表文件
    iaccouting_tmp.di_size = sizeof(userPsw);    //文件大小
    memset(iaccouting_tmp.di_addr, -1, sizeof(unsigned int) * NADDR);
    iaccouting_tmp.di_addr[0] = 1;                //根目录存在第一块
    iaccouting_tmp.di_uid = 0;                    //Root user id.
    iaccouting_tmp.di_grp = 1;
    iaccouting_tmp.permission = 320;
    iaccouting_tmp.icount = 0;
    t = time(0);
    strftime(iaccouting_tmp.time, sizeof(iaccouting_tmp.time), "%Y/%m/%d %X %A %jday %z", localtime(&t));
    iaccouting_tmp.time[64] = 0;
    fseek(fd, INODE_START + INODE_SIZE, SEEK_SET);
    fwrite(&iaccouting_tmp, sizeof(inode), 1, fd);
    
    //创建账户.
    userPsw paccouting_tmp;
    memset(paccouting_tmp.userName, 0, sizeof(char) * USER_NAME_LENGTH * ACCOUNT_NUM);
    memset(paccouting_tmp.password, 0, sizeof(char) * USER_PASSWORD_LENGTH * ACCOUNT_NUM);
    
    strcpy(paccouting_tmp.userName[0], "root");
    strcpy(paccouting_tmp.userName[1], "ming");
    strcpy(paccouting_tmp.password[0], "123456");
    strcpy(paccouting_tmp.password[1], "20184317");
    //0 代表管理员，其他数字就是userid
    for (unsigned short i = 0; i < ACCOUNT_NUM; i++)
    {
        paccouting_tmp.userID[i] = i;
    }
    paccouting_tmp.groupID[0] = 1;
    paccouting_tmp.groupID[1] = 2;
    //写入文件
    fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
    fwrite(&paccouting_tmp, sizeof(userPsw), 1, fd);
    
    //关闭文件.
    fclose(fd);
    
    return true;
};