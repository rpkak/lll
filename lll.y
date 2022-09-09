%code requires {
    #include "lll.h"
}

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "lll.h"

    void yyerror(const char *s)
    {
        printf("ERROR: %s", s);
    }
%}

%union {
    LLLType *type;
    LLLTypes *types;
    LLLDecleration *decl;
    LLLDeclerations *decls;
    LLLExpression *exp;
    LLLStatement *stmt;
    char *str;
    // LLLFunction *func;
    // LLLFunctionDecl *func_decl;
    LLLGlobal *glob;
    LLLGlobalStatement *glob_stmt;
    LLLArguments *args;
}

%token ADD
%token SUB
%token MUL
%token DIV
%token REM
%token EQ
%token EQEQ
%token NEQ
%token LESSER
%token GREATER
%token LEQ
%token GEQ
%token SEMICOLON
%token <str> IDENT
%token F16
%token F32
%token F64
%token F128
%token <str> UINT
%token <str> INT
%token VOID
%token <str> INT_VAL
%token <str> FLOAT_VAL
%token OPEN_SQUARE_BRACKET
%token CLOSE_SQUARE_BRACKET
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_CURLY_BRACE
%token CLOSE_CURLY_BRACE
%token START_STRUCT
%token COMMAR
%token START_FUNC
%token ARROW
%token IF
%token ELSE
%token LOOP

%left OPEN_BRACKET
%right EQ
%left EQEQ NEQ LEQ LESSER GEQ GREATER
%left ADD SUB
%left MUL DIV REM

%type <type> type num_type func_type
%type <types> types types_or_not
%type <decl> decl
%type <decls> decls decls_or_not
%type <exp> exp set_exp
%type <stmt> stmt block not_empty_block
// %type <func> func
// %type <func_decl> func_decl
%type <glob> global
%type <glob_stmt> glob_stmt exportable_glob_stmt
%type <args> args args_or_not


%start pro

%%
pro: global { LLLGlobal glob = *$1; LLL_r(glob); }
   ;

global: glob_stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLGlobal){.count = 1, .stmts = $1})); }
      | global glob_stmt { $$ = $1; $$->stmts = realloc($$->stmts, sizeof(LLLGlobalStatement) * ++$$->count); $$->stmts[$$->count - 1] = *$2; free($2); }

stmt: exp SEMICOLON { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_EXP, .data={.exp=$1}})); }
    | IF OPEN_BRACKET exp CLOSE_BRACKET stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_IF, .data={.ifData={.cond=$3, .ifTrue=$5}}})); }
    | IF OPEN_BRACKET exp CLOSE_BRACKET stmt ELSE stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_IF, .data={.ifData={.cond=$3, .ifTrue=$5, .ifFalse=$7}}})); }
    | EQ exp SEMICOLON { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_RETURN, .data={.exp=$2}})); }
    | LOOP OPEN_BRACKET exp CLOSE_BRACKET stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_LOOP, .data={.whileData={.cond=$3, .whileTrue=$5}}})); }
    | block { $$ = $1; }
    ;

block: OPEN_CURLY_BRACE not_empty_block CLOSE_CURLY_BRACE { $$ = $2; }
     | OPEN_CURLY_BRACE CLOSE_CURLY_BRACE { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_BLOCK})); }
     ;

not_empty_block: not_empty_block stmt { $$ = $1; $$->data.block.stmts = realloc($$->data.block.stmts, sizeof(LLLStatement) * ++$$->data.block.count); $$->data.block.stmts[$$->data.block.count-1] = *$2; free($2); }
               | stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLStatement){.type=LLL_STATEMENT_BLOCK, .data={.block={.stmts=$1, .count=1}}})); }
               ;

exp: exp ADD exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_ADD, .data={.binary={$1, $3}}})); }
   | exp SUB exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_SUB, .data={.binary={$1, $3}}})); }
   | exp MUL exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_MUL, .data={.binary={$1, $3}}})); }
   | exp DIV exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_DIV, .data={.binary={$1, $3}}})); }
   | exp REM exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_REM, .data={.binary={$1, $3}}})); }
   | exp EQEQ exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_EQEQ, .data={.binary={$1, $3}}})); }
   | exp NEQ exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NEQ, .data={.binary={$1, $3}}})); }
   | exp LESSER exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_LESSER, .data={.binary={$1, $3}}})); }
   | exp LEQ exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_LEQ, .data={.binary={$1, $3}}})); }
   | exp GREATER exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_GREATER, .data={.binary={$1, $3}}})); }
   | exp GEQ exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_GEQ, .data={.binary={$1, $3}}})); }
   | set_exp EQ exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_EQ, .data={.binary={$1, $3}}})); }
   | set_exp { $$ = $1; }
   | INT_VAL num_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=$1, .type=*$2}}})); free($2); }
   | FLOAT_VAL num_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=$1, .type=*$2}}})); free($2); }
   | SUB exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NEG, .data={.exp=$2}})); }
   | OPEN_BRACKET exp CLOSE_BRACKET { $$ = $2; }
   | exp OPEN_BRACKET args_or_not CLOSE_BRACKET {LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_FUNC_CALL, .data={.funcCall={.funcVal=$1, .args=*$3}}})); free($3); }
   ;

