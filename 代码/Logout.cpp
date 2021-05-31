#include"header.h"
//登出功能
void Logout()
{
    userID = ACCOUNT_NUM;
    memset(&users, 0, sizeof(users));
    memset(userName, 0, 6 + USER_NAME_LENGTH);
    Mount();
};