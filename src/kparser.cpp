
#include "kparser.h"
void KParser::parse() {
    printf("tokens size() = %d\n", m_tokens.size());
    _block(0, m_tokens.size(), BlockInfo("", "", 0));


    int tt = 0;
    for(auto i: m_il) {
        tt++;
        printf("eip[%d]: %s\n", tt, i.c_str());
    }
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
    } 

    else {
        return -1;
    }
    return i;
}

int KParser::_expr(int begin, int end, const BlockInfo& bi) {
    int i = begin;
    int t;
    t = _simple_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        TokenType tokenType = m_tokens[tryI].tokenType;
        if(tokenType != TokenType::equal) {
            break; 
        }
        tryI++;
        t = _simple_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        m_il.push_back("call eq ");
        i = t; 
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
            printf("line[%d]: 문장이 아닙니다.\n", m_tokens[i].line);
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
        return -1;
    } 
    i++; 
    m_il.push_back("push addr(" + m_tokens[idIndex].str + ")");

    int exprIndex = i;
    t = _expr(i, end, bi);
    if (t < 0) {
        return -1;
    } 
    i = t;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        return -1;
    }
    i++;
    m_il.push_back("call assign");
    return i;
}
