#include"header.h"
//修改文件权限
void Chmod(char* filename) {
    printf("0=文件，1=目录，请输入:");
    int tt;
    scanf("%d", &tt);
    if (filename == NULL || strlen(filename) > FILE_NAME_LENGTH)
    {
        printf("不合法.\n");
        return;
    }
    
    //1. 检查是否存在.
    int pos_in_directory = -1;
    inode* tmp_file_inode = new inode;
    do {
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
            return;
        }
        
        //2. 检查是否存在目录.
        int tmp_file_ino = currentDirectory.inodeID[pos_in_directory];
        fseek(fd, INODE_START + tmp_file_ino * INODE_SIZE, SEEK_SET);
        fread(tmp_file_inode, sizeof(inode), 1, fd);
    } while (tmp_file_inode->di_mode == tt);
    
    printf("请输入 0&1 串给予权限\n");
    printf("格式: rwerwerwe\n");
    char str[10];
    scanf("%s", &str);
    if (userID == tmp_file_inode->di_uid)
    {
        if (!(tmp_file_inode->permission & OWN_E)) {
            printf("权限不够.\n");
            return;
        }
    }
    else if (users.groupID[userID] == tmp_file_inode->di_grp) {
        if (!(tmp_file_inode->permission & GRP_E)) {
            printf("权限不够.\n");
            return;
        }
    }
    else {
        if (!(tmp_file_inode->permission & ELSE_E)) {
            printf("权限不够.\n");
            return;
        }
    }
    int temp = 0;
    for (int i = 0; i < 8; i++) {
        if (str[i] == '1')
            temp += 1 << (8 - i);
    }
    if (str[8] == '1') temp += 1;
    tmp_file_inode->permission = temp;
    fseek(fd, INODE_START + tmp_file_inode->i_ino * INODE_SIZE, SEEK_SET);
    fwrite(tmp_file_inode, sizeof(inode), 1, fd);
    return;
}
