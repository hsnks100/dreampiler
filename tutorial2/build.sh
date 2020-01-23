#!/bin/bash

flex scanner.l
g++ -std=c++11 lex.yy.cc main.cpp kparser.cpp  && ./a.out
