#ifndef BYTECODEINTERPRETER_H
#define BYTECODEINTERPRETER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm> 
#include <cctype>
#include "IR.h"

using namespace std;


struct StackValue {
    int value;
    bool isBoolean;
    StackValue() : value(0), isBoolean(false) {}
    StackValue(int v, bool isBool = false) : value(v), isBoolean(isBool) {}
};

struct ActivationRecord {
    map<string, StackValue> localVariables;
    int returnAddress;

    ActivationRecord(int returnAddress) : returnAddress(returnAddress) {}

};

class ByteCodeInterpreter {
private:
    vector<string> lines;
    vector<StackValue> stack;
    vector<ActivationRecord> activationStack;
    map<string, StackValue> globalVariables;
    map<string, int> methodPointers;
    int count = 0;
    int PriorAddress = 0;
    string priorMethodName, currentMethodName;

    void push(int value, bool isBoolean = false) {
        stack.emplace_back(StackValue(value, isBoolean));
    }
    
    void printMethodPointers() {
        cout << "Method pointers: " << endl;
        for (auto [key, value] : methodPointers) {
            cout << key << " : " << value << endl;
        }
    }

    void printActivationStack() {
        cout << "Activation stack: " << endl;
        for (auto &record : activationStack) {
            cout << "Return address: " << record.returnAddress << endl;
            cout << "Local variables: " << endl;
            for (auto [key, value] : record.localVariables) {
                cout << key << " : " << value.value << endl;
            }
        }
    }

    StackValue pop() {
        if (!stack.empty()) {
            StackValue value = stack.back();
            stack.pop_back();
            return value;
        } else {
            cerr << "Stack underflow\n";
            exit(EXIT_FAILURE);
        }
    }

    void printStack() {
        cout << "Stack: ";
        for (auto &value : stack) {
            cout << value.value;
            if (value.isBoolean) {
                cout << " (bool)";
            }
            cout << ", ";
        }
    cout << endl;
    }

    void executeInstruction(string instruction) {
        stringstream ss(instruction);
        string opcode;
        ss >> opcode;

        if (opcode == "iload") {
            string var;
            ss >> var;
            if (var == "True") {
                push(1, true);
            } else if (var == "False") {
                push(0, true);
            } else {
                auto &variables = activationStack.empty() ? globalVariables : activationStack.back().localVariables;
                auto it = variables.find(var);
                if (it != variables.end()) {
                    push(it->second.value, it->second.isBoolean);
                } else {
                    cerr << "Variable " << var << " not found\n";
                    exit(EXIT_FAILURE);
                }
            }
        } else if (opcode == "iconst") {
            int value;
            ss >> value;
            push(value);
        } else if (opcode == "istore") {
            string var;
            ss >> var;
            int idx = count, num = 0;
            if (strcmp(currentMethodName.c_str(), priorMethodName.c_str())  == 0){
                while (strcmp(lines[idx].substr(0,6).c_str(), "istore") == 0) {
                    num++;
                    idx++;
                }
            }
            vector<StackValue> temp;
            for (int i = num; i > 0; i--) {
                temp.push_back(pop());
            }
            
            for (int i = 0; i < num; i++) {
                push(temp[i].value, temp[i].isBoolean);
            }

            StackValue value = pop();
            auto &variables = activationStack.empty() ? globalVariables : activationStack.back().localVariables;
            variables[var] = value;
        } else if (opcode == "iadd") {
            StackValue a = pop();
            StackValue b = pop();
            push(a.value + b.value);
        } else if (opcode == "isub") {
            StackValue a = pop();
            StackValue b = pop();
            push(b.value - a.value);
        } else if (opcode == "imul") {
            StackValue a = pop();
            StackValue b = pop();
            push(a.value * b.value);
        } else if (opcode == "idiv") {
            StackValue a = pop();
            StackValue b = pop();
            push(b.value / a.value);
        } else if (opcode == "print") {
            StackValue value = pop();
            if (value.isBoolean) {
                cout << (value.value == 1 ? "true" : "false") << endl;
            } else {
                cout << value.value << endl;
            }
        } else if (opcode == "inot") {
            StackValue value = pop();
            push(value.value == 0 ? 1 : 0, true); 
        } else if (opcode == "iand") {
            StackValue a = pop();
            StackValue b = pop();
            push((a.value != 0 && b.value != 0) ? 1 : 0, true); 
        } else if (opcode == "ior") {
            StackValue a = pop();
            StackValue b = pop();
            push((a.value != 0 || b.value != 0) ? 1 : 0, true); 
        } else if (opcode == "igt") {
            StackValue a = pop();
            StackValue b = pop();
            push(b.value > a.value ? 1 : 0, true); 
        } else if (opcode == "ilt") {
            StackValue a = pop();
            StackValue b = pop();
            push(b.value < a.value ? 1 : 0, true);
        } else if (opcode == "ieq") {
            StackValue a = pop();
            StackValue b = pop();
            push(b.value == a.value ? 1 : 0, true);
        } else if (opcode == "goto") {
            string label;
            ss >> label;
            count = methodPointers[label]; 
        } else if (opcode == "iffalse") {
            string label = instruction.substr(13, instruction.length());
            StackValue condition = pop();
            if (condition.value == 0) {
                count = methodPointers[label];
            }
        } else if (opcode == "invokevirtual") {
            string methodName;
            ss >> methodName;
            int methodAddress = methodPointers[methodName];
            currentMethodName = methodName; 
            activationStack.emplace_back(count);
            count = methodAddress - 1;
        } else if (opcode == "ireturn") {
            if (!activationStack.empty()) {
                count = activationStack.back().returnAddress;
                activationStack.pop_back();
            } else {
                cerr << "Error: ireturn encountered with empty activation stack\n";
                exit(EXIT_FAILURE);
            }
        } else if (opcode == "stop") {
            exit(EXIT_SUCCESS);
        }        
        priorMethodName = opcode.substr(0, opcode.length() - 1);
    }

public:

    string Strip(string str) {
        auto it = find_if_not(str.begin(), str.end(), [](unsigned char ch) { return isspace(ch); });
        return string(it, str.end());
    }

    void Init(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        while (getline(file, line)) {
            line = Strip(line); 
            lines.push_back(line);
            if (line.back() == ':') {
                string methodName = line.substr(0, line.length() - 1);
                methodPointers[methodName] = lines.size() - 1;
            }
        }

        file.close();   
        Run();
    }

    void Run() {
        while (count < lines.size()) {
            executeInstruction(lines[count]);
            count++;
        }
    }
};


#endif