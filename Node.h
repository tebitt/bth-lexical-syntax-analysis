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
    Variable() { record = "variable"; }
};

class Method : public Record {
public:
    map<string, Variable*> variables;
    vector<string> parameters;
    int parametersCount = 0;

    Method() { record = "method"; }

    void printParameters() const {
        for (const auto& param : parameters) {
            cout << param << " ";
        }
        cout << endl;
    }

    void printVariables() const {
        cout << "Variables for " << name << " method: ";
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

    Class() { record = "class"; }

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
        if (rec->record == "class") {
            classes[dynamic_cast<Class*>(rec)->name] = dynamic_cast<Class*>(rec);
        } else if (rec->record == "method") {
            methods[dynamic_cast<Method*>(rec)->name] = dynamic_cast<Method*>(rec);
        } else if (rec->record == "variable") {
            variables[dynamic_cast<Variable*>(rec)->name] = dynamic_cast<Variable*>(rec);
        }
    }

    string findRecord(string key) {
        if (classes.find(key) != classes.end()) {
            return "class";
        } else if (methods.find(key) != methods.end()) {
            return "method";
        } else if (variables.find(key) != variables.end()) {
            return "variable";
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
        currentScope = new Scope(nextScopeId++, "GoalName", "Goal", "void");
        rootScope = currentScope;
    }

    void main(Node* node) {
        int id = node->id;
        string type = node->type;
        string value = node->value;
        int lineno = node->lineno;
        list<Node*> children = node->children;
        Node* child = *children.begin();

        // cout << "ID: " << id << " Type: " << type << " Value: " << value << " LineNo: "<< lineno << endl;
        if (node->type == "ClassDeclaration" || node->type == "MainClass") { ClassDeclaration(child); } 
        else if (node->type == "MethodDeclaration") { MethodDeclaration(node); }
        else if (node->type == "VarDeclaration") { VarDeclaration(child); } 
        else if (node->type == "Expression" || node->type == "Statement") { ExpressionOrStatement(node); }

        for (Node* &ch : node->children) { main(ch); } 

        if (type == "ClassDeclaration" || type == "MainClass" || type == "MethodDeclaration") { exitCurrentScope(); }
    }

    void ClassDeclaration(Node* node) {
        priorRecordType = "class";
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
        priorRecordType = "method";
        auto child = node->children.begin();
        string value = (*child)->value;
        string type = (*child)->type;
        lineno = (*child)->lineno;

        CurrentMethod = new Method();
        CurrentMethod->setType(type);
        CurrentMethod->setName((*++child)->value);
        CurrentClass->methods[CurrentMethod->name] = CurrentMethod;
        AddRecordToCurrentScope(CurrentMethod);
        AddNewToScope(CurrentMethod);

        child++;

        if (child != node->children.end()) {
            for (auto i = (*child)->children.begin(); i != (*child)->children.end(); ++i) {
                if (*i) {
                    VarDeclaration(*i);
                    if (!(*i)->children.empty()) {
                        auto ch = (*i)->children.begin();
                        if (ch != (*i)->children.end() && *ch) {
                            string childType = (*ch)->type;
                            CurrentMethod->parameters.push_back(childType);
                            CurrentMethod->parametersCount++;
                        }
                    }
                }
            }
        }
    }


    void VarDeclaration(Node* node) {
        Variable* newVariable = new Variable();
        auto child = node->children.begin();
        auto Check = node->children.begin();
        lineno = node->lineno;
        if (!(++Check == node->children.end())) {
            newVariable->setType((*child)->type);
            newVariable->setName((*++child)->value);
            if (priorRecordType == "method") {
                CurrentMethod->variables[newVariable->name] = newVariable;
            } else if (priorRecordType == "class") {
                CurrentClass->variables[newVariable->name] = newVariable;
            }
            AddRecordToCurrentScope(newVariable);
        } else {
            return;
        }
    }

    void ExpressionOrStatement(Node* node) {
        string type = node->type;
        string value = node->value;
        int id = node->id;
        list<Node*> children = node->children;

        if (type == "Identifier"){
            if (currentScope->findRecord(value) == "null") {
                reportSemanticError("Undeclared variable: " + value);
            }
            if (!(value == "NewIdentifier") || value == "Call")
                for (Node* &child : children) {
                    std::cout << "Processing child with ID: " << child->id << "\n";
                    ExpressionOrStatement(child);
            }
        }
    }

    void AddRecordToCurrentScope(Record* rec){
        cout << "Adding record to current scope: " << rec->name << " Type: " << rec->type << endl;
        cout << "Current Scope: " << currentScope->findRecord(rec->name) << endl;
        if ((currentScope->findRecord(rec->name)) != rec->record) {
            currentScope->addRecord(rec);
        } else {
            reportSemanticError("Duplicate record: " + rec->name);
        }
    }

    void AddNewToScope(Record* rec) {
        Scope* newScope = new Scope(nextScopeId++, rec->name, rec->record, rec->type);
        newScope->parent = currentScope;
        currentScope->children.push_back(newScope);
        currentScope = newScope;

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
        list<Node*> children = node->children;

        cout << "ID: " << id << " Type: " << type << " Value: " << value << " LineNo: "<< lineno << endl;
        if (type == "ClassDeclaration" || type == "MainClass") {
            enterScope(classDepth++);
        } else if (type == "MethodDeclaration") {
            enterScope(methodDepth++);
        } else if (type == "VarDeclaration") {
            auto child = children.begin();
            string varType = (*child)->type;
            if(!(varType == "BooleanType" || varType == "IntegerType" || varType == "ArrayType")) {
                if(CheckForClass(varType)) {
                    reportSemanticError("Identifier is not a Class");
                }
            }
        } else if (type == "Return") { Semantic_Analysis_Return(node);
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

        returnType = Semantic_Analysis_Expression(*node->children.begin());

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
            if (type_array != "ArrayType") {
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
            return "BooleanType";
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
            } return "BooleanType";
        } else if (value == "And" || value == "Or") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != "BooleanType" || type_post != "BooleanType") {
                reportSemanticError("Type mismatch in logical expression");
            } return "BooleanType";
        } else if (value == "Equal") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != type_post) {
                reportSemanticError("Type mismatch in equality expression");
            } return "BooleanType";
        } else if (value == "ArrayLookup") {
            auto child = node->children.begin();
            type_prior = Semantic_Analysis_Expression(*child);
            type_post = Semantic_Analysis_Expression(*(++child));
            if (type_prior != "ArrayType" || type_post != "IntegerType") {
                reportSemanticError("Type mismatch in array lookup");
            } return "IntegerType";
        } else if (value == "ArrayLength") {
            auto child = node->children.begin();
            string type_array = Semantic_Analysis_Expression(*child);
            if (type_array != "ArrayType") {
                reportSemanticError("Varialbe is not an Integer Array");
            } return "IntegerType";
        } else if (value == "Not") {
            auto child = node->children.begin();
            string type_class = Semantic_Analysis_Expression(*child);
            if (type_class != "BooleanType") {
                reportSemanticError("Type mismatch in logical expression");
            } return "BooleanType";
        } else if (value == "NewArray") {
            auto child = node->children.begin();
            string type_size = Semantic_Analysis_Expression(*child);
            if (type_size != "IntegerType") {
                reportSemanticError("Variable type is not an array");
            } return "ArrayType";
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
                if (paramType != method->second->parameters[i]) {
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
            if (condType == "BooleanType") {
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
            if (condType == "BooleanType") {
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
            if (condType == "BooleanType") {
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
        if (currentScope->findRecord(name)!= "class") {
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
        string tree = scope->record + ": " + scope->name + "\n\n";
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
