#ifndef LLL_H
#define LLL_H

#include <stddef.h>
#include <llvm-c/Core.h>
#include <llvm-c/Transforms/PassBuilder.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LLL_L_SAVE_STR                           \
    {                                            \
        yylval.str = malloc(strlen(yytext) + 1); \
        strcpy(yylval.str, yytext);              \
    }

#define LLL_Y_SAVE_AS_HEAP_PTR(dst, val) \
    {                                    \
        dst = malloc(sizeof(val));       \
        memcpy(dst, &val, sizeof(val));  \
    }

typedef struct LLLType LLLType;
typedef struct LLLExpression LLLExpression;
typedef struct LLLStatement LLLStatement;
typedef struct LLLDeclerationNode LLLDeclerationNode;
typedef struct LLLStatementNode LLLStatementNode;
typedef struct LLLDeclerations LLLDeclerations;

typedef struct LLLTypes
{
    LLLType *types;
    uint64_t count;
} LLLTypes;

typedef struct LLLArguments
{
    LLLExpression *args;
    uint64_t count;
} LLLArguments;

struct LLLType
{
    enum
    {
        LLL_TYPE_VOID,
        LLL_TYPE_UINT,
        LLL_TYPE_INT,
        LLL_TYPE_PTR,
        LLL_TYPE_F16,
        LLL_TYPE_F32,
        LLL_TYPE_F64,
        LLL_TYPE_F128,
        LLL_TYPE_ARR,
        LLL_TYPE_STRUCT,
        LLL_TYPE_FUNC,
    } type;
    union
    {
        uint32_t intBitsize;
        LLLType *ptrType;
        struct
        {
            LLLType *arrType;
            uint64_t size;
        } arr_data;
        LLLDeclerations *struct_data;
        struct
        {
            LLLTypes params;
            LLLType *returnType;
        } func_data;
    } data;
};

typedef struct LLLDecleration
{
    LLLType type;
    char *name;
} LLLDecleration;

typedef struct LLLVariable
{
    LLLDecleration decleration;
    LLVMValueRef pointer;
} LLLVariable;

typedef struct LLLVariableStorage LLLVariableStorage;
struct LLLVariableStorage
{
    LLLVariable *vars;
    uint64_t varCount;
    LLLVariableStorage *parent;
};

struct LLLDeclerations
{
    LLLDecleration *decls;
    uint64_t count;
};

struct LLLExpression
{
    enum
    {
        LLL_EXPRESSION_ADD,
        LLL_EXPRESSION_SUB,
        LLL_EXPRESSION_MUL,
        LLL_EXPRESSION_DIV,
        LLL_EXPRESSION_REM,
        LLL_EXPRESSION_LESSER,
        LLL_EXPRESSION_LEQ,
        LLL_EXPRESSION_GREATER,
        LLL_EXPRESSION_GEQ,
        LLL_EXPRESSION_EQEQ,
        LLL_EXPRESSION_NEQ,
        LLL_EXPRESSION_IDENT,
        LLL_EXPRESSION_IDENT_DECL,
        LLL_EXPRESSION_NUM_VAL,
        LLL_EXPRESSION_EQ,
        LLL_EXPRESSION_NEG,
        LLL_EXPRESSION_FUNC_CALL,
    } type;
    union
    {
        struct
        {
            LLLExpression *left;
            LLLExpression *right;
        } binary;
        char *str;
        LLLDecleration decl;
        struct
        {
            char *str;
            LLLType type;
        } num_val;
        LLLExpression *exp;
        struct
        {
            LLLExpression *funcVal;
            LLLArguments args;
        } funcCall;
    } data;
};

typedef struct LLLStatementBlock
{
    LLLStatement *stmts;
    uint64_t count;
    LLLVariableStorage vars;
} LLLStatementBlock;

struct LLLStatement
{
    enum
    {
        LLL_STATEMENT_EXP,
        LLL_STATEMENT_IF,
        LLL_STATEMENT_LOOP,
        LLL_STATEMENT_RETURN,
        LLL_STATEMENT_BLOCK,
    } type;
    union
    {
        LLLExpression *exp;
        struct
        {
            LLLExpression *cond;
            LLLStatement *ifTrue;
            LLLStatement *ifFalse;
        } ifData;
        struct
        {
            LLLExpression *cond;
            LLLStatement *whileTrue;
        } whileData;
        LLLStatementBlock block;
    } data;
};

// typedef struct LLLFunction
// {
//     LLLDeclerations params;
//     LLLType returnType;
//     LLLStatement body;
//     char *name;
// } LLLFunction;

// typedef struct LLLFunctionDecl
// {
//     LLLType type;
//     char *name;
// } LLLFunctionDecl;
typedef struct LLLGlobalStatement
{
    enum
    {
        LLL_GLOB_STMT_FUNCTION,
        LLL_GLOB_STMT_FUNCTION_DECL,
        LLL_GLOB_STMT_NUM_VAL,
    } type;
    union
    {
        struct
        {
            LLLDeclerations params;
            LLLType returnType;
            LLLStatement body;
        } func;
        LLLType funcDecl;
        struct
        {
            char *str;
            LLLType type;
        } numVal;
    } data;
    char *name;
    bool public;
} LLLGlobalStatement;

typedef struct LLLGlobal
{
    // LLLFunction funcs[64];
    // uint8_t funcCount;
    // LLLFunctionDecl decls[64];
    // uint8_t declCount;
    LLLGlobalStatement *stmts;
    uint64_t count;
} LLLGlobal;

LLVMTypeRef LLLType_getLLVMType(LLLType type);
void LLLExpression_Get(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef *out, LLLType *outType);
void LLLExpression_GetAsPointer(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef *out, LLLType *outType);
void LLLExpression_Set(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef in);
void LLLStatement_Build(LLLStatement *statement, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef func, LLVMValueRef returnPointer, LLVMBasicBlockRef returnBlock);
void LLLStatement_BuildDecls(LLLStatement *statement, LLVMBuilderRef builder, LLLVariableStorage *vars);
void LLLExpression_BuildDecls(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars);
void LLL_NumberCast(LLVMValueRef in, LLLType inType, LLLType outType, LLVMBuilderRef builder, LLVMValueRef *out);
void LLLType_combindNumbersTypes(LLLType types[2], LLLType *out);
void LLLVariableStorage_getVariable(LLLVariableStorage vars, char *name, LLLVariable *out);
// void LLLFunction_Build(LLLFunction *function, LLVMBuilderRef builder, LLVMModuleRef module);
// void LLLFunctionDecl_Build(LLLFunctionDecl *decl, LLVMModuleRef module);
void LLLGlobalStatement_Build(LLLGlobalStatement *globStmt, LLVMModuleRef module, LLVMBuilderRef builder, LLLVariableStorage *globalVars);
void LLL_r(LLLGlobal glob);

extern int yylex();

#include "lll.tab.h"

#endif /* LLL_H */