/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

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
    func mult(x, y, zadder) {
        sum := 0;
        j := y;
        while j != 0 { 
            sum = sum + x;
            j = j - 1;
        }
        return sum + zadder;
    }
    func main(a, b) {
        mult(4, 7, 100);
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
    int l = 0;
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
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    return 0;
}
