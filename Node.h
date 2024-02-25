#ifndef NODE_H
#define NODE_H

#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Node {
public:
    int id, lineno;
    string type, value;
    list<Node*> children;

    Node(string t, string v, int l) : type(t), value(v), lineno(l) {}
	Node()
	{
		type = "uninitialised";
		value = "uninitialised";
	} // Bison needs this.

    string getType() { return type; }
    string getValue() { return value; }
    int getLineNo() { return lineno; }

    void print_tree(int depth = 0)
	{
		for (int i = 0; i < depth; i++)
			cout << "  ";
		cout << type << ":" << value << endl; //<< " @line: "<< lineno << endl;
		for (auto i = children.begin(); i != children.end(); i++)
			(*i)->print_tree(depth + 1);
	}

	void generate_tree()
	{
		std::ofstream outStream;
		char *filename = "tree.dot";
		outStream.open(filename);

		int count = 0;
		outStream << "digraph {" << std::endl;
		generate_tree_content(count, &outStream);
		outStream << "}" << std::endl;
		outStream.close();

		printf("\nBuilt a parse-tree at %s. Use 'make tree' to generate the pdf version.\n", filename);
	}

	void generate_tree_content(int &count, ofstream *outStream)
	{
		id = count++;
		if (value != "")
		{
			*outStream << "n" << id << " [label=\"" << type << ":" << value << " line : " << lineno << "\"];" << endl;
		}
		else
		{
			*outStream << "n" << id << " [label=\"" << type << " line : " << lineno << "\"];" << endl;
		}

		for (auto i = children.begin(); i != children.end(); i++)
		{
			(*i)->generate_tree_content(count, outStream);
			*outStream << "n" << id << " -> n" << (*i)->id << endl;
		}
	}
};

class Record {
public:
    int id;
    string name;
    string record;
    string type;

    virtual ~Record() {}

    Record(): name(""), record(""), type("") {}

    void print() {
        cout << "Name: " << name << ", Record: " << record << ", Type: " << type << endl;
    }

    void setId(int i) { id = i; }
    void setName(string n) { name = n; }
    void setType(string t) { type = t; }
    int getId() { return id; }
    string getName() { return name; }
    string getRecord() { return record; }
    string getType() { return type; }

};



class Variable : public Record {
public:
    Variable() { record = "Variable"; }
};

class Method : public Record {
public:
    map<string, Variable*> variables;
    list<string> parameters;
    int parametersCount = 0;

    Method() { record = "Method"; }

    void printParameters() const {
        for (const auto& param : parameters) {
            cout << param << " ";
        }
        cout << endl;
    }

    void printVariables() const {
        cout << "Variables for " << name << " Method: ";
        for (const auto& var : variables) {
            cout << var.second->name << " ";
        }
        cout << endl;
    }
};

class Class : public Record {
public:
    map<string, Variable*> variables;
    map<string, Method*> methods;

    Class() { record = "Class"; }

    Method* getMethod(string key) {
        auto iter = methods.find(key);
        if (iter != methods.end()) {
            return iter->second;
        } else {
            return new Method();
        }
    }

    void printVariables() const {
        cout << "Variables for " << name << " class: ";
    }

    void printMethods() const {
        cout << "Methods for " << name << " class: ";
    }
};

class Scope {
public:
    Scope* parent;
    vector<Scope*> children;
    int depth = 0;
    int id;
    map<string, Class*> classes;
    map<string, Method*> methods;
    map<string, Variable*> variables;
    map<string, Variable*> parent_var;
    string name, record, type;

    Scope(int id, string name, string record, string type)
        : id(id), name(name), record(record), type(type) {}

    void addChildScope(int id, Record* rec) {
        Scope* child = new Scope(id, rec->name, rec->record, rec->type);
        child->parent = this;
        child->depth = this->depth + 1;
        child->parent_var = this->variables;
        children.push_back(child);

    }

    Scope* getChildScope(int id = -1) {
        if (id == -1) {
            return children.back();
        } else {
            auto it = children.begin();
            for (int i = 0; i < id; i++) {
                it++;
            }
            return *it;
        }
        return children.back();
    }


    void addRecord(Record* rec) {
        if (rec->record == "Class") {
            classes[dynamic_cast<Class*>(rec)->name] = dynamic_cast<Class*>(rec);
        } else if (rec->record == "Method") {
            methods[dynamic_cast<Method*>(rec)->name] = dynamic_cast<Method*>(rec);
        } else if (rec->record == "Variable") {
            variables[dynamic_cast<Variable*>(rec)->name] = dynamic_cast<Variable*>(rec);
        }
    }

