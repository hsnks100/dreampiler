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

using namespace std;

extern KParser kparser;

int main(int argc, char **argv) {
    std::istringstream is(R"(
    var ab ;
    func main() {
        ab = 1 * 2 + 3 / 1 + 33 + 100;
    }
    func sub1(a, b, c) {
        ab = a + b + c;
    }
    func sub2(a) {
        sub1(2, 3, 4);
        ab = a;
    }
    func sub3() {
        ab = 3;
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
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    // cout << m_scanner.get_next_token() << endl;
    return 0;
}
