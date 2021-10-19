#include "Format.h"
#include "blockinode.h"
#include "cat.h"
#include "cd.h"
#include "copy.h"
#include "del.h"
#include "header.h"
#include "ls.h"
#include "md.h"
#include "mf.h"
#include "rd.h"
#include <cstring>
#include <iostream>
#include <sstream>
// 超级块（1k）| 数据块位图（13K）| i节点表（73k）| 根目录块（1k）| 数据块（102312k）

int main() {
    disk.open(FileName, ios::in | ios::out | ios::binary);
    cout << disk.is_open() << '\n';
    //    readinode();
    //    for(int i = 2;i < 1024;i++){
    //        inode[i].blocknum = 0;
    //    }
    //    disk.seekp(14 *1024);
    //    for(auto &i:inode){
    //        disk.write((char*)&i,sizeof(i));
    //    }
    //    disk.close();
    //    return 0;
    //    superblock.freeinodenum = 1022;
    //    superblock.freeblocknum = 102311;
    //    for(auto &i : freeblock){
    //        i = pow2(32) -1;
    //    }
    //    freeblock[0] -= 3;
    //    cout << freeblock[0] << '\n'<< freeblock[1] << '\n';
    //    disk.seekp(0);
    //    disk.write((char *)&superblock,sizeof(superblock));
    //    disk.seekp(1024);
    //    disk.write((char *)freeblock,sizeof(freeblock));
    //    disk.close();
    //    return 0;
    //    format();
    //    INode temp{};
    //    temp.blocknum = 1;
    //    temp.blocknode[0] = 1;
    //    temp.fsize = 0;
    //    strcpy(temp.mode, "0111111");
    //    strcpy(temp.owner, "Admin");
    //    strcpy(temp.time,"20211015");
    //    disk.seekp(14 * 1024 + 72);
    //    disk.write((char*)&temp, sizeof(temp));
    //    Dir t{};
    //    t.num = 2;
    //    strcpy(t.dir[0].filename, "..");
    //    t.dir[0].inodeindex = 1;
    //    strcpy(t.dir[1].filename, ".");
    //    t.dir[1].inodeindex = 1;
    //    strcpy(t.dir[2].filename, "usr");
    //    t.dir[2].inodeindex = 1;
    //    disk.seekp(87 * 1024 + 1024);
    //    disk.write((char*)&t, sizeof(t));
    //    disk.close();
    //    return 0;
    //    disk.open(FileName,ios::in|ios::out|ios::binary);
    //    cout << disk.is_open() <<'\n';
    //    INode t{};
    //    disk.seekg(14 * 1024);
    //    disk.read((char*)&inode[0], sizeof(inode[0]));
    //    cout << inode[0].blocknum <<'\n';
    //    cout << inode[0].blocknode[0] << '\n';
    //    return 0;
    //    Dir tt{};
    //    disk.seekg(87*1024);
    //    disk.read((char*)&tt,sizeof(tt));
    //    cout << tt.num << '\n';
    //    cout << tt.dir[0].filename <<'\n';
    //    cout << tt.dir[1].filename <<'\n';
    //    cout << tt.dir[2].filename <<'\n';
    //    disk.close();
    //    return 0;
    readsuperblock();
    readbitmap();
    readinode();
    readrootdir();
    cout << nowdirname << ">";
    string in;
    stringstream input;
    while (getline(cin, in)) {
        input.clear();
        input.str(in);
        string order;
        input >> order;
        if (order == "exit" || order == "EXIT") {
            save();
            disk.close();
            return 0;
        }
        if (order == "ls") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            ls(temp);
            goto End;
        }
        if (order == "md") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            md(temp);
            goto End;
        }
        if (order == "cd") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            cd(temp);
            goto End;
        }
        if (order == "rd") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            rd(temp);
            goto End;
        }
        if (order == "mf") {
            vector<string> temp;
            string s;
            input >> s;
            temp.push_back(s);
            string content;
            string t;
            while (input >> t) {
                content.append(t + " ");
            }
            content = content.substr(0, content.length() - 1);
            temp.push_back(content);
            mf(temp);
            goto End;
        }
        if (order == "cat") {
            vector<string> temp;
            string s;
            string content;
            while (input >> s) {
                temp.push_back(s);
            }
            cat(temp, content);
            cout << content << '\n';
            goto End;
        }
        if (order == "del") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            del(temp);
            goto End;
        }
        if (order == "copy") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            copy(temp);
            goto End;
        }

        cout << "Error: Undefined instruction\n";
    End:
        cout << nowdirname << ">";
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
