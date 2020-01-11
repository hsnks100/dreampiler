
#include "kparser.h"
void KParser::parse() {
    printf("tokens size() = %d\n", m_tokens.size());
    _block(0, m_tokens.size(), 0); 

    for(auto& i: m_tree) {
        for(int j=0; j<i.first; j++) {
            printf(" ");
        }
        printf("%s\n",  i.second.c_str());
    }
}
int KParser::_statement(int begin, int end, int level) {
    // sleep(1);
    printf("%s::%d\n", __FUNCTION__, begin);
    int i = begin;
    int t = 0;
    if((t = _if(i, end, level)) >= 0) {
        i = t;
    } else if((t = _while(i, end, level)) >= 0) {
        i = t;
    }else if((t = _assign(i, end, level)) >= 0) {
        i = t;
    } else if((t = _block(i, end, level)) >= 0) {
        i = t;
    } else {
        return -1;
    }
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i;
}

int KParser::_expr(int begin, int end, int level) {
    m_tree.push_back(std::make_pair(level, "_expr"));
    int i = begin;
    int t;
    if ((t = _equal(i, end, level)) > 0) {
        i = t;
    } else {
        m_tree.pop_back();
        return -1;
    } 
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i;
}
int KParser::_equal(int begin, int end, int level) {
    int i = begin;
    int t;
    if( m_tokens[i].tokenType != TokenType::id ) {
        return -1;
    }
    i++;

    if( m_tokens[i].tokenType != TokenType::equal ) {
        return -1;
    }
    i++;

    if( m_tokens[i].tokenType != TokenType::integer ) {
        return -1;
    }
    i++;
    m_tree.push_back(std::make_pair(level, "_eq"));
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i; 
}
int KParser::_if(int begin, int end, int level) {
    m_tree.push_back(std::make_pair(level, "_if"));
    int i = begin;
    int t = 0;
    if( m_tokens[i].tokenType != TokenType::_if ) {
        m_tree.pop_back();
        return -1;
    }
    i++;

    if((t = _expr(i, end, level)) < 0) {
        m_tree.pop_back();
        return -1;
    } else {
        i = t;
    } 

    if((t = _block(i, end, level)) < 0) {
        m_tree.pop_back();
        return -1;
    } else {
        i = t;
    }
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i;
}
int KParser::_block(int begin, int end, int level) {
    int i = begin;
    int t;
    if(m_tokens[i].tokenType != TokenType::lbracket) {
        return -1;
    }
    i++;
    while(1) {
        if(m_tokens[i].tokenType == TokenType::rbracket) {
            i++;
            printf("end block\n");
            break;
        }
        t = _statement(i, end, level + 1);
        if (t < 0) {
            printf("line[%d]: 문장이 아닙니다.\n", m_tokens[i].line);
            return -1;
        }
        i = t; 
    }
    return i; 
}
int KParser::_while(int begin, int end, int level) {
    // return -1;
    m_tree.push_back(std::make_pair(level, "_while"));
    int i = begin;
    int t = 0;
    if( m_tokens[i].tokenType != TokenType::_while ) {
        m_tree.pop_back();
        return -1;
    }
    i++;

    if((t = _expr(i, end, level)) < 0) {
        m_tree.pop_back();
        return -1;
    } else {
        i = t;
    } 

    if((t = _block(i, end, level)) < 0) {
        m_tree.pop_back();
        return -1;
    } else {
        i = t;
    }
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i;
}
int KParser::_assign(int begin, int end, int level) {
    int i = begin;
    int t;
    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    } 
    i++; 
    if (m_tokens[i].tokenType != TokenType::assign) {
        return -1;
    } 
    i++; 

    if (m_tokens[i].tokenType != TokenType::integer) {
        return -1;
    } 
    i++;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        return -1;
    } 
    i++; 
    m_tree.push_back(std::make_pair(level, "_assign"));
    printf("%s::%d [OK]\n", __FUNCTION__, begin);
    return i;
}
