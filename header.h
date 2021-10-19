//
// Created by 62622 on 2021/9/15.
//

#ifndef OS_HEADER_H
#define OS_HEADER_H
#include <vector>
using namespace std;

inline const char *FileName = "../sd";
inline std::fstream disk;

struct INode {        //72 b
    int fsize;        //4b
    int blocknum;     //4b
    int blocknode[10];//40b
    char owner[6];    //文件拥有者 6b
    char mode[7];     //[0]1文件0目录 [1-3]owner权限 [4-6]other权限 r|w|x    7b
    char time[9];     //修改时间   9b
};

struct Dirnode {//20b
    char filename[16];
    int inodeindex;
};

struct Dir {//804b
    int num;
    Dirnode dir[40];
};

//磁盘块有102400块
//inode节点约为1%，即1024个

struct SuperBlock {
    int freeblocknum;
    int freeinodenum;
};
inline SuperBlock superblock;

inline uint32_t freeblock[3200];   //数据块位图 1可用 0不可用
inline INode inode[1024];     //i节点表
inline Dir rooddir;           //根目录
inline std::string nowdirname;//当前目录名称
inline int nowinodenum;       //当前i节点编号
inline std::vector<Dirnode> nowdir;
// 超级块（1k）| 数据块位图（13K）| i节点表（73k）| 根目录块（1k）| 数据块（102312k）

string pathdivide(string path, int &tempinodeindex) {
    vector<Dirnode> tempdir;
    if (path[0] == '/') {
        tempinodeindex = 0;//定位到根目录
        path = path.substr(1);
    } else {
        //定位到当前目录
        Dir temp{};
        disk.seekg((87+inode[nowinodenum].blocknode[0]) * 1024);
        disk.read((char*)&temp,sizeof(temp));
        tempinodeindex = temp.dir[1].inodeindex;
    }
    size_t pos = path.find_first_of('/');;
    while (pos != string::npos) {
        string needopen = path.substr(0, pos);
        path = path.substr(pos + 1);
        //读取目录
        for (int i = 0; i < inode[tempinodeindex].blocknum; i++) {
            Dir temp{};
            disk.seekg((87 + inode[tempinodeindex].blocknode[i]) * 1024);
            disk.read((char *) &temp, sizeof(temp));
            for (int j = 0; j < temp.num; j++) {
                string ss(temp.dir[j].filename);
                if (ss == needopen && inode[temp.dir[j].inodeindex].mode[0] == '0') {
                    tempinodeindex = temp.dir[j].inodeindex;
                    goto End;
                }
            }
        }
        cout << "Error: can't find the dir\n";
        return "|error|";
    End:
        pos = path.find_first_of('/');;
    }
    return path;
}

int pow2(int x) {
    int sum = 1;
    for (int i = 0; i < x; i++) {
        sum *= 2;
    }
    return sum;
}

#endif//OS_HEADER_H