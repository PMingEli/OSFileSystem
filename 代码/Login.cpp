#include "header.h"
//登录模块
bool Login(const char *user, const char *password)
{
    //检测参数
    if (user == NULL || password == NULL)
    {
        printf("用户名或密码不合法!\n\n");
        return false;
    }
    if (strlen(user) > USER_NAME_LENGTH || strlen(password) > USER_PASSWORD_LENGTH)
    {
        printf("用户名或密码不合法!\n");
        return false;
    }

    //检测是否登录
    if (userID != ACCOUNT_NUM)
    {
        printf("用户已经登录，请先退出.\n");
        return false;
    }

    //在账户文件中搜相应的账户名
    for (int i = 0; i < ACCOUNT_NUM; i++)
    {
        if (strcmp(users.userName[i], user) == 0)
        {
            //验证相应的密码
            if (strcmp(users.password[i], password) == 0)
            {
                //登录成功提示
                printf("登录成功！.\n");
                userID = users.userID[i];
                userGroup=users.groupID[i];
                //个性化设置
                memset(userName, 0, USER_NAME_LENGTH + 6);
                if (userID == 0)
                {
                    strcat(userName, "superman");
                    strcat(userName, "@");
                    strcat(userName, users.userName[i]);
                    //strcat(userName, "$");
                }
                else
                {
                    strcat(userName, users.userName[i]);
                    //strcat(userName, "#");
                }

                return true;
            }
            else
            {
                //密码错误的提示
                printf("密码错误.\n");
                return false;
            }
        }
    }

    //用户名未找到
    printf("登录失败，没有相应的用户.\n");
    return false;
};