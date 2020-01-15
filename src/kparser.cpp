
#include "kparser.h"
void KParser::parse() {
    printf("tokens size() = %d\n", m_tokens.size());
    BlockInfo bi("", "", 0);
    _root(0, m_tokens.size(), bi);


    for(auto i=m_errorMsgs.rbegin(); i != m_errorMsgs.rend(); i++) {
        printf("%s", i->c_str());
    }
    int tt = 0;
    for(auto i: m_il) {
        tt++;
        printf("eip[%d]: %s\n", tt, i.c_str());
    }

}

int KParser::_root(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;

    while (i < end) {
        t = _external(i, end, bi);
        i = t;
        if (t < 0) {
            break;
        }
        i = t;
    }
    return i; 
}
int KParser::_external(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType == TokenType::eof) {
        i++;
        return i;
    }

    t = _func(i, end, bi); 
    if (t >= 0) {
        return t;
    }

    t = _var(i, end, bi); 
    if (t >= 0) {
        return t;
    }
    char buf[256];
    sprintf(buf, "line[%d]: 함수 선언이나 변수 선언이 와야 합니다.\n", m_tokens[i].line);
    m_errorMsgs.push_back(buf);
    return -1;
}
int KParser::_func(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType != TokenType::_func) {
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    }
    m_il.push_back("func " + m_tokens[i].str);
    i++;

    t = _parameters(i, end, bi);
    if (t < 0) {
        char buf[256];
        sprintf(buf, "line[%d]: 함수 인자 정의가 잘못됐습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i = t;

    t = _block(i, end, bi);
    if (t < 0) {
        char buf[256];
        sprintf(buf, "line[%d]: 함수 몸체 정의가 잘못됐습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i = t;
    m_il.push_back("end func");
    return i;
}
int KParser::_parameters(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;
    if (m_tokens[i].tokenType != TokenType::lparen) {
        char buf[256];
        sprintf(buf, "line[%d]: ( 가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType == TokenType::rparen) {
        i++;
        return i;
    }
    // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
    if (m_tokens[i].tokenType != TokenType::id) {
        char buf[256];
        sprintf(buf, "line[%d]: 함수 인자에는 식별자가 와야합니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;

    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::comma) {
            break;
        }
        tryI++;
        if (m_tokens[tryI].tokenType != TokenType::id) {
            char buf[256];
            sprintf(buf, "line[%d]: , 뒤에 식별자를 기대했습니다.\n", m_tokens[i].line);
            m_errorMsgs.push_back(buf);
            return -1; 
        }
        tryI++;
        i = tryI;
    } 
    if (m_tokens[i].tokenType != TokenType::rparen) {
        char buf[256];
        sprintf(buf, "line[%d]: ) 가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    return i;
}
int KParser::_var(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType != TokenType::_var) {
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType != TokenType::id) {
        char buf[256];
        sprintf(buf, "line[%d]: 식별자가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        char buf[256];
        sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    return i;
}
int KParser::_statement(int begin, int end, const BlockInfo& bi) {
    // sleep(1);
    int i = begin;
    int t = 0;
    if((t = _if(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _while(i, end, bi)) >= 0) {
        i = t;
    }else if((t = _assign(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _block(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _break(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _call(i, end, bi)) >= 0) {
        i = t;
    }
    else {
        return -1;
    }
    return i;
}

int KParser::_call(int begin, int end, const BlockInfo& bi) {
    // some();
    // some(a);
    // some(a,b,c);
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen && 
        m_tokens[i + 2].tokenType == TokenType::rparen && 
        m_tokens[i + 3].tokenType == TokenType::semicolon
        ) {
        m_il.push_back("call " + m_tokens[i].str);
        return i + 4;
    }

    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen) {
        i = i + 2;
        // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
        t = _expr(i, end, bi);
        if (t < 0) {
            char buf[256];
            sprintf(buf, "line[%d]: 함수 인자에는 표현식이 와야합니다. %s\n", m_tokens[i].line, m_tokens[i].str.c_str());
            m_errorMsgs.push_back(buf);
            return -1;
        }
        i = t;

        while (1) {
            int tryI = i;
            std::string rightValue = m_tokens[tryI].str;
            TokenType tokenValue = m_tokens[tryI].tokenType;
            if(tokenValue != TokenType::comma) {
                break;
            }
            tryI++;
            t = _expr(tryI, end, bi);
            if (t < 0) {
                char buf[256];
                sprintf(buf, "line[%d]: , 뒤에 식별자를 기대했습니다.\n", m_tokens[i].line);
                m_errorMsgs.push_back(buf);
                return -1; 
            }
            tryI = t;
            i = tryI;
        } 
        if (m_tokens[i].tokenType != TokenType::rparen) {
            char buf[256];
            sprintf(buf, "line[%d]: ) 가 빠졌습니다.\n", m_tokens[i].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        i++;
        if (m_tokens[i].tokenType != TokenType::semicolon) {
            char buf[256];
            sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        i++;
        m_il.push_back("call " + m_tokens[begin].str);
        return i;
    }

    return -1; 
}

int KParser::_expr(int begin, int end, const BlockInfo& bi) {
    return _adv_expr(begin, end, bi);
    // int i = begin;
    // int t;
    // t = _simple_expr(i, end, bi);
    // if (t < 0) {
    //     return -1;
    // }
    // i = t;
    // while (1) {
    //     int tryI = i;
    //     TokenType tokenType = m_tokens[tryI].tokenType;
    //     if(tokenType != TokenType::equal) {
    //         break; 
    //     }
    //     tryI++;
    //     t = _simple_expr(tryI, end, bi);
    //     if (t < 0) {
    //         break;
    //     }
    //     m_il.push_back("call eq ");
    //     i = t; 
    // } 
    // return i;
}

int KParser::_adv_expr(int begin, int end, const BlockInfo& bi) {
    return _or_expr(begin, end, bi);
}
int KParser::_or_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _and_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::logical_or) {
            break;
        }
        tryI++;
        t = _and_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
    // printf("rrrrrrrrrrqweqwe\n");
    // exit(-1);
        // m_il.push_back(leftValue);
        // m_il.push_back(rightValue);
        m_il.push_back("call ||");
        i = t; 
    } 
    return i;
}
int KParser::_and_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _cmp_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::logical_and) {
            break;
        }
        tryI++;
        t = _cmp_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        // m_il.push_back(leftValue);
        // m_il.push_back(rightValue);
        m_il.push_back("call &&");
        i = t; 
    } 
    return i;
}
int KParser::_cmp_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _add_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;

    do {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::equal && tokenValue != TokenType::not_equal) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _add_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        // m_il.push_back(leftValue);
        // m_il.push_back(rightValue);
        if(tokenValue == TokenType::equal) {
            m_il.push_back("call ==");
        } else if(tokenValue == TokenType::not_equal) {
            m_il.push_back("call !=");
        }
        i = t; 
    } while(0);
    return i;
}
int KParser::_add_expr(int begin, int end, const BlockInfo& bi) {
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
        // m_il.push_back(leftValue);
        // m_il.push_back(rightValue);
        if(tokenValue == TokenType::plus) {
            m_il.push_back("call +");
        } else if(tokenValue == TokenType::minus) {
            m_il.push_back("call -");
        }
        i = t; 
    } 
    return i;
}
int KParser::_mul_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _sign_expr(i, end, bi);
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
        // m_il.push_back(leftValue);
        // m_il.push_back(rightValue);
        if(tokenValue == TokenType::mult) {
            m_il.push_back("call *");
        } else if(tokenValue == TokenType::devide) {
            m_il.push_back("call /");
        }
        i = t; 
    } 
    return i;
}

int KParser::_mul_op(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    if (m_tokens[i].tokenType != TokenType::mult && m_tokens[i].tokenType != TokenType::devide) {
        return -1;
    }
    i++;
    return i;
}
int KParser::_sign_expr(int begin, int end, const BlockInfo& bi) {
    int t;
    int i = begin;
    // if(m_tokens[i].tokenType == TokenType::plus || m_tokens[i].tokenType == TokenType::minus) {
    if(m_tokens[i].tokenType == TokenType::minus) {
        i++;
    }
    
    t = _factor_expr(i, end, bi);
    if(t < 0) {
        return -1;
    }
    i = t;
    return i;
}
int KParser::_factor_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    if(tokenType == TokenType::id) {
        m_il.push_back("push " + m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::integer) {
        m_il.push_back("push " + m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::lparen) {
        int tryI = i;
        tryI++;
        int t = _expr(tryI, end, bi);
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
        t++;
        i = t;
    } 
    else {
        return -1;
    }

    return i;
}


int KParser::_simple_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    if(m_tokens[i].tokenType == TokenType::plus || m_tokens[i].tokenType == TokenType::minus) {
        i++;
    }
    t = _term(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        TokenType tokenType = m_tokens[tryI].tokenType;
        if(tokenType != TokenType::plus && tokenType != TokenType::minus) {
            break; 
        }
        t = _term(tryI + 1, end, bi);
        if (t < 0) {
            break;
        }
        i = t;
        m_il.push_back("call " + m_tokens[tryI].str); 
    }
    return i;
}
int KParser::_term(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    if((t = _factor(i, end, bi)) < 0) {
        return -1;
    }
    i = t;

    while(1) {
        int tryI = i;
        TokenType tokenType = m_tokens[tryI].tokenType;
        if(tokenType != TokenType::mult && tokenType != TokenType::devide) {
            break; 
        }
        t = _factor(tryI + 1, end, bi);
        if (t < 0) {
            break;
        }
        i = t; 
        m_il.push_back("call " + m_tokens[tryI].str); 
    }
    return i;

}
int KParser::_factor(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    if(tokenType == TokenType::id) {
        m_il.push_back("push " + m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::integer) {
        m_il.push_back("push " + m_tokens[i].str);
        i++;
    } else {
        return -1;
    }

    return i;
}
int KParser::_equal(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    int idIndex = i;
    if( m_tokens[i].tokenType != TokenType::id ) {
        return -1;
    }
    i++;

    if( m_tokens[i].tokenType != TokenType::equal ) {
        return -1;
    }
    i++;

    int intIndex = i;
    if( m_tokens[i].tokenType != TokenType::integer ) {
        return -1;
    }
    i++;
    m_il.push_back("..eq push " + m_tokens[idIndex].str);
    m_il.push_back("..eq push " + m_tokens[intIndex].str);
    m_il.push_back("call eq");
    return i; 
}
int KParser::_if(int begin, int end, const BlockInfo& bi) {
    std::string sLabel = "LABEL" + std::to_string(rand() % 1000);
    std::string eLabel = "LABEL" + std::to_string(rand() % 1000);
    BlockInfo newBi{sLabel, eLabel, bi.level};
    int i = begin;
    int t = 0;
    if( m_tokens[i].tokenType != TokenType::_if ) {
        return -1;
    }
    i++;

    if((t = _expr(i, end, newBi)) < 0) {
        return -1;
    } else {
        i = t;
    } 

    m_il.push_back("jne " + eLabel);
    if((t = _block(i, end, newBi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    m_il.push_back(eLabel);
    return i;
}
int KParser::_block(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    if(m_tokens[i].tokenType != TokenType::lbracket) {
        return -1;
    }
    i++;
    while(1) {
        if(m_tokens[i].tokenType == TokenType::rbracket) {
            i++;
            break;
        }
        t = _statement(i, end, bi);
        if (t < 0) {
            char buf[256];
            sprintf(buf, "line[%d]: 문장이 아닙니다.\n", m_tokens[i].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        i = t; 
    }
    return i; 
}
int KParser::_while(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t = 0;

    std::string sLabel = "LABEL" + std::to_string(rand() % 1000);
    std::string eLabel = "LABEL" + std::to_string(rand() % 1000);
    BlockInfo newBi{sLabel, eLabel, bi.level};
    if( m_tokens[i].tokenType != TokenType::_while ) {
        return -1;
    }
    i++; 
    m_il.push_back(sLabel);
    if((t = _expr(i, end, newBi)) < 0) {
        return -1;
    } else {
        i = t;
    } 
    m_il.push_back("jne " + eLabel);


    if((t = _block(i, end, newBi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    m_il.push_back("jmp " + sLabel);
    m_il.push_back(eLabel);
    return i;
}
int KParser::_break(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;

    if (m_tokens[i].tokenType != TokenType::_break) {
        return -1;
    } 
    i++;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        return -1;
    }
    i++;
    m_il.push_back("jmp " + bi.eLabel);
    return i;
}
int KParser::_assign(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    int idIndex = i;
    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    } 
    i++; 
    if (m_tokens[i].tokenType != TokenType::assign) {
        printf("대입문을 기대했습니다.\n");
        return -1;
    } 
    i++; 
    m_il.push_back("push addr(" + m_tokens[idIndex].str + ")");

    int exprIndex = i;
    t = _expr(i, end, bi);
    if (t < 0) {
        char buf[256];
        sprintf(buf, "line[%d]: 표현식의 오류\n", m_tokens[i].line); // 대입문을 기대했습니다.\n");
        m_errorMsgs.push_back(buf);
        return -1;
    } 
    i = t;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        char buf[256];
        sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line); // 대입문을 기대했습니다.\n");
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    m_il.push_back("call assign");
    return i;
} 
