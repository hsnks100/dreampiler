#include <iostream>
#include <sstream>
#include "scanner.h"
#include "kparser.h"
#include "vm.h"

using namespace std;

extern KParser kparser;

int main(int argc, char **argv) {
    // driver();
    // std::cout << "---------\n";
    // return 0;
    std::istringstream is(R"(
    func fsum(until) { 
        i := 0;
        sum := 0;
        while i<=until {
            sum = sum + i;
            i = i + 1; 
        }
        return sum;
    }

    func main() {
        a := fsum(50);
        fsum(100);
        fsum(<<<);
        if 1 {
        }
        return 5;
    }
    )"); 
    Scanner m_scanner(&is);
    while(m_scanner.get_next_token()) {
    }

    // std::string seper = "";
    // for(auto& i: kparser.m_tokens) {
    //     // std::cout << seper << i.str << "(" << int(i.tokenType) << ")";
    //     std::cout << seper << i.str;
    //     seper = " ";
    // }

    std::istringstream whole;
    whole.str(is.str());
    std::string line;
    int l = 1;
    while (std::getline(whole, line)) {
        std::cout << "[" << l << "]" << line << std::endl;
        l++;
    }
    kparser.parse();
    Vm vm;
    for(auto i: kparser.m_il) {
        vm.addCommand(i);
    }
    vm.addCommand({"call", "main", "0"});
    vm.m_eip = vm.m_program.size() - 1;
    std::cout << "eip: " << vm.m_eip << std::endl;
    while(1) {
        if(vm.step()) {
            break;
        } 
    }
    std::vector<int> answer = {
        (200 + 12) * 34 + 56 - 100,
        200 + 12 * 34 + 56 - 100,
        200 + (12 * 34) + 56 - 100,
        -100,
        1 + -5,
        -(1 + -5),
        -(1 + -5)*4 + 100 * 3, 
        1 * -100,
        1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30)), 
        (1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30))), 
        (1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30))) * 3 - 100,
        100 - -100, 
        100 - (-100),
        100 - (-100 + 1 - 3 * 10 / 2 / 5 - 10 + 3838 - 1003 + 13) - 3 - 1-  3
    }; 
    for(auto i: answer) {
        printf("correct %d\n", i);
    }
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    return 0;
}
