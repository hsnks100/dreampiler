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
    std::cout << yytext << std::endl;
    return 11;
} 
[0-9][0-9]* {
    std::cout << yytext << std::endl;
    return 12;
} 
= {
    std::cout << yytext << std::endl;
    return 13;
} 
[\n\t ] {
} 
. { 
} 
<<EOF>> { 
    std::cout << "EOF" << std::endl;
    return yyterminate(); 
} 
%%
