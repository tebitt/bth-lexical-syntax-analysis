#ifndef NODE_H
#define	NODE_H

#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
class Node {
public:
	int id, lineno;
	string type, value;
	list<Node*> children;
	Node(string t, string v, int l) : type(t), value(v), lineno(l){}
	Node()
	{
		type = "uninitialised";
		value = "uninitialised"; }   // Bison needs this.
  
	void print_tree(int depth=0) {
		for(int i=0; i<depth; i++)
		cout << "  ";
		cout << type << ":" << value << endl; //<< " @line: "<< lineno << endl;
		for(auto i=children.begin(); i!=children.end(); i++)
		(*i)->print_tree(depth+1);
	}
  
	void generate_tree() {
		std::ofstream outStream;
		char* filename = "tree.dot";
	  	outStream.open(filename);

		int count = 0;
		outStream << "digraph {" << std::endl;
		generate_tree_content(count, &outStream);
		outStream << "}" << std::endl;
		outStream.close();

		printf("\nBuilt a parse-tree at %s. Use 'make tree' to generate the pdf version.\n", filename);
  	}

  	void generate_tree_content(int &count, ofstream *outStream) {
	  id = count++;
	  if(value.empty()) { 
			*outStream << "n" << id << " [label=\"" << type << "\"];" << endl;
	  } else {
			*outStream << "n" << id << " [label=\"" << type << ":" << value << "\"];" << endl;
	  }

	  for (auto i = children.begin(); i != children.end(); i++)
	  {
		  (*i)->generate_tree_content(count, outStream);
		  *outStream << "n" << id << " -> n" << (*i)->id << endl;
	  }
  }

};

class Record{
	public:
		string name;
		string record;
		string type;

		Record(): name(""), record(""), type(""){}

		void print(){
			cout << "name: " << name << " record: " << record << " type: " << type << endl;
		}
		
		void set_name(string n){
			name = n;
		}

		void set_type(string t){
			type = t;
		}
};

class Class: public Record{
	public:
		vector<Variable*> variables;
		vector<Method*> methods;
		Class()
		{
			record = "class";
		}

		void print_variables(){
			cout << "Variables for " << name << "class: ";
			for(int i = 0; i < variables.size(); i++) {
				cout << variables[i].name + " ";
			}
		}

		void print_methods(){
			cout << "Methods for " << name << "class: ";
			for(int i = 0; i < methods.size(); i++) {
				cout << methods[i].name + " ";
			}
		}

		Method get_method(string method_name){
			for(int i = 0; i < methods.size(); i++) {
				if(methods[i].name == method_name) {
					return methods[i];
				}
			} return new Method();
		}
};

class Method: public Record{
	public:
		map<string, Variable*> variables;
		vector<Variable> parameters;
		Method()
		{
			record = "method";
		}

		void print_parameters(){
			cout << "Parameters for " << name << "method: ";
			for(int i = 0; i < parameters.size(); i++)
			{
				cout << parameters[i].name + " ";
			}
			cout << endl;
		}

		void print_variables(){
			cout << "Variables for " << name << "method: ";
			for(int i = 0; i < variables.size(); i++)
			{
				cout << variables[i].name + " ";
			}
			cout << endl;
		}	
};

class Variable: public Record{
	public:
		Variable()
	{
		record = "variable";
	}
};

class Scope {
public:
    Scope* parent;
    vector<Scope*> children;
    int scopeId;
    string scopeType, scopeName, returnType;

    map<string, Class*> classes;
    map<string, Method*> methods;
    map<string, Variable*> variables;
    map<string, Variable*> inheritedVariables;

    Scope(int id, string type = "Default", string name = "DefaultName", string returnType = "void")
        : parent(nullptr), scopeId(id), scopeType(type), scopeName(name), returnType(returnType) {}

    ~Scope() {
        for (Scope* child : children) {
            delete child;
        }
    }

    void addChildScope(Scope* child) {
        if (child) {
            child->parent = this;
            children.push_back(child);
        }
    }

    Scope* createChildScope(int id, string type, string name, string returnType) {
        Scope* newScope = new Scope(id, type, name, returnType);
        addChildScope(newScope);
        return newScope;
    }

    void addRecord(Record* record) {
        if (!record) return;

        switch (record->getRecordType()) {
            case RecordType::Class:
                classes[record->getName()] = dynamic_cast<Class*>(record);
                break;
            case RecordType::Method:
                methods[record->getName()] = dynamic_cast<Method*>(record);
                break;
            case RecordType::Variable:
                variables[record->getName()] = dynamic_cast<Variable*>(record);
                break;
            default:
                // Handle error or unknown record type
                break;
        }
    }

    string findRecordType(const string& name) {
        if (variables.find(name) != variables.end()) return "Variable";
        if (methods.find(name) != methods.end()) return "Method";
        if (classes.find(name) != classes.end()) return "Class";
        if (inheritedVariables.find(name) != inheritedVariables.end()) return "Inherited Variable";
        return "Not Found";
    }

    Record* getRecord(const string& name) {
        if (variables.count(name)) return variables[name];
        if (methods.count(name)) return methods[name];
        if (classes.count(name)) return classes[name];
        if (inheritedVariables.count(name)) return inheritedVariables[name];
        return nullptr; // Record not found
    }

    Record* lookupRecord(const string& name) {
        for (Scope* current = this; current != nullptr; current = current->parent) {
            Record* record = current->getRecord(name);
            if (record) return record;
        }
        return nullptr; // Record not found in current scope or any parent scope
    }
};

class SymbolTable{
	public:
};

#endif