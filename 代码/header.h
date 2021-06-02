#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h> //c语言的的unix文件输入输出库
#include <fcntl.h> //根据文件描述词操作文件的特性
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

//以下定义常量

//文件可使用的最大数量节点
const unsigned int NADDR = 6;

//一个block的大小
const unsigned short BLOCK_SIZE = 512;

//文件最大尺寸
const unsigned int FILE_SIZE_MAX = (NADDR - 2) * BLOCK_SIZE + BLOCK_SIZE / sizeof(int) * BLOCK_SIZE;

//block的数量
const unsigned short BLOCK_NUM = 512;

//inode大小
const unsigned short INODE_SIZE = 128;

//inode数量
const unsigned short INODE_NUM = 256;

//inode的起始位置
const unsigned int INODE_START = 3 * BLOCK_SIZE;

//数据起始位置
const unsigned int DATA_START = INODE_START + INODE_NUM * INODE_SIZE;

//文件系统支持的用户数量
const unsigned int ACCOUNT_NUM = 10;

//子目录的最大数量
const unsigned int DIRECTORY_NUM = 16;

//文件名的最大长度
const unsigned short FILE_NAME_LENGTH = 14;

//用户名的最大长度
const unsigned short USER_NAME_LENGTH = 14;

//用户密码的最大长度
const unsigned short USER_PASSWORD_LENGTH = 14;

//文件最大权限
const unsigned short MAX_PERMISSION = 511;

//用户最大权限
const unsigned short MAX_OWNER_PERMISSION = 448;

//权限
const unsigned short ELSE_E = 1;
const unsigned short ELSE_W = 1 << 1;
const unsigned short ELSE_R = 1 << 2;
const unsigned short GRP_E = 1 << 3;
const unsigned short GRP_W = 1 << 4;
const unsigned short GRP_R = 1 << 5;
const unsigned short OWN_E = 1 << 6;
const unsigned short OWN_W = 1 << 7;
const unsigned short OWN_R = 1 << 8;

//inode设计
struct inode
{
    unsigned int i_ino;          //inode号.
    unsigned int di_addr[NADDR]; //Number of data blocks where the file stored.
    unsigned short di_number;    //Number of associated files.
    unsigned short di_mode;      //文件类型. 0代表目录 1代表文件
    unsigned short icount;       //连接数
    unsigned short permission;   //文件权限
    unsigned short di_uid;       //文件所属用户id.
    unsigned short di_grp;       //文件所属组
    unsigned short di_size;      //文件大小.
    char time[83];
};

//超级块设置
struct filsys
{
    unsigned short s_num_inode;  //inode总数
    unsigned short s_num_finode; //空闲inode数.
    unsigned short s_size_inode; //inode大小.
    unsigned short s_num_block;  //block的数量.
    unsigned short s_num_fblock; //空闲块的数量.
    unsigned short s_size_block; //block的大小.
    unsigned int special_stack[50];
    int special_free;
};

//目录设计
struct directory
{
    char fileName[20][FILE_NAME_LENGTH]; //目录名称
    unsigned int inodeID[DIRECTORY_NUM]; //inode号
};

//账户设计
struct userPsw
{
    unsigned short userID[ACCOUNT_NUM];               //用户id
    char userName[ACCOUNT_NUM][USER_NAME_LENGTH];     //用户名
    char password[ACCOUNT_NUM][USER_PASSWORD_LENGTH]; //用户密码
    unsigned short groupID[ACCOUNT_NUM];              //用户所在组id
};

//超级块
extern filsys superBlock;

//1代表已经使用，0表示空闲
extern unsigned short inode_bitmap[INODE_NUM];

//用户
extern userPsw users;

//用户id
extern unsigned short userID;

//用户名
extern char userName[USER_NAME_LENGTH + 6];

//当前目录
extern directory currentDirectory;

extern char ab_dir[100][14];

extern unsigned short dir_pointer;

extern vector<string> currentdir;

//功能函数声明
void CommParser(inode *&);

void Help(); //帮助信息

void Sys_start(); //启动文件系统

extern FILE *fd; //文件系统位置

int getch();

//寻找空闲块
void find_free_block(unsigned int &inode_number);

//初始化文件系统的各项功能
bool Mount();

//登出功能
void Logout();

//根据文件名创建文件
bool CreateFile(const char *filename);

//删除文件
bool DeleteFile(const char *filename);

//重置block
void recycle_block(unsigned int &inode_number);

//初始化文件系统
bool Format();

//终端显示目录
void Printsign();

//目录多层访问
void OpenMutipleDir(string route);

//文件多层访问
inode *OpenMutipleFile(string route);

//登录模块
bool Login(const char *user, const char *password);

//根据文件名打开文件
inode *OpenFile(const char *filename);

//在文件尾添加数据
int Write(inode &ifile, const char *content);

//输出文件信息
void PrintFile(inode &ifile);

//split函数
vector<string> split(const string &s, const string &seperator);

//创建新目录，新目录包含. ..
bool MakeDir(const char *dirname);

//删除一个目录和目录下的所有文件
bool RemoveDir(const char *dirname);

//打开一个目录
bool OpenDir(const char *dirname);

//Vim打开文件
void VimFile(inode &ifile);

//显示当前目录下的文件信息
void List();

//显示绝对目录.
void Ab_dir();

//修改文件权限
void Chmod(char *filename);

//改变文件所属
void Chown(char *filename);

//改变文件所属组.
void Chgrp(char *filename);

//修改密码
void Passwd();

//用户管理
void User_management();

//对文件或目录重命名
void Rename(char *filename);

//链接
bool ln(char *filename);

//文件复制
bool Copy(char *filename, inode *&currentInode);

#endif // HEADER_H_INCLUDED