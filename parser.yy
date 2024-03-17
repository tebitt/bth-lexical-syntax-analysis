%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}

%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
}

// definition of set of tokens. All tokens are of type string
%token <std::string> THIS NOT CLASS PUBLIC STATIC VOID MAIN STRING IF ELSE WHILE NEW TRUE FALSE PRINT INT BOOLEAN IDENTIFIER RETURN INTEGER_LITERAL LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMICOLON COMMA DOT ASSIGN PLUS MINUS MULT DIV AND OR LT GT EQ LENGTH
%token END 0 "end of file"

// Operator precedence (if needed)
%left OR
%left AND
%nonassoc LT GT EQ
%left PLUS MINUS
%left MULT DIV
%right NOT


// Type definitions for non-terminals
%type <Node *> Goal MainClass ClassDeclaration VarDeclaration MethodDeclaration Type Statement Expression Identifier ClassDeclarationList StatementList VarDeclarationList MethodDeclarationList TypeIdentList ExpressionList VarDeclarationOrStatementList Return ReturnType ClassConditional

%%
Goal: MainClass END { 
    root = new Node("Goal", "", yylineno);
    root->children.push_back($1);
    std::cout << "Goal w/o Class Declaration" << std::endl;
};
    | MainClass ClassDeclarationList END { 
    root = new Node("Goal", "", yylineno); 
    root->children.push_back($1);
    root->children.push_back($2);
    std::cout << "Goal w/ Class Declaration" << std::endl;
};
    

MainClass: PUBLIC CLASS Identifier LBRACE PUBLIC STATIC ReturnType Identifier LPAREN TypeIdentList RPAREN LBRACE StatementList RBRACE RBRACE {
    $$ = new Node("MainClassDeclaration", "", yylineno);
    $$->children.push_back($3); 
    Node* Main_Method = new Node("ClassConditional", "", yylineno);
    $$->children.push_back(Main_Method);
    $$->children.push_back($7);
    $$->children.push_back($8);
    $$->children.push_back($10);
    $$->children.push_back($13);
    std::cout << "Main Class" << std::endl;
};  

ClassDeclaration: CLASS Identifier LBRACE ClassConditional RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2); 
    $$->children.push_back($4);  
    std::cout << "Class Declaration" << std::endl;
};
    | CLASS Identifier LBRACE RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2);
    std::cout << "Class Declaration w/o VarDeclaration & MethodDeclaration" << std::endl;
};

ClassConditional: VarDeclarationList {
    $$ = new Node("ClassConditional", "", yylineno);
    $$->children.push_back($1);
    std::cout << "Class Conditional w/o MethodDeclaration" << std::endl;
};
    | MethodDeclarationList {
    $$ = new Node("ClassConditional", "", yylineno);
    $$->children.push_back($1);
    std::cout << "Class Conditional w/o VarDeclaration" << std::endl;
};
    | ClassConditional VarDeclarationList {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "Class Conditional VarDeclarationList Recursion" << std::endl;
};  | ClassConditional MethodDeclarationList {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "Class Conditional MethodDeclarationList Recursion" << std::endl;
};

VarDeclaration: Type Identifier SEMICOLON {
    $$ = new Node("VarDeclaration", "", yylineno);
    $$->children.push_back($1); 
    $$->children.push_back($2);
    std::cout << "Var Declaration" << std::endl;
};

MethodDeclaration: PUBLIC ReturnType Identifier LPAREN TypeIdentList RPAREN LBRACE VarDeclarationOrStatementList Return RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($5); 
    $$->children.push_back($8);
    $$->children.push_back($9); 
    std::cout << "Method Declaration" << std::endl;
}; 
    | PUBLIC ReturnType Identifier LPAREN TypeIdentList RPAREN LBRACE Return RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($5);
    $$->children.push_back($8);
    std::cout << "Method Declaration w/o VarOrState Declaration" << std::endl;
};
    | PUBLIC ReturnType Identifier LPAREN RPAREN LBRACE VarDeclarationOrStatementList Return RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno );
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($7);
    $$->children.push_back($8);
    std::cout << "Method Declaration w/o TypeIdent" << std::endl;
};
    | PUBLIC ReturnType Identifier LPAREN RPAREN LBRACE Return RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($7);
    std::cout << "Method Declaration w/o TypeIdent & VarOrState Declaration" << std::endl;
};

