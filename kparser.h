#pragma once

#include <string>
#include <vector>
#include <unistd.h>


enum TokenType {
    none,
    integer,
    id,
    assign,
    lbracket,
    rbracket,
    semicolon,
    equal,
    eof,
    _if,
    _while 
};
struct Token {
    std::string str;
    TokenType tokenType;
    int line;
    Token(std::string s, TokenType tt, int l = 0) {
        str = s;
        tokenType = tt;
        line = l;
    }
};

class KParser {
    public:
        std::vector<std::pair<int, std::string>> m_tree;
        std::vector<Token> m_tokens;
        void parse() ;
        int _statement(int begin, int end, int level) ;
        // int _expr(int begin, int end, int level);
        // int _binary_operator(int beign, int end, int level);

        int _expr(int begin, int end, int level) ;
        int _equal(int begin, int end, int level) ;
        int _if(int begin, int end, int level) ;
        int _block(int begin, int end, int level) ;
        int _while(int begin, int end, int level) ;
        int _assign(int begin, int end, int level) ;
};
