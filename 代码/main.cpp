#include "header.h"

//全局变量
FILE *fd = NULL; //文件系统位置

//超级块
filsys superBlock;

//1代表已经使用，0表示空闲
unsigned short inode_bitmap[INODE_NUM];

//用户
userPsw users;

//用户id
unsigned short userID = ACCOUNT_NUM;

//用户组
unsigned short userGroup = ACCOUNT_NUM;

//用户名
char userName[USER_NAME_LENGTH + 6];

//当前目录
directory currentDirectory;

inode curinode;

char ab_dir[100][14];

unsigned short dir_pointer;

vector<string> currentdir;

vector<string> para1_history;

string vimroute = "";

int para1_ptr = 0;

int main()
{
    memset(ab_dir, 0, sizeof(ab_dir));
    dir_pointer = 0;
    currentdir.push_back("~");
    //先查找文件卷.
    FILE *fs_test = fopen("data", "r");
    if (fs_test == NULL)
    {
        printf("文件卷没找到... 请稍后，正在新建文件卷...\n\n");
        Format();
    }
    Sys_start();
    //登录
    char tmp_userName[USER_NAME_LENGTH];
    char tmp_userPassword[USER_PASSWORD_LENGTH * 5];
    int count = 0;
    do
    {
        if (count == 3)
        {
            cout << "错误次数过多！" << endl;
            return 0;
        }
        memset(tmp_userName, 0, USER_NAME_LENGTH);
        memset(tmp_userPassword, 0, USER_PASSWORD_LENGTH * 5);

        printf("用户登录\n\n");
        printf("用户名:\t");
        scanf("%s", tmp_userName);
        printf("密码:\t");
        char c;
        scanf("%c", &c);
        int i = 0;
        count++;
        while (1)
        { // cout << loop_num;

            char ch;
            ch = getch();
            if (ch == '\b')
            {
                if (i != 0)
                {
                    // printf("\b \b");
                    i--;
                }
                else
                {
                    tmp_userPassword[i] = '\0';
                }
            }
            else if (ch == '\r')
            {
                tmp_userPassword[i] = '\0';
                printf("\n\n");
                break;
            }
            else
            {
                // putchar('*');
                tmp_userPassword[i++] = ch;
            }
        }
    } while (Login(tmp_userName, tmp_userPassword) != true);
    inode *currentInode = new inode;
    CommParser(currentInode);
    return 0;
}