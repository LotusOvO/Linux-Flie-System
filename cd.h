//
// Created by 62622 on 2021/10/18.
//

#ifndef OS_CD_H
#define OS_CD_H
#include "header.h"
#include <cstring>

void cd(vector<string> args = {}) {
    string temp = nowdirname;
    int tempindex = nowinodenum;
    if (args.empty()) {
        nowinodenum = 0;
        nowdirname.clear();
        nowdirname = "simdisk";
        goto End;
    } else if (args.size() == 1) {
        string path = args[0] + "/.";
//        string path = pathdivide(args[0] + "/.", tempinodenum);
//        if(path == "|error|"){    //找不到路径
//            cout << "Error: can't find the dir\n";
//            return;
//        }
        vector<Dirnode> tempdir;
        int tempinodeindex;
        if (path[0] == '/') {
            tempinodeindex = 0;//定位到根目录
            path = path.substr(1);
            nowdirname.clear();
            nowdirname += "simdisk";
        } else {
            tempinodeindex = nowinodenum;//定位到当前目录
        }
        size_t pos = path.find_first_of('/');;
        while (pos != string::npos) {
            string needopen = path.substr(0, pos);
            path = path.substr(pos + 1);
            if(tempinodeindex){ //不在根目录
                if(needopen == ".."){
                    size_t cat = nowdirname.find_last_of('/');
                    nowdirname = nowdirname.substr(0,cat);
                }
                else if(needopen != "."){
                    nowdirname += ("/" + needopen);
                }
            }
            else{   //在根目录
                if(needopen != "."&&needopen != ".."){
                    nowdirname += ("/" + needopen);
                }
            }
            //读取目录
            for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
                Dir temp{};
                disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
                disk.read((char *) &temp, sizeof(temp));
                for (int j = 0; j < temp.num; j++) {
                    string ss(temp.dir[j].filename);
                    if (ss == needopen && inode[temp.dir[j].inodeindex].mode[0] == '0') {
                        tempinodeindex = temp.dir[j].inodeindex;
                        nowinodenum = tempinodeindex;
                        goto End1;
                    }
                }
            }
            //无法打开，还原原本状态
            nowdirname = temp;
            nowinodenum = tempindex;
            cout << "Error: can't find the dir\n";
            return;
        End1:
            pos = path.find_first_of('/');;
        }
        goto End;
    }
    cout << "Error: Undefined instruction\n";
    End:
        ;
}

#endif//OS_CD_H
