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
    modular,
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
    std::string sLabel = "SLABEL";
    std::string eLabel = "EkhBEL";
    int level = 0;
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
        std::vector<std::pair<int, std::string>> m_tree; ///< 
        std::vector<Token> m_tokens; ///< 토큰화된 소스가 담기는 곳
        std::vector<std::vector<std::string>> m_il; ///< 파싱이 진행되면서 중간 생성코드가 여기에 저장된다.
        std::vector<std::string> m_errorMsgs; ///< 파싱이 실패했을 때, 에러메시지가 담긴다.  
        std::map<std::string, int> m_funcToParams; ///< 어떤 함수가 몇 개의 인자가 있는지 
        std::map<std::string, int> m_varToLocal; ///< 어떤 변수가 몇 번째 지역변수인지 저장
        int m_increaseLocals = 0; ///< 함수에서 지역변수가 선언이 될 때 증가함. 새로운 함수 진입시 0 으로 초기화.
        std::map<std::string, int> m_varToArg; ///< 해당 변수가 몇 번째 argument 인지 저장하는 테이블.
        int m_increaseArgs = 0; ///< m_varToArg 보조하기 위한 변수

        int m_parameters = 0;///< m_varToArg 보조하기 위한 변수
        int m_locals = 0;
        std::vector<std::vector<std::string>> m_codes;
        void addCode(std::string op, std::string opr1, std::string opr2) {
            m_codes.push_back(
                {op, opr1, opr2} 
                );
        }
        void parse() ;
        void addErrorString(int l, const std::string& a);

        /**
         * @brief 파싱의 시작 부분. 
         *
         * @param begin 파싱이 시작되는 위치
         * @param end
         * @param bi 파싱시에 전달되는 정보들
         *
         * @return 
         */
        int _root(int begin, int end, BlockInfo& bi); 
        int _external(int begin, int end, BlockInfo& bi); 
        /**
         * @brief 함수파싱의 시작 부분. 
         *
         * @param begin 파싱이 시작되는 위치
         * @param end
         * @param bi 파싱시에 전달되는 정보들
         *
         * @return 
         */
        int _func(int begin, int end, BlockInfo bi); 
        int _parameters(int begin, int end, BlockInfo bi); 
        int _var(int begin, int end, BlockInfo bi); 
        int _statement(int begin, int end, BlockInfo bi);

        int _call(int begin, int end, BlockInfo bi); 
        int _return(int begin, int end, BlockInfo bi); 
        int _assign(int begin, int end, BlockInfo bi);
        int _decassign(int begin, int end, BlockInfo bi); 
        int _expr(int begin, int end, BlockInfo bi);
        int _simple_expr(int begin, int end, BlockInfo bi);
        int _adv_expr(int begin, int end, BlockInfo bi);
        int _or_expr(int begin, int end, BlockInfo bi);
        int _and_expr(int begin, int end, BlockInfo bi);
        int _cmp_expr(int begin, int end, BlockInfo bi);
        int _add_expr(int begin, int end, BlockInfo bi);
        int _mul_expr(int begin, int end, BlockInfo bi);
        int _mul_op(int begin, int end, BlockInfo bi);
        int _sign_expr(int begin, int end, BlockInfo bi);
        int _factor_expr(int begin, int end, BlockInfo bi); 
        int _if(int begin, int end,  BlockInfo bi);
        int _block(int begin, int end,  BlockInfo bi);
        int _while(int begin, int end,  BlockInfo bi);
        int _break(int begin, int end,  BlockInfo bi);
        int _continue(int begin, int end,  BlockInfo bi);
};