Type: INT LBRACKET RBRACKET { $$ = new Node("Type", "IntArray", yylineno); };
    | BOOLEAN { $$ = new Node("Type", "Boolean", yylineno); };
    | INT { $$ = new Node("Type", "Int", yylineno); };
    | Identifier { $$ = $1; };
    | STRING { $$ = new Node("Type", "String", yylineno); };
    | STRING LBRACKET RBRACKET { $$ = new Node("Type", "StringArray", yylineno); };

Statement: LBRACE RBRACE { std::cout << "Empty Block" << std::endl; };
    | LBRACE StatementList RBRACE { $$ = new Node("Statement", "Block", yylineno); $$->children.push_back($2); };
    | IF LPAREN Expression RPAREN Statement ELSE Statement { $$ = new Node("Statement", "IfElse", yylineno); $$->children.push_back($3); $$->children.push_back($5); $$->children.push_back($7); std::cout << "If else" << std::endl; };
    | IF LPAREN Expression RPAREN Statement { $$ = new Node("Statement", "If", yylineno); $$->children.push_back($3); $$->children.push_back($5); std::cout << "If" << std::endl; };
    | WHILE LPAREN Expression RPAREN Statement { $$ = new Node("Statement", "While", yylineno); $$->children.push_back($3); $$->children.push_back($5); std::cout << "While" << std::endl; };
    | PRINT LPAREN Expression RPAREN SEMICOLON { $$ = new Node("Statement", "Print", yylineno); $$->children.push_back($3); std::cout << "Print" << std::endl; };
    | Identifier ASSIGN Expression SEMICOLON { $$ = new Node("Statement", "Assign", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Assign" << std::endl; };
    | Identifier LBRACKET Expression RBRACKET ASSIGN Expression SEMICOLON { 
        $$ = new Node("Statement", "Assign", yylineno); 
        Node* Index = new Node("ArrayIndex", "", yylineno);
        $$->children.push_back(Index);
        $$->children.push_back($6);
        Index->children.push_back($1);
        Index->children.push_back($3); 
        std::cout << "Array Assign" << std::endl; };

Expression: Expression PLUS Expression { $$ = new Node("Expression", "Plus", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Plus" << std::endl; };
    | Expression MINUS Expression { $$ = new Node("Expression", "Minus", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Minus" << std::endl; };
    | Expression MULT Expression { $$ = new Node("Expression", "Multiply", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Multiply" << std::endl; };
    | Expression DIV Expression { $$ = new Node("Expression", "Divide", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Divide" << std::endl; };
    | Expression AND Expression { $$ = new Node("Expression", "And", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "And" << std::endl;}
    | Expression OR Expression { $$ = new Node("Expression", "Or", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Or" << std::endl;}
    | Expression LT Expression { $$ = new Node("Expression", "LessThan", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Less Than" << std::endl;}
    | Expression GT Expression { $$ = new Node("Expression", "GreaterThan", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Greater Than" << std::endl;}
    | Expression EQ Expression { $$ = new Node("Expression", "Equal", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Equal" << std::endl; }; 
    | Expression LBRACKET Expression RBRACKET { $$ = new Node("Expression", "ArrayIndex", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Array Lookup" << std::endl; };
    | Expression DOT LENGTH { $$ = new Node("Expression", "ArrayLength", yylineno); $$->children.push_back($1); std::cout << "Array Length" << std::endl; }; 
    | Expression DOT Identifier LPAREN ExpressionList RPAREN { 
        $$ = new Node("Expression", "Call", yylineno); 
        $$->children.push_back($1); 
        Node* Called_Method = new Node("Called_Method", "", yylineno);
        $$->children.push_back(Called_Method);
        Called_Method->children.push_back($3);
        $$->children.push_back($5); 
        std::cout << "Call" << std::endl; };
    | INTEGER_LITERAL { $$ = new Node("IntegerLiteral", $1, yylineno); std::cout << "Integer Literal" << std::endl; };
    | TRUE { $$ = new Node("Expression", "True", yylineno); std::cout << "True" << std::endl; };
    | FALSE { $$ = new Node("Expression", "False", yylineno); std::cout << "False" << std::endl; };
    | Identifier { $$ = $1; std::cout << "Identifier" << std::endl;}
    | THIS { 
        $$ = new Node("Expression", "This", yylineno); 
        Node *This = new Node("This", "", yylineno);
        $$->children.push_back(This);
        std::cout << "This" << std::endl; };
    | NEW INT LBRACKET Expression RBRACKET { $$ = new Node("Expression", "NewArray", yylineno); $$->children.push_back($4); std::cout << "New Array" << std::endl; };
    | NEW Identifier LPAREN RPAREN { $$ = new Node("Expression", "NewIdentifier", yylineno); $$->children.push_back($2); std::cout << "New Object" << std::endl; };
    | NOT Expression { $$ = new Node("Expression", "Not", yylineno); $$->children.push_back($2); std::cout << "Not" << std::endl;}
    | LPAREN Expression RPAREN { $$ = $2; std::cout << "Parenthesis" << std::endl; };

ClassDeclarationList: ClassDeclaration {
    $$ = new Node("ClassDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "ClassDeclarationList" << std::endl;
};
    | ClassDeclarationList ClassDeclaration {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "ClassDeclarationList Recursion" << std::endl;
};

StatementList: Statement { 
    $$ = new Node("StatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "StatementList" << std::endl;
};
    | StatementList Statement {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "StatementList Recursion" << std::endl;
};

VarDeclarationList: VarDeclaration {
    $$ = new Node("VarDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarDeclarationList" << std::endl;
};
    | VarDeclarationList VarDeclaration {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarDeclarationList Recursion" << std::endl;
};

MethodDeclarationList: MethodDeclaration{
    $$ = new Node("MethodDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "MethodDeclarationList" << std::endl;
};
    | MethodDeclarationList MethodDeclaration{
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "MethodDeclarationList Recursion" << std::endl;
};

TypeIdentList: Type Identifier {
    $$ = new Node("TypeIdentList", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($2);
    std::cout << "TypeIdentList" << std::endl;
};
    | TypeIdentList COMMA Type Identifier {
    $$ = $1;
    $$->children.push_back($3);
    $$->children.push_back($4);
    std::cout << "TypeIdentList Recursion" << std::endl;
};

VarDeclarationOrStatementList: VarDeclaration {
    $$ = new Node("VarDeclarationOrStatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarOrState Variable" << std::endl;
};
    | Statement {
    $$ = new Node("VarDeclarationOrStatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarOrState Statement" << std::endl;
};
    | VarDeclarationOrStatementList Statement {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarOrState Statement Recursion" << std::endl;
};
    | VarDeclarationOrStatementList VarDeclaration {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarOrState Variable Recursion" << std::endl;
};

ReturnType: Type{
    $$ = $1;
    std::cout << "Return Type" << std::endl;
};
    | VOID {
    $$ = new Node("Type", "Void", yylineno);
    std::cout << "Void Return Type" << std::endl;
};

Return: RETURN ReturnType SEMICOLON{
    $$ = new Node("Return", "", yylineno);
    $$->children.push_back($2);
    std::cout << "Return" << std::endl;
};
    | RETURN Expression SEMICOLON {
    $$ = new Node("Return", "", yylineno);
    $$->children.push_back($2);
    std::cout << "Return w/ Expression" << std::endl;
};
    | RETURN SEMICOLON {
    $$ = new Node("Return", "", yylineno);
    std::cout << "Return w/o Expression or Return Type" << std::endl;
};

ExpressionList: Expression {
    $$ = new Node("ExpressionList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "Expression List" << std::endl; 
};
    | ExpressionList COMMA Expression {
    $$ = $1;
    $$->children.push_back($3);
    std::cout << "Expression List Recursion" << std::endl;
};
    | {
    $$ = new Node("ExpressionList", "", yylineno);
    std::cout << "Empty Expression List" << std::endl;
};

Identifier: IDENTIFIER { $$ = new Node("Identifier", $1, yylineno); std::cout << "Identifier " << $1 << std::endl; };
        | MAIN { $$ = new Node("Identifier ", "main", yylineno); std::cout << "Identifier main" << std::endl; };