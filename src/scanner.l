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

if {
    kparser.m_tokens.emplace_back(yytext, TokenType::_if, yylineno);
    cout << "Scanner: if [" << yytext << "]" << endl;
    return 12;
} 

while {
    kparser.m_tokens.emplace_back(yytext, TokenType::_while, yylineno);
    cout << "Scanner: while [" << yytext << "]" << endl;
    return 12;
} 

break {
    kparser.m_tokens.emplace_back(yytext, TokenType::_break, yylineno);
    cout << "Scanner: while [" << yytext << "]" << endl;
    return 12;
}

continue {
    kparser.m_tokens.emplace_back(yytext, TokenType::_continue, yylineno);
    cout << "Scanner: while [" << yytext << "]" << endl;
    return 12;
}

[a-z][a-z0-9]* { 
    kparser.m_tokens.emplace_back(yytext, TokenType::id, yylineno);
    cout << "Scanner: identifier [" << yytext << "]" << endl;
    return 11;
} 

[0-9][0-9]* {
    cout << "Scanner: number [" << yytext << "]" << endl;
    kparser.m_tokens.emplace_back(yytext, TokenType::integer, yylineno);
    return 12;
    // cout << "Scanner: decimal number: " << yytext << endl;
    // uint64_t number = strtoull(yytext, 0, 10);
}

= {
    kparser.m_tokens.emplace_back(yytext, TokenType::assign, yylineno);
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

== {
    cout << "Scanner: equal [" << yytext << "]" << endl;
    kparser.m_tokens.emplace_back(yytext, TokenType::equal, yylineno);
    return 12;
}

, {
    cout << "Scanner: ','" << endl; 
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
