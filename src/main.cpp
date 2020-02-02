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
    func fibo(n) {
        if n <= 2 {
            return 1;
        }
        return fibo(n-1) + fibo(n-2);
    } 
    func loop(n) {
        sum := 0;
        while n > 0 {
            sum = sum + n;
            n = n - 1;
        }
        return sum;
    }
    func threesum(a, b, c) {
        return a + b + c;
    }
    func breaktest() {
        i := 0;
        tot := 0;
        while 1 {
            i = i + 1;
            tot = tot + i;
            print(tot);
            if tot > 50 {
                break;
            }
        }
        return tot;
    }
    func continuetest() {
        i := 0;
        while i < 100 {
            i = i + 1;
            if i % 2 != 0 {
                continue;
            }
            print(i);
        }
        return 0;
    }
    func main() {
        g := breaktest();
        print(g);
        continuetest();
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
    return 0;
}
