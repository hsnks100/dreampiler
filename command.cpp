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

#include "command.h"

#include <iostream>
#include <sstream>

using namespace EzAquarii;
using std::cout;
using std::endl;

Command::Command(const std::string &name, const std::vector<uint64_t> arguments) :
    m_name(name),
    m_args(arguments)
{
}

Command::Command(const std::string &name) :
    m_name(name),
    m_args()
{
}

Command::Command() :
    m_name(),
    m_args()
{
}

Command::~Command()
{
}
    
std::string Command::str() const {
    std::stringstream ts;
    ts << "name = [" << m_name << "], ";
    ts << "arguments = [";
    
    for(int i = 0; i < m_args.size(); i++) {
        ts << m_args[i];
        if(i < m_args.size() - 1) {
            ts << ", ";
        }
    }
    
    ts << "]";
    return ts.str();
}

std::string Command::name() const {
    return m_name;
}