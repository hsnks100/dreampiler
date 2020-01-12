#pragma once

#include <unistd.h> 
#include <string>
#include <vector>

enum class TokenType {
    none,
    integer,
    id,
    assign,
    lbracket,
    rbracket,
    semicolon,
    logical_or,
    logical_and,
    equal,
    not_equal,
    plus,
    minus,
    mult,
    devide,
    lparen,
    rparen,
    _if,
    _while,
    _break,
    _continue,
    eof
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



struct BlockInfo {
    std::string sLabel = "SLABEL";
    std::string eLabel = "EkhBEL";
    int level = 0;
    BlockInfo(std::string a, std::string b, int c) {
        sLabel = a;
        eLabel = b;
        level = c;
    }
};
enum class Instruction { 
    INST_PUSH = 0x00, 
    INST_POP = 0x01,
    INST_SET_AGILITY = 0x02, 
    INST_PLAY_SOUND = 0x03,
    INST_SPAWN_PARTICLES = 0x04
};


class KParser {
    public:
        std::vector<std::pair<int, std::string>> m_tree;
        std::vector<Token> m_tokens;
        std::vector<std::string> m_il;
        std::vector<std::string> m_errorMsgs;
        void parse() ;
        int _statement(int begin, int end, const BlockInfo& bi);

        int _expr(int begin, int end, const BlockInfo& bi);
        int _adv_expr(int begin, int end, const BlockInfo& bi);
        int _or_expr(int begin, int end, const BlockInfo& bi);
        int _and_expr(int begin, int end, const BlockInfo& bi);
        int _cmp_expr(int begin, int end, const BlockInfo& bi);
        int _add_expr(int begin, int end, const BlockInfo& bi);
        int _mul_expr(int begin, int end, const BlockInfo& bi);
        int _mul_op(int begin, int end, const BlockInfo& bi);
        int _sign_expr(int begin, int end, const BlockInfo& bi);
        int _factor_expr(int begin, int end, const BlockInfo& bi);



            int _simple_expr(int begin, int end, const BlockInfo& bi);
            int _term(int begin, int end, const BlockInfo& bi);
            int _factor(int begin, int end, const BlockInfo& bi);
            int _equal(int begin, int end, const BlockInfo& bi);
            int _binary_operator(int beign, int end, const BlockInfo& bi);

        int _if(int begin, int end, const BlockInfo& bi);
        int _block(int begin, int end, const BlockInfo& bi);
        int _while(int begin, int end, const BlockInfo& bi);
        int _break(int begin, int end, const BlockInfo& bi);
        int _assign(int begin, int end, const BlockInfo& bi);
};