// glob_exp: INT_VAL num_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=$1, .type=*$2}}})); free($2); }
//         | SUB INT_VAL num_type { char *newStrVal = malloc(strlen($1) + 2); strcpy(newStrVal + 1 * sizeof(char), $1); newStrVal[0] = '-'; LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=newStrVal, .type=*$2}}})); free($2); }
//         | FLOAT_VAL num_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=$1, .type=*$2}}})); free($2); }
//         | SUB FLOAT_VAL num_type { char *newStrVal = malloc(strlen($1) + 2); strcpy(newStrVal + 1 * sizeof(char), $1); newStrVal[0] = '-'; LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_NUM_VAL, .data={.num_val={.str=newStrVal, .type=*$2}}})); free($2); }
//         ;


set_exp: IDENT { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_IDENT, .data={.str=$1}})); }
       | decl { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLExpression){.type=LLL_EXPRESSION_IDENT_DECL, .data={.decl=*$1}})); free($1); }
       ;


decl: type IDENT { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLDecleration){.type = *$1, .name = $2})); free($1); }
    ;

decls_or_not: decls { $$ = $1; }
            | { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLDeclerations){})); }
            ;

decls: decl { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLDeclerations){.count = 1, .decls=$1})); }
     | decls COMMAR decl { $$ = $1; $$->decls = realloc($$->decls, sizeof(LLLDecleration) * ++$$->count); $$->decls[$$->count-1] = *$3; free($3); }
     ;

types_or_not: types { $$ = $1; }
            | { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLTypes){})); }
            ;

types: type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLTypes){.count = 1, .types=$1})); }
     | types COMMAR type { $$ = $1; $$->types = realloc($$->types, sizeof(LLLType) * ++$$->count); $$->types[$$->count-1] = *$3; free($3); }
     ;

type: VOID { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_VOID})); }
    | type MUL { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_PTR, .data = {.ptrType = $1}})); }
    | type OPEN_SQUARE_BRACKET INT_VAL CLOSE_SQUARE_BRACKET { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_ARR, .data = {.arr_data = {.arrType = $1, .size = strtoull($3, NULL, 10)}}})); }
    | START_STRUCT decls_or_not CLOSE_CURLY_BRACE { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_STRUCT, .data = {.struct_data = $2}})); }
    | num_type { $$ = $1; }
    | func_type { $$ = $1; }
    ;

num_type: UINT { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_UINT, .data = {.intBitsize = strtoul($1+2*sizeof(char), NULL, 10)}})); }
        | INT { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_INT, .data = {.intBitsize = strtoul($1+2*sizeof(char), NULL, 10)}})); }
        | F16 { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_F16})); }
        | F32 { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_F32})); }
        | F64 { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_F64})); }
        | F128 { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_F128})); }
        ;

func_type: START_FUNC types_or_not ARROW type CLOSE_BRACKET { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLType){.type = LLL_TYPE_FUNC, .data = {.func_data = {.params = *$2, .returnType = $4}}})); free($2); }
         ;

// func: OPEN_BRACKET decls_or_not ARROW type CLOSE_BRACKET stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLFunction){.params = *$2, .returnType = *$4, .body = *$6})); free($2); free($4); free($6); }
//     | OPEN_BRACKET decls_or_not ARROW type CLOSE_BRACKET IDENT stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLFunction){.params = *$2, .returnType = *$4, .body = *$7, .name = $6})); free($2); free($4); free($7); }
//     ;

// func_decl: IDENT func_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLFunctionDecl){.type = *$2, .name = $1})); free($2); }
//          ;

exportable_glob_stmt: IDENT EQ OPEN_BRACKET decls_or_not ARROW type CLOSE_BRACKET stmt { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLGlobalStatement){.type = LLL_GLOB_STMT_FUNCTION, .data = {.func = {.params = *$4, .returnType = *$6, .body = *$8}}, .name = $1, .public = false})); free($4); free($6); free($8); }
                    | IDENT EQ INT_VAL num_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLGlobalStatement){.type=LLL_GLOB_STMT_NUM_VAL, .data={.numVal={.str=$3, .type=*$4}}, .name = $1, .public = false})); free($4); }

glob_stmt: exportable_glob_stmt { $$ = $1; }
         | IDENT func_type { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLGlobalStatement){.type = LLL_GLOB_STMT_FUNCTION_DECL, .data = {.funcDecl = *$2}, .name = $1, .public = false})); free($2); }
         | GREATER exportable_glob_stmt { $$ = $2; $$->public = true; }
         ;

args: exp { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLArguments){.count = 1, .args=$1})); }
    | args COMMAR exp { $$ = $1; $$->args = realloc($$->args, sizeof(LLLExpression) * ++$$->count); $$->args[$$->count-1] = *$3; free($3); }

args_or_not: args { $$ = $1; }
           | { LLL_Y_SAVE_AS_HEAP_PTR($$, ((LLLArguments){.count = 0})); }
%%
