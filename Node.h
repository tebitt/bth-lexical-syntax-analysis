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
#define DEBUG(x) do { std::cerr << x; } while (0)

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
    vector<Variable*> parameters;

    Method() { record = "method"; }

    void printParameters() const {
        cout << "Parameters for " << name << " method: ";
        for (const auto& param : parameters) {
            cout << param->name << " ";
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
    vector<Variable*> variables;
    vector<Method*> methods;

    Class() { record = "class"; }

    Method* getMethod(string methodName) {
        for (const auto& method : methods) {
            if (method->name == methodName) {
                return method;
            }
        }
        return new Method();
    }

    void printVariables() const {
        cout << "Variables for " << name << " class: ";
        for (const auto& var : variables) {
            cout << var->name << " ";
        }
        cout << endl;
    }

    void printMethods() const {
        cout << "Methods for " << name << " class: ";
        for (const auto& method : methods) {
            cout << method->name << " ";
        }
        cout << endl;
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
        if (id == -1 && !children.empty()) {
            return children.back();
        }

        for (const auto& child : children) {
            if (child->id == id) {
                return child;
            }
        }

        return nullptr;
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

    Record* findRecord(const string& key) const {
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
        cout << "ID: " << id << " Type: " << type << " Value: " << value << " LineNo: "<< lineno << endl;

        if (node->type == "ClassDeclaration" || node->type == "MainClass") { ClassDeclaration(child); } 
        else if (node->type == "MethodDeclaration") { MethodDeclaration(child); }
        else if (node->type == "VarDeclaration") { VarDeclaration(child); } 
        else if (node->type == "Expression" || node->type == "Statement") { ExpressionOrStatement(node); }

        for (Node* &ch : node->children) { main(ch); }

        if (type == "ClassDeclaration" || type == "MainClass" || type == "MethodDeclaration") { exitCurrentScope(); }
    }

    void ClassDeclaration(Node* node) {
        Class* newClass = new Class();
        newClass->setName(node->value);
        newClass->setType(node->value);
        currentScope->addRecord(newClass);
        AddNewToScope(newClass);
        
        Variable* newVariable = new Variable();
        newVariable->name = "this";
        newVariable->type = newClass->name;
        currentScope->addRecord(newVariable);
    }

    void MethodDeclaration(Node* node) {
        string type = node->type;
        string value = node->value;
        int id = node->id;
        list<Node*> children = node->children;
        cout << children.size() << endl;

        Method* newMethod = new Method();
        newMethod->setType((*node->children.begin())->children.front()->value);
        newMethod->setName((*++node->children.begin())->children.front()->value);
        cout << "line1" << endl;
        CurrentClass->methods.push_back(newMethod);
        cout << "line2" << endl;
        currentScope->addRecord(newMethod);
        AddNewToScope(newMethod);

        for (auto paramNode : (*++node->children.begin())->children) {
            VarDeclaration(paramNode->children.front());
            CurrentMethod->parameters.push_back(dynamic_cast<Variable*>(currentScope->variables[paramNode->children.front()->children.back()->value]));
        }
    }

    void VarDeclaration(Node* node) {
    Variable* newVariable = new Variable();
    auto child = node->children.begin();
    newVariable->setName((*child)->value);
    newVariable->setType((*child)->type);
    currentScope->addRecord(newVariable);
}

    void ExpressionOrStatement(Node* node) {
    string type = node->type;
    string value = node->value;
    int id = node->id;
    list<Node*> children = node->children;

    // Debugging output
    std::cout << "Debugging ExpressionOrStatement:\n";
    std::cout << "Type: " << type << "\n";
    std::cout << "Value: " << value << "\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Children count: " << children.size() << "\n";

    if (type == "Identifier"){
        if (currentScope->findRecord(value) == nullptr) {
            reportSemanticError("Undeclared variable: " + value);
        }
        if (!(value == "NewIdentifier") || value == "Call")
            for (Node* &child : children) {
                // Debugging output for each child
                std::cout << "Processing child with ID: " << child->id << "\n";
                ExpressionOrStatement(child);
            }
    }
}

    void AddNewToScope(Record* rec) {
        Scope* newScope = new Scope(nextScopeId++, rec->name, rec->record, rec->type);
        newScope->parent = currentScope;
        currentScope->children.push_back(newScope);
        currentScope = newScope;

    }

    void exitCurrentScope() {
        if (currentScope->parent) {
            currentScope = currentScope->parent;
        }
        DEBUG("exitCurrentScope: Exiting current scope\n");

    }

    void addRecord(Record* rec) {
        if (currentScope->findRecord(rec->name)) {
            reportSemanticError("Duplicate record: " + rec->name);
        } else {
            currentScope->addRecord(rec);
        }
        DEBUG("addRecord: " << rec->getName() << "\n");
    }

    void Semantic_Traversal(Node* root) {
        currentScope = rootScope;
        Semantic_Analysis(root);
    }

    void Semantic_Analysis(Node* node) {
        string type = node->type;
        string value = node->value;
        int id = node->id;
        list<Node*> children = node->children;

        if (type == "MethodDeclaration") {
            auto iter = children.begin();
            string methodName = (*(++iter))->value;
        } else if ((type == "ClassDeclaration") || (type == "MainClass")) {
            auto iter = children.begin();
            string className = (*iter)->value;
        } else if (type == "VarDeclaration") {
            auto iter = children.begin();
            string varType = (*iter)->value;
            if(!(varType == "BooleanType" || varType == "IntegerType" || varType == "ArrayType")) {
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
        if ((type == "ClassDeclaration") || (type == "MainClass") || (type == "MethodDeclaration")) {
            exitCurrentScope();
        }
    }

    void Semantic_Analysis_Return(Node* node) {
        string type = node->type;
        string value = node->value;
        string returnType;

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
            string methodName = (*child)->value;
            string className = Semantic_Analysis_Expression(*(++child));
            if (!CheckForClass(className)) {
                reportSemanticError("Identifier is not a Class");
            }
            if (className != CurrentClass->name) {
                if (CurrentClass->getMethod(methodName) == nullptr) {
                    reportSemanticError("Method not found");
                }
            } return CurrentClass->getMethod(methodName)->type;
        } else {
            reportSemanticError("Invalid expression");
        }
        return nullptr;
    }

    void Semantic_Analysis_If(Node* node) {
        string type = node->type;
        string value = node->value;
        
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
                reportSemanticError("Invalid expression");
            }
        } else {
            reportSemanticError("Invalid if statement");
        }
    }

    void Semantic_Analysis_IfElse(Node* node) {
        string type = node->type;
        string value = node->value;
        
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
        
        if ( type == "IntegerLiteral" || type == "Expression" || type == "Identifier") {
            string printType = Semantic_Analysis_Expression(node);
        } else {
            reportSemanticError("Invalid print statement");
        }
    }

    string IdentifierReturnType(string name) {
        Record* rec;
        rec = currentScope->findRecord(name);
        if (rec == nullptr) {
            reportSemanticError("Undeclared variable: " + name);
            return "";
        }
        return rec->type;
    }

    bool CheckForClass(string name) {
        if (currentScope->findRecord(name) == nullptr) {
            return false;
        }
        return true;
    }

    void reportSemanticError(const string& message) {
        cerr << "Semantic Error: " << message << endl;
        CorrectSemantic = false;
        DEBUG("reportSemanticError: " << message << "\n");
    
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
