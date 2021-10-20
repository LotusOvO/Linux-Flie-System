//
// Created by 62622 on 2021/10/19.
//

#ifndef OS_MF_H
#define OS_MF_H
#include "blockinode.h"
#include "header.h"
#include <cstring>

void mf(vector<string> args = {}) {
    if (args.size() == 1){
        args.push_back("");
    }
    if (args.size() == 2) {
        int tempinodeindex;
        string path = pathdivide(args[0], tempinodeindex);
        if (path == "|error|") {//找不到路径
            cout << "Error: can't find the dir\n";
            return;
        }
        Dir temp{};
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {//判断是否重名
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                if (temp.dir[j].filename == path) {
                    cout << "Error: This is the same name\n";
                    return;
                }
            }
        }
        int isnew = 0;
        if (temp.num == 40) {//数据块存满，新增数据块
            isnew = 1;
            if (inode[tempinodeindex].blocknum == 10) {
                cout << "Error: can't new more file\n";
                return;
            }
            int newblock = findfreeblock();
            inode[tempinodeindex].blocknode[inode[tempinodeindex].blocknum] = newblock;
            inode[tempinodeindex].blocknum += 1;
        }
        //添加新的文件，为文件寻找空闲的i节点
        int newinode = findfreeinode();
        int n = inode[tempinodeindex].blocknum;
        disk.seekg((87 + inode[tempinodeindex].blocknode[n - 1]) * 1024);
        disk.read((char *) &temp, sizeof(temp));
        if (isnew) temp.num = 0;
        strcpy(temp.dir[temp.num].filename, path.c_str());
        temp.dir[temp.num].inodeindex = newinode;
        temp.num += 1;
        //保存目录信息
        disk.seekp((87 + inode[tempinodeindex].blocknode[n - 1]) * 1024);
        disk.write((char *) &temp, sizeof(temp));
        //根据父目录填写文件属性
        inode[newinode].fsize = 0;
        strcpy(inode[newinode].mode, inode[tempinodeindex].mode);
        strcpy(inode[newinode].owner, inode[tempinodeindex].owner);
        inode[newinode].mode[0] = '1';
        strcpy(inode[newinode].time, gettime().c_str());
        inode[newinode].blocknum = 0;

        string content(args[1]);
        int len = content.size();
        for (int i = 0; i <= len / 1000; i++) {
            char c[1000];
            inode[newinode].blocknum += 1;
            inode[newinode].blocknode[i] = findfreeblock();
            string cutcontent = content.substr(i * 1000, 1000);
            strcpy(c,cutcontent.c_str());
            disk.seekp((87 + inode[newinode].blocknode[i]) * 1024);
//            cout << disk.tellp() <<"<- m\n";
            disk.write(c,1000);
        }
        inode[newinode].fsize = len;
        return;
    } else {
        cout << "Error: Undefined instruction\n";
        return;
    }
}

#endif//OS_MF_H
