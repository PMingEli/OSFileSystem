#include"header.h"
//修改密码
void Passwd() {
    printf("请输入旧密码:");
    char str[USER_PASSWORD_LENGTH];
    scanf("%s", str);
    str[USER_PASSWORD_LENGTH] = 0;
    if (strcmp(users.password[userID], str) == 0) {
        printf("请输入新密码:");
        scanf("%s", str);
        if (strcmp(users.password[userID], str) == 0) {
            printf("两次密码相同!\n");
        }
        else {
            strcpy(users.password[userID], str);
            fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
            fwrite(&users, sizeof(users), 1, fd);
            printf("修改成功\n");
        }
    }
    else {
        printf("密码错误!!!\n");
    }
}
