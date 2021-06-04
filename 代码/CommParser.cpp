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
        // getline(cin, system);
        int tmp_input_char;
        tmp_input_char = getch();
        int loop_num = 0;
        bool up_button = false;
        bool down_button = false;
        // cout << loop_num;
        while (tmp_input_char != 13)
        {
            // cout << "!!!!" << endl;
            if (tmp_input_char != 27)
            {
                if (tmp_input_char == 8)
                {
                    if (loop_num != 0)
                    {
                        printf("\b \b");
                        // cout << loop_num;
                        system = system.substr(0, system.length() - 1);
                        loop_num--;
                    }
                }
                else
                {
                    loop_num++;
                    cout << char(tmp_input_char);
                    system.push_back(char(tmp_input_char));
                }
            }
            else // if (tmp_input_char == 91)
            {
                tmp_input_char = getch();
                if (tmp_input_char == 91)
                {
                    tmp_input_char = getch();
                    if (para1_ptr >= 0 && para1_ptr < para1_history.size())
                    {
                        if (loop_num && para1_ptr != 0)
                        {
                            for (int i = 0; i < loop_num; i++)
                            {
                                printf("\b \b");
                            }
                            loop_num = 0;
                        }
                        if (tmp_input_char == 65)
                        {
                            if (para1_ptr != 0)
                            {
                                cout << para1_history.at(--para1_ptr);
                                // cout << para1_ptr;
                                up_button = true;
                                system.assign(para1_history.at(para1_ptr));
                                loop_num = para1_history.at(para1_ptr).size();
                            }
                            // para1_ptr--;
                        }
                        else if (tmp_input_char == 66)
                        {
                            if (para1_ptr == 0)
                            {
                                for (int i = 0; i < loop_num; i++)
                                {
                                    printf("\b \b");
                                }
                                loop_num = 0;
                                down_button = true;
                                cout << para1_history.at(++para1_ptr);
                                system.assign(para1_history.at(para1_ptr));
                                loop_num = para1_history.at(para1_ptr).size();
                            }
                            else if (para1_ptr != para1_history.size() - 1)
                            {
                                down_button = true;
                                cout << para1_history.at(++para1_ptr);
                                system.assign(para1_history.at(para1_ptr));
                                loop_num = para1_history.at(para1_ptr).size();
                            }
                            // para1_ptr++;
                        }
                    }
                }
            }
            tmp_input_char = getch();
            // cout << "???" << endl;
            // cout << tmp_input_char;
        }

        if (system.length() == 0)
        {
            continue;
        }
        // for (int i = 0; i < system.size(); i = i + 3)
        // {
        //     cout << "11111" << endl;
        //     if (int(system.c_str()[i]) == 27 && int(system.c_str()[i + 1]) == 91)
        //     {
        //         cout << "22222" << endl;
        //         if (int(system.c_str()[i + 2]) == 65 && --para1_ptr > 0)
        //         {
        //             cout << "3333" << endl;
        //             system.assign(para1_history.at(para1_ptr));
        //         }
        //         else if (int(system.c_str()[i + 2]) == 66 && para1_ptr < system.size())
        //         {
        //             system.assign(para1_history.at(++para1_ptr));
        //         }
        //         cout << system << endl;
        //     }
        // }
        cout << endl;
        //History(system);
        loop_num = 0;
        v = split(system, " "); //根据多个字符切片
        n = v.size();
        strcpy(para1, v[0].c_str());
        para1[10] = 0;
        //选择功能
        if (strcmp("ls", para1) == 0) //显示当前文件
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                List();
            }
            else if (n == 2)
            {
                if (strcmp(v[1].c_str(), "-a") == 0)
                {
                    List_a();
                }
                else
                {
                    cout << "ls：参数错误" << endl;
                    cout << "请尝试执行“help”来获取更多信息" << endl;
                }
            }
            else
            {
                cout << "ls : 操作数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        //文件复制
        else if (strcmp("cp", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                cout << "cp: 缺少了文件操作数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 2)
            {
                cout << "cp: 在'" << v[1] << "' 后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 3)
            {
                if (strcmp(v[1].c_str(), "-l") == 0)
                {
                    cout << "cp: 在'" << v[2] << "' 后缺少了要操作的目标文件" << endl;
                    cout << "请尝试执行“help”来获取更多信息" << endl;
                }
                else
                {
                    Copy(v[1], v[2], currentInode);
                }
            }
            else if (n == 4)
            {
                ln(v[2], v[3]);
            }
            else
            {
                cout << "cp: 操作数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
        }
        //重命名
        else if (strcmp("mv", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                cout << "mv: 缺少了文件操作数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 2)
            {
                cout << "mv: 在'" << v[1] << "' 后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 3)
            {
                cout << "mv: 操作数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                char para3[1024];
                memset(para3, 0, 1024);
                strcpy(para2, v[1].c_str());
                strcpy(para3, v[2].c_str());
                para2[1023] = 0; //安全保护
                para3[1023] = 0;
                Rename(para2, para3);
            }
        }
        //显示当前目录
        else if (strcmp("pwd", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            Ab_dir();
        }
        //修改密码
        else if (strcmp("passwd", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            Passwd();
        }
        //用户权限
        else if (strcmp("chmod", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                cout << "chmod : 在chmod后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 2)
            {
                cout << "chmod : 缺少了操作数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 3)
            {
                cout << "chmod : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                Chmod(v[1], v[2]);
            }
        }
        //更改用户权限
        else if (strcmp("chown", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                cout << "chown : 在chown后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 2)
            {
                cout << "chown : 缺少操作数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 3)
            {
                cout << "chown : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                Chown(v[1], v[2]);
            }
        }
        //更改所属组
        else if (strcmp("chgrp", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                cout << "chgrp : 在chgrp后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n == 2)
            {
                cout << "chgrp : 缺少操作数" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 3)
            {
                cout << "chgrp : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                Chgrp(v[1], v[2]);
            }
        }
        //显示信息
        else if (strcmp("info", para1) == 0)
        {
            VimWriteBack();
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
            VimWriteBack();
            flag = false;
            for (int i = 1; i < n; i++)
            {
                CreateFile(v[i]);
            }
        }
        //删除文件
        else if (strcmp("rm", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            for (int i = 1; i < n; i++)
            {
                DeleteFile(v[i]);
            }
        }
        //打开文件
        else if (strcmp("open", para1) == 0)
        {
            VimWriteBack();
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
        //清空文件
        else if (strcmp("clear", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            for (int i = 1; i < n; i++)
            {
                ClearFile(v[i]);
            }
        }
        //Vim打开文件
        else if (strcmp("vim", para1) == 0)
        {
            VimWriteBack();
            flag = true;
            if (n < 2)
            {
                cout << "vim : 在vim后缺少了要操作的目标文件" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else if (n > 2)
            {
                cout << "vim : 参数过多" << endl;
                cout << "请尝试执行“help”来获取更多信息" << endl;
            }
            else
            {
                vimroute = v[1];
                currentInode = OpenMutipleFile(v[1]);
                VimFile(*currentInode);
            }
        }
        //写文件
        else if (strcmp("insert", para1) == 0)
        {
            VimWriteBack();
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
                    //cout << currentInode->i_ino << endl;
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
            VimWriteBack();
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
            VimWriteBack();
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
            VimWriteBack();
            flag = false;
            if (n < 2)
                cout << "mkdir: 缺少操作数" << endl
                     << "请尝试执行 \"help \" 来获取更多信息。" << endl;
            else
            {
                if (v.at(1).c_str()[0] == '-')
                {
                    if (v.at(1).c_str()[1] == 'p')
                    {
                        if (n < 3)
                        {
                            cout << "mkdir: 缺少操作数" << endl
                                 << "请尝试执行 \"help \" 来获取更多信息。";
                        }
                        else
                        {
                            for (int m = 2; m < n; m++)
                            {
                                vector<string> mul_dir = split(v.at(m), "/");
                                int i;
                                for (i = 0; i < mul_dir.size(); i++)
                                {
                                    strcpy(para2, mul_dir.at(i).c_str());
                                    para2[1023] = 0; //security protection
                                    if (!MakeDir(para2))
                                    {
                                        break;
                                    }
                                    OpenDir(para2);
                                }
                                for (int j = 0; j < i; j++)
                                {
                                    OpenDir("..");
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int m = 1; m < n; m++)
                    {
                        if (v.at(m).find('/') == v.at(m).npos)
                        {
                            strcpy(para2, v.at(m).c_str());
                            para2[1023] = 0;
                            MakeDir(para2);
                            // OpenDir("..");
                        }
                        else
                        {
                            cout << "mkdir: 无法创建目录 “" << v.at(m) << ": 没有那个文件或目录" << endl;
                        }
                    }
                }
            }
        }
        //删除目录
        else if (strcmp("rmdir", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n < 2)
            {
                cout << "rmdir: 缺少操作数" << endl
                     << "请尝试执行 \"help \" 来获取更多信息。" << endl;
            }
            if (v.at(1).c_str()[0] == '-')
            {
                if (v.at(1).c_str()[1] == 'p')
                {
                    for (int i = 2; i < n; i++)
                    {
                        vector<string> mul_dir = split(v.at(i), "/");
                        if (mul_dir.size() < 1)
                            cout << "rmdir: 缺少操作数" << endl
                                 << "请尝试执行 \"help \" 来获取更多信息。";
                        else
                        {
                            if (OpenMutipleDir(v.at(i)))
                            {
                                for (int j = mul_dir.size() - 1; j >= 0; j--)
                                {
                                    strcpy(para2, mul_dir.at(j).c_str());
                                    para2[1023] = 0; //security protection
                                    if (RemoveDir(para2))
                                    {
                                        break;
                                    }
                                    OpenDir("..");
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for (int i = 1; i < n; i++)
                {
                    strcpy(para2, v.at(i).c_str());
                    para2[1023] = 0;
                    RemoveDir(para2);
                }
            }
        }
        //登出系统
        else if (strcmp("logout", para1) == 0)
        {
            VimWriteBack();
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
        //登录
        else if (strcmp("su", para1) == 0)
        {
            VimWriteBack();
            flag = false;
            if (n == 1)
            {
                Logout();
                char para3[USER_PASSWORD_LENGTH * 5];
                memset(para3, 0, USER_PASSWORD_LENGTH + 1);
                //strcpy(para2, v[1].c_str());
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
            VimWriteBack();
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
            VimWriteBack();
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
                    Format();
                    dir_pointer = 1;
                    Mount();
                    dir_pointer = 1;
                }
                else
                {
                    printf("您的权限不够，请联系管理员操作！\n");
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
            VimWriteBack();
            flag = false;
            Help();
        }
    }
    cout << endl;
};