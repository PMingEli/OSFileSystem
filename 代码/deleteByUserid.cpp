  
#include "header.h"
//删除一个UserId的所有文件
void DeleteByUserID(int userid)
{
  for (int i = 2; i < DIRECTORY_NUM; i++)
    {
        if (strlen(currentDirectory.fileName[i]) > 0)
        {
            inode tmp_inode;
            fseek(fd, INODE_START + currentDirectory.inodeID[i] * INODE_SIZE, SEEK_SET);
            fread(&tmp_inode, sizeof(inode), 1, fd);
        
            //const char *tmp_type = tmp_inode.di_mode == 0 ? "d" : "-";
            const char *tmp_user = users.userName[tmp_inode.di_uid];
            const int tmp_grpID = tmp_inode.di_grp;
            
            if(tmp_inode.di_uid==userid){
                if(tmp_inode.di_mode==0){
                    cout<<currentDirectory.fileName[i]<<endl;
                    RemoveDir(currentDirectory.fileName[i]);
                    //return ;
                }else {
                    string rute="root/";
                    rute.append(currentDirectory.fileName[i]);
                    cout<<rute<<endl;
                    DeleteFile(rute);
                    //return ;
                }
            }

        }
    }
    
}