#pragma once

#include <unistd.h> 
#include <string>
#include <vector>
#include <map>

enum class TokenType {
    none,
    _var,
    _func,
    integer,
    id,
    assign,
    decassign,
    lbracket,
    rbracket,
    semicolon,
    logical_or,
    logical_and,
    equal,
    not_equal,
    less_than, // <
    less_than_equal, // <=
    greater_than, // >
    greater_than_equal,  // >=
    plus,
    minus,
    mult,
    devide,
    lparen,
    rparen,
    comma,
    _if,
    _while,
    _break,
    _continue,
    _return,
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
};

class KParser {
    public:
        std::vector<Token> m_tokens; ///< 토큰화된 소스가 담기는 곳
        std::vector<std::string> m_errorMsgs; ///< 파싱이 실패했을 때, 에러메시지가 담긴다.  
        std::vector<std::string> m_totalCode; ///< 파싱이 진행되면서 중간 생성코드가 여기에 저장된다.
        void parse() ;

        /**
         * @brief 파싱의 시작 부분. 
         *
         * @param begin 파싱이 시작되는 위치
         * @param end
         * @param bi 파싱시에 전달되는 정보들
         *
         * @return 
         */
        int _root(int begin, int end, BlockInfo bi); 
        int _add_expr(int begin, int end,  BlockInfo bi);
        int _mul_expr(int begin, int end,  BlockInfo bi);
        int _mul_op(int begin, int end,  BlockInfo bi);
        int _sign_expr(int begin, int end,  BlockInfo bi);
        int _factor_expr(int begin, int end,  BlockInfo bi);
};
