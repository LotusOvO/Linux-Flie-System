//
// Created by 62622 on 2021/10/20.
//

#ifndef OS_CHECK_H
#define OS_CHECK_H
#include "header.h"

void check(vector<string> args){
    if(args.size() == 0){
        cout << "Check over!\n";
    }else{
        cout << "Error: Undefined instruction\n";
        return;
    }
}
#endif //OS_CHECK_H
