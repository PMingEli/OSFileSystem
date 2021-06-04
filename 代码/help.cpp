#include "header.h"
void Help()
{
    printf("系统当前支持指令:\n");
    printf("\t01.退出系统.........................(exit)\n");
    printf("\t02.显示帮助信息......................(help)\n");
    printf("\t03.显示当前目录......................(pwd)\n");
    printf("\t04.列出(全部）文件或目录....................(ls （-a）)\n");
    printf("\t05.cd到其他目录......................(cd + dirname)\n");
    printf("\t06.返回上级目录...................... (cd ..)\n");
    printf("\t07.创建新目录........................(mkdir + dirname)\n");
    printf("\t08.删除目录..........................(rmdir + dirname)\n");
    printf("\t09.新建文件..........................(create + filename)\n");
    printf("\t10.打开文件..........................(open + filename)\n");
    printf("\t11.读取文件..........................(cat + filename)\n");
    printf("\t12.尾部写入文件..........................(insert + filename + <content>)\n");
    printf("\t13.复制文件..........................(cp + filename + dirname)\n");
    printf("\t14.链接文件..........................(cp + -l + filename + dirname)\n");
    printf("\t15.删除文件..........................(rm + filename)\n");
    printf("\t16.清空文件..........................(clear + filename)\n");
    printf("\t17.Vim读写文件..........................(vim + filename)\n");
    printf("\t18.查看系统信息.......................(info)\n");
    printf("\t19.退出当前用户.......................(logout)\n");
    printf("\t20.改变当前用户名.....................(su + username)\n");
    printf("\t21.改变文件权限.......................(chmod + filename + 权限)\n");
    printf("\t22.改变文件所有者.....................(chown + filename + Username)\n");
    printf("\t23.改变所属组.........................(chgrp + filename + 组别)\n");
    printf("\t24.重命名............................(mv + filename + newfilename)\n");
    printf("\t25.改密码.............................(passwd)\n");
    printf("\t26.初始化文件系统.......................(rm-rf)\n");
    printf("\t27.用户管理界面.........................(Muser)\n");
    printf("\t28.移动目录..........................(chdir + filename(dirname) + dirname)\n");
};