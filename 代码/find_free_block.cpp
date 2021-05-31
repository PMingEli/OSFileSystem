#include"header.h"
//寻找空闲块
void find_free_block(unsigned int &inode_number)
{
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fread(&superBlock, sizeof(filsys), 1, fd);
    if (superBlock.special_free == 0)
    {
        if (superBlock.special_stack[0] == 0)
        {
            printf("No value block!\n");
            return;
        }
        unsigned int stack[51];
        
        for (int i = 0; i < 50; i++)
        {
            stack[i] = superBlock.special_stack[i];
        }
        stack[50] = superBlock.special_free;
        fseek(fd, DATA_START + (superBlock.special_stack[0] - 50) * BLOCK_SIZE, SEEK_SET);
        fwrite(stack, sizeof(stack), 1, fd);
        
        fseek(fd, DATA_START + superBlock.special_stack[0] * BLOCK_SIZE, SEEK_SET);
        fread(stack, sizeof(stack), 1, fd);
        for (int i = 0; i < 50; i++)
        {
            superBlock.special_stack[i] = stack[i];
        }
        superBlock.special_free = stack[50];
    }
    inode_number = superBlock.special_stack[superBlock.special_free];
    superBlock.special_free--;
    superBlock.s_num_fblock--;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);
}