    string findRecord(string key) {
        if (classes.find(key) != classes.end()) {
            return "Class";
        } else if (methods.find(key) != methods.end()) {
            return "Method";
        } else if (variables.find(key) != variables.end()) {
            return "Variable";
        } else {
            return "null";
        }
    }

    Record* getRecordFromName(const string& key) const {
        if (classes.find(key) != classes.end()) {
            return classes.at(key);
        } else if (methods.find(key) != methods.end()) {
            return methods.at(key);
        } else if (variables.find(key) != variables.end()) {
            return variables.at(key);
        } else {
            return nullptr;
        }
    }
};

class SymbolTable {
public:
    Scope* currentScope;
    Scope* rootScope;
    int nextScopeId = 0;
    bool CorrectSemantic = true;
    Class* CurrentClass;
    Method* CurrentMethod;
    int lineno, classDepth, methodDepth;
    string priorRecordType;
    
    SymbolTable() {
        currentScope = new Scope(0, "GoalName", "Goal", "void");
        nextScopeId++;
        rootScope = currentScope;
    }

    void main(Node* node) {
        int id = node->id;
        int lineno = node->lineno;
        list<Node*> children = node->children;
        auto child = children.begin();

        // cout << "ID: " << id << " Type: " << node->type << " Value: " << node->value << " LineNo: "<< lineno << endl;
        if (node->type == "ClassDeclaration" || node->type == "MainClassDeclaration") { ClassDeclaration(*child); } 
        else if (node->type == "MethodDeclaration") { MethodDeclaration(node); }
        else if (node->type == "VarDeclaration") { 
            VarDeclaration(*child, node); } 

        for (Node* &ch : node->children) { main(ch); } 

        if (node->type == "ClassDeclaration" || node->type == "MainClassDeclaration" || node->type == "MethodDeclaration") { 
        exitCurrentScope(); }
    }

    void ClassDeclaration(Node* node) {
        priorRecordType = "c";
        lineno = node->lineno;
        CurrentClass = new Class();
        CurrentClass->setName(node->value);
        CurrentClass->setType(node->value);
        AddRecordToCurrentScope(CurrentClass);
        AddNewToScope(CurrentClass);

        Variable* newVariable = new Variable();
        newVariable->name = "this";
        newVariable->setType(currentScope->name);
        AddRecordToCurrentScope(newVariable);
    }

    void MethodDeclaration(Node* node) {
        priorRecordType = "m";
        auto child = node->children.begin();
        lineno = (*child)->lineno;
        CurrentMethod = new Method();
        string MethodType = (*child)->value;
        string MethodName = (*++child)->value;
        
        // cout << "\tType : " << (*child)->type << "; Value: " << (*child)->value << endl;
    
        // cout << "\t++Type : " << (*child)->type << "; ++Value: " << MethodName << endl;


        CurrentMethod->setType(MethodType);
        CurrentMethod->setName(MethodName); 
        CurrentClass->methods[CurrentMethod->name] = CurrentMethod;
        AddRecordToCurrentScope(CurrentMethod);
        AddNewToScope(CurrentMethod);
        child++;

        // cout << "\t\tType: " << (*child)->type << "; Value: " << (*child)->value << endl;

        if (child != node->children.end()) {
            for (auto i = (*child)->children.begin(); i != (*child)->children.end(); ++i) {
                // cout << "\t\t\tMType: " << (*i)->type << "; Value: " << (*i)->value << endl;
                if ((*child)->type == "Return") { 
                    break; }
 
                VarDeclaration(*i, *child);

                if ((*child)->type == "TypeIdentList") {
                    break;
                }
                CurrentMethod->parameters.push_back((*i)->value);
                CurrentMethod->parametersCount++;
            } 
        }
    }


