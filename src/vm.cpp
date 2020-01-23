#include <iostream>
#include <sstream>
#include "vm.h"

using namespace std;

int driver() {
    Vm vm;
    std::istringstream is(R"(push constant 7
push constant 4
call mult 2
exit
func mult 2
push constant 0
pop local 0 // sum
push argument 1 // y
pop local 1 // j = y
label LABEL440
push local 1
push constant 0
neq
jz LABEL165
push local 0 // sum
push argument 0 // x
add
pop local 0 // sum += x
push local 1 // j
push constant 1
sub
pop local 1 // j = j - 1
jmp LABEL440
label LABEL165
push local 0
return
    )"); 

    std::istringstream is2(R"(func mult 2
push constant 0
pop local 0
push argument 1
pop local 1
label LABEL440
push local 1
push constant 0
neq
jz LABEL165
push local 0
push argument 0
add
pop local 0
push local 1
push constant 1
sub
pop local 1
jmp LABEL440
label LABEL165
push local 0
push argument 2
add
return
func main 0
push constant 4
push constant 7
push constant 100
call mult 3
push constant 5
return 
)");
    vm.loadProgram(is2.str());

    while(1) {
        if(vm.step()) {
            break;
        } 
    }
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
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
