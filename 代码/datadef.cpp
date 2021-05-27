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
    unsigned short di_mode;      //文件类型.
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

//功能函数声明
void CommParser(inode *&);

void Help(); //帮助信息

void Sys_start(); //启动文件系统

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

//用户名
char userName[USER_NAME_LENGTH + 6];

//当前目录
directory currentDirectory;

char ab_dir[100][14];

unsigned short dir_pointer;

//23t872guergfu54biuvbtrui druebfu