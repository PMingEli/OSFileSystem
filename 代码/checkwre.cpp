#include "header.h"
bool checkwre(inode * currentInode , char type){
    //inode *currentInode = OpenMutipleFile(filename);
    int current_file_uid = currentInode->di_uid;
    int current_file_permission=currentInode->permission;
    int current_file_group=currentInode->di_grp;

    switch(type){
        case 'r':{

            //检查用户
            if(current_file_uid==userID){
                if(current_file_permission&OWN_R){
                    return true;
                }else {
                    return false;
                } 
            }else
            //检查用户组
            if(current_file_group==userGroup){
                if(current_file_permission&GRP_R){
                    return true;
                }else {
                    return false;
                } 
            }
            //检查其他用户
            else{
                if(current_file_permission&ELSE_R){
                    return true;
                }else {
                    return false;
                } 
            }
        }break;

        case 'w':{
            //检查用户
            if(current_file_uid==userID){
                if(current_file_permission&GRP_W){
                    return true;
                }else {
                    return false;
                } 
            }else
            //检查用户组
            if(current_file_group=userGroup){
                if(current_file_permission&GRP_W){
                    return true;
                }else{
                    return false;
                }
            }
            //检查其他用户
            else{
                if(current_file_permission&ELSE_W){
                    return true;
                }else {
                    return false;
                } 
            }
        }break;

        case 'e':{
               //检查用户
            if(current_file_uid==userID){
                if(current_file_permission&OWN_E){
                    return true;
                }else {
                    return false;
                } 
            }else
            //检查用户组
            if(current_file_group==userGroup){
                if(current_file_permission&GRP_E){
                    return true;
                }else {
                    return false;
                } 
            }
            //检查其他用户
            else{
                if(current_file_permission&ELSE_E){
                    return true;
                }else {
                    return false;
                } 
            }
        }break;

    }

}