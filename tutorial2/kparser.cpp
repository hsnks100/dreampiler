
#include "kparser.h"
#include <iostream>
void KParser::parse() {
    printf("tokens size() = %d\n", m_tokens.size());
    BlockInfo bi;
    _root(0, m_tokens.size(), bi);


    for(auto i=m_errorMsgs.rbegin(); i != m_errorMsgs.rend(); i++) {
        printf("%s", i->c_str());
    }
    int tt = 0;

    std::vector<int> calc;
    for(auto i: m_totalCode) {
        // tt++;
        // printf("eip[%3d]: %s\n", tt, i.c_str());
        if (i == "add") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(a + b);
        } else if (i == "mult") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(a * b);
        } else if (i == "div") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(b / a);
        } else if (i == "sub") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(b - a);
        } else if (i == ";") {
            printf("result %d\n", calc[0]);
            calc.clear();
        } else if (i == "sign") {
            int a;
            a = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(-a);
        } else {
            calc.push_back(std::stoi(i));
        }

    } 
}

int KParser::_root(int begin, int end, BlockInfo bi) {
    int i = begin;
    int t = 0; 
    while (i < end) {
        t = _add_expr(i, end, bi);
        if (t < 0) {
            return -1;
        }
        i = t;
        if (m_tokens[i].tokenType != TokenType::semicolon) {
            return -1;
        }
        m_totalCode.push_back(";");
        i++;
    }
    return i; 
}

int KParser::_add_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _mul_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::plus && tokenValue != TokenType::minus) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _mul_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        if(tokenValue == TokenType::plus) {
            m_totalCode.push_back("add");
        } else if(tokenValue == TokenType::minus) {
            m_totalCode.push_back("sub");
        }
        i = t; 
    } 
    return i;
}
int KParser::_mul_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    std::string leftValue = m_tokens[i].str;
    int t = _sign_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        t = _mul_op(tryI, end, bi);
        if(t < 0) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _sign_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        if(tokenValue == TokenType::mult) {
            m_totalCode.push_back("mult");
        } else if(tokenValue == TokenType::devide) {
            m_totalCode.push_back("div");
        }
        i = t; 
    } 
    return i;
}

int KParser::_mul_op(int begin, int end, BlockInfo bi) {
    int i = begin;
    if (m_tokens[i].tokenType != TokenType::mult && m_tokens[i].tokenType != TokenType::devide) {
        return -1;
    }
    i++;
    return i;
}
int KParser::_sign_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    bool isMinus = false;
    if(m_tokens[i].tokenType == TokenType::minus) {
        isMinus = true;
        i++;
    } 
    int t = _factor_expr(i, end, bi);
    if(t < 0) {
        return -1;
    }
    if (isMinus) {
        m_totalCode.push_back("sign");
    }
    return t;
}
int KParser::_factor_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    if(tokenType == TokenType::integer) {
        m_totalCode.push_back(m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::lparen) {
        int tryI = i + 1;
        int t = _add_expr(tryI, end, bi);
        if(t < 0) {
            char buf[256];
            sprintf(buf, "line[%d]: 표현식의 오류.\n", m_tokens[tryI].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        if(m_tokens[t].tokenType != TokenType::rparen) {
            char buf[256];
            sprintf(buf, "line[%d]: 닫는 괄호가 빠졌습니다.\n", m_tokens[tryI].line);
            m_errorMsgs.push_back(buf);
            return -1; 
        }
        i = t + 1;
    } 
    else {
        return -1;
    } 
    return i;
}

