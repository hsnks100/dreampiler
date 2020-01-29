
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
    for(auto i: m_il) {
        std::string tot = "";
        std::string seper = "";
        for(auto j: i) {
            if (j != "") {
                tot += seper + j;
                seper = " "; 
            }
        }
        printf("eip[%3d]: %s\n", tt, tot.c_str());
        tt++;
    } 
}

void KParser::addErrorString(int l, const std::string& a) {
    char buf[256];
    sprintf(buf, "line[%d]: %s\n", m_tokens[l].line, a.c_str());
    m_errorMsgs.push_back(buf);
}
int KParser::_root(int begin, int end,  BlockInfo& bi) {
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
int KParser::_external(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType == TokenType::eof) {
        i++;
        return i;
    }

    t = _func(i, end, bi); 
    if (t >= 0) {
        m_errorMsgs.clear();
        return t;
    }

    t = _var(i, end, bi); 
    if (t >= 0) {
        return t;
    }
    addErrorString(i, "함수 선언이나 변수 선언이 와야 합니다.");
    return -1;
}
int KParser::_func(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t = 0; 
    if (m_tokens[i].tokenType != TokenType::_func) {
        return -1;
    }
    i++; 
    if (m_tokens[i].tokenType != TokenType::id) {
        addErrorString(i, "func 뒤에는 식별자가 와야합니다.");
        return -1;
    } 
    std::string funcName = m_tokens[i].str;
    bi.il.clear();
    bi.addCode("func", funcName, "");
    i++; 
    m_parameters = 0; 
    t = _parameters(i, end, bi);
    if (t < 0) {
        addErrorString(i, "함수인자 정의가 잘못 됐습니다.");
        return -1;
    }
    m_funcToParams[funcName] = m_parameters;
    i = t;

    m_locals = 0;
    m_increaseLocals = 0;
    t = _block(i, end, bi);
    if (t < 0) {
        addErrorString(i, "함수몸체 정의가 잘못 됐습니다.");
        return -1;
    }
    i = t;
    // m_funcToLocals[funcName] = m_locals;
    // bi.addCode("end", "func", ""); 
    bi.il[0][2] = std::to_string(m_locals); 
    for(auto& i: bi.il) {
        if(m_varToArg.find(i[1]) != m_varToArg.end()) {
            i[2] = std::to_string(m_varToArg[i[1]]);
            i[1] = "argument";
        }
        if(m_varToLocal.find(i[1]) != m_varToLocal.end()) {
            i[2] = std::to_string(m_varToLocal[i[1]]);
            i[1] = "local";
        }
    }

    m_il.insert(m_il.end(), bi.il.begin(), bi.il.end());
    return i;
}
int KParser::_parameters(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t = 0;
    if (m_tokens[i].tokenType != TokenType::lparen) {
        addErrorString(i, "( 가 빠졌습니다.");
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType == TokenType::rparen) {
        i++;
        return i;
    }
    // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
    int idIndex = i;
    if (m_tokens[idIndex].tokenType != TokenType::id) {
        addErrorString(idIndex, "함수 인자에는 식별자가 와야합니다.");
        return -1;
    }
    i++;
    m_varToArg[m_tokens[idIndex].str] = m_parameters;
    m_parameters++;

    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::comma) {
            break;
        }
        tryI++;
        if (m_tokens[tryI].tokenType != TokenType::id) {
            addErrorString(tryI, ", 뒤에 식별자를 기대했습니다.");
            return -1; 
        }
        m_varToArg[m_tokens[tryI].str] = m_parameters;
        m_parameters++;
        tryI++;
        i = tryI;
    } 
    if (m_tokens[i].tokenType != TokenType::rparen) {
        addErrorString(i, ", 뒤에 식별자를 기대했습니다.");
        return -1;
    }
    i++;
    return i;
}
int KParser::_var(int begin, int end,  BlockInfo& bi) {
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
int KParser::_statement(int begin, int end,  BlockInfo& bi) {
    // sleep(1);
    int i = begin;
    int t = 0;
    if((t = _if(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _while(i, end, bi)) >= 0) {
        i = t;
    }else if((t = _assign(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _decassign(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _block(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _break(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _simple_expr(i, end, bi)) >= 0) {
        i = t;
    } else if((t = _return(i, end, bi)) >= 0) {
        i = t;
    } else {
        return -1;
    }
    return i;
}

int KParser::_call(int begin, int end,  BlockInfo& bi) {
    // some();
    // some(a);
    // some(a,b,c);
    int i = begin;
    int t = 0;

    // 인자가 없는 경우 
    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen && 
        m_tokens[i + 2].tokenType == TokenType::rparen && 
        m_tokens[i + 3].tokenType == TokenType::semicolon
        ) {
        // bi.addCode("call " + m_tokens[i].str + " " + std::to_string( m_funcToParams[m_tokens[i].str]));
        bi.addCode("call", m_tokens[i].str,  std::to_string( m_funcToParams[m_tokens[i].str]));
        return i + 4;
    }

    // 인자가 하나이상 있는 경우
    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen) {
        i = i + 2;
        // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
        t = _expr(i, end, bi);
        if (t < 0) {
            addErrorString(i, "함수 인자에는 표현식이 와야합니다.");
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
                addErrorString(i, ", 뒤에 표현식을 기대했습니다.");
                return -1; 
            }
            tryI = t;
            i = tryI;
        } 
        if (m_tokens[i].tokenType != TokenType::rparen) {
            addErrorString(i, ") 가 빠졌습니다.");
            return -1;
        }
        i++;
        // if (m_tokens[i].tokenType != TokenType::semicolon) {
        //     char buf[256];
        //     sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line);
        //     m_errorMsgs.push_back(buf);
        //     return -1;
        // }
        // i++;
        bi.addCode("call", m_tokens[begin].str,  std::to_string( m_funcToParams[m_tokens[begin].str]));
        return i;
    }

    return -1; 
}

int KParser::_return(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType != TokenType::_return) {
        return -1;
    }
    i++;

    if ((t = _expr(i, end, bi)) < 0) {
        addErrorString(i, "표현식이 와야합니다.");
        return -1;
    } 
    i = t;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        addErrorString(i, "; 이 와야합니다.");
        return -1;
    }
    i++;
    bi.addCode("return", "", "");
    return i;
}

int KParser::_expr(int begin, int end,  BlockInfo& bi) {
    return _adv_expr(begin, end, bi);
}

int KParser::_simple_expr(int begin, int end,  BlockInfo& bi) {
    // <expr>;
    int i = begin;
    int t = 0;
    if ((t = _expr(i, end, bi)) < 0) {
        return -1;
    }
    i = t;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        addErrorString(i, "; 이 와야합니다.");
        return -1;
    }
    i++;
    // 대입문이 없으므로 빈 스택을 pop 해준다.
    bi.addCode("pop", "", ""); 
    return i;
}
int KParser::_adv_expr(int begin, int end,  BlockInfo& bi) {
    return _or_expr(begin, end, bi);
}

