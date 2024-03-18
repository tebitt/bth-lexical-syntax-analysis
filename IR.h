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

    string FormatText() {
        if (strcmp(op.c_str(), "=") == 0) return result + " := " + rhs; //Copy
        else if (strcmp(op.c_str(), "temporary_storage") == 0) return "";
        else if (strcmp(op.c_str(), "New") == 0) return result + " := " + op + " " + lhs + " " + rhs; //New
        else if (lhs.empty() && rhs.empty()) return op + " " + result; //Print, Return
        else if (strcmp(op.c_str(), "param") == 0) return op + " " + result; 
        else if (strcmp(op.substr(0,5).c_str(), "Call ") == 0) return result + " := " + op + ", " + rhs; //Call
        else if (strcmp(op.c_str(), "Goto") == 0) return op + " " + rhs; //Jump
        else if (strcmp(op.c_str(), "iffalse") == 0) return op + " " + result + " Goto " + rhs; //ConditionalJump
        else if (strcmp(op.c_str(), "stop") == 0) return "stop"; //Stop
        else return result + " := " + lhs + " " + op + " " + rhs; //Expression
    }

    list<string> FormatByteCode() {
        unordered_map<string, string> opMap = {{"+", "iadd"}, {"-", "isub"}, {"*", "imul"}, {"/", "idiv"}, {"<", "ilt"}, {">", "igt"}, {"==", "ieq"}, {"&&", "iand"}, {"||", "ior"}, {"!", "inot"}, {"Return", "ireturn"}, {"Print", "print"}};
        
        if (strcmp(op.c_str(), "+") == 0 || strcmp(op.c_str(), "-") == 0 || strcmp(op.c_str(), "*") == 0 || strcmp(op.c_str(), "/") == 0 || strcmp(op.c_str(), "<") == 0 || strcmp(op.c_str(), ">") == 0 || strcmp(op.c_str(), "==") == 0 || strcmp(op.c_str(), "&&") == 0 || strcmp(op.c_str(), "||") == 0) return {LOAD(lhs), LOAD(rhs), opMap[op], "istore " + result};
        else if (strcmp(op.c_str(), "!") == 0) return { LOAD(rhs), opMap[op], "istore " + result};
        else if (strcmp(op.c_str(), "=") == 0) return { LOAD(rhs), "istore " + result};
        else if (strcmp(op.c_str(), "Goto") == 0) return {"goto " + rhs};
        else if (strcmp(op.c_str(), "iffalse") == 0) return { LOAD(result), "iffalse goto " + rhs};
        else if (strcmp(op.c_str(), "Print") == 0 ||strcmp(op.c_str(), "Return") == 0) return { LOAD(result), opMap[op]};
        else if (strcmp(op.c_str(), "param") == 0 && (strcmp(lhs.c_str(), "header") != 0)) return { LOAD(result) };
        else if (strcmp(op.substr(0,5).c_str(), "Call ") == 0 ) return { "invokevirtual " + op.substr(5, op.size()), "istore " + result};
        else if (strcmp(op.c_str(), "stop") == 0) return {"stop"};
        else if (strcmp(op.c_str(), "temporary_storage") == 0) return {"istore " + rhs};
        else return {};
    }

    string LOAD(string var){
        if (strcmp(var.c_str(),"true") == 0 || strcmp(var.c_str(),"false") == 0 || isNum(var)) return "iconst " + var;
        else return "iload " + var;
    }

    bool isNum(string var){
        for (int i = 0; i < var.size(); i++) {
            if (!isdigit(var[i])) return false;
        }
        return true;
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
    bool isExit = false;
    bool Visited = false;

    BasicBlock() : idx(0), block("block_" + to_string(idx)), name(""), Entry(false) {}

    BasicBlock(int i) : idx(i), block("block_" + to_string(idx)) {}

    BasicBlock(int i, string name, bool IsEntry) : idx(i), block("block_" + to_string(idx)), name(name), Entry(IsEntry) {}

    void printInstructions() {
        for (auto i = Instructions.begin(); i != Instructions.end(); i++) {
            cout << (*i)->FormatText() << endl;
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
    map<string, int> methodCount;
    string MainClassName;


    void IRInit(Node* root) {
        generateIR(root);
    }

    void generateIR(Node* node) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;

        if ((strcmp(type.c_str(),"ClassDeclaration") == 0) || (strcmp(type.c_str(),"MainClassDeclaration") == 0)) {
            auto i = node->children.begin();
            if (strcmp(type.c_str(),"MainClassDeclaration") == 0) MainClassName = (*i)->value;
            CurrentClass = (*i)->value;
        }

        if (strcmp(type.c_str(),"MethodDeclaration") == 0 ) {
            auto i = node->children.begin();
            string Name;
            i++;
            CurrentMethod = (*i)->value;
            if (CurrentMethod.empty()) Name = CurrentClass;
            else Name = CurrentClass + "." + CurrentMethod;
            BasicBlock* Entry = new BasicBlock(idx++, Name, true);
            Statement.push_back(Entry);
            EntryBlocks.push_back(Entry);
            if (strcmp(((*++i)->type).c_str(), "TypeIdentList") == 0 ) {
                for (auto param = (*i)->children.begin(); param != (*i)->children.end(); param++) {
                    string Type = (*param)->value;
                    string Name = (*++param)->value;
                    if (strcmp(Type.c_str(), "Int") == 0 || strcmp(Type.c_str(), "Boolean") == 0) {
                        TAC* in = new TAC("temporary_storage", Type, Name, "");
                        Entry->Instructions.push_back(in);
                    }
                }
            }
            currentBlock = Entry;
            BasicBlock* FirstBlock = new BasicBlock(idx++);
            Statement.push_back(FirstBlock);
            Entry->TrueExit = FirstBlock;
            currentBlock = FirstBlock;
            FirstBlock->printInstructions();
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
  
         if (strcmp(type.c_str(), "MethodDeclaration") == 0) {
            string Name;
            if (CurrentMethod.empty()) Name = CurrentClass;
            else Name = CurrentClass + "." + CurrentMethod;
            BasicBlock* Exit = new BasicBlock(idx++, "Exit." + Name, false);
            Exit->isExit = true;
            if (strcmp(CurrentClass.c_str(), MainClassName.c_str()) == 0) {
                TAC* in = new TAC("stop", "", "", "");
                Exit->Instructions.push_back(in);
            }
            Statement.push_back(Exit);
            currentBlock->TrueExit = Exit;
        }
    }

    void STMT(Node* node) {
        string type = node->type;
        string value = node->value;
        string name, lhs, rhs, result, cond;
        lineno = node->lineno;

        if (strcmp(value.c_str(), "StatementList") == 0){
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                STMT(*i);
            }
        } else if (strcmp(value.c_str(), "If") == 0) {
            auto i = node->children.begin();
            cond = EXP(*i);
            BasicBlock* TrueExit = new BasicBlock(idx++);
            Statement.push_back(TrueExit);
            BasicBlock* Exit = new BasicBlock(idx++);
            Exit->isExit = true;
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
            BasicBlock* IfBlock = new BasicBlock(idx++);
            Statement.push_back(IfBlock);
            BasicBlock* ElseBlock = new BasicBlock(idx++);
            Statement.push_back(ElseBlock);
            BasicBlock* Exit = new BasicBlock(idx++);
            Exit->isExit = true;
            Statement.push_back(Exit);

            currentBlock->FalseExit = ElseBlock;
            TAC *in = new TAC("iffalse", "", ElseBlock->block, cond);
            currentBlock->Instructions.push_back(in);

            currentBlock->TrueExit = IfBlock;
            TAC *in2 = new TAC("Goto", "", IfBlock->block, "");
            currentBlock->Instructions.push_back(in2);
            
            currentBlock = IfBlock;
            STMT(*++i);
            currentBlock->TrueExit = Exit;
            TAC *in3 = new TAC("Goto", "", Exit->block, "");
            currentBlock->Instructions.push_back(in3);
            
            currentBlock = ElseBlock;
            STMT(*++i);
            ElseBlock->TrueExit = Exit;
            TAC *in4 = new TAC("Goto", "", Exit->block, "");
            currentBlock->Instructions.push_back(in4);

            currentBlock = Exit;
        } else if (strcmp(value.c_str(), "While") == 0) {
            auto i = node->children.begin();
            cond = EXP(*i);

            BasicBlock* Cond = new BasicBlock(idx++);
            Statement.push_back(Cond);

            for (auto In = currentBlock->Instructions.begin(); In != currentBlock->Instructions.end(); In++) {
                if ((*In)->op == "<" || (*In)->op.substr(0,4) == "Goto" || (*In)->op == ">" || (strcmp((*In)->op.c_str(), "==") == 0)  || (*In)->op == "&&" || (*In)->op == "||" || (*In)->op == "!") {
                    Cond->Instructions.push_back(*In);
                }
            }

            for (auto In = Cond->Instructions.begin(); In != Cond->Instructions.end(); In++) {
                currentBlock->Instructions.remove(*In);
            }

            BasicBlock* Loop = new BasicBlock(idx++);
            Statement.push_back(Loop);
            BasicBlock* Exit = new BasicBlock(idx++);
            Exit->isExit = true;
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
            currentBlock->TrueExit = Cond;
            TAC *in4 = new TAC("Goto", "", Cond->block, "");
            currentBlock->Instructions.push_back(in4);

            currentBlock = Exit;
        } else if (strcmp(value.c_str(), "Assign") == 0) {
            auto i = node->children.begin();
            name = EXP(*i);
            rhs = EXP(*++i);
            TAC* in = new TAC("=", "", rhs, name);
            currentBlock->Instructions.push_back(in);
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
            string op;
            string rhs = EXP(*i);
            string name = genName();
            if (strcmp(value.c_str(), "Not") == 0) op = "!";
            else op = "length";
            TAC* in = new TAC(op, "", rhs, name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if ((strcmp(value.c_str(), "And") == 0) || (strcmp(value.c_str(), "Or") == 0) || (strcmp(value.c_str(), "Plus") == 0) || (strcmp(value.c_str(), "Minus") == 0) || (strcmp(value.c_str(), "Multiply") == 0) || (strcmp(value.c_str(), "Divide") == 0) || (strcmp(value.c_str(), "LessThan") == 0) || (strcmp(value.c_str(), "GreaterThan") == 0) || (strcmp(value.c_str(), "Equal") == 0))  {
            unordered_map<string, string> opMap = {{"And", "&&"}, {"Or", "||"}, {"Plus", "+"}, {"Minus", "-"}, {"Multiply", "*"}, {"Divide", "/"}, {"LessThan", "<"}, {"GreaterThan", ">"}, {"Equal", "=="}};
            auto i = (node->children).begin();
            string lhs = EXP(*i);
            string rhs = EXP(*++i);
            string name = genName();;
            TAC* in = new TAC(opMap[value], lhs, rhs, name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if (strcmp(value.c_str(), "NewArray") == 0) {
            auto i = node->children.begin();
            string rhs = EXP(*i);
            string name = genName();
            TAC* in = new TAC("NewArray", "IntArray", rhs, name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if (strcmp(value.c_str(), "NewIdentifier") == 0) {
            auto i = node->children.begin();
            string lhs = EXP(*i);
            string name = genName();
            TAC* in = new TAC("New", lhs, "", name);
            currentBlock->Instructions.push_back(in);
            return name;
        } else if (strcmp(value.c_str(), "Call") == 0) {
            auto i = node->children.begin();
            string param = EXP(*i);
            TAC* in = new TAC("param", "header", "", param);
            currentBlock->Instructions.push_back(in);
            string methodName;
            if (strcmp((*i)->value.c_str(), "This") == 0) methodName = CurrentClass;
            else if (strcmp((*i)->value.c_str(), "NewIdentifier") == 0) methodName = (*(*i)->children.begin())->value; 
            else methodName = (*i)->value;
            i++;
            methodName += "." + (*(*i)->children.begin())->value;
            for (auto param = (*i)->children.begin(); param != (*i)->children.end(); param++) {
                string arg = EXP(*param);
                TAC* in1 = new TAC("param", "header", "", arg);
                currentBlock->Instructions.push_back(in1);
            } 
            i++;
            string Args;
            for (auto param = (*i)->children.begin(); param != (*i)->children.end(); param++) {
                Args = EXP(*param);
                TAC* in2 = new TAC("param", "", "", Args);
                currentBlock->Instructions.push_back(in2);
            }  
            string name = genName();
            TAC* in3 = new TAC("Call " + methodName, Args, to_string((*i)->children.size()), name);
            currentBlock->Instructions.push_back(in3);
            return name;
        } else if ((strcmp(value.c_str(), "True") == 0) || (strcmp(value.c_str(), "False") == 0)){
            return value;
        } else if (strcmp(type.c_str(), "IntegerLiteral") == 0) { 
            int num = stoi(value);
            return to_string(num);
        } else if (strcmp(type.c_str(), "Identifier") == 0) {
            return value;
        } else if (strcmp(value.c_str(), "This") == 0) { 
            return CurrentClass;
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

        for (auto In = Block->Instructions.begin(); In != Block->Instructions.end(); In++) {
            if (strcmp((*In)->op.c_str(),"temporary_storage") != 0) tree += (*In)->FormatText() + "\n";
        }
        *outStream << "n" << idx << " [label=\"" << tree << "\"];" << endl;

        if (Block->TrueExit != NULL) {
            *outStream << "n" << idx << " -> n" << Block->TrueExit->idx << " [xlabel= \" true \"];" << endl;
        }
        
        if (Block->FalseExit != NULL) {
            *outStream << "n" << idx << " -> n" << Block->FalseExit->idx << " [xlabel= \" false \"];" << endl;
        }
    }

    void generateBytecode() {
        std::ofstream outStream;
        char *filename = "bytecode.class";
        outStream.open(filename);

        for (auto Block = EntryBlocks.begin(); Block != EntryBlocks.end(); Block++) {
            generateBytecodeContent(&outStream, *Block);
        }
        outStream.close();
    }
    
    void generateBytecodeContent(std::ofstream *outStream, BasicBlock* Block) {
        if (Block->Visited) return;
        Block->Visited = true;
        if (Block->Entry) {
            if (!(Block->name.empty())) {
                *outStream << Block->name + ":" << std::endl;
                FormatByteCode(outStream ,Block);
            }
        } else {
            *outStream << "\t" + Block->block + ":" << std::endl;
            FormatByteCode(outStream, Block);
        }  

        if (Block->TrueExit != NULL){
            generateBytecodeContent(outStream, Block->TrueExit);
        }
        if (Block->FalseExit != NULL) {
            generateBytecodeContent(outStream, Block->FalseExit);
        }
    }
    
    void FormatByteCode(std::ofstream *outStream, BasicBlock* Block) {
        for (auto In = Block->Instructions.begin(); In != Block->Instructions.end(); In++) {
            list<string> ByteCode = (*In)->FormatByteCode();
            for (auto i = ByteCode.begin(); i != ByteCode.end(); i++) {
                if (Block->Entry) *outStream << "\t" + *i << std::endl;
                else if (Block->isExit )*outStream << "\t" + *i << std::endl;
                else *outStream << "\t\t" + *i << std::endl;
            }
        }
    }     
};

#endif