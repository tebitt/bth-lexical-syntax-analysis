// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 13 "parser.yy"

  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;

#line 54 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 127 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_TypeIdentList: // TypeIdentList
      case symbol_kind::S_VarDeclarationOrStatementList: // VarDeclarationOrStatementList
      case symbol_kind::S_ExpressionList: // ExpressionList
      case symbol_kind::S_Identifier: // Identifier
        value.YY_MOVE_OR_COPY< Node * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MULT: // MULT
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LENGTH: // LENGTH
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_TypeIdentList: // TypeIdentList
      case symbol_kind::S_VarDeclarationOrStatementList: // VarDeclarationOrStatementList
      case symbol_kind::S_ExpressionList: // ExpressionList
      case symbol_kind::S_Identifier: // Identifier
        value.move< Node * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MULT: // MULT
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LENGTH: // LENGTH
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_TypeIdentList: // TypeIdentList
      case symbol_kind::S_VarDeclarationOrStatementList: // VarDeclarationOrStatementList
      case symbol_kind::S_ExpressionList: // ExpressionList
      case symbol_kind::S_Identifier: // Identifier
        value.copy< Node * > (that.value);
        break;

      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MULT: // MULT
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LENGTH: // LENGTH
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_TypeIdentList: // TypeIdentList
      case symbol_kind::S_VarDeclarationOrStatementList: // VarDeclarationOrStatementList
      case symbol_kind::S_ExpressionList: // ExpressionList
      case symbol_kind::S_Identifier: // Identifier
        value.move< Node * > (that.value);
        break;

      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MULT: // MULT
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LENGTH: // LENGTH
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_TypeIdentList: // TypeIdentList
      case symbol_kind::S_VarDeclarationOrStatementList: // VarDeclarationOrStatementList
      case symbol_kind::S_ExpressionList: // ExpressionList
      case symbol_kind::S_Identifier: // Identifier
        yylhs.value.emplace< Node * > ();
        break;

      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_PRINT: // PRINT
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LPAREN: // LPAREN
      case symbol_kind::S_RPAREN: // RPAREN
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_PLUS: // PLUS
      case symbol_kind::S_MINUS: // MINUS
      case symbol_kind::S_MULT: // MULT
      case symbol_kind::S_DIV: // DIV
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LENGTH: // LENGTH
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // Goal: MainClass "end of file"
#line 39 "parser.yy"
                    { 
    root = new Node("Goal", "", yylineno); 
    std::cout << "Goal w/o Class Declaration" << std::endl;
}
#line 821 "parser.tab.cc"
    break;

  case 3: // Goal: MainClass ClassDeclarationList "end of file"
#line 43 "parser.yy"
                                        { 
    root = new Node("Goal", "", yylineno); 
    root->children.push_back(yystack_[1].value.as < Node * > ());
    std::cout << "Goal w/ Class Declaration" << std::endl;
}
#line 831 "parser.tab.cc"
    break;

  case 4: // MainClass: PUBLIC CLASS Identifier LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACKET RBRACKET Identifier RPAREN LBRACE StatementList RBRACE RBRACE
#line 50 "parser.yy"
                                                                                                                                                       {
    yylhs.value.as < Node * > () = new Node("MainClass", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[15].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[5].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); 
    std::cout << "Main Class" << std::endl;
}
#line 843 "parser.tab.cc"
    break;

  case 5: // ClassDeclaration: CLASS Identifier LBRACE VarDeclarationList MethodDeclarationList RBRACE
