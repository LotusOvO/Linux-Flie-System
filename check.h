//
// Created by 62622 on 2021/10/20.
//

#ifndef OS_CHECK_H
#define OS_CHECK_H
#include "header.h"
#include <set>

void check(vector<string> args){
    if(args.size() == 0){
        int usedinode[1024] = {0};    //i节点是否被使用
        vector<int> ind;        //待检查的i节点编号
        set<int> blo;        //已使用的数据块编号
        ind.push_back(0);
        while(!ind.empty()){
            auto index = ind.back();
            ind.pop_back();
            usedinode[index] = 1;
            if(inode[index].mode[0] == '0'){ //如果为目录则把子目录的i节点编号压入
                vector<Dirnode> tempdir;
                for (int i = 0; i < inode[index].blocknum; i++) {
                    Dir temp{};
                    disk.seekg((87 + inode[index].blocknode[i]) * 1024);
                    disk.read((char *) &temp, sizeof(temp));
                    for (int j = 0; j < temp.num; j++) {
                        tempdir.push_back(temp.dir[j]);
                    }
                    if(!blo.insert(inode[index].blocknode[i]).second){//如果磁盘块已经被使用过
                        //重新分配磁盘块
                        inode[index].blocknode[i] = findfreeblock();
                        blo.insert(inode[index].blocknode[i]);
                        disk.seekp((87 + inode[index].blocknode[i]) * 1024);
                        disk.write((char *) &temp, sizeof(temp));
                    }
                }
                for(int i = 2 ;i < tempdir.size();i++){
                    //将子目录的i节点放入
                    ind.push_back(tempdir[i].inodeindex);
                }
            }
        }
        int sum = 1024;
        for(auto &i : usedinode){
            sum -= i;
        }
        if(sum != superblock.freeinodenum){     //i节点数目不对
            for(int i =0;i <1024;i++){
                if(usedinode[i] == 0){
                    inode[i].blocknum = 0;
                }
            }
            superblock.freeinodenum = sum;
        }
        if(blo.size() != 102313 - superblock.freeblocknum){ //数据块数目不对
            for (auto &i: freeblock) {
                i = pow2(32) - 1;
            }
            for(auto &i: blo){
                freeblock[i/ 32] += pow2(i % 32);
            }
            superblock.freeblocknum = 102313 - blo.size();
        }
        outss << "Check over!\n";
    }else{
        outss << "Error: Undefined instruction\n";
        return;
    }
}
#endif //OS_CHECK_H
