#ifndef IR_H
#define IR_H

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
#include <memory>

using namespace std;

class TAC {
    public:
    string op;       // Operation
    string lhs;      // Left-hand side
    string rhs;      // Right-hand side
    string result;   // Result variable

    TAC(string operation, string left, string right, string res) : op(operation), lhs(left), rhs(right), result(res) {}

    void setOP(string operation) { op = operation; }
    void setLHS(string left) { lhs = left; }
    void setRHS(string right) { rhs = right; }
    void setResult(string res) { result = res; }
    string getOP() { return op; }
    string getLHS() { return lhs; }
    string getRHS() { return rhs; }
    string getResult() { return result; }

    string Format(){
        if (strcmp(op.c_str(), "=") == 0) return result + " " + op + " " + rhs;
        else if (lhs.empty() && rhs.empty()) return op + " " + result;
        else if (strcmp(op.c_str(), "iffalse") == 0) return op + " " + result + " Goto " + rhs;
        else if (strcmp(op.c_str(), "Goto") == 0) return op + " " + rhs;
        else if (strcmp(op.c_str(), "new") == 0) return result + " := " + op + " " + lhs + " " + rhs;
        else return result + " := " + lhs + " " + op + " " + rhs;
    }
};

class BasicBlock {
public:
    int idx;
    string name, block;
    list<TAC*> Instructions;
    TAC* Condition;
    BasicBlock* TrueExit = NULL;
    BasicBlock* FalseExit = NULL;
    bool Entry = false;

    BasicBlock() : idx(0), block("block_" + to_string(idx)), name(""), Entry(false) {}

    BasicBlock(int i) : idx(i), block("block_" + to_string(idx)) {}

    BasicBlock(int i, string name, bool IsEntry) : idx(i), block("block_" + to_string(idx)), name(name), Entry(IsEntry) {}

    void printInstructions() {
        for (auto i = Instructions.begin(); i != Instructions.end(); i++) {
            cout << (*i)->Format() << endl;
        }
    }
};

class IR {
public:
    int temp = 0;
    int idx = 0;
    int lineno;
    string CurrentClass, CurrentMethod; 
    BasicBlock* currentBlock;
    list<string> BlockNames;
    list<BasicBlock*> Statement;
    list<BasicBlock*> EntryBlocks;
    string name, lhs, rhs, result;


    void IRInit(Node* root) {
        generateIR(root);
    }

    void generateIR(Node* node) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;

        if ((strcmp(type.c_str(),"ClassDeclaration") == 0) || (strcmp(type.c_str(),"MainClassDeclaration") == 0)) {
            auto i = node->children.begin();
            CurrentClass = (*i)->value;
        }

        if (strcmp(type.c_str(),"MethodDeclaration") == 0 || (strcmp(type.c_str(),"MainClassDeclaration") == 0)) {
            auto i = node->children.begin();
            string Name;
            if (((*++i)->value).empty()) Name = CurrentClass;
            else Name = CurrentClass + "." + (*i)->value;
            BasicBlock* Entry = new BasicBlock(idx++, Name, true);
            cout << "val: " << (*i)->value << endl;
            Statement.push_back(Entry);
            EntryBlocks.push_back(Entry);
            currentBlock = Entry;
        }  

        if (strcmp(type.c_str(), "Return") == 0) {
            string type = node->type;
            string value = node->value;
            string name, lhs, rhs, result;
            auto i = node->children.begin();
            name = EXP(*i);
            TAC* in = new TAC("Return", "", "", name);
            currentBlock->Instructions.push_back(in);
        } else if (strcmp(type.c_str(), "Statement") == 0) {
            STMT(node);
        } else if (strcmp(type.c_str(), "Expression") == 0) { 
            EXP(node); 
        } else {
            for (Node* &ch : node->children) { 
                generateIR(ch);
            }
        }
  
