//
// Created by 62622 on 2021/10/19.
//

#ifndef OS_DEL_H
#define OS_DEL_H
#include "header.h"

void del(vector<string> args){
    if(args.size() == 1){
        int tempinodeindex; //当前目录的i节点
        string path = pathdivide(args[0], tempinodeindex);
        if (path == "|error|") {//找不到路径
            outss << "Error: can't find the dir\n";
            return;
        }
        vector<Dirnode> tempdir;    //当前目录
        //读取目录
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
            Dir temp{};
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                tempdir.push_back(temp.dir[j]);
            }
        }
        int dirindex = 0; // 需删除文件在目录第i项
        for (int i = 2; i < tempdir.size(); i++) {
            if (path == tempdir[i].filename) {
                //判断是否是文件
                if (inode[tempdir[i].inodeindex].mode[0] == '1') {
                    dirindex = i;
                } else {
                    outss << "Error: This is not a file\n";
                    return;
                }
            }
        }
        delinode(tempdir[dirindex].inodeindex); //删除i节点以及数据块
        tempdir.erase(tempdir.begin() + dirindex);
        int bn = 0;//新的块数量
        //保存新的目录信息到数据块
        for (int i = 0; i < tempdir.size() / 40; i++) {
            Dir t{};
            bn++;
            t.num = 40;
            for (int j = 0; j < 40; j++) {
                strcpy(t.dir[j].filename, tempdir[j + i * 40].filename);
                t.dir[j].inodeindex = tempdir[j + i * 40].inodeindex;
            }
            disk.seekp((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.write((char *) &t, sizeof(t));
        }
        int lastnum = tempdir.size() % 40;  //最后一块的目录项数量
        if (lastnum) {
            Dir t{};
            t.num = lastnum;
            for (int i = 0; i < lastnum; i++) {
                strcpy(t.dir[i].filename, tempdir[i + bn * 40].filename);
                t.dir[i].inodeindex = tempdir[i + bn * 40].inodeindex;
            }
            disk.seekp((87 + inode[tempinodeindex].blocknode[bn]) * 1024);
            disk.write((char *) &t, sizeof(t));
            bn++;
        }
        if (bn == inode[tempinodeindex].blocknum - 1) {
            inode[tempinodeindex].blocknum -= 1;
            delblock(inode[tempinodeindex].blocknode[bn]);
        }
        //保存inode数据
        disk.seekp(14 * 1024 + 72 * tempinodeindex);
        disk.write((char *) &inode[tempinodeindex], sizeof(inode[tempinodeindex]));
    }
    else{
        outss << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_DEL_H
