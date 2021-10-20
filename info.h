//
// Created by 62622 on 2021/10/20.
//

#ifndef OS_INFO_H
#define OS_INFO_H
#include "header.h"

void info(vector<string> args){
    if(args.size() == 0){
        cout << "Maximum memory: 100M\n";
        cout << "Free memory : ";
        cout << superblock.freeblocknum <<"K\n";
        cout << "Instructions:\n";
        cout << "ls (<dir>) (-a|-l|-la)\n";
        cout << "cd (<dir>)\n";
        cout << "md <dir>\n";
        cout << "rd <dir>\n";
        cout << "mf <dir/filename> (content)\n";
        cout << "cat <dir/filename>\n";
        cout << "del <dir/filename>\n";
        cout << "copy (@)<dir/filename> (@)<dir/filename>\n";
        cout << "check\n";

    }
    else{
        cout << "Error: Undefined instruction\n";
    }
}

#endif //OS_INFO_H
