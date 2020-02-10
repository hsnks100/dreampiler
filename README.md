# Dreampiler 꿈파일러
나의 어릴 적 잊혀졌던 컴파일러, 꿈파일러

# 어릴 적 이야기 및 제작동기
어릴 적 이야기. 나는 학교에서 한 반에 있을 법한 컴퓨터를 매우 좋아하는 아이였다. 뭐 누구나 그렇겠지만 좋아하면 더 하고 싶고, 더 공부하고 싶고 그런다. 흘러 흘러 프로그래밍을 배우게 되었고 중3 겨울방학 Win32 API 에 빠져있던 무렵, 나에게 프로그래밍을 알려주는 멘토와 같은 형에게 물어봤다. 

Q. 컴파일러 만드는거 어려워? 

A. __어렵지 않을까?__ 

그 때 당시는 프로그래밍에 대해서는 걸음마를 하고 있던터라 감히 컴파일러를 만들고 싶은 생각은 무참히 꺾였다. 실제로 알아 보니까 컴파일러를 만드는 과정은 정규 컴퓨터공학과정에서도 4학년에 다룰 정도로 __난이도가 있었다.__ 사실은 수 년전에 컴파일러 비슷한 것을 만들긴 했다. 하지만 그 퀄리티가 너무 떨어지고 너무 급하게 진행했고 툴(lex, yacc)의 도움을 너무 심하게 받았기 때문에 아는 것이 __제대로 없는 상태로__ 지나갔다.

직장생활을 계속 하던 중 머릿속에는 계속 __컴파일러 만드는 과정을 한바퀴는 돌려봐야 하지 않을까?__ 라는 생각이 자꾸 들었다. 그 와중 회사동료가 1인 집짓기를 3년에 걸쳐서 하는 과정을 담은 링크를 보내줬다. 그 과정을 보면서 3년에 걸쳐서 집도 짓는데 키보드만 두들기면 되는 컴파일러 만들기를 못할 이유가 있을까? 생각이 들었고, 곧장 그 작업을 하기 시작했다.

이 때 참고한 두 서적은 컴파일러 책과 가상머신에 대한 책이다.

