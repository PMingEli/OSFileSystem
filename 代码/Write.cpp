#include "header.h"
//在文件尾添加数据
int Write(inode &ifile, const char *content)
{
    if (&ifile == NULL)
    {
        return -1;
    }
    if (content == NULL)
    {
        printf("不合法的文件名.\n");
        return -1;
    }
    //权限检测
    if (userID == ifile.di_uid)
    {
        if (!(ifile.permission & OWN_W))
        {
            printf("权限不够.\n");
            return -1;
        }
    }
    else if (users.groupID[userID] == ifile.di_grp)
    {
        if (!(ifile.permission & GRP_W))
        {
            printf("权限不够.\n");
            return -1;
        }
    }
    else
    {
        if (!(ifile.permission & ELSE_W))
        {
            printf("权限不够.\n");
            return -1;
        }
    }

    // 1.检测文件是否达到了最大大小
    int len_content = strlen(content);
    unsigned int new_file_length = len_content + ifile.di_size;
    if (new_file_length >= FILE_SIZE_MAX)
    {
        printf("文件不能再大了.\n");
        return -1;
    }

    //2. 取得所需的inode，并查看是否有空闲.
    unsigned int block_num;
    if (ifile.di_addr[0] == -1)
        block_num = -1;
    else
    {
        for (int i = 0; i < NADDR - 2; i++)
        {
            if (ifile.di_addr[i] != -1)
                block_num = ifile.di_addr[i];
            else
                break;
        }
        int f1[128];
        fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
        int num;
        if (ifile.di_size % BLOCK_SIZE == 0)
            num = ifile.di_size / BLOCK_SIZE;
        else
            num = ifile.di_size / BLOCK_SIZE + 1;
        if (num > 4 && num <= 132)
        {
            fseek(fd, DATA_START + ifile.di_addr[NADDR - 2] * BLOCK_SIZE, SEEK_SET);
            fread(f1, sizeof(f1), 1, fd);
            block_num = f1[num - 4];
        }
    }
    int free_space_firstBlock = BLOCK_SIZE - ifile.di_size % BLOCK_SIZE;
    unsigned int num_block_needed;
    if (len_content - free_space_firstBlock > 0)
    {
        num_block_needed = (len_content - free_space_firstBlock) / BLOCK_SIZE + 1;
    }
    else
    {
        num_block_needed = 0;
    }
    //检查是否有空闲
    if (num_block_needed > superBlock.s_num_fblock)
    {
        printf("内存不够.\n");
        return -1;
    }

    //3. 写入第一块.
    if (ifile.di_addr[0] == -1)
    {
        // find_free_block(block_num);
        ifile.di_addr[0] = block_num;
        // fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
        fseek(fd, DATA_START + block_num * BLOCK_SIZE + ifile.di_size % BLOCK_SIZE, SEEK_SET);
    }
    else
        fseek(fd, DATA_START + block_num * BLOCK_SIZE + ifile.di_size % BLOCK_SIZE, SEEK_SET);
    char data[BLOCK_SIZE];
    if (num_block_needed == 0)
    {
        fwrite(content, len_content, 1, fd);
        fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
        fread(data, sizeof(data), 1, fd);
        ifile.di_size += len_content;
    }
    else
    {
        fwrite(content, free_space_firstBlock, 1, fd);
        fseek(fd, DATA_START + block_num * BLOCK_SIZE, SEEK_SET);
        fread(data, sizeof(data), 1, fd);
        ifile.di_size += free_space_firstBlock;
    }

    //4. 写入其他块，更新文件信息。
    char write_buf[BLOCK_SIZE];
    unsigned int new_block_addr = -1;
    unsigned int content_write_pos = free_space_firstBlock;
    //循环写入
    if ((len_content + ifile.di_size) / BLOCK_SIZE + ((len_content + ifile.di_size) % BLOCK_SIZE == 0 ? 0 : 1) <= NADDR - 2)
    {
        for (int i = 0; i < num_block_needed; i++)
        {
            find_free_block(new_block_addr);
            if (new_block_addr == -1)
                return -1;
            for (int j = 0; j < NADDR - 2; j++)
            {
                if (ifile.di_addr[j] == -1)
                {
                    ifile.di_addr[j] = new_block_addr;
                    break;
                }
            }
            memset(write_buf, 0, BLOCK_SIZE);
            unsigned int tmp_counter = 0;
            for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
            {
                if (content[content_write_pos + tmp_counter] == '\0')
                    break;
                write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
            }
            content_write_pos += tmp_counter;
            fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
            fwrite(write_buf, tmp_counter, 1, fd);
            fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
            fread(data, sizeof(data), 1, fd);
            ifile.di_size += tmp_counter;
        }
    }
    else if ((len_content + ifile.di_size) / BLOCK_SIZE + ((len_content + ifile.di_size) % BLOCK_SIZE == 0 ? 0 : 1) > NADDR - 2)
    {
        for (int i = 0; i < NADDR - 2; i++)
        {
            if (ifile.di_addr[i] != -1)
                continue;

            memset(write_buf, 0, BLOCK_SIZE);
            new_block_addr = -1;

            find_free_block(new_block_addr);
            if (new_block_addr == -1)
                return -1;
            ifile.di_addr[i] = new_block_addr;
            unsigned int tmp_counter = 0;
            for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
            {
                if (content[content_write_pos + tmp_counter] == '\0')
                {
                    break;
                }
                write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
            }
            content_write_pos += tmp_counter;
            fseek(fd, DATA_START + new_block_addr * BLOCK_SIZE, SEEK_SET);
            fwrite(write_buf, tmp_counter, 1, fd);
            ifile.di_size += tmp_counter;
        }
        unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = {0};

        new_block_addr = -1;
        find_free_block(new_block_addr);
        if (new_block_addr == -1)
            return -1;
        ifile.di_addr[NADDR - 2] = new_block_addr;
        for (int i = 0; i < BLOCK_SIZE / sizeof(unsigned int); i++)
        {
            new_block_addr = -1;
            find_free_block(new_block_addr);
            if (new_block_addr == -1)
                return -1;
            else
                f1[i] = new_block_addr;
        }
        fseek(fd, DATA_START + ifile.di_addr[4] * BLOCK_SIZE, SEEK_SET);
        fwrite(f1, sizeof(f1), 1, fd);
        bool flag = 0;
        for (int j = 0; j < BLOCK_SIZE / sizeof(int); j++)
        {
            fseek(fd, DATA_START + f1[j] * BLOCK_SIZE, SEEK_SET);
            unsigned int tmp_counter = 0;
            for (; tmp_counter < BLOCK_SIZE; tmp_counter++)
            {
                if (content[content_write_pos + tmp_counter] == '\0')
                {
                    //tmp_counter--;
                    flag = 1;
                    break;
                }
                write_buf[tmp_counter] = content[content_write_pos + tmp_counter];
            }
            content_write_pos += tmp_counter;
            fwrite(write_buf, tmp_counter, 1, fd);
            ifile.di_size += tmp_counter;
            if (flag == 1)
                break;
        }
    }
    time_t t = time(0);
    strftime(ifile.time, sizeof(ifile.time), "%Y/%m/%d %X %A %jday", localtime(&t));
    ifile.time[64] = 0;
    fseek(fd, INODE_START + ifile.i_ino * INODE_SIZE, SEEK_SET);
    fwrite(&ifile, sizeof(inode), 1, fd);

    //5.更新超级块
    fseek(fd, BLOCK_SIZE, SEEK_SET);
    fwrite(&superBlock, sizeof(superBlock), 1, fd);

    return len_content;
};