int KParser::_or_expr(int begin, int end,  BlockInfo& bi) {
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
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        bi.addCode("or", "", "");
        i = t; 
    } 
    return i;
}

int KParser::_and_expr(int begin, int end,  BlockInfo& bi) {
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
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        bi.addCode("and", "", "");
        i = t; 
    } 
    return i;
}

int KParser::_cmp_expr(int begin, int end,  BlockInfo& bi) {
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
        if(tokenValue != TokenType::equal 
           && tokenValue != TokenType::not_equal
           && tokenValue != TokenType::less_than
           && tokenValue != TokenType::less_than_equal
           && tokenValue != TokenType::greater_than
           && tokenValue != TokenType::greater_than_equal
           ) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _add_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        if(tokenValue == TokenType::equal) {
            bi.addCode("eq", "", "");
        } else if(tokenValue == TokenType::not_equal) {
            bi.addCode("neq", "", "");
        }else if(tokenValue == TokenType::less_than) {
            printf("lt!!\n");
            bi.addCode("lt", "", "");
        }else if(tokenValue == TokenType::less_than_equal) {
            printf("lte!!\n");
            bi.addCode("lte", "", "");
        }else if(tokenValue == TokenType::greater_than) {
            bi.addCode("gt", "", "");
        }else if(tokenValue == TokenType::greater_than_equal) {
            bi.addCode("gte", "", "");
        } 
        i = t; 
    } while(0);
    return i;
}
int KParser::_add_expr(int begin, int end,  BlockInfo& bi) {
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
            bi.addCode("add", "", "");
        } else if(tokenValue == TokenType::minus) {
            bi.addCode("sub", "", "");
        }
        i = t; 
    } 
    return i;
}
int KParser::_mul_expr(int begin, int end,  BlockInfo& bi) {
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
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        if(tokenValue == TokenType::mult) {
            bi.addCode("mult", "", "");
        } else if(tokenValue == TokenType::devide) {
            bi.addCode("div", "", "");
        }
        i = t; 
    } 
    return i;
}