#line 58 "parser.yy"
                                                                                          {
    yylhs.value.as < Node * > () = new Node("ClassDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); 
    std::cout << "Class Declaration" << std::endl;
}
#line 855 "parser.tab.cc"
    break;

  case 6: // ClassDeclaration: CLASS Identifier LBRACE MethodDeclarationList RBRACE
#line 65 "parser.yy"
                                                           {
    yylhs.value.as < Node * > () = new Node("ClassDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
    std::cout << "Class Declaration w/o VarDeclaration" << std::endl;
}
#line 866 "parser.tab.cc"
    break;

  case 7: // ClassDeclaration: CLASS Identifier LBRACE VarDeclarationList RBRACE
#line 71 "parser.yy"
                                                        {
    yylhs.value.as < Node * > () = new Node("ClassDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
    std::cout << "Class Declaration w/o MethodDeclaration" << std::endl;
}
#line 877 "parser.tab.cc"
    break;

  case 8: // ClassDeclaration: CLASS Identifier LBRACE RBRACE
#line 77 "parser.yy"
                                     {
    yylhs.value.as < Node * > () = new Node("ClassDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
    std::cout << "Class Declaration w/o VarDeclaration & MethodDeclaration" << std::endl;
}
#line 887 "parser.tab.cc"
    break;

  case 9: // VarDeclaration: Type Identifier SEMICOLON
#line 83 "parser.yy"
                                          {
    yylhs.value.as < Node * > () = new Node("VarDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); 
    std::cout << "Var Declaration" << std::endl;
}
#line 898 "parser.tab.cc"
    break;

  case 10: // MethodDeclaration: PUBLIC Type Identifier LPAREN TypeIdentList RPAREN LBRACE VarDeclarationOrStatementList RETURN Expression SEMICOLON RBRACE
#line 90 "parser.yy"
                                                                                                                                              {
    yylhs.value.as < Node * > () = new Node("MethodDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[10].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[9].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[7].value.as < Node * > ()); 
    yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(new Node("Return", yystack_[3].value.as < std::string > (), yylineno));
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); 
    std::cout << "Method Declaration" << std::endl;
}
#line 913 "parser.tab.cc"
    break;

  case 11: // MethodDeclaration: PUBLIC Type Identifier LPAREN TypeIdentList RPAREN LBRACE RETURN Expression SEMICOLON RBRACE
#line 100 "parser.yy"
                                                                                                   {
    yylhs.value.as < Node * > () = new Node("MethodDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[9].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[8].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[6].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(new Node("Return", yystack_[3].value.as < std::string > (), yylineno));
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
    std::cout << "Method Declaration w/o VarOrState Declaration" << std::endl;
}
#line 927 "parser.tab.cc"
    break;

  case 12: // MethodDeclaration: PUBLIC Type Identifier LPAREN RPAREN LBRACE VarDeclarationOrStatementList RETURN Expression SEMICOLON RBRACE
#line 109 "parser.yy"
                                                                                                                   {
    yylhs.value.as < Node * > () = new Node("MethodDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[9].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[8].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(new Node("Return", yystack_[3].value.as < std::string > (), yylineno));
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
    std::cout << "Method Declaration w/o TypeIdent" << std::endl;
}
#line 941 "parser.tab.cc"
    break;

  case 13: // MethodDeclaration: PUBLIC Type Identifier LPAREN RPAREN LBRACE RETURN Expression SEMICOLON RBRACE
#line 118 "parser.yy"
                                                                                     {
    yylhs.value.as < Node * > () = new Node("MethodDeclaration", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[8].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[7].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(new Node("Return", yystack_[3].value.as < std::string > (), yylineno));
    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
    std::cout << "Method Declaration w/o TypeIdent & VarOrState Declaration" << std::endl;
}
#line 954 "parser.tab.cc"
    break;

  case 14: // Type: INT LBRACKET RBRACKET
#line 127 "parser.yy"
                            { yylhs.value.as < Node * > () = new Node("ArrayType", "int", yylineno); }
#line 960 "parser.tab.cc"
    break;

  case 15: // Type: BOOLEAN
#line 128 "parser.yy"
              { yylhs.value.as < Node * > () = new Node("BooleanType", "", yylineno); }
#line 966 "parser.tab.cc"
    break;

  case 16: // Type: INT
#line 129 "parser.yy"
          { yylhs.value.as < Node * > () = new Node("IntegerType", "", yylineno); }
#line 972 "parser.tab.cc"
    break;

  case 17: // Type: Identifier
#line 130 "parser.yy"
                 { yylhs.value.as < Node * > () = yystack_[0].value.as < Node * > (); }
#line 978 "parser.tab.cc"
    break;

  case 18: // Statement: LBRACE RBRACE
#line 132 "parser.yy"
                         { std::cout << "Empty Block" << std::endl; }
#line 984 "parser.tab.cc"
    break;

  case 19: // Statement: LBRACE StatementList RBRACE
#line 133 "parser.yy"
                                  { yylhs.value.as < Node * > () = new Node("Block", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); }
#line 990 "parser.tab.cc"
    break;

  case 20: // Statement: IF LPAREN Expression RPAREN Statement ELSE Statement
#line 134 "parser.yy"
                                                           { yylhs.value.as < Node * > () = new Node("IfElse", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "If else" << std::endl;}
#line 996 "parser.tab.cc"
    break;

  case 21: // Statement: IF LPAREN Expression RPAREN Statement
#line 135 "parser.yy"
                                            { yylhs.value.as < Node * > () = new Node("If", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "If" << std::endl;}
#line 1002 "parser.tab.cc"
    break;

  case 22: // Statement: WHILE LPAREN Expression RPAREN Statement
#line 136 "parser.yy"
                                               { yylhs.value.as < Node * > () = new Node("While", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "While" << std::endl;}
#line 1008 "parser.tab.cc"
    break;

  case 23: // Statement: PRINT LPAREN Expression RPAREN SEMICOLON
#line 137 "parser.yy"
                                               { yylhs.value.as < Node * > () = new Node("Print", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); std::cout << "Print" << std::endl;}
#line 1014 "parser.tab.cc"
    break;

  case 24: // Statement: Identifier ASSIGN Expression SEMICOLON
#line 138 "parser.yy"
                                             { yylhs.value.as < Node * > () = new Node("Assign", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); std::cout << "Assign" << std::endl;}
#line 1020 "parser.tab.cc"
    break;

  case 25: // Statement: Identifier LBRACKET Expression RBRACKET ASSIGN Expression SEMICOLON
#line 139 "parser.yy"
                                                                          { yylhs.value.as < Node * > () = new Node("ArrayAssign", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[6].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); std::cout << "Array Assign" << std::endl;}
#line 1026 "parser.tab.cc"
    break;

  case 26: // Expression: Expression PLUS Expression
#line 141 "parser.yy"
                                       { yylhs.value.as < Node * > () = new Node("Plus", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Plus" << std::endl;}
#line 1032 "parser.tab.cc"
    break;

  case 27: // Expression: Expression MINUS Expression
#line 142 "parser.yy"
                                  { yylhs.value.as < Node * > () = new Node("Minus", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Minus" << std::endl;}
#line 1038 "parser.tab.cc"
    break;

  case 28: // Expression: Expression MULT Expression
#line 143 "parser.yy"
                                 { yylhs.value.as < Node * > () = new Node("Multiply", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Multiply" << std::endl;}
#line 1044 "parser.tab.cc"
    break;

  case 29: // Expression: Expression DIV Expression
#line 144 "parser.yy"
                                { yylhs.value.as < Node * > () = new Node("Divide", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Divide" << std::endl;}
#line 1050 "parser.tab.cc"
    break;

  case 30: // Expression: Expression AND Expression
#line 145 "parser.yy"
                                { yylhs.value.as < Node * > () = new Node("And", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "And" << std::endl;}
#line 1056 "parser.tab.cc"
    break;

  case 31: // Expression: Expression OR Expression
#line 146 "parser.yy"
                               { yylhs.value.as < Node * > () = new Node("Or", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Or" << std::endl;}
#line 1062 "parser.tab.cc"
    break;

  case 32: // Expression: Expression LT Expression
#line 147 "parser.yy"
                               { yylhs.value.as < Node * > () = new Node("LessThan", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Less Than" << std::endl;}
#line 1068 "parser.tab.cc"
    break;

  case 33: // Expression: Expression GT Expression
#line 148 "parser.yy"
                               { yylhs.value.as < Node * > () = new Node("GreaterThan", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Greater Than" << std::endl;}
#line 1074 "parser.tab.cc"
    break;

  case 34: // Expression: Expression EQ Expression
#line 149 "parser.yy"
                               { yylhs.value.as < Node * > () = new Node("Equal", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Equal" << std::endl;}
#line 1080 "parser.tab.cc"
    break;

  case 35: // Expression: Expression LBRACKET Expression RBRACKET
#line 150 "parser.yy"
                                              { yylhs.value.as < Node * > () = new Node("ArrayLookup", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); std::cout << "Array Lookup" << std::endl;}
#line 1086 "parser.tab.cc"
    break;

  case 36: // Expression: Expression DOT LENGTH
#line 151 "parser.yy"
                            { yylhs.value.as < Node * > () = new Node("ArrayLength", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); std::cout << "Array Length" << std::endl;}
#line 1092 "parser.tab.cc"
    break;

  case 37: // Expression: Expression DOT Identifier LPAREN ExpressionList RPAREN
#line 152 "parser.yy"
                                                             { yylhs.value.as < Node * > () = new Node("Call", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[5].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); std::cout << "Call" << std::endl;}
#line 1098 "parser.tab.cc"
    break;

  case 38: // Expression: Expression DOT Identifier LPAREN RPAREN
#line 153 "parser.yy"
                                              { yylhs.value.as < Node * > () = new Node("Call", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ()); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); std::cout << "Call w/o Expression" << std::endl;}
#line 1104 "parser.tab.cc"
    break;

  case 39: // Expression: INTEGER_LITERAL
#line 154 "parser.yy"
                      { yylhs.value.as < Node * > () = new Node("IntegerLiteral", yystack_[0].value.as < std::string > (), yylineno); std::cout << "Integer Literal" << std::endl;}
#line 1110 "parser.tab.cc"
    break;

  case 40: // Expression: TRUE
#line 155 "parser.yy"
           { yylhs.value.as < Node * > () = new Node("BooleanLiteral", "true", yylineno); std::cout << "True" << std::endl;}
#line 1116 "parser.tab.cc"
    break;

  case 41: // Expression: FALSE
#line 156 "parser.yy"
            { yylhs.value.as < Node * > () = new Node("BooleanLiteral", "false", yylineno); std::cout << "False" << std::endl;}
#line 1122 "parser.tab.cc"
    break;

  case 42: // Expression: Identifier
#line 157 "parser.yy"
                 { yylhs.value.as < Node * > () = yystack_[0].value.as < Node * > (); std::cout << "Identifier" << std::endl;}
#line 1128 "parser.tab.cc"
    break;

  case 43: // Expression: THIS
#line 158 "parser.yy"
           { yylhs.value.as < Node * > () = new Node("This", "", yylineno); std::cout << "This" << std::endl;}
#line 1134 "parser.tab.cc"
    break;

  case 44: // Expression: NEW INT LBRACKET Expression RBRACKET
#line 159 "parser.yy"
                                           { yylhs.value.as < Node * > () = new Node("NewArray", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ()); std::cout << "New Array" << std::endl;}
#line 1140 "parser.tab.cc"
    break;

  case 45: // Expression: NEW Identifier LPAREN RPAREN
#line 160 "parser.yy"
                                   { yylhs.value.as < Node * > () = new Node("NewIdentifier", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ()); std::cout << "New Object" << std::endl;}
#line 1146 "parser.tab.cc"
    break;

  case 46: // Expression: NOT Expression
#line 161 "parser.yy"
                     { yylhs.value.as < Node * > () = new Node("Not", "", yylineno); yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ()); std::cout << "Not" << std::endl;}
#line 1152 "parser.tab.cc"
    break;

  case 47: // Expression: LPAREN Expression RPAREN
#line 162 "parser.yy"
                               { yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > (); std::cout << "Parenthesis" << std::endl; }
#line 1158 "parser.tab.cc"
    break;

  case 48: // ClassDeclarationList: ClassDeclaration
#line 164 "parser.yy"
                                      {
    yylhs.value.as < Node * > () = new Node("ClassDeclarationList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "ClassDeclarationList" << std::endl;
}
#line 1168 "parser.tab.cc"
    break;

  case 49: // ClassDeclarationList: ClassDeclarationList ClassDeclaration
#line 169 "parser.yy"
                                           {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "ClassDeclarationList Recursion" << std::endl;
}
#line 1178 "parser.tab.cc"
    break;

  case 50: // StatementList: Statement
#line 175 "parser.yy"
                        { 
    yylhs.value.as < Node * > () = new Node("StatementList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "StatementList" << std::endl;
}
#line 1188 "parser.tab.cc"
    break;

  case 51: // StatementList: StatementList Statement
#line 180 "parser.yy"
                             {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "StatementList Recursion" << std::endl;
}
#line 1198 "parser.tab.cc"
    break;

  case 52: // VarDeclarationList: VarDeclaration
#line 186 "parser.yy"
                                   {
    yylhs.value.as < Node * > () = new Node("VarDeclarationList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarDeclarationList" << std::endl;
}
#line 1208 "parser.tab.cc"
    break;

  case 53: // VarDeclarationList: VarDeclarationList VarDeclaration
#line 191 "parser.yy"
                                        {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarDeclarationList Recursion" << std::endl;
}
#line 1218 "parser.tab.cc"
    break;

  case 54: // MethodDeclarationList: MethodDeclaration
#line 197 "parser.yy"
                                        {
    yylhs.value.as < Node * > () = new Node("MethodDeclarationList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "MethodDeclarationList" << std::endl;
}
#line 1228 "parser.tab.cc"
    break;

  case 55: // MethodDeclarationList: MethodDeclarationList MethodDeclaration
#line 202 "parser.yy"
                                             {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "MethodDeclarationList Recursion" << std::endl;
}
#line 1238 "parser.tab.cc"
    break;

  case 56: // TypeIdentList: Type Identifier
#line 208 "parser.yy"
                               {
    yylhs.value.as < Node * > () = new Node("TypeIdentList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "TypeIdentList" << std::endl;
}
#line 1249 "parser.tab.cc"
    break;

  case 57: // TypeIdentList: TypeIdentList COMMA Type Identifier
#line 214 "parser.yy"
                                          {
    yylhs.value.as < Node * > () = yystack_[3].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "TypeIdentList Recursion" << std::endl;
}
#line 1260 "parser.tab.cc"
    break;

  case 58: // VarDeclarationOrStatementList: VarDeclaration
#line 221 "parser.yy"
                                              {
    yylhs.value.as < Node * > () = new Node("VarDeclarationOrStatementList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarOrState Variable" << std::endl;
}
#line 1270 "parser.tab.cc"
    break;

  case 59: // VarDeclarationOrStatementList: Statement
#line 226 "parser.yy"
                {
    yylhs.value.as < Node * > () = new Node("VarDeclarationOrStatementList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarOrState Statement" << std::endl;
}
#line 1280 "parser.tab.cc"
    break;

  case 60: // VarDeclarationOrStatementList: VarDeclarationOrStatementList Statement
#line 231 "parser.yy"
                                              {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarOrState Statement Recursion" << std::endl;
}
#line 1290 "parser.tab.cc"
    break;

  case 61: // VarDeclarationOrStatementList: VarDeclarationOrStatementList VarDeclaration
#line 236 "parser.yy"
                                                   {
    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "VarOrState Variable Recursion" << std::endl;
}
#line 1300 "parser.tab.cc"
    break;

  case 62: // ExpressionList: Expression
#line 242 "parser.yy"
                           {
    yylhs.value.as < Node * > () = new Node("ExpressionList", "", yylineno);
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "Expression List" << std::endl; 
}
#line 1310 "parser.tab.cc"
    break;

  case 63: // ExpressionList: ExpressionList COMMA Expression
#line 247 "parser.yy"
                                      {
    yylhs.value.as < Node * > () = yystack_[2].value.as < Node * > ();
    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
    std::cout << "Expression List Recursion" << std::endl;
}
#line 1320 "parser.tab.cc"
    break;

  case 64: // Identifier: IDENTIFIER
#line 253 "parser.yy"
                       { yylhs.value.as < Node * > () = new Node("Identifier ", yystack_[0].value.as < std::string > (), yylineno); std::cout << "Identifier " << yystack_[0].value.as < std::string > () << std::endl; }
#line 1326 "parser.tab.cc"
    break;


#line 1330 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -20;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
       0,     4,    18,     8,     5,   -20,   -20,     5,   -20,    19,
     -20,    32,    42,   -20,   -20,    25,    73,    33,   103,    14,
     -20,   -20,   -20,   -20,     5,   136,    -2,   -20,    63,     5,
      46,    53,   -20,   -20,    -1,   -20,   -20,    81,    66,   -20,
     -20,   -20,    71,    43,    89,    77,     5,     7,    85,   435,
     -20,    90,   103,    88,    95,   100,    13,   101,   403,   -20,
     -20,   446,    17,   457,     5,     5,    13,    13,   -20,    13,
      37,   -20,   -20,   -20,    13,   200,   -20,    13,   -20,   -20,
     407,    17,    13,   -20,   -20,    13,    13,    13,   468,   -20,
     102,   137,   153,    50,   106,   104,   169,    13,   111,   -10,
      13,    13,    13,    13,    13,    13,    13,    13,    13,   185,
     -20,   -20,   215,   230,   245,   260,    13,   116,   472,   472,
      13,   117,   -20,   275,   -20,   -20,   119,    16,    16,    50,
      50,   361,   350,   372,   372,   372,    98,   121,   122,   -20,
     124,   290,   472,   141,   -20,   305,   -20,   -20,   115,   -20,
     -20,    13,   -20,   134,   421,   472,   -20,   -20,   335,    59,
     320,   -20,   135,   -20,   -20,    13,   -20,   -20,   335
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     1,     2,     0,    48,     0,
      64,     0,     0,     3,    49,     0,     0,     0,     0,    16,
      15,     8,    52,    54,     0,     0,     0,    17,     0,     0,
       0,     0,     7,    53,     0,     6,    55,     0,     0,    14,
       9,     5,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,     0,    17,     0,     0,     0,     0,     0,    43,     0,
       0,    40,    41,    39,     0,     0,    42,     0,    18,    50,
       0,     0,     0,    61,    60,     0,     0,     0,     0,    57,
       0,     0,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,     0,    13,    36,     0,    26,    27,    28,
      29,    30,    31,    32,    33,    34,     0,     0,     0,    24,
       0,     0,     0,    21,    22,     0,    45,    35,     0,    23,
      12,     0,    11,     0,     0,     0,    44,    38,    62,     0,
       0,    10,     0,    20,    37,     0,    25,     4,    63
  };

  const short
  parser::yypgoto_[] =
  {
     -20,   -20,   -20,   152,   -15,   -19,   -16,    -8,     1,   -20,
      20,   -20,   140,   -20,   118,   -20,    -4
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     8,    59,    23,    24,    60,    75,     9,
      80,    25,    26,    47,    61,   159,    76
  };

  const short
  parser::yytable_[] =
  {
      11,    22,    29,    12,    18,    18,     1,    36,     6,     4,
      33,    10,    27,     7,    27,    36,    68,    69,     5,    13,
      31,    27,    35,    41,     7,    38,    10,    46,    70,    71,
      72,    17,   125,    51,    10,    73,    64,    52,    74,    27,
      28,    30,    50,    97,    85,    62,    83,    99,    27,    86,
      79,   102,   103,    84,    81,    15,    94,    62,    10,    62,
      89,    90,    19,    20,    10,    16,    95,    91,    92,    45,
      93,    37,   111,    83,    39,    96,    81,    97,   109,    18,
      84,    99,    40,   112,    62,   164,   113,   114,   115,   165,
      42,    43,    19,    20,    10,   126,    44,    21,   123,    48,
      49,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     143,   144,    53,    63,    81,    81,    65,   141,    68,    69,
      66,   145,    19,    20,    10,    67,    77,   149,   117,   121,
      70,    71,    72,   120,    79,   124,    10,    73,    81,   142,
      74,   157,    18,   146,   148,   150,   111,   163,   152,   158,
      81,    81,   160,   155,   151,    19,    20,    10,   161,   167,
      32,    14,   154,   118,    97,    34,   168,     0,    99,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   119,
      97,    88,     0,     0,    99,     0,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   122,    97,     0,     0,     0,
      99,     0,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   136,    97,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,     0,    98,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    97,     0,   137,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,   138,     0,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    97,     0,   139,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,     0,   140,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    97,   147,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,     0,   153,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    97,   156,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,     0,   166,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    97,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    97,     0,     0,
       0,    99,     0,   100,   101,   102,   103,   104,    97,   106,
     107,   108,    99,     0,   100,   101,   102,   103,     0,    97,
     106,   107,   108,    99,     0,   100,   101,   102,   103,     0,
       0,    -1,    -1,    -1,    54,     0,    55,     0,    54,     0,
      55,    57,     0,     0,    10,    57,    58,    78,    10,     0,
      58,   110,    54,     0,    55,     0,     0,     0,     0,    57,
       0,     0,    10,     0,    58,   162,    54,     0,    55,    56,
       0,     0,     0,    57,    19,    20,    10,    54,    58,    55,
      82,     0,     0,     0,    57,    19,    20,    10,    54,    58,
      55,    87,     0,     0,     0,    57,    19,    20,    10,    54,
      58,    55,   116,    54,     0,    55,    57,    19,    20,    10,
      57,    58,     0,    10,     0,    58
  };

  const short
  parser::yycheck_[] =
  {
       4,    16,    18,     7,     6,     6,     6,    26,     0,     5,
      25,    21,    16,     5,    18,    34,     3,     4,     0,     0,
      24,    25,    24,    24,     5,    29,    21,    43,    15,    16,
      17,     6,    42,    26,    21,    22,    52,    30,    25,    43,
       7,    27,    46,    27,    27,    49,    61,    31,    52,    32,
      58,    35,    36,    61,    58,    23,    19,    61,    21,    63,
      64,    65,    19,    20,    21,    23,    70,    66,    67,    26,
      69,     8,    80,    88,    28,    74,    80,    27,    77,     6,
      88,    31,    29,    82,    88,    26,    85,    86,    87,    30,
       9,    25,    19,    20,    21,    99,    25,    24,    97,    10,
      23,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     118,   119,    27,    23,   118,   119,    28,   116,     3,     4,
      25,   120,    19,    20,    21,    25,    25,    29,    26,    25,
      15,    16,    17,    27,   142,    24,    21,    22,   142,    23,
      25,    26,     6,    26,    25,    24,   154,   155,    24,   148,
     154,   155,   151,    12,    32,    19,    20,    21,    24,    24,
      24,     9,   142,    26,    27,    25,   165,    -1,    31,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    26,
      27,    63,    -1,    -1,    31,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    26,    27,    -1,    -1,    -1,
      31,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    26,    27,    -1,    -1,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    -1,    29,
      -1,    31,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    27,    -1,    29,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    28,    -1,
      -1,    31,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    27,    -1,    29,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    -1,    29,
      -1,    31,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    27,    28,    -1,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    -1,    29,
      -1,    31,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    27,    28,    -1,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    -1,    29,
      -1,    31,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    27,    -1,    -1,    -1,    31,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    27,    -1,    -1,
      -1,    31,    -1,    33,    34,    35,    36,    37,    27,    39,
      40,    41,    31,    -1,    33,    34,    35,    36,    -1,    27,
      39,    40,    41,    31,    -1,    33,    34,    35,    36,    -1,
      -1,    39,    40,    41,    11,    -1,    13,    -1,    11,    -1,
      13,    18,    -1,    -1,    21,    18,    23,    24,    21,    -1,
      23,    24,    11,    -1,    13,    -1,    -1,    -1,    -1,    18,
      -1,    -1,    21,    -1,    23,    24,    11,    -1,    13,    14,
      -1,    -1,    -1,    18,    19,    20,    21,    11,    23,    13,
      14,    -1,    -1,    -1,    18,    19,    20,    21,    11,    23,
      13,    14,    -1,    -1,    -1,    18,    19,    20,    21,    11,
      23,    13,    14,    11,    -1,    13,    18,    19,    20,    21,
      18,    23,    -1,    21,    -1,    23
  };

  const signed char
  parser::yystos_[] =
  {
       0,     6,    44,    45,     5,     0,     0,     5,    46,    52,
      21,    59,    59,     0,    46,    23,    23,     6,     6,    19,
      20,    24,    47,    48,    49,    54,    55,    59,     7,    49,
      27,    59,    24,    47,    55,    24,    48,     8,    59,    28,
      29,    24,     9,    25,    25,    26,    49,    56,    10,    23,
      59,    26,    30,    27,    11,    13,    14,    18,    23,    47,
      50,    57,    59,    23,    49,    28,    25,    25,     3,     4,
      15,    16,    17,    22,    25,    51,    59,    25,    24,    50,
      53,    59,    14,    47,    50,    27,    32,    14,    57,    59,
      59,    51,    51,    51,    19,    59,    51,    27,    29,    31,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    51,
      24,    50,    51,    51,    51,    51,    14,    26,    26,    26,
      27,    25,    26,    51,    24,    42,    59,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    26,    29,    28,    29,
      29,    51,    23,    50,    50,    51,    26,    28,    25,    29,
      24,    32,    24,    29,    53,    12,    28,    26,    51,    58,
      51,    24,    24,    50,    26,    30,    29,    24,    51
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    43,    44,    44,    45,    46,    46,    46,    46,    47,
      48,    48,    48,    48,    49,    49,    49,    49,    50,    50,
      50,    50,    50,    50,    50,    50,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    56,    57,    57,
      57,    57,    58,    58,    59
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     3,    18,     6,     5,     5,     4,     3,
      12,    11,    11,    10,     3,     1,     1,     1,     2,     3,
       7,     5,     5,     5,     4,     7,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     3,     6,     5,     1,
       1,     1,     1,     1,     5,     4,     2,     3,     1,     2,
       1,     2,     1,     2,     1,     2,     2,     4,     1,     1,
       2,     2,     1,     3,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "THIS", "NOT", "CLASS",
  "PUBLIC", "STATIC", "VOID", "MAIN", "STRING", "IF", "ELSE", "WHILE",
  "RETURN", "NEW", "TRUE", "FALSE", "PRINT", "INT", "BOOLEAN",
  "IDENTIFIER", "INTEGER_LITERAL", "LBRACE", "RBRACE", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "SEMICOLON", "COMMA", "DOT", "ASSIGN", "PLUS",
  "MINUS", "MULT", "DIV", "AND", "OR", "LT", "GT", "EQ", "LENGTH",
  "$accept", "Goal", "MainClass", "ClassDeclaration", "VarDeclaration",
  "MethodDeclaration", "Type", "Statement", "Expression",
  "ClassDeclarationList", "StatementList", "VarDeclarationList",
  "MethodDeclarationList", "TypeIdentList",
  "VarDeclarationOrStatementList", "ExpressionList", "Identifier", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    39,    39,    43,    50,    58,    65,    71,    77,    83,
      90,   100,   109,   118,   127,   128,   129,   130,   132,   133,
     134,   135,   136,   137,   138,   139,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   164,   169,
     175,   180,   186,   191,   197,   202,   208,   214,   221,   226,
     231,   236,   242,   247,   253
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1960 "parser.tab.cc"

