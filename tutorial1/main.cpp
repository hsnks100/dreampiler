#include <sstream>
#include <iostream>
#include "scanner.h" 
int main() {
    std::istringstream is(R"(
    apple 5543 hello world =
    )"); 
    Scanner scanner(&is);
    while(1) {
        int t = scanner.yylex();
        if(t == 0) {
            break;
        }
        std::cout << t << std::endl;
    }
}