    void VarDeclaration(Node* node, Node* parent) {
        if (node == nullptr) {
            return;
        }
        string varType, varName;
        auto child = node->children.begin();
        auto Check = node->children.begin();
            
        lineno = node->lineno;

        if (!(++Check == node->children.end())) {
            Variable* newVariable = new Variable();
            varType = (*child)->value;
            varName = (*++child)->value;
            
            if (((*child)->value == "NewIdentifier") || ((*child)->value == "Call") || ((*child)->type == "Return") || ((*child)->value == "Assign")) { return; 
            } else if ((*child)->value == "This") {
                varName = varType;
                varType = IdentifierReturnType("this");
            }
            cout << "++Type : " << varType << "; ++Value: " << (*child)->value << endl;

        } else if (parent->type == "TypeIdentList") {
            cout << "\t\tTypeIdentList" << endl;
            for (auto child = parent->children.begin(); child != parent->children.end(); ++child) {

                Variable* newVariable = new Variable();
                varType = (*child)->value;
                cout << "\t\t\tType : " << (*child)->type << "; Value: " << (*child)->value << endl;
                varName = (*++child)->value;
                cout << "\t\t\t++Type : " << (*child)->type << "; ++Value: " << (*child)->value << endl;
                if (((*child)->value == "NewIdentifier") || ((*child)->value == "Call") || ((*child)->type == "Return")) { return; 
                } else if ((*child)->value == "This") {
                    varName = varType;
                    varType = IdentifierReturnType("this");
                }

                newVariable->setName(varName);
                newVariable->setType(varType);
                if (priorRecordType == "m") {
                    CurrentMethod->variables[newVariable->name] = newVariable;
                } else if (priorRecordType == "c") {
                    CurrentClass->variables[newVariable->name] = newVariable;
                }
                AddRecordToCurrentScope(newVariable);
                
            } return;
            
        } else {
            auto child = parent->children.begin(); 
            varType = (*child)->value;
            varName = (*++child)->value;

            if (((*child)->value == "NewIdentifier") || ((*child)->value == "Call") || ((*child)->type == "Return") || ((*child)->value == "Assign")) { return; 
            } else if ((*child)->value == "This") {
                varName = varType;
                varType = IdentifierReturnType("this");
            }
        }
        Variable* newVariable = new Variable();
        
        cout << "Added Variable: " << varName << " of type: " << varType << endl;

        newVariable->setName(varName);
        newVariable->setType(varType);
        if (priorRecordType == "m") {
            CurrentMethod->variables[newVariable->name] = newVariable;
        } else if (priorRecordType == "c") {
            CurrentClass->variables[newVariable->name] = newVariable;
        }
        AddRecordToCurrentScope(newVariable);
    }

    void AddRecordToCurrentScope(Record* rec){
        if ((currentScope->findRecord(rec->name)) != rec->record) {
            currentScope->addRecord(rec);
        } else {
            reportSemanticError("Duplicate record: " + rec->name);
        }
    }

    void AddNewToScope(Record* rec) {
        currentScope->addChildScope(nextScopeId++, rec);
        nextScopeId++;
        enterScope();
    }

    void enterScope(int i = -1) {
        currentScope = currentScope->getChildScope(i);
        // cout << "Entering Method: " << CurrentMethod->name << "; Class: " << CurrentClass->name << endl;
    }

    void exitCurrentScope() {
        // cout << "Exiting Method: " << CurrentMethod->name << "; Class: " << CurrentClass->name << endl;
        if (currentScope->parent) {
            currentScope = currentScope->parent;
        }
    }

    void Semantic_Init(Node* root) {
        currentScope = rootScope;
        classDepth = 0;
        methodDepth = 0;
        Semantic_Analysis(root);
    }

    void Semantic_Analysis(Node* node) {
        string type = node->type;
        string value = node->value;
        int id = node->id;
        lineno = node->lineno;
        list<Node*> children = node->children;

        // cout << "ID: " << id << " Type: " << type << " Value: " << value << " LineNo: "<< lineno << endl;


        if (type == "ClassDeclaration" || type == "MainClassDeclaration") {
            enterScope(classDepth++);
        } else if (type == "MethodDeclaration") {
            enterScope(methodDepth++);
        } else if (type == "VarDeclaration") {
            auto child = children.begin();
            string varType = (*child)->value;
            if(!(varType == "Boolean" || varType == "IntegerType" || varType == "IntArray")) {
                if(CheckForClass(varType)) {
                    reportSemanticError("Identifier is not a Class");
                }
            }
        } else if (type == "Return") { Semantic_Analysis_Return(*children.begin());
        } else if (type == "Statement") {
            if (value == "Assign") { Semantic_Analysis_Assign(*children.begin()); }
            else if (value == "IfElse") { Semantic_Analysis_IfElse(*children.begin()); }
            else if (value == "If") { Semantic_Analysis_If(*children.begin()); }
            else if (value == "While") { Semantic_Analysis_While(*children.begin()); }
            else if (value == "Print") { Semantic_Analysis_Print(*children.begin()); }
        } for (auto i = children.begin(); i != children.end(); i++) {
            Semantic_Analysis(*i);
        }
        if ((type == "ClassDeclaration") || (type == "MainClass"))  {
            exitCurrentScope();
            methodDepth = 0;
        } else if (type == "MethodDeclaration") {
            exitCurrentScope();
        }
    }

