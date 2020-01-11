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

%%

if {
    kparser.m_tokens.emplace_back(yytext, TokenType::_if);
    cout << "Scanner: if [" << yytext << "]" << endl;
    return 12;
} 

while {
    kparser.m_tokens.emplace_back(yytext, TokenType::_while);
    cout << "Scanner: while [" << yytext << "]" << endl;
    return 12;
} 
[a-z][a-z0-9]* { 
    kparser.m_tokens.emplace_back(yytext, TokenType::id);
    cout << "Scanner: identifier [" << yytext << "]" << endl;
    return 11;
} 

[0-9][0-9]* {
    cout << "Scanner: number [" << yytext << "]" << endl;
    kparser.m_tokens.emplace_back(yytext, TokenType::integer);
    return 12;
    // cout << "Scanner: decimal number: " << yytext << endl;
    // uint64_t number = strtoull(yytext, 0, 10);
}

= {
    cout << "Scanner: assign [" << yytext << "]" << endl;
    kparser.m_tokens.emplace_back(yytext, TokenType::assign);
    return 12;
}

\{ { 
    kparser.m_tokens.emplace_back(yytext, TokenType::lbracket);
    return 13;
}

\} { 
    kparser.m_tokens.emplace_back(yytext, TokenType::rbracket);
    return 13;
}
; {
    kparser.m_tokens.emplace_back(yytext, TokenType::semicolon);
    return 12;
}

== {
    cout << "Scanner: equal [" << yytext << "]" << endl;
    kparser.m_tokens.emplace_back(yytext, TokenType::equal);
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
    return yyterminate(); 
}


%%