         if (strcmp(type.c_str(), "MethodDeclaration") == 0  || (strcmp(type.c_str(),"MainClassDeclaration") == 0)) {
            string Name;
            if (CurrentMethod.empty()) Name = CurrentClass;
            else Name = CurrentClass + "." + CurrentMethod;
            BasicBlock* Exit = new BasicBlock(idx++, "Exit." + Name, false);
            Statement.push_back(Exit);
            currentBlock->TrueExit = Exit;
            for (auto a = Statement.begin(); a != Statement.end(); a++) {
                (*a)->printInstructions();
            }
        }
    }

    void STMT(Node* node) {
        string type = node->type;
        string value = node->value;
        string name, lhs, rhs, result, cond;
        lineno = node->lineno;

        if (strcmp(type.c_str(), "StatementList") == 0){
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                STMT(*i);
            }
        } else if (strcmp(value.c_str(), "If") == 0) {
            auto i = node->children.begin();
            cond = EXP(*i);
            BasicBlock* TrueExit = new BasicBlock(idx++);
            Statement.push_back(TrueExit);
            BasicBlock* Exit = new BasicBlock(idx++);
            Statement.push_back(Exit);
            currentBlock->FalseExit = Exit;
            TAC *in = new TAC("iffalse", "", Exit->block, cond);
            currentBlock->Instructions.push_back(in);
            currentBlock->TrueExit = TrueExit;
            TAC *in2 = new TAC("Goto", "", TrueExit->block, "");
            currentBlock->Instructions.push_back(in2);
            currentBlock = TrueExit;
            STMT(*++i);
            TrueExit->TrueExit = Exit;
            TAC *in3 = new TAC("Goto", "", Exit->block, "");
            currentBlock->Instructions.push_back(in3);
            currentBlock = Exit;
        } else if (strcmp(value.c_str(), "IfElse") == 0) {
            auto i = node->children.begin();
            cond = EXP(*i);
            BasicBlock* TrueExit = new BasicBlock(idx++);
            Statement.push_back(TrueExit);
            BasicBlock* FalseExit = new BasicBlock(idx++);
            Statement.push_back(FalseExit);
            BasicBlock* Exit = new BasicBlock(idx++);
            Statement.push_back(Exit);
            currentBlock->FalseExit = FalseExit;
            TAC *in = new TAC("iffalse", "", FalseExit->block, cond);
            currentBlock->Instructions.push_back(in);
            currentBlock->TrueExit = TrueExit;
            TAC *in2 = new TAC("Goto", "", TrueExit->block, "");
            currentBlock->Instructions.push_back(in2);
            currentBlock = TrueExit;
            STMT(*++i);
            TrueExit->TrueExit = Exit;
            TAC *in3 = new TAC("Goto", "", Exit->block, "");
            currentBlock->Instructions.push_back(in3);
            currentBlock = FalseExit;
            STMT(*++i);
            FalseExit->TrueExit = Exit;
            TAC *in4 = new TAC("Goto", "", Exit->block, "");
            currentBlock->Instructions.push_back(in4);
            currentBlock = Exit;
        } else if (strcmp(value.c_str(), "While") == 0) {
            auto i = node->children.begin();
            cond = EXP(*i);
            BasicBlock* Cond = new BasicBlock(idx++);
            Statement.push_back(Cond);
            BasicBlock* Loop = new BasicBlock(idx++);
            Statement.push_back(Loop);
            BasicBlock* Exit = new BasicBlock(idx++);
            Statement.push_back(Exit);
            currentBlock->TrueExit = Cond;
            TAC *in = new TAC("Goto", "", Cond->block, "");
            currentBlock->Instructions.push_back(in);
            currentBlock = Cond;
            Cond->FalseExit = Exit;
            TAC *in2 = new TAC("iffalse", "", Exit->block, cond);
            currentBlock->Instructions.push_back(in2);
            currentBlock->TrueExit = Loop;
            TAC *in3 = new TAC("Goto", "", Loop->block, "");
            currentBlock->Instructions.push_back(in3);
            currentBlock = Loop;
            STMT(*++i);
            Loop->TrueExit = Cond;
            TAC *in4 = new TAC("Goto", "", Cond->block, "");
            currentBlock->Instructions.push_back(in4);
            currentBlock = Exit;
        } else if (strcmp(value.c_str(), "Assign") == 0) {
            auto i = node->children.begin();
            name = EXP(*i);
            rhs = EXP(*++i);
            TAC* in = new TAC("=", "", rhs, name);
            currentBlock->Instructions.push_back(in);
            currentBlock->printInstructions();
        } else if (strcmp(value.c_str(), "Print") == 0) {
            auto i = node->children.begin();
            name = EXP(*i);
            TAC* in = new TAC("Print", "", "", name);
            currentBlock->Instructions.push_back(in);
        }
    }

    string EXP(Node* node) {
        auto type = node->type;
        auto value = node->value;
        int id = node->id;
        lineno = node->lineno;

        if (strcmp(value.c_str(), "Not") == 0 || strcmp(value.c_str(), "ArrayLength") == 0) { 
            auto i = node->children.begin();
            name = genName();
            lhs = "";
            rhs = EXP(*i);
            TAC* in = new TAC(value, "", rhs, name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if ((strcmp(value.c_str(), "And") == 0) || (strcmp(value.c_str(), "Or") == 0) || (strcmp(value.c_str(), "Plus") == 0) || (strcmp(value.c_str(), "Minus") == 0) || (strcmp(value.c_str(), "Multiply") == 0) || (strcmp(value.c_str(), "Divide") == 0) || (strcmp(value.c_str(), "LessThan") == 0) || (strcmp(value.c_str(), "GreaterThan") == 0) || (strcmp(value.c_str(), "Equal") == 0))  {
            unordered_map<string, string> opMap = {{"And", "&&"}, {"Or", "||"}, {"Plus", "+"}, {"Minus", "-"}, {"Multiply", "*"}, {"Divide", "/"}, {"LessThan", "<"}, {"GreaterThan", ">"}, {"Equal", "=="}};
            auto i = (node->children).begin();
            name = genName();
            lhs = EXP(*i);
            rhs = EXP(*++i);
            TAC* in = new TAC(opMap[value], lhs, rhs, name);
            currentBlock->Instructions.push_back(in);
            currentBlock->printInstructions();
            return name;
        } else if (strcmp(value.c_str(), "NewArray") == 0) {
            auto i = node->children.begin();
            name = genName();
            rhs = EXP(*i);
            TAC* in = new TAC("NewArray", "IntArray", rhs, name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if (strcmp(value.c_str(), "NewIdentifier") == 0) {
            auto i = node->children.begin();
            name = genName();
            lhs = EXP(*i);
            TAC* in = new TAC("New", lhs, "", name);
            currentBlock->printInstructions();
            currentBlock->Instructions.push_back(in);
            return name;
        } else if (strcmp(value.c_str(), "Call") == 0) {
            auto i = node->children.begin();
            string param = EXP(*i);
            TAC* in = new TAC("param", "", "", param);
            currentBlock->Instructions.push_back(in);
            i++;

            string methodName = EXP(*i++);
            for (auto param = (*i)->children.begin(); param != (*i)->children.end(); param++) {
                string arg = EXP(*param);
                TAC* in = new TAC("param", "", "", arg);
                currentBlock->Instructions.push_back(in);
            }
            name = genName();
            TAC* in2 = new TAC("Call"+methodName, "", to_string((*i)->children.size() + 1), name);
            return name;
        } else if ((strcmp(value.c_str(), "True") == 0) || (strcmp(value.c_str(), "False") == 0)){
            return value;
        } else if (strcmp(type.c_str(), "IntegerLiteral") == 0) { 
            int num = stoi(value);
            return to_string(num);
        } else if (strcmp(type.c_str(), "Identifier") == 0) {
            return value;
        } 
        return "null";
    }

    string genName() {
        return "_t" + to_string(temp++);
    }

    void generate_ir_tree() {
        std::ofstream outStream;
		char *filename = "ir.dot";
		outStream.open(filename);

        int count = -1;
		outStream << "digraph {" << std::endl;
        outStream << "graph [splines=ortho]" << std::endl;
        outStream << "node [shape=box];" << std::endl;
        for (auto Block = Statement.begin(); Block != Statement.end(); Block++) {
            generate_ir_content(&outStream, *Block);
        }
		outStream << "}" << std::endl;
		outStream.close();

		printf("\nBuilt an ir-tree at %s. Use 'make ir' to generate the pdf version.\n", filename);
    }

    void generate_ir_content(std::ofstream *outStream, BasicBlock* Block) {
        int idx = Block->idx;
        string BlockName = Block->block;
        string tree;

        if (Block->name != "") tree = "[" + BlockName + " : " + Block->name + "]" + "\n.....\n";
        else tree = "[" + BlockName + "]" + "\n.....\n";

        for (auto In = Block->Instructions.begin(); In != Block->Instructions.end(); In++) tree += (*In)->Format() + "\n";

        *outStream << "n" << idx << " [label=\"" << tree << "\"];" << endl;

        if (Block->TrueExit != NULL) {
            *outStream << "n" << idx << " -> n" << Block->TrueExit->idx << " [xlabel= \" true \"];" << endl;
        }
        
        if (Block->FalseExit != NULL) {
            *outStream << "n" << idx << " -> n" << Block->FalseExit->idx << " [xlabel= \" false \"];" << endl;
        }
    }

    void generate_byte_code() {

    }
};

#endif