    void Semantic_Analysis_Return(Node* node) {
        string type = node->type;
        string value = node->value;
        string returnType;

        lineno = node->lineno;

        returnType = Semantic_Analysis_Expression(node);

        if (returnType != CurrentMethod->type) {
            reportSemanticError("Return type does not match method type");
        }
    }

    void Semantic_Analysis_Assign(Node* node) {
        string type = node->type;
        string value = node->value;
        Record rec;
        string type_prior, type_post;

        lineno = node->lineno;

        if (type=="Identifier") {
            type_prior = IdentifierReturnType(value);
            type_post = Semantic_Analysis_Expression(++node);
            if (type_prior != type_post) {
                reportSemanticError("Type mismatch in assignment");
            }
        } else if (type=="ArrayAssign") {
            auto child = node->children.begin();
            string type_array = IdentifierReturnType((*child)->value);
            if (type_array != "IntArray") {
                reportSemanticError("Varialbe is not an Integer Array");
            }
            string type_idx = Semantic_Analysis_Expression(*(++child));
            if (type_idx != "IntegerType") {
                reportSemanticError("Index is not an Integer");
            }
            type_post = Semantic_Analysis_Expression(++node);
            if (type_post != "IntegerType") {
                reportSemanticError("Type mismatch in assignment");
            }
        } else {
            reportSemanticError("Invalid assignment");
        }
    }

