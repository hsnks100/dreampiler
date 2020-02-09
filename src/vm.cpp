#include <iostream>
#include <sstream>
#include "vm.h"

using namespace std;

int driver() {
    return 0;
}

// 스택번지 | 값
// 1000 | 100
// 1001 | 200
// 1002 | 돌아갈 주소
// 1003 | 이전 localPointer
// 1004 | 이전 argPointer
// 1005 | 0
// 1006 | 0
//
// argPointer = 1000
// localPointer = 1005
//
