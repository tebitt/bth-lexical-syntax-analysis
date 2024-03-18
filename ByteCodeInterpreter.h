#ifndef BYTECODEINTERPRETER_H
#define BYTECODEINTERPRETER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

class ByteCodeInterpreter {
private:
    vector<string> lines;
    map<string, int> variables;
    vector<int> stack;

    void push(int value) {
        stack.push_back(value);
    }

    int pop() {
        if (!stack.empty()) {
            int value = stack.back();
            stack.pop_back();
            return value;
        } else {
            cerr << "Stack underflow" << endl;
            exit(EXIT_FAILURE);
        }
    }

    void printStack() {
        cout << "Stack: ";
        for (int value : stack) {
            cout << value << " ";
        }
        cout << endl;
    }

public:
    void Init(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        for (string line; getline(file, line);) {
            lines.push_back(line);
        }
        file.close();
        main();
    }

    void main() {
        for (const string& inst : lines) {
            stringstream ss(inst);
            string opcode;
            ss >> opcode;
            // cout << "Executing: " << opcode << endl;

            if (opcode == "iload") {
                string var;
                ss >> var;
                push(variables[var]);
            } else if (opcode == "iconst") {
                int value;
                ss >> value;
                push(value);
            } else if (opcode == "istore") {
                string var;
                ss >> var;
                variables[var] = pop();
            } else if (opcode == "iadd") {
                int a = pop();
                int b = pop();
                push(a + b);
            } else if (opcode == "isub") {
                int b = pop();
                int a = pop();
                push(a - b);
            } else if (opcode == "imul") {
                int a = pop();
                int b = pop();
                push(a * b);
            } else if (opcode == "idiv") {
                int b = pop();
                int a = pop();
                push(a / b);
            } else if (opcode == "print") {
                cout << pop() << endl;
            } else if (opcode == "inot") {
                push(!pop());
            } else if (opcode == "iand") {
                int a = pop();
                int b = pop();
                push(a && b);
            } else if (opcode == "ieq") {
                int a = pop();
                int b = pop();
                push(a == b);
            } else if (opcode == "igt") {
                int b = pop();
                int a = pop();
                push(a > b);
            } else if (opcode == "ilt") {
                int b = pop();
                int a = pop();
                push(a < b);
            } else if (opcode == "ior") {
                int a = pop();
                int b = pop();
                push(a || b);
            } else if (opcode == "stop") {
                exit(EXIT_SUCCESS);
            }
            // Add more opcode handling as necessary
        }
    }
};

#endif
