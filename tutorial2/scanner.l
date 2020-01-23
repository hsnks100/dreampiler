%{
#include <iostream>
#include <cstdlib>
#include "scanner.h"
using namespace std;
#define yyterminate() 0;
%}
%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option yylineno
%%
[a-z][a-z0-9]* { 
    m_kparser.m_tokens.emplace_back(yytext, TokenType::id, yylineno);
    return 11;
} 

[0-9][0-9]* {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::integer, yylineno);
    return 12;
    // cout << "Scanner: decimal number: " << yytext << endl;
    // uint64_t number = strtoull(yytext, 0, 10);
}
 

\+ {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::plus, yylineno);
    return 12;
}
- {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::minus, yylineno);
    return 12;
}
\* {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::mult, yylineno);
    return 12;
}
\/ {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::devide, yylineno);
    return 12;
}
\{ { 
    m_kparser.m_tokens.emplace_back(yytext, TokenType::lbracket, yylineno);
    return 13;
}

\} { 
    m_kparser.m_tokens.emplace_back(yytext, TokenType::rbracket, yylineno);
    return 13;
}
; {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::semicolon, yylineno);
    return 12;
}

\|\| {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::logical_or, yylineno);
    return 12;
}

&& {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::logical_and, yylineno);
    return 12;
}

!= {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::not_equal, yylineno);
    return 12;
}

== {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::equal, yylineno);
    return 12;
}

\( {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::lparen, yylineno);
    return 12;
}

\) {
    m_kparser.m_tokens.emplace_back(yytext, TokenType::rparen, yylineno);
    return 12;
}

[\n\t ] {
} 
. { 
} 


<<EOF>> { 
    return yyterminate(); 
} 
%%
