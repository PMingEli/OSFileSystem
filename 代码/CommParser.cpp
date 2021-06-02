#include "header.h"

//主要功能选择集合
void CommParser(inode *&currentInode)
{
    char para1[11]; //存储功能前缀
    char para2[1024];
    bool flag = false;
    int n = 0;
    vector<string> v;
    while (true)
    {
        unsigned int f1[BLOCK_SIZE / sizeof(unsigned int)] = {0};
        fseek(fd, DATA_START + 8 * BLOCK_SIZE, SEEK_SET);
        fread(f1, sizeof(f1), 1, fd);
        memset(para1, 0, 11);
        memset(para2, 0, 1024);
        Printsign();
        string system;
        getline(cin, system);
        if (system.length() == 0)
        {
            continue;
        }
        v = split(system, " "); //根据多个字符切片
        n = v.size();
        strcpy(para1, v[0].c_str());
        para1[10] = 0;
        //选择功能
        if (strcmp("ls", para1) == 0) //显示当前文件
        {
            flag = false;
            List();
        }
        else if (strcmp("cp", para1) == 0)
        { //文件复制
            flag = false;
            scanf("%s", para2);
            para2[1023] = 0; //安全保护
            Copy(para2, currentInode);
        }
        else if (strcmp("mv", para1) == 0)
        { //重命名
            flag = false;
            scanf("%s", para2);
            para2[1023] = 0; //安全保护
            Rename(para2);
        }
        //显示当前目录
        else if (strcmp("pwd", para1) == 0)
        {
            flag = false;
            Ab_dir();
        }
        //修改密码
        else if (strcmp("passwd", para1) == 0)
        {
            flag = false;
            Passwd();
        }
        //用户权限
        else if (strcmp("chmod", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                cout << "chmod : 在chgrp后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "chmod : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                strcpy(para2, v[1].c_str());
                para2[1023] = 0;
                Chmod(para2);
            }
        }
        //更改用户权限
        else if (strcmp("chown", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                cout << "chown : 在chgrp后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "chown : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                strcpy(para2, v[1].c_str());
                para2[1023] = 0;
                Chown(para2);
            }
        }
        //更改所属组
        else if (strcmp("chgrp", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                cout << "chgrp : 在chgrp后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "chgrp : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                strcpy(para2, v[1].c_str());
                para2[1023] = 0;
                Chgrp(para2);
            }
        }
        //显示信息
        else if (strcmp("info", para1) == 0)
        {
            if (n == 1)
            {
                printf("系统信息:\n总共的block:%d\n空闲block:%d\n总inode:%d\n剩余inode:%d\n\n", superBlock.s_num_block, superBlock.s_num_fblock, superBlock.s_num_inode, superBlock.s_num_finode);
                for (int i = 0; i < 50; i++)
                {
                    if (i == 0)
                    {
                        printf("%d\t", superBlock.special_free);
                    }
                    else
                    {
                        if (i > superBlock.special_free)
                            printf("-1\t");
                        else
                            printf("%d\t", superBlock.special_stack[i]);
                        if (i % 10 == 9)
                            printf("\n");
                    }
                }
                printf("\n\n");
            }
            else
            {
                cout << "info : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        //创建文件
        else if (strcmp("create", para1) == 0)
        {
            flag = false;
            strcpy(para2, v[1].c_str());
            para2[1023] = 0;
            CreateFile(para2);
        }
        //删除文件
        else if (strcmp("rm", para1) == 0)
        {
            flag = false;
            //scanf("%s", para2);
            strcpy(para2, v[1].c_str());
            para2[1023] = 0;
            DeleteFile(para2);
        }
        //打开文件
        else if (strcmp("open", para1) == 0)
        {
            flag = true;
            if (n < 2)
            {
                cout << "open : 在open后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "open : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                currentInode = OpenMutipleFile(v[1]);
            }
        }
        //写文件
        else if (strcmp("insert", para1) == 0)
        {
            if (flag)
            {
                for (int i = 1; i < n - 1; i++)
                {
                    strcat(para2, v[i].c_str());
                    strcat(para2, " ");
                }
                if (n > 1)
                {
                    strcat(para2, v[n - 1].c_str());
                    para2[1023] = 0;
                    Write(*currentInode, para2);
                }
            }
            else
            {
                if (n < 2)
                {
                    cout << "insert : 在insert后缺少了要操作的目标文件" << endl;
                    cout << "请尝试执行“help”来获取更多信息" << endl;
                }
                else
                {
                    currentInode = OpenMutipleFile(v[1]);
                    for (int i = 2; i < n - 1; i++)
                    {
                        strcat(para2, v[i].c_str());
                        strcat(para2, " ");
                    }
                    if (n > 2)
                    {
                        strcat(para2, v[n - 1].c_str());
                        para2[1023] = 0;
                        Write(*currentInode, para2);
                    }
                }
            }
        }
        //读文件
        else if (strcmp("cat", para1) == 0)
        {
            if (flag)
            {
                if (n == 1)
                {
                    PrintFile(*currentInode);
                }
                else
                {
                    flag = false;
                    if (n < 2)
                    {
                        cout << "cat : 在cat后缺少了要操作的目标文件" << endl;
                        cout << "请尝试执行“help”来获取更多信息" << endl;
                    }
                    else if (n > 2)
                    {
                        cout << "cat : 参数过多" << endl;
                        cout << "请尝试执行“help”来获取更多信息" << endl;
                    }
                    else
                    {
                        currentInode = OpenMutipleFile(v[1]);
                        PrintFile(*currentInode);
                    }
                }
            }
            else
            {
                flag = false;
                if (n < 2)
                {
                    cout << "cat : 在cat后缺少了要操作的目标文件" << endl;
                    cout << "请尝试执行“help”来获取更多信息" << endl;
                }
                else if (n > 2)
                {
                    cout << "cat : 参数过多" << endl;
                    cout << "请尝试执行“help”来获取更多信息" << endl;
                }
                else
                {
                    currentInode = OpenMutipleFile(v[1]);
                    PrintFile(*currentInode);
                }
            }
        }
        //打开一个目录
        else if (strcmp("cd", para1) == 0)
        {
            flag = false;
            if (n == 2)
            {
                flag = false;
                OpenMutipleDir(v[1]);
            }
            else if (n > 2)
            {
                cout << "cd : 参数过多" << endl;
            }
        }
        //创建目录
        else if (strcmp("mkdir", para1) == 0)
        {
            vector<string> mul_dir = split(v.at(1), "/");
            flag = false;
            if (mul_dir.size() < 1)
                cout << "mkdir: 缺少操作数" << endl
                     << "请尝试执行help来获取更多信息" << endl;
            else
            {
                for (int i = 0; i < mul_dir.size(); i++)
                {
                    strcpy(para2, mul_dir.at(i).c_str());
                    para2[1023] = 0; //security protection
                    MakeDir(para2);
                    OpenDir(para2);
                }
                for (int i = 0; i < mul_dir.size(); i++)
                {
                    OpenDir("..");
                }
            }
        }
        //删除目录
        else if (strcmp("rmdir", para1) == 0)
        {
            if (v.at(1).c_str()[0] == '-')
            {
                if (v.at(1).c_str()[0] == 'p')
                {
                }
            }
            vector<string> mul_dir = split(v.at(1), "/");
            flag = false;
            if (mul_dir.size() < 1)
                cout << "mkdir: 缺少操作数" << endl
                     << "请尝试执行help来获取更多信息";
            else
            {
                // for (int i = 0; i < mul_dir.size(); i++)
                // {
                //     strcpy(para2, mul_dir.at(i).c_str());
                //     para2[1023] = 0; //security protection
                //     MakeDir(para2);
                //     OpenDir(para2);
                // }
                OpenMutipleDir(v.at(1));
                for (int i = mul_dir.size() - 1; i >= 0; i--)
                {
                    strcpy(para2, mul_dir.at(i).c_str());
                    para2[1023] = 0; //security protection
                    RemoveDir(para2);
                    OpenDir("..");
                }
            }
        }
        //登出系统
        else if (strcmp("logout", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                Logout();
                char tmp_userName[USER_NAME_LENGTH], tmp_userPassword[USER_PASSWORD_LENGTH * 5];
                do
                {
                    memset(tmp_userName, 0, USER_NAME_LENGTH);
                    memset(tmp_userPassword, 0, USER_PASSWORD_LENGTH * 5);
                    printf("用户登录\n\n");
                    printf("用户名:\t");
                    scanf("%s", tmp_userName);
                    printf("密码:\t");
                    char c;
                    scanf("%c", &c);
                    int i = 0;
                    while (1)
                    {
                        char ch;
                        ch = getch();
                        if (ch == '\b')
                        {
                            if (i != 0)
                            {
                                printf("\b \b");
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
                            putchar('*');
                            tmp_userPassword[i++] = ch;
                        }
                    }
                } while (Login(tmp_userName, tmp_userPassword) != true);

                inode *currentInode = new inode;
            }
            else
            {
                cout << "logout : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        else if (strcmp("ln", para1) == 0)
        {
            flag = false;
            if (n < 2)
            {
                cout << "ln : 缺少参数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "ln : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                strcpy(para2, v[1].c_str());
                para2[1023] = 0; //security protection
                ln(para2);
            }
        }
        //登录
        else if (strcmp("su", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                Logout();
                char para3[USER_PASSWORD_LENGTH * 5];
                memset(para3, 0, USER_PASSWORD_LENGTH + 1);
                scanf("%s", para2);
                para2[1023] = 0;
                printf("密码: ");
                char c;
                scanf("%c", &c);
                int i = 0;
                while (1)
                {
                    char ch;
                    ch = getch();
                    if (ch == '\b')
                    {
                        if (i != 0)
                        {
                            printf("\b \b");
                            i--;
                        }
                    }
                    else if (ch == '\r')
                    {
                        para3[i] = '\0';
                        printf("\n\n");
                        break;
                    }
                    else
                    {
                        putchar('*');
                        para3[i++] = ch;
                    }
                }
                para3[USER_PASSWORD_LENGTH] = 0; //security protection
                Login(para2, para3);
            }
            else
            {
                cout << "su : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        else if (strcmp("Muser", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                User_management();
            }
            else
            {
                cout << "Muser : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        //退出系统
        else if (strcmp("exit", para1) == 0)
        {
            flag = false;
            if (n == 1)
            {
                break;
            }
            else
            {
                cout << "exit : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        else if ((strcmp("rm-rf", para1) == 0))
        {
            flag = false;
            if (n == 1)
            {
                if (userID == 0)
                {
                    printf("您真的要删除跑路吗？[y/n]");
                    char label;
                    cin >> label;
                    while (label != 'n' && label != 'N')
                    {
                        if (label == 'y' || label == 'Y')
                        {
                            Format();
                            Mount();
                            break;
                        }
                        else
                        {
                            printf("输入错误，请重新输入[y/n]");
                            cin >> label;
                        }
                    }
                }
                else
                {
                    printf("您的权限不够，请联系管理员操作！");
                }
            }
            else
            {
                cout << "rm-rf : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        //help
        else
        {
            flag = false;
            if (n == 1)
            {
                Help();
            }
            else
            {
                cout << "help : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
    }
};