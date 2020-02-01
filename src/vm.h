#include <iostream>
#include <vector>
#include <string>
#include <map>

int driver();
class Vm {
    public:
        static const int STACK_FRAME_SIZE = 3;
        std::vector<std::vector<std::string>> m_program; 
        std::vector<int> m_stack; 
        std::map<std::string, int> m_labelToEip;
        std::map<std::string, int> m_funcToEip;
        std::map<std::string, int> m_funcToLocals;
        int m_eip = 0;
        int m_argPointer = 78;
        int m_localPointer = 77;
        int m_stackPointer = 0;

        int getStackTop() {
            return m_stack[m_stack.size() - 1];
        }

        // void popStack() {
        //     m_stack.pop_back();
        // }
        std::vector<std::string> split(std::string prg, std::string delimiter) {
            std::string s = prg;
            std::vector<std::string> ret; 
            size_t pos = 0;
            std::string token;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                ret.push_back(token);
                s.erase(0, pos + delimiter.length());
            } 
            ret.push_back(s);
            return ret;
        }
        void loadProgram(std::string prg) {
            auto i = split(prg, "\n");
            int eip = 0;
            for(auto j: i) {
                auto k = split(j, " ");
                if (k[0] == "func") {
                    m_funcToEip[k[1]] = eip;
                    m_funcToLocals[k[1]] = std::stoi(k[2]); 
                } else if(k[0] == "label") {
                    m_labelToEip[k[1]] = eip;
                }
                m_program.push_back(k);
                eip++;
            }
            m_program.push_back({"call", "main", "0"});
            eip++;
            int line = 0;
            for(auto i: m_program) {
                std::cout << line << " ";
                for(auto j: i) {
                    std::cout << j << " ";
                }
                std::cout << std::endl;
                line++;
            }
            m_eip = eip - 1;
            std::cout << "entry point: " << m_eip << std::endl;
        }
        void addCommand(std::vector<std::string> k) {
            if (k[0] == "func") {
                m_funcToEip[k[1]] = m_program.size();
                m_funcToLocals[k[1]] = std::stoi(k[2]); 
            } else if(k[0] == "label") {
                m_labelToEip[k[1]] = m_program.size();
            }
            m_program.push_back(k);
        }
        int step() {
            // hana0720
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
                        std::cout << t << "built-in-function\n";
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
                    std::cout << "돌아갈 주소: " << m_eip << std::endl;
                } else if (cmd == "exit") {
                    return -1;
                } 

                // for(auto m: m_stack) {
                //     std::cout << m << std::endl;
                // }

                // std::cout << "------------\n";
                return 0;
            } 
            return -1;
        } 
};

