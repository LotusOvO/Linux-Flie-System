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
#include "info.h"
#include "check.h"
#include <cstring>
#include <iostream>
#include <sstream>
// 超级块（1k）| 数据块位图（13K）| i节点表（73k）| 根目录块（1k）| 数据块（102312k）

int main() {
    //初始化函数
//    format();
//    return 0;
    disk.open(FileName, ios::in | ios::out | ios::binary);
    cout << disk.is_open() << '\n';
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
        if (order == "info") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            info(temp);
            goto End;
        }
        if (order == "check") {
            vector<string> temp;
            string s;
            while (input >> s) {
                temp.push_back(s);
            }
            check(temp);
            goto End;
        }

        cout << "Error: Undefined instruction\n";
    End:
        cout << nowdirname << ">";
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
