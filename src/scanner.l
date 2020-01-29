%{
#include <iostream>
#include <cstdlib>
#include "scanner.h"
#include "kparser.h"

using namespace std;
#define yyterminate() 0;

KParser kparser;
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option yylineno

%%



var {
    kparser.m_tokens.emplace_back(yytext, TokenType::_var, yylineno);
}

func {
    kparser.m_tokens.emplace_back(yytext, TokenType::_func, yylineno);
}

, {
    kparser.m_tokens.emplace_back(yytext, TokenType::comma, yylineno);
}

if {
    kparser.m_tokens.emplace_back(yytext, TokenType::_if, yylineno);
    return 12;
} 

while {
    kparser.m_tokens.emplace_back(yytext, TokenType::_while, yylineno);
    return 12;
} 

break {
    kparser.m_tokens.emplace_back(yytext, TokenType::_break, yylineno);
    return 12;
}

continue {
    kparser.m_tokens.emplace_back(yytext, TokenType::_continue, yylineno);
    return 12;
}

return {
    kparser.m_tokens.emplace_back(yytext, TokenType::_return, yylineno);
    return 12; 
}

[a-z][a-z0-9]* { 
    kparser.m_tokens.emplace_back(yytext, TokenType::id, yylineno);
    return 11;
} 

[0-9][0-9]* {
    kparser.m_tokens.emplace_back(yytext, TokenType::integer, yylineno);
    return 12;
    // cout << "Scanner: decimal number: " << yytext << endl;
    // uint64_t number = strtoull(yytext, 0, 10);
}

= {
    kparser.m_tokens.emplace_back(yytext, TokenType::assign, yylineno);
    return 12;
}

:= {
    kparser.m_tokens.emplace_back(yytext, TokenType::decassign, yylineno);
    return 12;
}

\+ {
    kparser.m_tokens.emplace_back(yytext, TokenType::plus, yylineno);
    return 12;
}
- {
    kparser.m_tokens.emplace_back(yytext, TokenType::minus, yylineno);
    return 12;
}
\* {
    kparser.m_tokens.emplace_back(yytext, TokenType::mult, yylineno);
    return 12;
}
\/ {
    kparser.m_tokens.emplace_back(yytext, TokenType::devide, yylineno);
    return 12;
}
\{ { 
    kparser.m_tokens.emplace_back(yytext, TokenType::lbracket, yylineno);
    return 13;
}

\} { 
    kparser.m_tokens.emplace_back(yytext, TokenType::rbracket, yylineno);
    return 13;
}
; {
    kparser.m_tokens.emplace_back(yytext, TokenType::semicolon, yylineno);
    return 12;
}

\|\| {
    kparser.m_tokens.emplace_back(yytext, TokenType::logical_or, yylineno);
    return 12;
}

&& {
    kparser.m_tokens.emplace_back(yytext, TokenType::logical_and, yylineno);
    return 12;
}

!= {
    kparser.m_tokens.emplace_back(yytext, TokenType::not_equal, yylineno);
    return 12;
}

== {
    kparser.m_tokens.emplace_back(yytext, TokenType::equal, yylineno);
    return 12;
}

\< {
    printf("plz1\n");
    kparser.m_tokens.emplace_back(yytext, TokenType::less_than, yylineno);
    return 12;
}

\<= {
    printf("plz\n");
    kparser.m_tokens.emplace_back(yytext, TokenType::less_than_equal, yylineno);
    return 12;
}

\> {
    kparser.m_tokens.emplace_back(yytext, TokenType::greater_than, yylineno);
    return 12;
}

\>= {
    kparser.m_tokens.emplace_back(yytext, TokenType::greater_than_equal, yylineno);
    return 12;
}

\( {
    kparser.m_tokens.emplace_back(yytext, TokenType::lparen, yylineno);
    return 12;
}

\) {
    kparser.m_tokens.emplace_back(yytext, TokenType::rparen, yylineno);
    return 12;
}
, {
}

[\n\t ] {
    // cout << "Scanner: whitechar (ignored)" << endl; 
}


. { 
    cout << "Scanner: unknown character [" << yytext << "]" << endl; 
}

<<EOF>> { 
    kparser.m_tokens.emplace_back(yytext, TokenType::eof, yylineno);
    return yyterminate(); 
}


%%
