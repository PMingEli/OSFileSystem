#include "header.h"

//重置block
void recycle_block(unsigned int &inode_number)
{
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fread(&superBlock, sizeof(filsys), 1, fd);
    if (superBlock.special_free == 49)
    {
        unsigned int block_num;
        unsigned int stack[51];
        if (superBlock.special_stack[0] == 0)
            block_num = 499;
        else
            block_num = superBlock.special_stack[0] - 50;
        for (int i = 0; i < 50; i++)
        {
            stack[i] = superBlock.special_stack[i];
        }
        stack[50] = superBlock.special_free;
        fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
        fwrite(stack, sizeof(stack), 1, fd);
        block_num -= 50;
        fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
        fread(stack, sizeof(stack), 1, fd);
        for (int i = 0; i < 50; i++)
        {
            superBlock.special_stack[i] = stack[i];
        }
        superBlock.special_free = stack[50];
    }
    superBlock.special_free++;
    superBlock.s_num_fblock++;
    superBlock.special_stack[superBlock.special_free] = inode_number;
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(filsys), 1, fd);
}