#ifndef SCANNER_H
#define SCANNER_H

#if ! defined(yyFlexLexerOnce)
// #undef yyFlexLexer
// #define yyFlexLexer EzAquarii_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif

#include <string> 
class Scanner : public yyFlexLexer {
    public:
        Scanner(std::istream *in) : yyFlexLexer(in) {} 
        virtual int yylex(); 
};
#endif
