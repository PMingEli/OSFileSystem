#include"header.h"
//修改密码
void Passwd() {
    printf("请输入旧密码:");
    char str[USER_PASSWORD_LENGTH];
    char newpassword[USER_PASSWORD_LENGTH];
    scanf("%s", str);
    str[USER_PASSWORD_LENGTH] = 0;
    if (strcmp(users.password[userID], str) == 0) {
        printf("请输入新密码:");
        scanf("%s", str);
        if (strcmp(users.password[userID], str) == 0) {
            printf("新密码不能和旧密码相同!\n");
        }else {
            strcpy(newpassword,str);
            printf("请再次输入新密码:");
            scanf("%s", str);
        }if(strcmp(newpassword, str) == 0){
            strcpy(users.password[userID], str);
            fseek(fd, DATA_START + BLOCK_SIZE, SEEK_SET);
            fwrite(&users, sizeof(users), 1, fd);
            printf("修改成功\n");
        }else {
             printf("两次输入的密码不相同!!!\n");
        }
    }else{
        printf("密码错误!!!\n");
    }

}