![image](https://user-images.githubusercontent.com/3623889/72685915-05cdd300-3b32-11ea-9aaf-85a813dff641.png)

![image](https://user-images.githubusercontent.com/3623889/72685923-2007b100-3b32-11ea-98c4-e5cae341df11.png)

하지만 내 글에서 __이론적인 이야기는 최대한 배제__하고 진행할 것이다. 어릴 적 간직하고 있는 꿈을 실현한다는 의미로 컴파일러의 이름을 꿈파일러로~~


# 방법에 대한 개요

![image](https://user-images.githubusercontent.com/3623889/72682192-dc02b500-3b0d-11ea-973b-d82562f943b8.png)

일반적인 컴파일러는 __토큰화(Lexical Analysis 라고도 부른다) - 구문분석 - 의미분석 - 중간코드생성 - 최적화 - 코드생성__ 과정을 거친다. 내 글에서는 의미분석과 최적화 코드생성은 __개나 줘라__ 마인드로 과감하게 버린다. 컴파일러에 밀접한 것들만 다루기에도 벅차며, 코드생성에서 아키텍쳐에 대해 종속적인 요소가 있게 되면 컴파일러를 만드는 과정보다 아키텍쳐공부가 더 크다. 

구현 방식은 가장 구현하기 편한 방식으로 진행한다. 토큰화만 flex 를 쓰고 나머지 구문분석, 중간코드 생성 및 가상머신 설계까지 다 손수 코딩을 하며 진행한다. 구문분석은 Top-Down 재귀방식으로 구현하겠다. 토큰화 과정을 flex 를 쓴 이유에 대해서 말하자면, 토큰화 과정은 순수하게 노가다과정이며 툴의 도움을 받아도 전체 과정을 이해하기 부족하지 않다. 그리고 타입은 없는 것으로 한다. 정적 타입도 좋지만 해보면 알겠지만 동적 타입이 더 만들기 쉽다.

# 전체 흐름

__토큰화 - 구분분석 - 가상머신이 실행할 수 있는 형태로 코드 생성 - 가상머신 동작__

가상머신은 스택기반의 가상머신을 이용한다. 여기서 지역변수를 어떻게 다루는지 이해를 하면서 봐야한다.

# 토큰화

__토큰화는 입력받은 문자열을 기본적인 카테고리로 분류하는 작업을 말한다.__ 보통 __예약 키워드, 자료형, 식별자, 숫자, 사용되는 기호(+, -, *, /, %, !=, == 등등)로 분류한다.__ 이 과정에는 식별자가 함수이름인지 변수이름인지 모르는 단계다. 예를 들면 
```
abc = 33;
abcd(11, 22, 33);
if a == 33 {
    
}
```
라는 코드가 있고 토큰화과정을 거친다면 순서대로 배열에 다음과 같이 담길 수 있다.

[id, '=', number, ';', id, '(', number, ',', number, ',', number, ')', ';', if, id, '==', number, '{', '}']

말 그대로 1차적인 분류다. 이 분류를 이용해 구문분석이 이뤄진다.

지면이 길어져 이만하고 다음장으로 넘긴다.

# 토큰화하는 방법

이전 장에서 토큰화가 무엇인지 알아봤다. 

우리는 산업역군의 프로그래머이기 때문에 구현가능한 형태를 원한다. 예제: flex 를 이용해서 간단히 토큰화를 어떻게 하는지 살펴보겠다. 

flex 가 무엇인지 살펴보면

>flex는 《fast lexical analyzer generator》의 줄임말로 lex의 기능을 개선한 자유 소프트웨어이다. 주로 bison과 쌍을 이루어 구문 분석기를 만드는 데 사용된다. flex를 이용하면 C로 구문 문석 코드를 만들 수 있다. 한편 C++ 코드를 만들어 주는 비슷한 기능을 하는 프로그램으로 flex++가 있으며 flex와 함께 배포된다. 작성자는 "Vern Paxson"씨로 1987년도에 처음 만들어졌다.

앞 장에서 봤듯 입력된 문법이 적법한지 상관없이 어떤 종류의 문자열인가를 판단하는 1차 분류로 생각하면 된다.

```
%{
  Prologue
%} 
Bison declarations 
%%
Grammar rules
%% 
Epilogue
```

구조는 위와 같은데, 우리는 마지막 Epilogue 는 쓰지 않을 것이다. 처음 %{ ... %} 사이에는 include 문들이 오고, Bison declarations 에는 flex 가 해당 문법파일을 어떻게 컴파일할것인지 옵션이 담긴다. 다음으로 Grammer Rules 에 우리가 분류해야 할 토큰 규칙들이 위치한다. 자세한 것은 인터넷 검색을 통해 해결하도록 하자. flex 에 대한 것들을 자세히 설명하기보다 바로 예제를 통해 이해하도록 한다.

예를 들어 보자.


```cpp
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
```

%% 사이의 문법만 주목해서 보자. 정규식 { ... } 형태로 기술되는데, 블록{} 사이에는 C/C++ 코드가 들어갈 수 있다. 

이것만 기억하면 된다. `좌측에 정규식이 평가될 때 {} 사이의 코드가 실행된다.`




```cpp
// scanner.h
class Scanner : public yyFlexLexer {
    public:
        Scanner(std::istream *in) : yyFlexLexer(in) {} 
        virtual int yylex(); 
};

// lex.yy.cc
#define YY_DECL int Scanner::yylex() 
...
#include "scanner.h"
...
YY_DECL
{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;
    ...
}

case 1:
YY_RULE_SETUP
#line 14 "scanner.l"
{ 
    std::cout << yytext << std::endl;
    return 11;
} 
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 18 "scanner.l"
{
    std::cout << yytext << std::endl;
    return 12;
} 
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 22 "scanner.l"
{
    std::cout << yytext << std::endl;
    return 13;
} 
	YY_BREAK
case 4:
/* rule 4 can match eol */
YY_RULE_SETUP
#line 26 "scanner.l"
{
} 
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 28 "scanner.l"
{ 
} 
	YY_BREAK
case YY_STATE_EOF(INITIAL):
#line 30 "scanner.l"
{ 
    std::cout << "EOF" << std::endl;
    return yyterminate(); 
} 
	YY_BREAK

```

위 코드를 보면 scanner.l 이 flex를 이용하여 생성된 lex.yy.cc 파일에 블록 {} 사이의 코드가 그대로 들어간 모습을 볼 수 있다.

```
▾ tutorial1/
    main.cpp
    scanner.h
    scanner.l
```
```
$ cd tutorial1 
$ flex scanner.l && g++ lex.yy.cc main.cpp -std=c++11 && ./a.out 
```

일부러 흐름을 잘 보여주기 위해 당분간은 CMake 를 쓰지 않겠다. 컴파일 되는 흐름을 잘 이해해야 한다. 먼저 flex 명령어를 통해 lex.yy.cc 가 생긴다. lex.yy.cc 안에는 `%option yyclass="Scanner"` 에 의해 `YY_DECL` 매크로가 정의된다. 

또한, Scanner::yylex 가 overriding 으로 구현된다. 

렉서파일에 scanner.h 를 쓰고 또 Scanner 클래스가 flex 를 쓰고, 생성된 파일(lex.yy.cc) 에서 scanner.h 를 쓰는 이상한 기분이지만 이 상호작용을 잘 이해하고 넘어가기 바란다.

```cpp
// main.cpp
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

```

위 프로그램에 의해 flex 를 실행할 수 있다.

`apple 5543 hello world =` 입력된 문자열이 이와 같을 때 토큰 분류기는 `11 12 11 11 13` 과 같이 반환된다. 이렇게 토큰화를 통해 숫자형 배열에 담은 후 구문분석(Parsing)을 하게 된다. 

동작하는 예제는 git clone https://github.com/hsnks100/dreampiler.git 의 tutorial1



# 구문분석이란

이전 장에서 토큰화가 무엇인지 알아봤다. 토큰화과정만으로는 우리가 할 수 있는 것이 거의 없다. 

위키 인용

>컴퓨터 과학에서 파싱((syntactic) parsing)은 일련의 문자열을 의미있는 토큰(token)으로 분해하고 이들로 이루어진 파스 트리(parse tree)를 만드는 과정을 말한다.

![image](https://user-images.githubusercontent.com/3623889/72686221-9f4ab400-3b35-11ea-865d-307ef90669a3.png)

- 하향식 구문 분석(top-down parsing)
- 상향식 구문 분석(bottom-up parsing)

구문분석에는 정말 크게 분류하면 아래와 같은 방법이 있다. 이 중에서 우리는 가장 구현하기 쉬운 하향식 구문 분석을 선택하겠다.  

# 구문분석 하는 방법
처음부터 while 문 if 문 function 에 대해서 다루면 좋겠지만 컴파일러를 이제 막 만들어보고 싶다는 생각을 가졌다면 걷기는 커녕 기어다니기도 힘든 지경일거다. 파싱 구현에 대해 감을 가장 쉽게 잡을 수 있고, 제일 구현하기 쉬운 형태의 예제를 구현하면서 점차 절차지향적 프로그래밍에서 사용되는 while, if 문으로 확장할 것이다. 

# 1차 구문분석의 예제
간단한 수식을 입력 받아 계산된 결과를 보여주는 프로그램을 작성해본다. 사칙연산을 지원하고 세미콜론(;)을 만나면 값을 출력하는 간단한 예제를 만들어보자.

```cpp
(200 + 12) * 34 + 56 - 100;
200 + 12 * 34 + 56 - 100;
200 + (12 * 34) + 56 - 100;
-100;
1 + -5;
-(1 + -5);
-(1 + -5)*4 + 100 * 3;
1 * -100;
1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30));
(1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30)));
(1 * -100 + 30 / 3 - (1 + -3) - (10 + 1030 + 30) * 3 * 2 -((123 + 30))) * 3 - 100;
100 - -100;
100 - (-100);
100 - (-100 + 1 - 3 * 10 / 2 / 5 - 10 + 3838 - 1003 + 13) - 3 - 1-  3; 
```

위는 우리가 올바르게 통과해야할 수식의 목록이다. 

구현사항을 보면 만만치 않아보인다. 하지만 귀찮을 뿐이지 난이도가 높은 작업이 아니다. 차근차근 접근해보자.


그 전에 잠깐! Context Free Grammar 문법을 표현하는 방식중 하나인 배커스-나우르(BNF) 표기법에 약간이라도 알아야 한다.
https://ko.wikipedia.org/wiki/%EB%B0%B0%EC%BB%A4%EC%8A%A4-%EB%82%98%EC%9A%B0%EB%A5%B4_%ED%91%9C%EA%B8%B0%EB%B2%95

BNF
```cpp
<E> ::= <E> + <T> | <E> - <T> | <T> 
<T> ::= <F> * <T> | <F> / <T> | <F> 
<F> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
```

위 문법을 이용하면 사칙연산에 대한 규칙을 표현할 수 있을 것만 같다. 세상일 쉬운거 없다. 위 문법은 사칙연산에 대한 우선순위를 표현은 되지만, 프로그래밍으로 표현하기에 무한 재귀함수호출에 걸린다(좌항을 함수라고 치고 구문분석을 구현할 경우). 나중에 구현부를 직접 보면 감이 올거다.

프로그래밍으로 구현하기에 BNF 는 부족한점이 여러모로 많다. 그래서 EBNF 표기법을 이용하면 프로그래밍으로 구현하기 적합한 형태로 바꿀 수 있다. EBNF 에서 여러가지 확장된 표현이 있지만, 우리는 이것저것 공부할 시간 없다. 필요한 것만 소개한다.

EBNF 에서는 {}, [], (a\|b) 표현이 있다. 각각 0번이상 반복, 생략가능, a or b 양자택일. 사칙연산 표현을 EBNF 으로 표기해보자.

EBNF
```cpp
<E> ::= <T> { ('+' | '-') <T> }
<T> ::= <F> { ('*' | '/') <F> }
<F> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 
```

이제 좌항을 함수라고 했을 때, 우항에 좌항이 표현되지 않는다. 이는 곧 프로그래밍으로 구현했을 때 직접 재귀호출이 걸리지 않는 것을 의미한다. 

이제 실제 사칙연산에 대한 EBNF 를 작성해보자.

```cpp
<stmt> ::= <add_expr> ';'
<add_expr> ::= <mult_expr> { ('+' | '-') <mult_expr> }
<mult_expr> ::= <sign_expr> { ('*' | '/') <sign_expr> }
<sign_expr> ::= ['-'] <factor_expr> 
<factor_expr> ::= <N> | '(' <add_expr> ')' 
<N> ::= number
```

우선순위가 높은게 뒤로 배치된다. 평가할 우선순위가 제일 높은 숫자, 괄호() 가 제일 뒤에 배치되며 맨 처음 <stmt> 와 매치를 시도한다.

어떻게 구현할까? 사람마다 다르지만 나는 우선순위가 높은 것 부터 먼저 작성한다. 그러면 다 만들지 않고도 테스트가 가능하다. <factor_expr> 부터 구현해보면 

```cpp
// begin: 파싱을 시작할 문자의 위치
// end: unused 
// bi: unused
// return: 다음 파싱할 토큰의 위치, 실패시 -1
int KParser::_factor_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    if(tokenType == TokenType::integer) {
        m_totalCode.push_back(m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::lparen) {
        int tryI = i + 1;
        int t = _add_expr(tryI, end, bi);
        if(t < 0) {
            char buf[256];
            sprintf(buf, "line[%d]: 표현식의 오류.\n", m_tokens[tryI].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        if(m_tokens[t].tokenType != TokenType::rparen) {
            char buf[256];
            sprintf(buf, "line[%d]: 닫는 괄호가 빠졌습니다.\n", m_tokens[tryI].line);
            m_errorMsgs.push_back(buf);
            return -1; 
        }
        i = t + 1;
    } 
    else {
        return -1;
    } 
    return i;
}
```

( add_expr ) 때문에 우선순위 반대로 작성하는것이 여의치 않기 때문에 실제 구현시에는 ( add_expr ) 부분은 생략하고 나중에 완성시킨다. factor_expr 구현은 단순하다. terminal 문자가 바로 있기 때문에 최종적으로 평가할 스택에 넣기만 하면 된다.

```cpp
<mult_expr> ::= <sign_expr> { ('*' | '/') <sign_expr> }
<sign_expr> ::= ['-'] <factor_expr> 
```

구현을 살펴보자.

```cpp

int KParser::_mul_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    std::string leftValue = m_tokens[i].str;
    int t = _sign_expr(i, end, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        t = _mul_op(tryI, end, bi);
        if(t < 0) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _sign_expr(tryI, end, bi);
        if (t < 0) {
            break;
        }
        if(tokenValue == TokenType::mult) {
            m_totalCode.push_back("mult");
        } else if(tokenValue == TokenType::devide) {
            m_totalCode.push_back("div");
        }
        i = t; 
    } 
    return i;
}

int KParser::_sign_expr(int begin, int end, BlockInfo bi) {
    int i = begin;
    bool isMinus = false;
    if(m_tokens[i].tokenType == TokenType::minus) {
        isMinus = true;
        i++;
    } 
    int t = _factor_expr(i, end, bi);
    if(t < 0) {
        return -1;
    }
    if (isMinus) {
        m_totalCode.push_back("sign");
    }
    return t;
}
```

EBNF 명세 그대로 구현한 모습을 알 수 있다. 말 그대로 - 문자는 옵션으로 처리하고, 부호 단일 명령이기 때문에 바로 명령스택에 sign 을 넣는다. 다음으로 mult_expr 를 보면 명세대로 구현하되 sign_expr 부분은 말 그대로 다른 함수 호출로 퉁치는 모습이다. 그러니까 현재 작성하는 문법에 집중하면 되고 다른 non-terminal 이 등장하면 그냥 함수 호출로 대체시키면 된다. 그런 다음 *, / 가 들어온 경우에 실행스택에 mult, div 문자열을 넣는다. 

이런식으로 EBNF 를 정의한 후 거꾸로 따라 올라가면서 구현하면 된다. 실행 스택에 들어간 명령어는 스택머신에 의해 수행된다.

스택머신은 숫자를 두개 혹은 한개를 입력받아 결과를 다시 스택에 넣는 기계라고 생각하면 된다. 예를 들면 

```cpp
                 # stack  (왼쪽이 top):
 push A          #           A
 push B          #     B     A
 push C          # C   B     A
 sub             #   B-C     A
 mult            #           A*(B-C)
 push D          #     D     A*(B-C)
 push E          # E   D     A*(B-C)
 add             #     D+E   A*(B-C)
 add             #           A*(B-C)+(D+E)
```

스택머신에서 push 명령어는 top 에 값을 채우는 것이고 연산자를 만나면 두개를 연산해서 스택에 다시 넣는다. 

`(200 + 12) * 34 + 56 - 100` 는 

```cpp
200
12
add
34
mult
56
add
100
sub
```
처럼 스택이 만들어지고 스택 기반의 간단한 머신을 만들고 값을 출력하면 된다. 구현은 매우 간단하다.
```cpp
    for(auto i: m_totalCode) {
        // tt++;
        // printf("eip[%3d]: %s\n", tt, i.c_str());
        if (i == "add") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(a + b);
        } else if (i == "mult") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(a * b);
        } else if (i == "div") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(b / a);
        } else if (i == "sub") {
            int a, b;
            a = calc[calc.size() - 1];
            calc.pop_back();
            b = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(b - a);
        } else if (i == ";") {
            printf("result %d\n", calc[0]);
            calc.clear();
        } else if (i == "sign") {
            int a;
            a = calc[calc.size() - 1];
            calc.pop_back();
            calc.push_back(-a);
        } else {
            calc.push_back(std::stoi(i));
        } 
    } 
```

전체 완성된 소스는 `git clone https://github.com/hsnks100/dreampiler.git` 의 tutorial2

이제 실제 프로그래밍 언어를 만들기 위한 기초적인 체력이 되었다. 간단한 계산기지만 괄호 및 부호를 고려한 계산기를 만드는것은 매우 중요한 과정이니 실제로 코딩해봐야한다. 

다음장부터는 실제 실제 프로그래밍언어를 만들기 위해 어떤게 필요한지 알아보겠다.  


# 언어 명세

드디어 우리는 간단한 프로그래밍 언어를 만들 수 있는 능력이 갖춰졌다. 

처음에도 말했는데, 우리가 만들 언어에는 타입이 없다. 되게 큰 줄기만 한번 설계해보자.

- 함수 지원
- 지역변수 지원
- 전역변수 지원
- 재귀호출 지원

위 내용을 스택기반의 가상머신을 하나 설계를 하면서 진행하겠다.

# 가상머신 

우리는 고수준의(high-level) 프로그램을 중간코드로 만들고, 중간코드를 실행하는 프로그램을 만들 건데 ~

기본개념은 플랫폼에서 실행하는것이 아니고 __가상머신에서 실행가능한 중간코드를__ 만드는거다.

가상머신을 구현하는 기본핵심 기준 중 하나는 __연산자와 피연산자의 형태는 어떻게 처리 할건지다.__ 우리는 스택머신모델을 사용한다.

스택머신모델에서는 기본적인 산술명령은 스택의 top 에 있는 숫자를 __필요한__ 만큼 꺼내서 연산후 다시 top 에 넣는다. 

# 스택머신모델에서의 연산의 예

```cpp
A = 3 * (1 + 2) * 3 - 3 + 1 
```

위 연산은 다음과 같은 스택기반의 명령어로 표현가능하다.

```
          <--------------------top-----------------------------------
push 3    |  3    |       |            |            |               |
push 1    |  3    |   1   |            |            |               | 
push 2    |  3    |   1   |     2      |            |               |   
plus      |  3    |   3   |            |            |               |  
mult      |  9    |       |            |            |               | 
push 3    |  9    |   3   |            |            |               | 
mult      |  27   |       |            |            |               |   
push 3    |  27   |   3   |            |            |               |  
sub       |  24   |       |            |            |               |       
push 1    |  24   |   1   |            |            |               |      
plus      |  25   |       |            |            |               |     
pop A     |       |       |            |            |               |     
---------------------------------------------------------------------
```

여기서 pop 명령어는 피연산자의 있는 변수에 스택의 top 을 넣는다.

보다시피 push 와 연산자로 피연산자와 연산자의 개념을 표현할 수 있다. 

# 가상머신의 자세한 명세

자 보자. 프로그래밍언어에는 단순 계산에 대한식말고도 __반복문, 함수 표현, 메모리접근과__ 같은 요소도 있다. 이 명령의 체계를 나눠보자면 

- 산술명령
- 메모리 접근
- 프로그램 흐름제어 
- 함수호출 및 정의
       
산술명령은 위의 예로 충분히 개념은 전달되었다고 생각하는데, 메모리 접근하는 방식에 대해 알아보자.

메모리 접근은 다음과 같은 형식으로 접근한다. 

- `push segment index` : segment index 값을 스택에 push
- `pop segment index`  : segment index 에 스택의 top 내용을 저장하면서 pop

segment 의 종류는 `argument, local, constant` 이것으로만 구현하도록 한다. 각각 __인자, 지역변수, 상수를__ 나타낸다.

실전예를 보는 것이 가장 좋다. 

```cpp
/*
someFunction(int arg0) {
int local0;
local0 = arg0 + 3;
}
*/
push argument 0
push constant 3
add
pop local 0 
```

고급프로그램을 저수준의 스택머신으로 표현한 모습이다.  프로그램 흐름제어도 한번 다뤄보자. 몇가지의 흐름제어를 위해 필요한 명령어를 써보자.

```
eq: 스택의 위치한 두개의 값이 같으면 0이 아닌 값
neq: 스택의 위치한 두개의 값이 다르면 0이 아닌 값
jmp LABEL: LABEL 위치로 무조건 점프
jnz LABEL: LABEL 위치로 스택의 두값이 0이 아니면 점프 (Jmp if value is Not Zero)
jz LABEL: LABEL 위치로 스택의 두값이 0이면 점프 (Jmp if value is Zero): 
label LABEL_NAME: 프로그램 코드의 위치를 나타내는 지시어
```

배운게 도둑질이라고 intel x86 기반의 어셈을 따라하게 됐다. MASM 의 지시어도 일부 따라한 부분이다. 


먼저 if 문의 스택머신표현을 보자.
```cpp
ifStatement() {
    a := 5;
    if a == 5 {
        a = a + 1;
    }
}
push constant 5
pop local 0                  // a := 5
push local 0
push constant 5
eq                           // a == 5
jz LABEL555
push local 0
push constant 1 
add
pop local 0                  // a = a + 1
label LABEL555
```

소스의 의미는 의미가 없지만 if 문을 잘 표현하고 있다. 다음으로 while 문의 표현을 보자. if 와 다른점은 블록끝} 에서 __무조건 점프가__ 들어갔다. 조금만 생각해보면 if 문과 while 문의 한끗차이~

```cpp
someFunction() {
    a := 5;
    sum := 0;
    while a != 0 {
        sum = sum + a;
    }
}

push constant 5
pop local 0                  // a := 5
push constant 0 
pop local 1                  // sum := 0
label LABEL222 <---------------------------------------------
push local 0                                                |
push constant 0                                             | 
neq                          // a != 0                      |
jz LABEL111     ---------------                             |    
push local 1                  |                             |    
push local 0                  |                             |    
add                           | // sum + a                  |
pop local 0                   | // sum = sum + a            |   
jmp LABEL222    --------------|------------------------------
                              |
label LABEL111  <--------------
```

본의 아니게 아스키아트 

해당 while 문은 영원히 탈출하지 못하는 while 문이지만 while 문이 어떻게 스택머신으로 나타내는지 표현하기에 충분하니 넘어가도록 하자~


다음으로 함수표현을 살펴보자. 되게 중요하게 설계해야 하는 부분이다. 이 부분을 어떻게 하느냐에 따라 언어가 재귀호출을 지원하는지 스코프를 지원하는지 여부가 갈린다.

```
function functionName nLocals: functionName 은 nLocals 개의 지역변수를 가짐.
call functionName nArgs: nArgs 개의 인자를 받은 함수 호출
return: 함수호출이 일어난 부분의 다음 부분으로 점프 
```

문제를 간단하게 하기 위해 한가지 가정을 하자. __모든 함수는 리턴을 해야 한다.__

함수라는 것을 만들기 위해서 어떤것이 필요할까? 일단 먼저 해야할 생각은 함수가 콜됐을 때, 함수는 자기자신이 돌아가야 할 위치를 알 수가 없다. __부르는 쪽에서 돌아갈 주소를 결정해야 한다.__ 합리적으로 보인다. 함수콜이 끝났을 때에는 __돌아간 곳에서는 함수호출하기 위해 준비했던 직전의 스택상태로 돌아가야 한다.__ 

- 함수를 부르는 쪽, 즉 caller 쪽에서 돌아갈 위치를 정해줘야 한다.
- 함수콜이 끝났을 때에 직전의 스택상태로 돌아가야 한다.

인자의 위치를 다루기 위한, __지역변수의 위치를 다루기 위한 가상머신 내부의 변수가 필요하다.__ 보통 cpu 모델에서 이를 담당하는 놈은 register 라고 부른다. 먹은 밥이 인텔 어셈이기 때문에 이 역시 그 컨셉을 최대한 비슷하게 한다. 궁금하신 분들은 EBP 로 검색해보시라. 

우리는 EBP 대신에 __ArgPointer, LocalPointer__ 라는 가상머신의 변수(레지스터)를 생각한다. 
N 번째 인자 argN 과 N 번째 지역변수 localN 에 대해
__argN 과 localN 은 각각 stack[ArgPointer + N], stack[LocalPointer + N] 으로 접근할 수 있다.__ 예를 들어 설명하면,

3 개의 변수를 더해서 리턴하는 함수가 있다고 가정하자 `func sum(a, b, c) { return a + b + c;}`

`a := sum(1, 2, 3)` 이 실행이 되었을 때(a 는 첫번 째 지역변수로 가정), CPU 상태와 STACK 의 상태를 그려보면

```
[     CPU         ]
EIP     |   COMMAND
102     |   push 1
103     |   push 2 
104     |   push 3 
105     |   call sum 3 // 인자가 3개
106     |   pop local 0

[    STACK        ]
SP      |   VALUE
4004    |   1        
4005    |   2    
4006    |   3    
4007    |  106 // 돌아갈 주소
4008    |  PrevLocalPointer   
4009    |  PrevArgPointer

[   REGISTER     ]
LocalPointer = 4010
ArgPointer = 4004 
```                 

차근차근 설명해보겠다. 먼저 우리의 가상머신은 push 1, push 2, push3 을 수행하면 STACK 의 상태는 4004 부터 4006 까지 1, 2, 3 의 숫자가 들어간다. 이 때 `call sum 3` 이 실행되면 가상머신은 함수호출을 하기 위한 몇 가지 준비를 한다.

먼저 ArgPointer = 함수에 인자를 넣는 push 가 시작된 위치 (4004) 

- 들어갈 주소
- 이전 지역변수 포인터
- 이전 함수 인자 포인터

를 차례대로 스택에 넣는다. __그리고 LocalPointer 를 SP 로 세팅한다.__ 그 값은 이 예에서는 4010 이 된다. 이 위치 부터 지역변수 개수만큼 스택을 키운다.

가상머신은 return 을 만나면 LocalPointer, ArgPointer 를 복구하고 SP 가 4005 가 될 때 까지 STACK 을 pop 한다.

가상머신 실제 코드
```
int frame = m_localPointer;
int ret = m_stack[frame - STACK_FRAME_SIZE]; // return address, STACK_FRAME_SIZE: const 3
m_stack[m_argPointer] = getStackTop(); // setting return value
int pops = m_argPointer + 1; // to pop data for m_stack.size() > pops 
m_argPointer = m_stack[frame - 1]; // restore
m_localPointer = m_stack[frame - 2]; // restore
while (m_stack.size() > pops) {
    m_stack.pop_back();
}
m_eip = ret;  // setting return address
``` 

# 다시 쓰는 EBNF 

우리가 다룰 가상머신에 대해서는 충분히 설명이 되었다. 이제 상세 EBNF 를 정의하고 이를 구현해야 한다.

EBNF 에 앞서서 전체적인 언어의 구조는 다음과 같다. 프로그램의 시작은 함수 정의로 시작한다. 함수안에서 여러가지 if, while, assign 과 같은 statement 가 이어진다. 

```
external {
    func {
        statement {
            if | while | assign | simple_expr | assign | return | break ...
        }
    }
}
```

한번 호흡 가다듬고 우리가 만들 언어의 전체 EBNF 를 정리하여 써보면 

> EBNF 에서는 {}, [], (a\|b) 표현이 있다. 각각 0번이상 반복, 생략가능, a or b 양자택일. 

``` 
<ROOT> ::= {<function>}
<function> ::= "func" <id> <parameters> <block>
<parameters> ::= "(" ")" | "(" <id> {"," <id>}  ")" 
<block> ::= "{" {<statement>} "}"
<statement> ::= <if> | <while> | "break" ";" | "continue" ";" | <assign> | <decassign> | <block> | <simple_expr> | <return>
<if> ::= "if" <expr> <block>
<while> ::= "while" <expr> <block>
<assign> ::= <id> "=" <expr> ";"
<decassign> ::= <id> ":=" <expr> ";"
<simple_expr> ::= <expr> ";"
<return> ::= "return" <expr> ";"
--------------------------------
<expr> ::= <or_expr> {"&&" <or_expr>} <------------------------------------------------------
<or_expr> ::= <and_expr> {"||" <and_expr>}                                                  |
<and_expr> ::= <add_expr> {("==" | "!=" | "<" | ">" | "<=" | ">=") <add_expr>}              |
<add_expr> ::= <mul_expr> {("+" | "-") <mul_expr>}                                          |
<mul_expr> ::= <sign_expr> {("*" | "/") <sign_expr>}                                        |
<sign_expr> ::= ["-"] <factor_expr>                                                         |
<factor_expr> ::= <call> | <id> | <number> | "(" <expr> ")"                     -------------
<call> ::= <id> "(" ")" | <id> "(" <expr> {"," <expr> } ")"
```

우리는 산업역군의 프로그래머니까 고상한 이론은 집어치우고 빨리 구현체를 보고 싶을 것이다. 말로는 누가 못 짜나.

다음 장에서는 이렇게 정의한 가상머신과 EBNF 에 대해 파서구현에 대해 다뤄보겠다. 


# 자료형 정의

먼저 Token 을 정의한다. str 은 토큰의 내용이고, tokenType 은 분류된 토큰의 종류를 말한다. 그리고 해당 토큰이 몇 번째 라인에서 발견됐는지 알려주는 line 변수. (에러를 표시할 때 유용하게 쓰인다.)

```cpp
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
```

TokenType 은 다음과 같다. 각종 예약어와 while, for, 지시어등이 담긴다. 

```cpp
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
}; 
```

우리가 작성할 Parser

```cpp
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

        ...
```

# Parser 구현부

```cpp
int KParser::_root(int begin,  BlockInfo& bi) {
    int i = begin;
    int t = 0;

    while (i < m_tokens.size()) {
        t = _external(i, bi);
        i = t;
        if (t < 0) {
            break;
        }
        i = t;
    }
    return i; 
}
int KParser::_external(int begin,  BlockInfo& bi) { 
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType == TokenType::eof) {
        i++;
        return i;
    }

    t = _func(i, bi); 
    if (t >= 0) {
        m_errorMsgs.clear();
        return t;
    }

    t = _var(i, bi); 
    if (t >= 0) {
        return t;
    }
    addErrorString(i, "함수 선언이나 변수 선언이 와야 합니다.");
    return -1;
}
int KParser::_func(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0; 
    if (m_tokens[i].tokenType != TokenType::_func) {
        return -1;
    }
    i++; 
    if (m_tokens[i].tokenType != TokenType::id) {
        addErrorString(i, "func 뒤에는 식별자가 와야합니다.");
        return -1;
    } 
    std::string funcName = m_tokens[i].str;
    m_codes.clear();
    addCode("func", funcName, "");
    i++; 
    m_parameters = 0; 
    t = _parameters(i, bi);
    if (t < 0) {
        addErrorString(i, "함수인자 정의가 잘못 됐습니다.");
        return -1;
    }
    m_funcToParams[funcName] = m_parameters;
    i = t;

    m_locals = 0;
    m_increaseLocals = 0;
    t = _block(i, bi);
    if (t < 0) {
        addErrorString(i, "함수몸체 정의가 잘못 됐습니다.");
        return -1;
    }
    i = t;
    m_codes[0][2] = std::to_string(m_locals); 
    for(auto& i: m_codes) {
        if(m_varToArg.find(i[1]) != m_varToArg.end()) {
            i[2] = std::to_string(m_varToArg[i[1]]);
            i[1] = "argument";
        }
        if(m_varToLocal.find(i[1]) != m_varToLocal.end()) {
            i[2] = std::to_string(m_varToLocal[i[1]]);
            i[1] = "local";
        }
    }

    m_il.insert(m_il.end(), m_codes.begin(), m_codes.end());
    return i;
}
int KParser::_parameters(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;
    if (m_tokens[i].tokenType != TokenType::lparen) {
        addErrorString(i, "( 가 빠졌습니다.");
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType == TokenType::rparen) {
        i++;
        return i;
    }
    // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
    int idIndex = i;
    if (m_tokens[idIndex].tokenType != TokenType::id) {
        addErrorString(idIndex, "함수 인자에는 식별자가 와야합니다.");
        return -1;
    }
    i++;
    m_varToArg[m_tokens[idIndex].str] = m_parameters;
    m_parameters++;

    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::comma) {
            break;
        }
        tryI++;
        if (m_tokens[tryI].tokenType != TokenType::id) {
            addErrorString(tryI, ", 뒤에 식별자를 기대했습니다.");
            return -1; 
        }
        m_varToArg[m_tokens[tryI].str] = m_parameters;
        m_parameters++;
        tryI++;
        i = tryI;
    } 
    if (m_tokens[i].tokenType != TokenType::rparen) {
        addErrorString(i, ", 뒤에 식별자를 기대했습니다.");
        return -1;
    }
    i++;
    return i;
}
int KParser::_block(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    if(m_tokens[i].tokenType != TokenType::lbracket) {
        return -1;
    }
    i++;
    while(1) {
        if(m_tokens[i].tokenType == TokenType::rbracket) {
            i++;
            break;
        }
        t = _statement(i, bi);
        if (t < 0) {
            char buf[256];
            sprintf(buf, "line[%d]: 문장이 아닙니다.\n", m_tokens[i].line);
            m_errorMsgs.push_back(buf);
            return -1;
        }
        i = t; 
    }
    return i; 
}
int KParser::_var(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType != TokenType::_var) {
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType != TokenType::id) {
        char buf[256];
        sprintf(buf, "line[%d]: 식별자가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        char buf[256];
        sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line);
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    return i;
}
int KParser::_statement(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;
    if((t = _if(i, bi)) >= 0) {
        i = t;
    } else if((t = _while(i, bi)) >= 0) {
        i = t;
    }else if((t = _assign(i, bi)) >= 0) {
        i = t;
    } else if((t = _decassign(i, bi)) >= 0) {
        i = t;
    } else if((t = _block(i, bi)) >= 0) {
        i = t;
    } else if((t = _break(i, bi)) >= 0) {
        i = t;
    } else if((t = _continue(i, bi)) >= 0) {
        i = t;
    }else if((t = _simple_expr(i, bi)) >= 0) {
        i = t;
    } else if((t = _return(i, bi)) >= 0) {
        i = t;
    } else {
        return -1;
    }
    return i;
}

int KParser::_call(int begin,  BlockInfo bi) {
    // some();
    // some(a);
    // some(a,b,c);
    int i = begin;
    int t = 0;

    // 인자가 없는 경우 
    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen && 
        m_tokens[i + 2].tokenType == TokenType::rparen
        ) {
        addCode("call", m_tokens[i].str,  std::to_string( m_funcToParams[m_tokens[i].str]));
        return i + 3;
    }

    // 인자가 하나이상 있는 경우
    if (m_tokens[i].tokenType == TokenType::id && 
        m_tokens[i + 1].tokenType == TokenType::lparen) {
        i = i + 2;
        // 이 까지 왔으면 매개변수가 하나이상 있는 것이다.
        t = _expr(i, bi);
        if (t < 0) {
            addErrorString(i, "함수 인자에는 표현식이 와야합니다.");
            return -1;
        }
        i = t;

        while (1) {
            int tryI = i;
            std::string rightValue = m_tokens[tryI].str;
            TokenType tokenValue = m_tokens[tryI].tokenType;
            if(tokenValue != TokenType::comma) {
                break;
            }
            tryI++;
            t = _expr(tryI, bi);
            if (t < 0) {
                addErrorString(i, ", 뒤에 표현식을 기대했습니다.");
                return -1; 
            }
            tryI = t;
            i = tryI;
        } 
        if (m_tokens[i].tokenType != TokenType::rparen) {
            addErrorString(i, "_call: ) 가 빠졌습니다.");
            return -1;
        }
        i++;
        addCode("call", m_tokens[begin].str,  std::to_string( m_funcToParams[m_tokens[begin].str]));
        return i;
    }

    return -1; 
}

int KParser::_return(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;

    if (m_tokens[i].tokenType != TokenType::_return) {
        return -1;
    }
    i++;

    if ((t = _expr(i, bi)) < 0) {
        addErrorString(i, "표현식이 와야합니다.");
        return -1;
    } 
    i = t;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        addErrorString(i, "; 이 와야합니다.");
        return -1;
    }
    i++;
    addCode("return", "", "");
    return i;
}

int KParser::_expr(int begin,  BlockInfo bi) {
    return _adv_expr(begin, bi);
}

int KParser::_simple_expr(int begin,  BlockInfo bi) {
    // <expr>;
    int i = begin;
    int t = 0;
    if ((t = _expr(i, bi)) < 0) {
        return -1;
    }
    i = t;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        addErrorString(i, "; 이 와야합니다.");
        return -1;
    }
    i++;
    // 대입문이 없으므로 빈 스택을 pop 해준다.
    addCode("pop", "", ""); 
    return i;
}
int KParser::_adv_expr(int begin,  BlockInfo bi) {
    return _or_expr(begin, bi);
}

int KParser::_or_expr(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _and_expr(i, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::logical_or) {
            break;
        }
        tryI++;
        t = _and_expr(tryI, bi);
        if (t < 0) {
            break;
        }
        addCode("or", "", "");
        i = t; 
    } 
    return i;
}

int KParser::_and_expr(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _cmp_expr(i, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        std::string rightValue = m_tokens[tryI].str;
        TokenType tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::logical_and) {
            break;
        }
        tryI++;
        t = _cmp_expr(tryI, bi);
        if (t < 0) {
            break;
        }
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        addCode("and", "", "");
        i = t; 
    } 
    return i;
}

int KParser::_cmp_expr(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _add_expr(i, bi);
    if (t < 0) {
        return -1;
    }
    i = t;

    do {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::equal 
           && tokenValue != TokenType::not_equal
           && tokenValue != TokenType::less_than
           && tokenValue != TokenType::less_than_equal
           && tokenValue != TokenType::greater_than
           && tokenValue != TokenType::greater_than_equal
           ) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _add_expr(tryI, bi);
        if (t < 0) {
            break;
        }
        // bi.addCode(leftValue);
        // bi.addCode(rightValue);
        if(tokenValue == TokenType::equal) {
            addCode("eq", "", "");
        } else if(tokenValue == TokenType::not_equal) {
            addCode("neq", "", "");
        }else if(tokenValue == TokenType::less_than) {
            printf("lt!!\n");
            addCode("lt", "", "");
        }else if(tokenValue == TokenType::less_than_equal) {
            printf("lte!!\n");
            addCode("lte", "", "");
        }else if(tokenValue == TokenType::greater_than) {
            addCode("gt", "", "");
        }else if(tokenValue == TokenType::greater_than_equal) {
            addCode("gte", "", "");
        } 
        i = t; 
    } while(0);
    return i;
}
int KParser::_add_expr(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _mul_expr(i, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        if(tokenValue != TokenType::plus && tokenValue != TokenType::minus) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _mul_expr(tryI, bi);
        if (t < 0) {
            break;
        }
        if(tokenValue == TokenType::plus) {
            addCode("add", "", "");
        } else if(tokenValue == TokenType::minus) {
            addCode("sub", "", "");
        }
        i = t; 
    } 
    return i;
}
int KParser::_mul_expr(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    std::string leftValue = m_tokens[i].str;
    t = _sign_expr(i, bi);
    if (t < 0) {
        return -1;
    }
    i = t;
    while (1) {
        int tryI = i;
        auto tokenValue = m_tokens[tryI].tokenType;
        t = _mul_op(tryI, bi);
        if(t < 0) {
            break;
        }
        tryI++;
        std::string rightValue = m_tokens[tryI].str;
        t = _sign_expr(tryI, bi);
        if (t < 0) {
            break;
        }
        if(tokenValue == TokenType::mult) {
            addCode("mult", "", "");
        } else if(tokenValue == TokenType::devide) {
            addCode("div", "", "");
        } else if(tokenValue == TokenType::modular) {
            addCode("mod", "", "");
        }

        i = t; 
    } 
    return i;
}

int KParser::_mul_op(int begin,  BlockInfo bi) {
    int i = begin;
    if (m_tokens[i].tokenType != TokenType::mult && m_tokens[i].tokenType != TokenType::devide && 
        m_tokens[i].tokenType != TokenType::modular) {
        return -1;
    }
    i++;
    return i;
}
int KParser::_sign_expr(int begin,  BlockInfo bi) {
    int i = begin;
    bool isMinus = false;
    if(m_tokens[i].tokenType == TokenType::minus) {
        isMinus = true;
        i++;
    } 
    int t = _factor_expr(i, bi);
    if(t < 0) {
        return -1;
    }
    if (isMinus) {
        addCode("neg", "", "");
    }
    return t;
}

int KParser::_factor_expr(int begin,  BlockInfo bi) {
    int i = begin;
    TokenType tokenType = m_tokens[i].tokenType;
    int t = 0;

    if((t = _call(i, bi)) >= 0) {
        i = t;
    } else if(tokenType == TokenType::id) {
        addCode("push", m_tokens[i].str, "");
        i++;
    } else if(tokenType == TokenType::integer) {
        addCode("push", "constant", m_tokens[i].str);
        i++;
    } else if(tokenType == TokenType::lparen) {
        int tryI = i;
        tryI++;
        int t = _expr(tryI, bi);
        if(t < 0) {
            addErrorString(i, "표현식의 오류.");
            return -1;
        }
        if(m_tokens[t].tokenType != TokenType::rparen) {
            addErrorString(i, "닫는 괄호가 빠졌습니다.\n");
            return -1; 
        }
        t++;
        i = t;
    } 
    else {
        return -1;
    } 
    return i;
}


int KParser::_if(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;
    if( m_tokens[i].tokenType != TokenType::_if ) {
        return -1;
    }
    i++;

    if((t = _expr(i, bi)) < 0) {
        return -1;
    } else {
        i = t;
    } 
    std::string eLabel = "LABEL" + std::to_string(m_labelNumber++);

    addCode("jz", eLabel, "");
    if((t = _block(i, bi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    addCode("label", eLabel, "");
    return i;
}

int KParser::_while(int begin,  BlockInfo bi) {
    int i = begin;
    int t = 0;

    if( m_tokens[i].tokenType != TokenType::_while ) {
        return -1;
    }
    i++; 
    std::string sLabel = "LABEL" + std::to_string(m_labelNumber++);
    std::string eLabel = "LABEL" + std::to_string(m_labelNumber++);
    bi.sLabel = sLabel;
    bi.eLabel = eLabel;
    addCode("label", sLabel, "");
    if((t = _expr(i, bi)) < 0) {
        return -1;
    } else {
        i = t;
    } 
    addCode("jz", eLabel, "");


    if((t = _block(i, bi)) < 0) {
        return -1;
    } else {
        i = t;
    }
    addCode("jmp", sLabel, "");
    addCode("label", eLabel, "");
    return i;
}
int KParser::_break(int begin,  BlockInfo bi) {
    int i = begin;
    int t;

    if (m_tokens[i].tokenType != TokenType::_break) {
        return -1;
    } 
    i++;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        return -1;
    }
    i++;
    addCode("jmp", bi.eLabel, "");
    return i;
}

int KParser::_continue(int begin,  BlockInfo bi) {
    int i = begin;
    int t;

    if (m_tokens[i].tokenType != TokenType::_continue) {
        return -1;
    } 
    i++;
    if (m_tokens[i].tokenType != TokenType::semicolon) {
        return -1;
    }
    i++;
    addCode("jmp", bi.sLabel, "");
    return i;
}

int KParser::_assign(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    int idIndex = i;
    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    } 
    i++; 
    if (m_tokens[i].tokenType != TokenType::assign) {
        return -1;
    } 
    i++; 

    int exprIndex = i;
    t = _expr(i, bi);
    if (t < 0) {
        char buf[256];
        sprintf(buf, "line[%d]: 표현식의 오류\n", m_tokens[i].line); 
        m_errorMsgs.push_back(buf);
        return -1;
    } 
    i = t;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        char buf[256];
        sprintf(buf, "line[%d]: ; 가 빠졌습니다.\n", m_tokens[i].line); 
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    addCode("pop", m_tokens[idIndex].str, "");
    return i;
} 

int KParser::_decassign(int begin,  BlockInfo bi) {
    int i = begin;
    int t;
    int idIndex = i;
    if (m_tokens[i].tokenType != TokenType::id) {
        return -1;
    } 
    i++; 
    if (m_tokens[i].tokenType != TokenType::decassign) {
        return -1;
    } 
    i++; 

    int exprIndex = i;
    t = _expr(i, bi);
    if (t < 0) {
        char buf[256];
        sprintf(buf, "line[%d]: decassign 표현식의 오류\n", m_tokens[i].line); 
        m_errorMsgs.push_back(buf);
        return -1;
    } 
    i = t;

    if (m_tokens[i].tokenType != TokenType::semicolon) {
        char buf[256];
        sprintf(buf, "line[%d]: _decassign ; 가 빠졌습니다.\n", m_tokens[i].line); 
        m_errorMsgs.push_back(buf);
        return -1;
    }
    i++;
    m_locals++;
    addCode("pop", m_tokens[idIndex].str, "");
    m_varToLocal[m_tokens[idIndex].str] = m_increaseLocals++;
    printf("%s => %d\n",  m_tokens[idIndex].str.c_str(), m_increaseLocals);
    return i;
}
```

굉장히 길다. 길기만 길지 어려운 부분은 없다. 앞서 정의한 EBNF 를 그대로 코드로 옮겨논 것에 불과하다. 이 옮기는 과정에서 버그는 나올 수 있고, 내가 작성한 코드 역시 버그가 있을 수 있다. 앞장에서 정의한 EBNF 과 이 코드와 비교하면서 읽어보기를 권한다.

# 지역변수, 함수인자의 심볼 제거

우리는 가상머신을 정의할 때 `segment index` 식으로 표기하기로 했다. 즉 가상머신코드로 변환이 될 때 지역변수이름, 함수인자이름 대신에 index 형태로 나타나야한다는 것을 뜻한다. 어떻게 숫자를 부여하며 구현했는지 설명한다.

여기서 가상머신과 결합하는 부분의 포인트만 짚고 넘어간다. `_decassign` 함수 부분은 지역변수를 선언하는 부분이다.    decassign 함수의  `m_varToLocal[m_tokens[idIndex].str] = m_increaseLocals++;` 를 보면 함수에서 쓰인 지역변수가 몇 번째 지역변수인지 탐색된 순서부터 숫자가 부여된다. 함수 해석이 다 끝날 때 `_func` 함수의 끝에 보면 

```cpp
for(auto& i: m_codes) {
    if(m_varToArg.find(i[1]) != m_varToArg.end()) {
        i[2] = std::to_string(m_varToArg[i[1]]);
        i[1] = "argument";
    }
    if(m_varToLocal.find(i[1]) != m_varToLocal.end()) {
        i[2] = std::to_string(m_varToLocal[i[1]]);
        i[1] = "local";
    }
}
```

여기서 `push variable_name` 와 같은 형식이 `push local 0` 와 같은 형식으로 바뀌게 된다. 이는 변수 선언할 때 `m_varToLocal` 로 인해 매겨진 순서에 의해 변환이 된다. local 뿐 아니라 arg 도 마찬가지로 심볼릭에서 인자의 숫자로 변환된다. 


# 함수의 인자의 개수  구하기

가상머신코드를 정의할 때 caller 쪽에서 부르는 함수가 인자가 몇 갠지 알아야 한다고 했다. `_func` 함수 밑에 parameter 해석하는 부분이 끝난 후 `m_funcToParams` 테이블에 어떤 함수이름이 몇 개의 인자를 가지는지 m_parameters 로 조사한 후, 테이블에 값을 넣는다. 

`m_funcToParams[funcName] = m_parameters;`

그리고  `_call` 부분에서 함수이름을 이용하여 바이트코드를 완성시킨다.

# 가상머신 구현

이상 앞에서 다룬 부분은 구문분석 단계에서 스택기반의 코드를 생성하는 것이 목표였다. 이제 생성된 스택기반의 코드를 실행하는 실행기를 만들어야 한다.

```cpp
class Vm {
    public:
        static const int STACK_FRAME_SIZE = 3;
        std::vector<std::vector<std::string>> m_program; 
        std::vector<int> m_stack; 
        std::map<std::string, int> m_labelToEip;
        std::map<std::string, int> m_funcToEip;
        int m_eip = 0;
        int m_argPointer = 0;
        int m_localPointer = 0;
        int m_stackPointer = 0;

        ...
```

vm.h 정의 부분이다. 4장에서 jump label 을 다루는 변수는 `m_labelToEip` 다. 이 변수는 label 에서 eip 로의 테이블이다. 마찬가지로 `m_funcToEip` 역시 어떤 함수가 몇 번째 코드에 있는지 나타낸다. 

# 어떻게 테이블을 채우나?

```cpp

main.cpp:
    kparser.parse();
    Vm vm;
    for(auto i: kparser.m_il) {
        vm.addCommand(i);
    }

vm.h:
    void addCommand(std::vector<std::string> k) {
        if (k[0] == "func") {
            m_funcToEip[k[1]] = m_program.size();
        } else if(k[0] == "label") {
            m_labelToEip[k[1]] = m_program.size();
        }
        m_program.push_back(k);
    }
```

명령어가 들어오면 현재 m_program.size() 가 하나씩 증가하는데 레이블과 관련된 func, label 키워드가 들어오면 labelToEip 테이블에 값을 넣는다. 추후 call 이나 jump 명령어가 들어오면 이 테이블을 참조하여 eip 를 얻는다.

# 명령어의 실행

```cpp
        int step() {
            if (m_program[m_eip].size() && m_eip <= m_program.size() - 1) {
                auto cmd = m_program[m_eip][0]; 
                if (cmd == "neq") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t1 != t2);
                    m_eip++;
                } else if (cmd == "eq") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t1 == t2);
                    m_eip++;
                } 
                else if (cmd == "lt") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 < t1);
                    m_eip++;
                }else if (cmd == "lte") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 <= t1);
                    m_eip++;
                }else if (cmd == "gt") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 > t1);
                    m_eip++;
                }else if (cmd == "gte") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 >= t1);
                    m_eip++;
                } 
                else if (cmd == "add") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t1 + t2);
                    m_eip++;
                } else if (cmd == "sub") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 - t1);
                    m_eip++;
                } else if (cmd == "mult") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 * t1);
                    m_eip++;
                } else if (cmd == "div") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 / t1);
                    m_eip++;
                } else if (cmd == "mod") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 % t1);
                    m_eip++;
                } else if (cmd == "neg") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    m_stack.push_back(-t1);
                    m_eip++;
                } else if (cmd == "and") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 && t1);
                    m_eip++;
                } else if (cmd == "or") {
                    int t1 = getStackTop();
                    m_stack.pop_back();
                    int t2 = getStackTop();
                    m_stack.pop_back(); 
                    m_stack.push_back(t2 || t1);
                    m_eip++;
                }

                else if (cmd == "push") {
                    if (m_program[m_eip].size() < 3) {
                        return -1;
                    } 
                    auto next = m_program[m_eip][1];
                    if (next == "local") {
                        auto pos = std::stoi(m_program[m_eip][2]);
                        m_stack.push_back(m_stack[m_localPointer + pos]);
                    } else if (next == "argument") {
                        auto pos = std::stoi(m_program[m_eip][2]);
                        m_stack.push_back(m_stack[m_argPointer + pos]);
                    } else if (next == "constant") {
                        auto pos = std::stoi(m_program[m_eip][2]);
                        m_stack.push_back(pos);
                    } else {
                        return -1;
                    }
                    m_eip++;
                } else if (cmd == "pop") {
                    if (m_program[m_eip].size() < 3) {
                        m_stack.pop_back();
                    } else {
                        auto next = m_program[m_eip][1];
                        if (next == "local") {
                            auto pos = std::stoi(m_program[m_eip][2]);
                            m_stack[m_localPointer + pos] = getStackTop();
                            m_stack.pop_back();
                        } else if (next == "argument") {
                            auto pos = std::stoi(m_program[m_eip][2]);
                            m_stack[m_argPointer + pos] = getStackTop();
                            m_stack.pop_back();
                        } else if (next == "constant") {
                            // not op
                        } else if (next == "") {
                            m_stack.pop_back(); 
                        } else {
                            return -1;
                        }
                    }
                    m_eip++;
                } 
                else if (cmd == "label") {
                    m_eip++;
                } else if (cmd == "jmp") {
                    auto next = m_program[m_eip][1];
                    m_eip = m_labelToEip[next]; // [next] = m_eip + 1;
                } else if (cmd == "jz") {
                    auto next = m_program[m_eip][1];
                    if (getStackTop() == 0) {
                        m_eip = m_labelToEip[next];
                    } else {
                        m_eip++;
                    }
                    m_stack.pop_back(); 
                } else if (cmd == "jnz") {
                    auto next = m_program[m_eip][1];
                    if (getStackTop() != 0) {
                        m_eip = m_labelToEip[next];
                    } else {
                        m_eip++;
                    }
                    m_stack.pop_back(); 
                } 
                else if (cmd == "call") {
                    // call fn 2
                    if (m_program[m_eip].size() < 3) {
                        std::cout << "<3\n";
                        return -1;
                    }
                    auto fn = m_program[m_eip][1];
                    if(fn == "print") {
                        int t = *m_stack.rbegin();
                        // m_stack.pop_back(); 
                        std::cout << t << "\n"; //  << "built-in-function\n";
                        m_eip++;
                    } else {
                        int argNumber = std::stoi(m_program[m_eip][2]);
                        int oldLocalPointer = m_localPointer;
                        int oldArgPointer = m_argPointer;
                        m_argPointer = m_stack.size() - argNumber;
                        m_stack.push_back(m_eip + 1);
                        m_stack.push_back(oldLocalPointer);
                        m_stack.push_back(oldArgPointer);
                        m_localPointer = m_stack.size();
                        m_eip = m_funcToEip[fn]; 
                    }
                } else if (cmd == "func") {
                    // call fn 2
                    if (m_program[m_eip].size() < 3) {
                        return -1;
                    }
                    auto fn = m_program[m_eip][1];
                    int locals = std::stoi(m_program[m_eip][2]);
                    for(int i=0; i<locals; i++) {
                        m_stack.push_back(0); // 지역변수를 전부 0 으로 초기화 함.
                    }
                    m_eip++;
                } else if (cmd == "return") {
                    int frame = m_localPointer;
                    int ret = m_stack[frame - STACK_FRAME_SIZE]; // 돌아갈 주소
                    m_stack[m_argPointer] = getStackTop();
                    int pops = m_argPointer + 1; // 해당 함수에서 단 하나의 숫자가 남을 때 까지 팝하기 위해서
                    m_argPointer = m_stack[frame - 1];
                    m_localPointer = m_stack[frame - 2];
                    while (m_stack.size() > pops) {
                        m_stack.pop_back();
                    }
                    m_eip = ret; 
                } else if (cmd == "exit") {
                    return -1;
                } 
                return 0;
            } 
            return -1;
        } 

```

step 함수의 구현부의 일부는 이전장에서 다뤘었다.

`push, pop, return, func, call, jnz, jz` 부분을 중심으로 테이블 변수가 어떻게 쓰이는지 스택 프레임을 어떻게 구성하는지 주의깊게 보면 좋겠다.
# 전체 Source

https://github.com/hsnks100/dreampiler

전체 소스는 위 github 에서 배포한다. 테스트코드는 main.cpp 에 하드코딩으로 넣어놨다. 파일로 빼는 것이 좋겠지만, 상용적으로 쓰이는 것이 아닌 학습의 목적이 있기 때문에 따로 테스트코드를 파일로 처리하진 않았다.  

