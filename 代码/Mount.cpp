#include"header.h"
//初始化文件系统的各项功能
bool Mount()
{
    //打开文件卷
    fd = fopen("data", "rb+");
    if (fd == NULL)
    {
        printf("文件系统没有找到!\n");
        return false;
    }
    
    //读取超级块信息
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fread(&superBlock, sizeof(superBlock), 1, fd);
    
    //读取节点映射表
    fseek(fd, 2 * BLOCK_SIZE, SEEK_SET);
    fread(inode_bitmap, sizeof(unsigned short) * INODE_NUM, 1, fd);
    
    //读取当前目录
    fseek(fd, DATA_START, SEEK_SET);
    fread(&currentDirectory, sizeof(directory), 1, fd);
    
    //读取账户资料
    fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
    fread(&users, sizeof(userPsw), 1, fd);
    
    return true;
};