    string Semantic_Analysis_Expression(Node* node) {
        string type = node->type;
        string value = node->value;
        string type_prior, type_post;

        lineno = node->lineno;

        if (type == "Identifier") {
            return IdentifierReturnType(value);
        } else if (type == "IntegerLiteral") {
            return "IntegerType";
        } else if (value == "true" || value == "false") {
            return "Boolean";
        } else if (value == "This") {
            return IdentifierReturnType("this");
        } else if (value == "Plus" || value == "Minus" || value == "Multiply" || value == "Divide") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if(type_prior != "IntegerType" || type_post != "IntegerType") {
                reportSemanticError("Type mismatch in arithmetic expression");
            } return "IntegerType";
        } else if (value == "LessThan" || value == "GreaterThan") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != "IntegerType" || type_post != "IntegerType") {
                reportSemanticError("Type mismatch in comparison expression");
            } return "Boolean";
        } else if (value == "And" || value == "Or") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != "Boolean" || type_post != "Boolean") {
                reportSemanticError("Type mismatch in logical expression");
            } return "Boolean";
        } else if (value == "Equal") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != type_post) {
                reportSemanticError("Type mismatch in equality expression");
            } return "Boolean";
        } else if (value == "ArrayLookup") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != "IntArray" || type_post != "IntegerType") {
                reportSemanticError("Type mismatch in array lookup");
            } return "IntegerType";
        } else if (value == "ArrayLength") {
            auto child = node->children.begin();
            string type_array = Semantic_Analysis_Expression(*child);
            if (type_array != "IntArray") {
                reportSemanticError("Varialbe is not an Integer Array");
            } return "IntegerType";
        } else if (value == "Not") {
            auto child = node->children.begin();
            string type_class = Semantic_Analysis_Expression(*child);
            if (type_class != "Boolean") {
                reportSemanticError("Type mismatch in logical expression");
            } return "Boolean";
        } else if (value == "NewArray") {
            auto child = node->children.begin();
            string type_size = Semantic_Analysis_Expression(*child);
            if (type_size != "IntegerType") {
                reportSemanticError("Variable type is not an array");
            } return "IntArray";
        } else if (value == "NewIdentifier") {
            auto child = node->children.begin();
            string childType = (*child)->type;
            string childValue = (*child)->value;
            if(childType != "Identifier") {
                reportSemanticError("Invalid expression");
            } else if (!CheckForClass(childValue)) {
                reportSemanticError("Identifier is not a Class");
            } return childValue;
        } else if (value == "Call") {
            auto child = node->children.begin();
            string ObjectCalled = Semantic_Analysis_Expression(*child);
            string MethodUsed = (*(*(++child))->children.begin())->value;
            string MethodType = (*(*(child))->children.begin())->type;

            if (MethodType != "Identifier") {
                reportSemanticError("Object called is not an identifier");
                return "False";
            }
            //check if object is a class
            if (!CheckForClass(ObjectCalled)) {
                reportSemanticError("Object called is not a class");
                return "False";
            }
            //check if method is in the class
            if (CurrentClass->methods.find(MethodUsed) == CurrentClass->methods.end()) {
                reportSemanticError("Method is not in the class");
                return "False";
            }
            //check parameter
            auto method = CurrentClass->methods.find(MethodUsed);
            if (method->second->parametersCount != (*child)->children.size() - 1) {
                reportSemanticError("Invalid number of parameters");
                return "False";
            }
            //check each parameter
            auto iter = (*child)->children.begin();
            for (int i = 0; i < method->second->parametersCount; i++) {
                string paramType = Semantic_Analysis_Expression(*(++iter));
                if (paramType != method->second->parameters.front()) {
                    reportSemanticError("Invalid parameter type");
                    //return the type of the method
                    return method->second->type;
                }
            } return method->second->type; 
        }
        return "False";
    }

    void Semantic_Analysis_If(Node* node) {
        string type = node->type;
        string value = node->value;

        lineno = node->lineno;

        if (type == "Expression" || type == "Identifier") {
            string condType = Semantic_Analysis_Expression(node);
            if (condType == "Boolean") {
                string StatementType = (++node)->type;
                if (StatementType == "Statement") {
                    Semantic_Analysis(node);
                } else {
                    reportSemanticError("Invalid statement");
                }
            } 
        } 
    }

    void Semantic_Analysis_IfElse(Node* node) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;
        
        if (type == "Expression" || type == "Identifier") {
            string condType = Semantic_Analysis_Expression(node);
            if (condType == "Boolean") {
                string StatementType = (++node)->type;
                if (StatementType == "Statement") {
                    Semantic_Analysis(node);
                    if ((++node)->type == "Statement") {
                        Semantic_Analysis(node);
                    } else {
                        reportSemanticError("Invalid else statement");
                    }
                } else {
                    reportSemanticError("Invalid if statement");
                }
            } else {
                reportSemanticError("Invalid if conditional expression");
            }
        } else {
            reportSemanticError("Invalid if else statement");
        }
    }

    void Semantic_Analysis_While(Node* node) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;
        
        if (type == "Expression" || type == "Identifier") {
            string condType = Semantic_Analysis_Expression(node);
            if (condType == "Boolean") {
                string StatementType = (++node)->type;
                if (StatementType == "Statement") {
                    Semantic_Analysis(node);
                } else {
                    reportSemanticError("Invalid statement");
                }
            } else {
                reportSemanticError("Invalid while conditional expression");
            }
        } else {
            reportSemanticError("Invalid while statement");
        }
    }

    void Semantic_Analysis_Print(Node* node) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;
        
        if ( type == "IntegerLiteral" || type == "Expression" || type == "Identifier") {
            string printType = Semantic_Analysis_Expression(node);
        } else {
            reportSemanticError("Invalid print statement");
        }
    }

    string IdentifierReturnType(string name) {
        Record* rec;
        rec = currentScope->getRecordFromName(name);
        if (rec == nullptr) {
            return "";
        }
        return rec->type;
    }
    
    bool CheckForClass(string name) {
        if (currentScope->findRecord(name)!= "Class") {
            return false;
        }
        return true;
    }

    void reportSemanticError(string message) {
		cerr << "\t@error at line " << lineno << ". " << message << endl;
        CorrectSemantic = false;
    }

    void generate_tree()
	{
		std::ofstream outStream;
		char *filename = "symbol_table.dot";
		outStream.open(filename);

		outStream << "digraph {" << std::endl;
		generate_tree_content(&outStream, rootScope);
		outStream << "}" << std::endl;
		outStream.close();

		printf("\nBuilt a symbol-table-tree at %s. Use 'make st' to generate the pdf version.\n", filename);
	}

    void generate_tree_content(ofstream* outStream, Scope* scope) {
        string tree = scope->record + ": " + scope->name + "\n";
        for (auto cl : scope->classes) {
            tree += "Name: " + cl.second->name + "; Record: " + cl.second->record + "; Type: " + cl.second->type + "\n";
        }
        for (auto method : scope->methods) {
            tree += "Name: " + method.second->name + "; Record: " + method.second->record + "; Type: " + method.second->type + "\n";
        }
        for (auto variable : scope->variables) {
            tree += "Name: " + variable.second->name + "; Record: " + variable.second->record + "; Type: " + variable.second->type + "\n";
        }
        *outStream << "n" << scope->id << " [label=\"" << tree << "\"];" << endl;
        
        for (auto child : scope->children) {
            generate_tree_content(outStream, child);
            *outStream << "n" << scope->id << " -> n" << child->id << endl;
        }
    }
};

#endif
