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
#include <string>

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
    int LineNo = 0;
    int parametersCount = 0;

    Method() { record = "Method"; LineNo = 0; }

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
    
    void setLineNo(int l) { LineNo = l; }
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

    string findVariable(string key) {
        if (variables.find(key) != variables.end()) {
            return "Variable";
        } else {
            return "null";
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
    list<Scope*> children;
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

    Scope* getChildScope(int idx = -1) {
        if (idx > -1) {
            auto child = children.begin();
            for (int i = 0; i < idx; i++) {
                child++;
            }
            return *child;
        } else { return children.back(); }
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

    string checkRecord(string key) {
        string rec = findRecord(key);
        if (rec == "null") {
            if (parent_var.find(key) != parent_var.end()) {
                return "Variable";
            } 
        } else { return rec; }
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

    Record* getRecordFromName(string key)  {
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

    Record* LookupRecord(string key) {
        Record* rec = getRecordFromName(key);
        if (rec != nullptr) { return rec; 
        } else {
            Scope* childScope = this;
            while (childScope->parent != nullptr) {
                childScope = childScope->parent;
                rec = childScope->getRecordFromName(key);
                if (rec != nullptr) { return rec; }
            } return new Record();
        }
    }

    int getMethodLineNo(string key) {
        if (methods.find(key) != methods.end()) {
            return methods.at(key)->LineNo;
        } else {
            return -1;
        }
    }

    void printRecords() {
        cout << "Records for '" << name << "' scope: ";
        for (const auto& cl : classes) {
            cout << cl.second->name << " ";
        }
        for (const auto& method : methods) {
            cout << method.second->name << " " << method.second->LineNo << " ";
        }
        for (const auto& var : variables) {
            cout << var.second->name << " ";
        }
        cout << endl;
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

        if (node->type == "ClassDeclaration" || node->type == "MainClassDeclaration") { ClassDeclaration(*child); } 
        else if (node->type == "MethodDeclaration") { MethodDeclaration(node); }
        else if (node->type == "VarDeclaration") { VarDeclaration(*child, node); } 
        if ((node->type == "Expression") || (node->type == "Statement")) { 
            ExpressionOrStatement(node); }

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
        CurrentClass->printVariables();
    }


    void MethodDeclaration(Node* node) {
        priorRecordType = "m";
        auto child = node->children.begin();
        lineno = (*child)->lineno;
        CurrentMethod = new Method();
        string MethodType = (*child)->value;
        string MethodName = (*++child)->value;

        CurrentMethod->setType(MethodType);
        CurrentMethod->setName(MethodName); 
        CurrentMethod->setLineNo(lineno);
        CurrentClass->methods[CurrentMethod->name] = CurrentMethod;
        AddRecordToCurrentScope(CurrentMethod);
        AddNewToScope(CurrentMethod);
        child++;

        if (child != node->children.end()) {
            for (auto i = (*child)->children.begin(); i != (*child)->children.end(); i++) {
                if (((*child)->type == "Return") || ((*child)->type == "VarDeclarationOrStatementList")){ 
                    break; }
                lineno = (*i)->lineno;
                
                VarDeclaration(*i, *child);
        
                CurrentMethod->parameters.push_back((*i)->value);
                CurrentMethod->parametersCount++;
                i++;
            } 
        } 
    }

    void VarDeclaration(Node* node, Node* parent){
        auto child = parent->children.begin();
        Variable* newVariable = new Variable();
        lineno = node->lineno;
        newVariable->setType(node->value);
        node++;
        newVariable->setName(node->value);
        if (priorRecordType == "m") {
            CurrentMethod->variables[newVariable->name] = newVariable;
        } else if (priorRecordType == "c") {
            CurrentClass->variables[newVariable->name] = newVariable;
        }
        AddRecordToCurrentScope(newVariable);
    }

    void ExpressionOrStatement (Node* node) {
        string type = node->type;
        string value = node->value;
        
        lineno = node->lineno;
        char *t = (char*)type.c_str();

        if (strcmp(t, "Identifier") == 0){
            if (strcmp(currentScope->findRecord(value).c_str(),"null") == 0 && (strcmp(CurrentClass->findVariable(value).c_str(),"null") == 0)){
                reportSemanticError("Undeclared variable: " + value);
            }
        }
        char* v = (char*)value.c_str();
        if ((strcmp(v, "NewIdentifier") != 0) && (strcmp(v, "Call") != 0)) {
            for (Node* &i : node->children) {
                ExpressionOrStatement(i);}
        }
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
    }

    void exitCurrentScope() {
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

        if (type == "ClassDeclaration" || type == "MainClassDeclaration") {
            enterScope(classDepth++);
        } else if (type == "MethodDeclaration") {
            enterScope(methodDepth++);
        } else if (type == "VarDeclaration") {
            auto child = node->children.begin();
            string varType = (*child)->value;
            if(!(varType == "Boolean" || varType == "Int" || varType == "IntArray")) {
                if((currentScope->LookupRecord(varType)->record) != "Class") {
                    reportSemanticError("Identifier is not a Class");
                }
            }
        } else if (type == "Return") { 
            Semantic_Analysis_Return(*(node->children).begin(), node);
        } else if (type == "Statement") {
            if (value == "Assign") { Semantic_Analysis_Assign(*(node->children).begin(), node); }
            else if (value == "IfElse") { Semantic_Analysis_IfElse(*(node->children).begin(), node); }
            else if (value == "If") { Semantic_Analysis_If(*(node->children).begin(), node); }
            else if (value == "While") { Semantic_Analysis_While(*(node->children).begin(), node); }
            else if (value == "Print") { Semantic_Analysis_Print(*(node->children).begin(), node); }


        } for (auto i = node->children.begin(); i != node->children.end(); i++) {
            Semantic_Analysis(*i);
        }
        if ((type == "ClassDeclaration") || (type == "MainClassDeclaration"))  {
            exitCurrentScope();
            methodDepth = 0;
        } else if (type == "MethodDeclaration") {
            exitCurrentScope();
        }
    }

    void Semantic_Analysis_Return(Node* node, Node* parent) {
        string type = node->type;
        string value = node->value;
        string returnType;
        while (parent->type != "MethodDeclaration") {
            parent++;
        } 
        auto child = parent->children.begin();

        returnType = Semantic_Analysis_Expression(node, parent);
        lineno = (*child)->lineno;

        if (returnType != currentScope->type) {
            if (returnType == "null") { return; } 
            reportSemanticError("Return type does not match method type");
        }
    }

    void Semantic_Analysis_Assign(Node* node, Node* parent) {
        string type = node->type;
        string value = node->value;
        Record rec;
        string type_prior, type_post;

        lineno = node->lineno;
        if (type=="Identifier") {
            auto child = parent->children.begin();
            type_prior = IdentifierReturnType(value);
            type_post = Semantic_Analysis_Expression(*++child, parent);

            if (type_prior != type_post) {
                reportSemanticError("Type mismatch in assignment (Identifier)");
            }

        } else if (type=="ArrayIndex") {
            auto child = node->children.begin();
            auto child2 = parent->children.begin();
            string type_array = IdentifierReturnType((*child)->value);
            char* c = (char*)type_array.c_str();
            if (strcmp(c, "IntArray") != 0) {
                reportSemanticError("Variable is not an Integer Array");
                return;
            }
            string type_idx = Semantic_Analysis_Expression(*(++child), node);
            char *c_idx = (char*)type_idx.c_str();
            if (strcmp(c_idx, "Int") != 0) {
                reportSemanticError("Index is not an Integer");
                return;
            }
            type_post = Semantic_Analysis_Expression(*++child2, parent);
            char* c_post = (char*)type_post.c_str();
            if (strcmp(c_post, "Int") != 0) {
                reportSemanticError("Type mismatch in assignment (Call)");
                return;
            }
        } 
    }

    string Semantic_Analysis_Expression(Node* node, Node* parent) {
        string type = node->type;
        string value = node->value;
        string type_prior, type_post;

        lineno = node->lineno;
    
        char* c = (char*)type.c_str();
        char* v = (char*)value.c_str();

        if (strcmp(c, "Identifier") == 0){
            return IdentifierReturnType(value);
        } else if (strcmp(v, "IntegerLiteral") == 0) {
            return "Int";
        } else if (strcmp(v, "True") == 0 || strcmp(v, "False") == 0) {
            return "Boolean";
        } else if (strcmp(v, "This") == 0) {
            return IdentifierReturnType("this");
        } else if (strcmp(v, "Plus") == 0 || strcmp(v, "Minus") == 0 || strcmp(v, "Multiply") == 0 || strcmp(v, "Divide") == 0) {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child, node);
            type_post = Semantic_Analysis_Expression(*(++child), node);
            char* pre = (char*)type_prior.c_str();
            char* post = (char*)type_post.c_str();
            if(strcmp(pre, "Int") != 0 || strcmp(post, "Int") != 0) {
                reportSemanticError("Type mismatch in arithmetic expression");
                return "null";
            } return "Int";
        } else if (strcmp(v, "LessThan") == 0 || strcmp(v, "GreaterThan") == 0) {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child, node);
            type_post = Semantic_Analysis_Expression(*(++child), node);
            char* pre = (char*)type_prior.c_str();
            char* post = (char*)type_post.c_str();
            if(strcmp(pre, "Int") != 0 || strcmp(post, "Int") != 0) {
                reportSemanticError("Type mismatch in comparison expression");
                return "null";
            } return "Boolean";
        } else if (strcmp(v, "And") == 0 || strcmp(v, "Or") == 0) {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child, node);
            type_post = Semantic_Analysis_Expression(*(++child), node);
            if (type_prior != "Boolean" || type_post != "Boolean") {
                reportSemanticError("Type mismatch in logical expression");
                return "null";
            } return "Boolean";
        } else if (strcmp(v, "Equal") == 0) {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child, node);
            type_post = Semantic_Analysis_Expression(*(++child), node);
            char* pre = (char*)type_prior.c_str();
            char* post = (char*)type_post.c_str();
            if (strcmp(pre, post) != 0) {
                reportSemanticError("Type mismatch in equality expression");
                return "null";
            } return "Boolean";
        } else if (strcmp(v, "ArrayIndex") == 0) {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child, node);
            type_post = Semantic_Analysis_Expression(*(++child), node);
            char* pre = (char*)type_prior.c_str();
            char* post = (char*)type_post.c_str();
            if(strcmp(pre, "IntArray") != 0 || strcmp(post, "Int") != 0) {                
                reportSemanticError("Type mismatch in array lookup");
                return "null";
            } return "Int";
        } else if (strcmp(v, "ArrayLength") == 0) {
            auto child = node->children.begin();
            string type_array = Semantic_Analysis_Expression(*child, node);
            c = (char*)type_array.c_str();
            if (strcmp(c, "IntArray") != 0) {
                reportSemanticError("Variable is not an Integer Array");
                return "null";
            } return "Int";
        } else if (strcmp(v, "Not") == 0) {
            auto child = node->children.begin();
            string type_class = Semantic_Analysis_Expression(*child, node);
            if (type_class != "Boolean") {
                reportSemanticError("Type mismatch in logical expression");
                return "null";
            } return "Boolean";
        } else if (strcmp(v, "NewArray") == 0) {
            auto child = node->children.begin();
            string childType = Semantic_Analysis_Expression(*child, node);
            char *c = (char*)childType.c_str();
            if (strcmp(c, "Int") != 0) {
                reportSemanticError("Variable type is not an array");
                return "null";
            } return "IntArray";
        } else if (strcmp(v, "NewIdentifier") == 0) {
            auto child = node->children.begin();
            string childType = (*child)->type;
            char* c = (char*)childType.c_str();
            string childValue = (*child)->value;
            
            if(strcmp(c, "Identifier") != 0) {
                reportSemanticError("Invalid expression");
                return "null";
            } else if (strcmp(rootScope->findRecord(childValue).c_str(), "Class") != 0)  {
                reportSemanticError("Identifier is not a class");
                return "null";
            } return childValue;
        } else if (strcmp(v, "Call") == 0) {
            auto child = node->children.begin();
            string childType ,childClass = Semantic_Analysis_Expression(*child, node);
            if ((*child)->value == "This") {
                childType = "Identifier";
            } else if ((*child)->value == "NewIdentifier") {
                childType = (*(*child)->children.begin())->type;
            } else {
                childType = Semantic_Analysis_Expression(*child, node);
                char* c = (char*)childType.c_str();
                if (strcmp(c, "null") != 0) {
                    childType = "Identifier";
                }
            }

            string  childMethod = (*++child)->value;
            if (strcmp((*child)->type.c_str(),"Called_Method") == 0) {
                auto child2 = (*child)->children.begin();
                childMethod = (*child2)->value;
                child++;
            }
            char* c = (char*)childType.c_str();
            if (strcmp(c, "Identifier") != 0) {
                reportSemanticError("Object called is not an identifier");
                return "null";
            }
            if (strcmp(rootScope->findRecord(childClass).c_str(), "Class") != 0) {
                reportSemanticError("Object called is not a class");
                return "null";
            }
            Record* rec = rootScope->LookupRecord(childClass);
            auto classRecord = dynamic_cast<Class*>(rec);
            auto methodRecord = classRecord->getMethod(childMethod);
            if (strcmp(methodRecord->name.c_str(),childMethod.c_str()) != 0) {
                reportSemanticError("Method not found");
                return "null";
            }

            if (methodRecord->parametersCount != (*child)->children.size()) {
                reportSemanticError("Invalid number of parameters");
                return "null";
            }
            auto paramList = methodRecord->parameters.begin();
    
            auto chi = (*child)->children.begin();
            for (auto param = methodRecord->parameters.begin(); param != methodRecord->parameters.end(); param++) {
                string paramType = Semantic_Analysis_Expression(*chi++, *child);
                if (paramType != *param) {
                    reportSemanticError("Invalid parameter type");
                    return "null";
                }
            } return methodRecord->type; 
        }
    }

    void Semantic_Analysis_If(Node* node, Node* parent) {
        auto child = parent->children.begin();
        string type = (*child)->type;
        string value = (*child)->value;
        lineno = node->lineno;
        char* t = (char*)type.c_str();
        
        if ((strcmp(t,"Expression") == 0) || (strcmp(t,"Identifier") == 0)) {
            string condType = Semantic_Analysis_Expression(*child, parent);
            char* c = (char*)condType.c_str();
            if (strcmp(c,"Boolean") == 0) {
                string StatementType = (*++child)->type;
                char* s = (char*)StatementType.c_str();
                if (strcmp(s,"Statement") == 0) {
                    Semantic_Analysis(node);
                } else {
                    reportSemanticError("Invalid statement in if statement");
                }
            } 
        } 
    }

    void Semantic_Analysis_IfElse(Node* node, Node* parent) {
        auto child = parent->children.begin();
        string type = (*child)->type;
        string value = (*child)->value;
        lineno = node->lineno;
        char* t = (char*)type.c_str();
        
        if ((strcmp(t,"Expression") == 0) || (strcmp(t,"Identifier") == 0)) {
            string condType = Semantic_Analysis_Expression(*child, parent);
            char* c = (char*)condType.c_str();
            if (strcmp(c,"Boolean") == 0) {
                string StatementType = (*++child)->type;
                char* s = (char*)StatementType.c_str();
                if (strcmp(s,"Statement") == 0) {
                    Semantic_Analysis(node);
                    char* s2 = (char*)(*++child)->type.c_str();
                    if (strcmp(s2,"Statement") == 0) {
                        Semantic_Analysis(node);
                    } else {
                        reportSemanticError("Invalid else statement");
                        return;
                    }
                } else {
                    reportSemanticError("Invalid if statement");
                    return;
                }
            } else {
                reportSemanticError("Invalid if conditional expression");
                return;
            }
        } else {
            reportSemanticError("Invalid if else statement");
            return;
        }
    }

    void Semantic_Analysis_While(Node* node, Node* parent) {
        auto child = parent->children.begin();
        string type = (*child)->type;
        string value = (*child)->value;
        lineno = node->lineno;
        char* t = (char*)type.c_str();

        if ((strcmp(t,"Expression") == 0) || (strcmp(t,"Identifier") == 0)) {
            string condType = Semantic_Analysis_Expression(*child, parent);
            char* c = (char*)condType.c_str();
            if (strcmp(c,"Boolean") == 0) {
                string StatementType = (*++child)->type;
                char* s = (char*)StatementType.c_str();
                if (strcmp(s,"Statement") == 0) {
                    Semantic_Analysis(node);
                } else {
                    reportSemanticError("Invalid statement in while loop");
                    return;
                }
            } else {
                reportSemanticError("Invalid while conditional expression");
                return;
            }
        } else {
            reportSemanticError("Invalid while statement");
            return;
        }
    }

    void Semantic_Analysis_Print(Node* node, Node* parent) {
        string type = node->type;
        string value = node->value;
        lineno = node->lineno;
        if ( type == "IntegerLiteral" || type == "Expression" || type == "Identifier") {
            string printType = Semantic_Analysis_Expression(node, parent);
        } else {
            reportSemanticError("Invalid print statement");
        }
    }

    string IdentifierReturnType(string name) {
        Record* rec;
        rec = currentScope->LookupRecord(name);
        if (rec->type == "") { 
            reportSemanticError("Identifier not found: " + name); 
            return "null"; 
        } else {
            return rec->type;
        }
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
