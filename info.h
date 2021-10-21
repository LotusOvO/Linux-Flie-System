//
// Created by 62622 on 2021/10/20.
//

#ifndef OS_INFO_H
#define OS_INFO_H
#include "header.h"

void info(vector<string> args){
    if(args.size() == 0){
        outss << "Maximum memory: 100M\n";
        outss << "Free memory : ";
        outss << superblock.freeblocknum <<"K\n";
        outss << "Instructions:\n";
        outss << "ls (<dir>) (-a|-l|-la)\n";
        outss << "cd (<dir>)\n";
        outss << "md <dir>\n";
        outss << "rd <dir>\n";
        outss << "mf <dir/filename> (content)\n";
        outss << "cat <dir/filename>\n";
        outss << "del <dir/filename>\n";
        outss << "copy (@)<dir/filename> (@)<dir/filename>\n";
        outss << "check\n";
        outss << "exit\n";

    }
    else{
        outss << "Error: Undefined instruction\n";
    }
}

#endif //OS_INFO_H