int KParser::_mul_op(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    if (m_tokens[i].tokenType != TokenType::mult && m_tokens[i].tokenType != TokenType::devide) {
        return -1;
    }
    i++;
    return i;
}
int KParser::_sign_expr(int begin, int end,  BlockInfo& bi) {
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
        bi.addCode("neg", "", "");
    }
    return t;
}
int KParser::_factor_expr(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    int t = 0;

    if((t = _call(i, end, bi)) >= 0) {
        i = t;
    } else if(tokenType == TokenType::id) {
        bi.addCode("push", m_tokens[i].str, "");
        i++;
    } else if(tokenType == TokenType::integer) {
        bi.addCode("push", "constant", m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::lparen) {
        int tryI = i;
        tryI++;
        int t = _expr(tryI, end, bi);
        if(t < 0) {
            addErrorString(i, "표현식의 오류.");
            return -1;
        }
        if(m_tokens[t].tokenType != TokenType::rparen) {
            addErrorString(i, "닫는 괄호가 빠졌습니다.\n");
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


int KParser::_if(int begin, int end,  BlockInfo& bi) {
    std::string sLabel = "LABEL" + std::to_string(rand() % 1000);
    std::string eLabel = "LABEL" + std::to_string(rand() % 1000);
    bi.sLabel = sLabel;
    bi.eLabel = eLabel;
    // BlockInfo newBi = bi;
    // BlockInfo newBi{sLabel, eLabel, bi.level};
    int i = begin;
    int t = 0;
    if( m_tokens[i].tokenType != TokenType::_if ) {
        return -1;
    }
    i++;

    if((t = _expr(i, end, bi)) < 0) {
        return -1;
    } else {
        i = t;
    } 

    bi.addCode("jnz", eLabel, "");
    if((t = _block(i, end, bi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    bi.addCode("label", eLabel, "");
    return i;
}
int KParser::_block(int begin, int end,  BlockInfo& bi) {
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
int KParser::_while(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t = 0;

    std::string sLabel = "LABEL" + std::to_string(rand() % 1000);
    std::string eLabel = "LABEL" + std::to_string(rand() % 1000);
    bi.sLabel = sLabel;
    bi.eLabel = eLabel;
    // BlockInfo newBi{sLabel, eLabel, bi.level};
    if( m_tokens[i].tokenType != TokenType::_while ) {
        return -1;
    }
    i++; 
    bi.addCode("label", sLabel, "");
    if((t = _expr(i, end, bi)) < 0) {
        return -1;
    } else {
        i = t;
    } 
    bi.addCode("jz", eLabel, "");


    if((t = _block(i, end, bi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    bi.addCode("jmp", sLabel, "");
    bi.addCode("label", eLabel, "");
    return i;
}
int KParser::_break(int begin, int end,  BlockInfo& bi) {
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
    bi.addCode("jmp", bi.eLabel, "");
    return i;
}
int KParser::_assign(int begin, int end,  BlockInfo& bi) {
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
    bi.addCode("pop", m_tokens[idIndex].str, "");
    return i;
} 

int KParser::_decassign(int begin, int end,  BlockInfo& bi) {
    int i = begin;
    int t;
    int idIndex = i;
    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    } 
    i++; 
    if (m_tokens[i].tokenType != TokenType::decassign) {
        return -1;
    } 
    i++; 

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
    m_locals++;
    bi.addCode("pop", m_tokens[idIndex].str, "");
    m_varToLocal[m_tokens[idIndex].str] = m_increaseLocals++;
    return i;
}
