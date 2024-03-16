#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <map>
#include <string>
#include <vector>
#include <string>
#include "Node.h"

using namespace std;

class ArithmeticOperationNode : public Node {
public:
    ArithmeticOperationNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string irCode;
        // Assume children[0] and children[1] are the operands
        auto operand1 = children.front()->value;
        auto operand2 = children.back()->value;

        // Generating IR code for a simple addition operation
        if (type == "Addition") {
            irCode = "t1 = " + operand1 + " + " + operand2 + ";";
        }
        return irCode;
    }
};

class AssignmentNode : public Node {
public:
    AssignmentNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string irCode;
        // Assuming the first child is the variable and the second is the expression
        auto variable = children.front()->value;
        auto expression = children.back()->generateIR(); // Get IR code for the expression

        irCode = variable + " = " + expression + ";";
        return irCode;
    }
};

class IfNode : public Node {
public:
    IfNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string conditionIR = children.front()->generateIR(); // Assume the condition is the first child
        // Example for IfNode where you want the second (true branch) and optionally the third child (false branch)
        auto it = children.begin();
        std::advance(it, 1); // Move to the second child
        string trueBranchIR = (*it)->generateIR();

        string falseBranchIR = "";
        if (std::distance(children.begin(), children.end()) > 2) {
            std::advance(it, 1); // Move to the third child, if it exists
            falseBranchIR = (*it)->generateIR();
}


        // Generate labels for IR
        string labelTrue = "labelTrue";
        string labelFalse = "labelFalse";
        string labelEnd = "labelEnd";

        string irCode = "if " + conditionIR + " goto " + labelTrue + ";\n"
                        "goto " + labelFalse + ";\n"
                        + labelTrue + ": " + trueBranchIR + "\n"
                        "goto " + labelEnd + ";\n"
                        + labelFalse + ": " + (falseBranchIR.empty() ? "" : falseBranchIR + "\n")
                        + labelEnd + ": ;";

        return irCode;
    }
};

class WhileNode : public Node {
public:
    WhileNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string conditionIR = children.front()->generateIR(); // Condition
        string bodyIR = children.back()->generateIR(); // Body

        // Generate labels for IR
        string labelStart = "labelStart";
        string labelBody = "labelBody";
        string labelEnd = "labelEnd";

        string irCode = labelStart + ": ;\n"
                        "if " + conditionIR + " goto " + labelBody + ";\n"
                        "goto " + labelEnd + ";\n"
                        + labelBody + ": " + bodyIR + "\n"
                        "goto " + labelStart + ";\n"
                        + labelEnd + ": ;";

        return irCode;
    }
};

class FunctionCallNode : public Node {
public:
    FunctionCallNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string irCode = "call " + value + "(";
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it != children.begin()) irCode += ", ";
            irCode += (*it)->generateIR(); // Assuming the children generate the IR for arguments
        }
        irCode += ");";

        return irCode;
    }
};

class FunctionDefinitionNode : public Node {
public:
    FunctionDefinitionNode(string t, string v, int l) : Node(t, v, l) {}

    string generateIR() override {
        string functionName = value; // Assuming the function name is stored in `value`
        string irCode = "func_begin " + functionName + ":\n";
        
        // Generate IR for the function body, assuming the body is the last child
        string bodyIR = children.back()->generateIR();
        irCode += bodyIR;
        
        irCode += "\nfunc_end " + functionName + ";\n";
        return irCode;
    }
};

