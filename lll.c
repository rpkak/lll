#include "lll.h"

LLVMTypeRef LLLType_getLLVMType(LLLType type)
{
    LLVMTypeRef *children;
    switch (type.type)
    {
    case LLL_TYPE_VOID:
        return LLVMVoidType();
    case LLL_TYPE_UINT:
        return LLVMIntType(type.data.intBitsize);
    case LLL_TYPE_INT:
        return LLVMIntType(type.data.intBitsize);
    case LLL_TYPE_PTR:
        return LLVMPointerType(LLLType_getLLVMType(*type.data.ptrType), 0);
    case LLL_TYPE_F16:
        return LLVMHalfType();
    case LLL_TYPE_F32:
        return LLVMFloatType();
    case LLL_TYPE_F64:
        return LLVMDoubleType();
    case LLL_TYPE_F128:
        return LLVMFP128Type();
    case LLL_TYPE_ARR:
        return LLVMArrayType(LLLType_getLLVMType(*type.data.arr_data.arrType), type.data.arr_data.size);
    case LLL_TYPE_STRUCT:
        children = malloc(sizeof(LLVMTypeRef) * type.data.struct_data->count);
        for (uint64_t i = 0; i < type.data.struct_data->count; i++)
        {
            children[i] = LLLType_getLLVMType(type.data.struct_data->decls[i].type);
        }
        return LLVMStructType(children, type.data.struct_data->count, false);
    case LLL_TYPE_FUNC:
        children = malloc(sizeof(LLVMTypeRef) * type.data.func_data.params.count);
        for (uint64_t i = 0; i < type.data.func_data.params.count; i++)
        {
            children[i] = LLLType_getLLVMType(type.data.func_data.params.types[i]);
        }

        return LLVMFunctionType(LLLType_getLLVMType(*type.data.func_data.returnType), children, type.data.func_data.params.count, false);
    default:
        return NULL;
    }
}
// void LLLExpression_Access(LLLExpression *expression, bool set, LLVMBuilderRef builder, LLLVariableStorage vars, LLVMValueRef *value)
// {
//     switch (expression->type)
//     {
//     case LLL_EXPRESSION_ADD:
//         LLVMValueRef left;
//         LLVMValueRef right;
//         LLLExpression_Access(expression->data.binary.left, false, builder, vars, &left);
//         LLLExpression_Access(expression->data.binary.right, false, builder, vars, &right);

//         switch (returnType.type)
//         {
//         case LLL_TYPE_UINT:
//         case LLL_TYPE_PTR:
//             *value = LLVMBuildNUWAdd(builder, left, right, "");
//             break;
//         case LLL_TYPE_INT:
//             *value = LLVMBuildNSWAdd(builder, left, right, "");
//             break;
//         case LLL_TYPE_F16:
//         case LLL_TYPE_F32:
//         case LLL_TYPE_F64:
//         case LLL_TYPE_F128:
//             *value = LLVMBuildFAdd(builder, left, right, "");
//             break;
//         default:
//             printf("LLLExpression_Access: ADD: Unhandled type %d\n", returnType.type);
//             break;
//         }

//         break;
//     case LLL_EXPRESSION_SUB:
//         LLVMValueRef left;
//         LLVMValueRef right;
//         LLLExpression_Access(expression->data.binary.left, false, builder, vars, &left);
//         LLLExpression_Access(expression->data.binary.right, false, builder, vars, &right);

//         switch (returnType.type)
//         {
//         case LLL_TYPE_UINT:
//         case LLL_TYPE_PTR:
//             *value = LLVMBuildNUWSub(builder, left, right, "");
//             break;
//         case LLL_TYPE_INT:
//             *value = LLVMBuildNSWSub(builder, left, right, "");
//             break;
//         case LLL_TYPE_F16:
//         case LLL_TYPE_F32:
//         case LLL_TYPE_F64:
//         case LLL_TYPE_F128:
//             *value = LLVMBuildFSub(builder, left, right, "");
//             break;
//         default:
//             printf("LLLExpression_Access: SUB: Unhandled type %d\n", returnType.type);
//             break;
//         }
//         break;
//     case LLL_EXPRESSION_MUL:
//         LLVMValueRef left;
//         LLVMValueRef right;
//         LLLExpression_Access(expression->data.binary.left, false, builder, vars, &left);
//         LLLExpression_Access(expression->data.binary.right, false, builder, vars, &right);

//         switch (returnType.type)
//         {
//         case LLL_TYPE_UINT:
//         case LLL_TYPE_PTR:
//             *value = LLVMBuildNUWMul(builder, left, right, "");
//             break;
//         case LLL_TYPE_INT:
//             *value = LLVMBuildNSWMul(builder, left, right, "");
//             break;
//         case LLL_TYPE_F16:
//         case LLL_TYPE_F32:
//         case LLL_TYPE_F64:
//         case LLL_TYPE_F128:
//             *value = LLVMBuildFMul(builder, left, right, "");
//             break;
//         default:
//             printf("LLLExpression_Access: MUL: Unhandled type %d\n", returnType.type);
//             break;
//         }
//         break;
//     case LLL_EXPRESSION_DIV:
//         LLVMValueRef left;
//         LLVMValueRef right;
//         LLLExpression_Access(expression->data.binary.left, false, builder, vars, &left);
//         LLLExpression_Access(expression->data.binary.right, false, builder, vars, &right);

//         switch (returnType.type)
//         {
//         case LLL_TYPE_UINT:
//         case LLL_TYPE_PTR:
//             *value = LLVMBuildUDiv(builder, left, right, "");
//             break;
//         case LLL_TYPE_INT:
//             *value = LLVMBuildSDiv(builder, left, right, "");
//             break;
//         case LLL_TYPE_F16:
//         case LLL_TYPE_F32:
//         case LLL_TYPE_F64:
//         case LLL_TYPE_F128:
//             *value = LLVMBuildFDiv(builder, left, right, "");
//             break;
//         default:
//             printf("LLLExpression_Access: DIV: Unhandled type %d\n", returnType.type);
//             break;
//         }
//         break;
//     }
// }

/*
struct LLLStatement
{
    enum
    {
        LLL_STATEMENT_EXP,
        LLL_STATEMENT_IF,
        LLL_STATEMENT_WHILE,
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

typedef struct LLLStatementBlock
{
    LLLStatement *stmts;
    uint64_t count;
} LLLStatementBlock;
*/

void LLLStatement_BuildDecls(LLLStatement *statement, LLVMBuilderRef builder, LLLVariableStorage *vars)
{
    switch (statement->type)
    {
    case LLL_STATEMENT_EXP:
    case LLL_STATEMENT_RETURN:
        LLLExpression_BuildDecls(statement->data.exp, builder, vars);
        break;
    case LLL_STATEMENT_IF:
        LLLExpression_BuildDecls(statement->data.ifData.cond, builder, vars);
        LLLStatement_BuildDecls(statement->data.ifData.ifTrue, builder, vars);
        if (statement->data.ifData.ifFalse != NULL)
        {
            LLLStatement_BuildDecls(statement->data.ifData.ifFalse, builder, vars);
        }
        break;
    case LLL_STATEMENT_LOOP:
        LLLExpression_BuildDecls(statement->data.whileData.cond, builder, vars);
        LLLStatement_BuildDecls(statement->data.whileData.whileTrue, builder, vars);
        break;
    case LLL_STATEMENT_BLOCK:
        statement->data.block.vars.parent = vars;
        for (uint64_t i = 0; i < statement->data.block.count; i++)
        {
            LLLStatement_BuildDecls(&statement->data.block.stmts[i], builder, &statement->data.block.vars);
        }
        break;
    }
}

void LLLExpression_BuildDecls(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars)
{
    switch (expression->type)
    {
    case LLL_EXPRESSION_ADD:
    case LLL_EXPRESSION_SUB:
    case LLL_EXPRESSION_MUL:
    case LLL_EXPRESSION_DIV:
    case LLL_EXPRESSION_REM:
    case LLL_EXPRESSION_LESSER:
    case LLL_EXPRESSION_LEQ:
    case LLL_EXPRESSION_GREATER:
    case LLL_EXPRESSION_GEQ:
    case LLL_EXPRESSION_EQEQ:
    case LLL_EXPRESSION_NEQ:
    case LLL_EXPRESSION_EQ:
        LLLExpression_BuildDecls(expression->data.binary.left, builder, vars);
        LLLExpression_BuildDecls(expression->data.binary.right, builder, vars);
        break;
    case LLL_EXPRESSION_IDENT:
    case LLL_EXPRESSION_NUM_VAL:
        break;
    case LLL_EXPRESSION_IDENT_DECL:
        if (vars->varCount == 0)
        {
            vars->vars = malloc(sizeof(LLLVariable));
            vars->varCount = 1;
        }
        else
        {
            vars->vars = realloc(vars->vars, sizeof(LLLVariable) * ++vars->varCount);
        }
        vars->vars[vars->varCount - 1] = (LLLVariable){
            .decleration = expression->data.decl,
            .pointer = LLVMBuildAlloca(builder, LLLType_getLLVMType(expression->data.decl.type), "varPtr"),
        };
        break;
    case LLL_EXPRESSION_NEG:
        LLLExpression_BuildDecls(expression->data.exp, builder, vars);
        break;
    }
}

void LLLExpression_GetAsPointer(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef *out, LLLType *outType)
{
    LLVMValueRef noPointer;
    LLLVariable var;

    switch (expression->type)
    {
    case LLL_EXPRESSION_IDENT:
        LLLVariableStorage_getVariable(*vars, expression->data.str, &var);

        *out = var.pointer;
        *outType = var.decleration.type;
        break;
    case LLL_EXPRESSION_IDENT_DECL:
        LLLVariableStorage_getVariable(*vars, expression->data.decl.name, &var);

        *out = var.pointer;
        *outType = var.decleration.type;
        break;
    default:
        LLLExpression_Get(expression, builder, vars, &noPointer, outType);
        *out = LLVMBuildAlloca(builder, LLLType_getLLVMType(*outType), "ptr");
        LLVMBuildStore(builder, noPointer, *out);
    }
}

void LLLExpression_Get(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef *out, LLLType *outType)
{
    LLVMValueRef left;
    LLVMValueRef right;
    LLVMValueRef leftUncasted;
    LLVMValueRef rightUncasted;
    LLLType leftOuttype;
    LLLType rightOuttype;
    LLLType combindedType;
    LLLVariable var;
    LLVMTypeRef varType;

    LLVMValueRef *args;

    switch (expression->type)
    {
    case LLL_EXPRESSION_ADD:

        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, outType);

        LLL_NumberCast(leftUncasted, leftOuttype, *outType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, *outType, builder, &right);

        switch (outType->type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildNUWAdd(builder, left, right, "uAdd");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildNSWAdd(builder, left, right, "iAdd");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFAdd(builder, left, right, "FAdd");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_SUB:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, outType);

        LLL_NumberCast(leftUncasted, leftOuttype, *outType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, *outType, builder, &right);

        switch (outType->type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildNUWSub(builder, left, right, "uSub");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildNSWSub(builder, left, right, "iSub");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFSub(builder, left, right, "fSub");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_MUL:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, outType);

        LLL_NumberCast(leftUncasted, leftOuttype, *outType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, *outType, builder, &right);

        switch (outType->type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildNUWMul(builder, left, right, "uMul");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildNSWMul(builder, left, right, "iMul");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFMul(builder, left, right, "fMul");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_DIV:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, outType);

        LLL_NumberCast(leftUncasted, leftOuttype, *outType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, *outType, builder, &right);

        switch (outType->type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildUDiv(builder, left, right, "uDiv");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildSDiv(builder, left, right, "iDiv");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFDiv(builder, left, right, "fDiv");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_REM:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, outType);

        LLL_NumberCast(leftUncasted, leftOuttype, *outType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, *outType, builder, &right);

        switch (outType->type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildURem(builder, left, right, "uRem");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildSRem(builder, left, right, "iRem");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFRem(builder, left, right, "fRem");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_EQEQ:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntEQ, left, right, "iEq");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealOEQ, left, right, "fEq");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_NEQ:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntNE, left, right, "iNe");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealUNE, left, right, "fNe");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_LESSER:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildICmp(builder, LLVMIntULT, left, right, "uLesser");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntSLT, left, right, "iLesser");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealOLT, left, right, "fLesser");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_LEQ:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildICmp(builder, LLVMIntULE, left, right, "uLeq");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntSLE, left, right, "iLeq");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealOLE, left, right, "fLeq");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_GREATER:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildICmp(builder, LLVMIntUGT, left, right, "uGreater");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntSGT, left, right, "iGreater");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealOGT, left, right, "fGreater");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_GEQ:
        LLLExpression_Get(expression->data.binary.left, builder, vars, &leftUncasted, &leftOuttype);
        LLLExpression_Get(expression->data.binary.right, builder, vars, &rightUncasted, &rightOuttype);

        LLLType_combindNumbersTypes((LLLType[2]){leftOuttype, rightOuttype}, &combindedType);

        LLL_NumberCast(leftUncasted, leftOuttype, combindedType, builder, &left);
        LLL_NumberCast(rightUncasted, rightOuttype, combindedType, builder, &right);

        switch (combindedType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildICmp(builder, LLVMIntUGE, left, right, "uGeq");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildICmp(builder, LLVMIntSGE, left, right, "iGeq");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFCmp(builder, LLVMRealOGE, left, right, "fGeq");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }

        *outType = (LLLType){
            .type = LLL_TYPE_UINT,
            .data = {
                .intBitsize = 1,
            },
        };

        break;
    case LLL_EXPRESSION_IDENT:
        LLLVariableStorage_getVariable(*vars, expression->data.str, &var);

        *out = LLVMBuildLoad2(builder, LLLType_getLLVMType(var.decleration.type), var.pointer, "varVal");
        *outType = var.decleration.type;
        break;
    case LLL_EXPRESSION_IDENT_DECL:
        LLLVariableStorage_getVariable(*vars, expression->data.decl.name, &var);

        *out = LLVMBuildLoad2(builder, LLLType_getLLVMType(var.decleration.type), var.pointer, "varVal");
        *outType = var.decleration.type;
        break;
    case LLL_EXPRESSION_EQ:
        LLLExpression_Get(expression->data.binary.right, builder, vars, out, outType);
        LLLExpression_Set(expression->data.binary.left, builder, vars, *out);
        break;
    case LLL_EXPRESSION_NUM_VAL:
        if (expression->data.num_val.type.type == LLL_TYPE_UINT || expression->data.num_val.type.type == LLL_TYPE_INT)
        {
            *out = LLVMConstIntOfString(LLLType_getLLVMType(expression->data.num_val.type), expression->data.num_val.str, 10);
        }
        else
        {
            *out = LLVMConstRealOfString(LLLType_getLLVMType(expression->data.num_val.type), expression->data.num_val.str);
        }
        *outType = expression->data.num_val.type;
        break;
    case LLL_EXPRESSION_NEG:
        LLLExpression_Get(expression->data.exp, builder, vars, &left, outType);

        if (outType->type == LLL_TYPE_UINT)
        {
            outType->type = LLL_TYPE_INT;
        }

        switch (outType->type)
        {
        case LLL_TYPE_INT:
            *out = LLVMBuildNeg(builder, left, "iNeg");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            *out = LLVMBuildFNeg(builder, left, "fNeg");
            break;
        default:
            // printf("LLLExpression_Get: ADD: Unhandled type %d\n", expression->valueType->type);
            break;
        }
        break;
    case LLL_EXPRESSION_FUNC_CALL:
        LLLExpression_GetAsPointer(expression->data.funcCall.funcVal, builder, vars, &left, &leftOuttype);

        *outType = *leftOuttype.data.func_data.returnType;

        args = malloc(sizeof(LLVMValueRef) * expression->data.funcCall.args.count);

        for (uint64_t i = 0; i < expression->data.funcCall.args.count; i++)
        {
            LLLExpression_Get(expression->data.funcCall.funcVal, builder, vars, &right, &rightOuttype);
            args[i] = right;
        }

        LLVMBuildCall2(builder, LLLType_getLLVMType(leftOuttype), left, args, expression->data.funcCall.args.count, "ret");
        break;
    }
}

void LLL_NumberCast(LLVMValueRef in, LLLType inType, LLLType outType, LLVMBuilderRef builder, LLVMValueRef *out)
{
    switch (inType.type)
    {
    case LLL_TYPE_UINT:
    case LLL_TYPE_INT:
        switch (outType.type)
        {
        case LLL_TYPE_UINT:
            if (inType.data.intBitsize == outType.data.intBitsize)
            {
                *out = in;
            }
            else if (inType.data.intBitsize < outType.data.intBitsize)
            {
                *out = LLVMBuildZExt(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            else
            {
                *out = LLVMBuildTrunc(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        case LLL_TYPE_INT:
            if (inType.data.intBitsize == outType.data.intBitsize)
            {
                *out = in;
            }
            else if (inType.data.intBitsize < outType.data.intBitsize)
            {
                *out = LLVMBuildSExt(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            else
            {
                *out = LLVMBuildTrunc(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        case LLL_TYPE_PTR:
            *out = LLVMBuildIntToPtr(builder, in, LLLType_getLLVMType(outType), "cast");
            break;
        case LLL_TYPE_F16:
        case LLL_TYPE_F32:
        case LLL_TYPE_F64:
        case LLL_TYPE_F128:
            if (inType.type == LLL_TYPE_UINT)
            {
                *out = LLVMBuildUIToFP(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            else
            {
                *out = LLVMBuildSIToFP(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        default:
            printf("LLL_NumberCast: Unsupported cast from int/uint to %d\n", outType.type);
            break;
        }
        break;
    case LLL_TYPE_PTR:
        switch (outType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildPtrToInt(builder, in, LLLType_getLLVMType(outType), "cast");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildPtrToInt(builder, in, LLLType_getLLVMType(outType), "cast");
            break;
        case LLL_TYPE_PTR:
            *out = in;
            break;
        default:
            printf("LLL_NumberCast: Unsupported cast from ptr to %d\n", outType.type);
            break;
        }
        break;
    case LLL_TYPE_F16:
    case LLL_TYPE_F32:
    case LLL_TYPE_F64:
    case LLL_TYPE_F128:
        switch (outType.type)
        {
        case LLL_TYPE_UINT:
            *out = LLVMBuildFPToUI(builder, in, LLLType_getLLVMType(outType), "cast");
            break;
        case LLL_TYPE_INT:
            *out = LLVMBuildFPToSI(builder, in, LLLType_getLLVMType(outType), "cast");
            break;
        case LLL_TYPE_F16:
            if (inType.type == LLL_TYPE_F16)
            {
                *out = in;
            }
            else
            {
                *out = LLVMBuildFPTrunc(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        case LLL_TYPE_F32:
            if (inType.type == LLL_TYPE_F32)
            {
                *out = in;
            }
            else if (inType.type == LLL_TYPE_F16)
            {
                *out = LLVMBuildFPExt(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            else
            {
                *out = LLVMBuildFPTrunc(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        case LLL_TYPE_F64:
            if (inType.type == LLL_TYPE_F64)
            {
                *out = in;
            }
            else if (inType.type == LLL_TYPE_F128)
            {
                *out = LLVMBuildFPTrunc(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            else
            {
                *out = LLVMBuildFPExt(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        case LLL_TYPE_F128:
            if (inType.type == LLL_TYPE_F128)
            {
                *out = in;
            }
            else
            {
                *out = LLVMBuildFPExt(builder, in, LLLType_getLLVMType(outType), "cast");
            }
            break;
        default:
            printf("LLL_NumberCast: Unsupported cast from float to %d\n", outType.type);
            break;
        }
        break;
    default:
        printf("LLL_NumberCast: %d is not a Number\n", inType.type);
        break;
    }
}

void LLLType_combindNumbersTypes(LLLType types[2], LLLType *out)
{
    enum
    {
        UINT = 0,
        INT = 1,
        FLOAT = 2,
    } formatType = UINT;
    uint32_t bitsize = 0;
    for (uint8_t i = 0; i < 2; i++)
    {
        if (types[i].type == LLL_TYPE_PTR)
        {
            // TODO: DataLayout
            bitsize = 64;
        }
        else if (types[i].type == LLL_TYPE_INT || types[i].type == LLL_TYPE_UINT)

        {
            bitsize = types[i].data.intBitsize > bitsize ? types[i].data.intBitsize : bitsize;
            if (types[i].type == LLL_TYPE_INT && formatType < INT)
            {
                formatType = INT;
            }
        }
        else if (types[i].type == LLL_TYPE_F16 || types[i].type == LLL_TYPE_F32 || types[i].type == LLL_TYPE_F64 || types[i].type == LLL_TYPE_F128)
        {
            formatType = FLOAT;
            switch (types[i].type)
            {
            case LLL_TYPE_F16:
                if (bitsize < 16)
                {
                    bitsize = 16;
                }
                break;
            case LLL_TYPE_F32:
                if (bitsize < 32)
                {
                    bitsize = 32;
                }
                break;
            case LLL_TYPE_F64:
                if (bitsize < 64)
                {
                    bitsize = 64;
                }
                break;
            case LLL_TYPE_F128:
                if (bitsize < 128)
                {
                    bitsize = 128;
                }
                break;
            default:
                break;
            }
        }
    }

    switch (formatType)
    {
    case FLOAT:
        if (bitsize <= 16)
        {
            *out = (LLLType){LLL_TYPE_F16};
        }
        else if (bitsize <= 32)
        {
            *out = (LLLType){LLL_TYPE_F32};
        }
        else if (bitsize <= 64)
        {
            *out = (LLLType){LLL_TYPE_F64};
        }
        else
        {
            *out = (LLLType){LLL_TYPE_F128};
        }
        break;
    case UINT:
        *out = (LLLType){
            LLL_TYPE_UINT,
            {.intBitsize = bitsize},
        };
        break;
    case INT:
        *out = (LLLType){
            LLL_TYPE_INT,
            {.intBitsize = bitsize},
        };
        break;
    }
}

void LLLExpression_Set(LLLExpression *expression, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef in)
{
    LLLVariable var;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (expression->type)
    {
    case LLL_EXPRESSION_IDENT:
        // for (uint64_t i = 0; i < vars->varCount; i++)
        // {
        //     if (!strcmp(vars->vars[i].decleration.name, expression->data.str))
        //     {
        //         LLVMBuildStore(builder, in, vars->vars[i].pointer);
        //         break;
        //     }
        // }
        LLLVariableStorage_getVariable(*vars, expression->data.str, &var);

        LLVMBuildStore(builder, in, var.pointer);
        break;
    case LLL_EXPRESSION_IDENT_DECL:
        // for (uint64_t i = 0; i < vars->varCount; i++)
        // {
        //     if (!strcmp(vars->vars[i].decleration.name, expression->data.decl.name))
        //     {
        //         LLVMBuildStore(builder, in, vars->vars[i].pointer);
        //         break;
        //     }
        // }
        LLLVariableStorage_getVariable(*vars, expression->data.decl.name, &var);

        LLVMBuildStore(builder, in, var.pointer);
        break;
    }
#pragma clang diagnostic pop
}

void LLLStatement_Build(LLLStatement *statement, LLVMBuilderRef builder, LLLVariableStorage *vars, LLVMValueRef func, LLVMValueRef returnPointer, LLVMBasicBlockRef returnBlock)
{

    LLVMValueRef expRes;
    LLVMValueRef cond;
    LLLType type;
    LLVMBasicBlockRef ifTrue;
    LLVMBasicBlockRef ifFalse;
    LLVMBasicBlockRef endIf;

    switch (statement->type)
    {
    case LLL_STATEMENT_EXP:
        LLLExpression_Get(statement->data.exp, builder, vars, &expRes, &type);
        break;
    case LLL_STATEMENT_IF:
        LLLExpression_Get(statement->data.ifData.cond, builder, vars, &expRes, &type);

        LLL_NumberCast(expRes, type, (LLLType){.type = LLL_TYPE_UINT, .data = {.intBitsize = 1}}, builder, &cond);

        ifTrue = LLVMAppendBasicBlock(func, "ifTrue");
        endIf = LLVMAppendBasicBlock(func, "endIf");

        if (statement->data.ifData.ifFalse == NULL)
        {
            LLVMBuildCondBr(builder, cond, ifTrue, endIf);
        }
        else
        {
            ifFalse = LLVMAppendBasicBlock(func, "ifFalse");
            LLVMBuildCondBr(builder, cond, ifTrue, ifFalse);
            LLVMPositionBuilderAtEnd(builder, ifFalse);
            LLLStatement_Build(statement->data.ifData.ifFalse, builder, vars, func, returnPointer, returnBlock);
            LLVMBuildBr(builder, endIf);
        }

        LLVMPositionBuilderAtEnd(builder, ifTrue);
        LLLStatement_Build(statement->data.ifData.ifTrue, builder, vars, func, returnPointer, returnBlock);
        LLVMBuildBr(builder, endIf);

        LLVMPositionBuilderAtEnd(builder, endIf);
        break;
    case LLL_STATEMENT_RETURN:
        LLLExpression_Get(statement->data.exp, builder, vars, &expRes, &type);
        LLVMBuildStore(builder, expRes, returnPointer);
    case LLL_STATEMENT_BLOCK:
        for (uint64_t i = 0; i < statement->data.block.count; i++)
        {
            LLLStatement_Build(&statement->data.block.stmts[i], builder, &statement->data.block.vars, func, returnPointer, returnBlock);
        }
        break;
    }
}

void LLLVariableStorage_getVariable(LLLVariableStorage vars, char *name, LLLVariable *out)
{
    for (uint64_t i = 0; i < vars.varCount; i++)
    {
        if (!strcmp(vars.vars[i].decleration.name, name))
        {
            *out = vars.vars[i];
            return;
        }
    }
    if (vars.parent != NULL)
    {
        LLLVariableStorage_getVariable(*vars.parent, name, out);
    }
}

// void LLLFunction_Build(LLLFunction *function, LLVMBuilderRef builder, LLVMModuleRef module)
// {
//     LLVMTypeRef *children = malloc(sizeof(LLVMTypeRef) * function->params.count);
//     for (uint64_t i = 0; i < function->params.count; i++)
//     {
//         children[i] = LLLType_getLLVMType(function->params.decls[i].type);
//     }

//     LLVMTypeRef functionType = LLVMFunctionType(LLLType_getLLVMType(&function->returnType), children, function->params.count, false);
//     LLVMValueRef funcVal;
//     if (function->name == NULL)
//     {
//         funcVal = LLVMAddFunction(module, "", functionType);
//     }
//     else
//     {
//         funcVal = LLVMAddFunction(module, function->name, functionType);
//     }

//     LLVMBasicBlockRef entry = LLVMAppendBasicBlock(funcVal, "entry");
//     LLVMBasicBlockRef returnBlock = LLVMAppendBasicBlock(funcVal, "return");
//     LLVMValueRef returnPointer;

//     LLVMPositionBuilderAtEnd(builder, entry);

//     if (function->returnType.type != LLL_TYPE_VOID)
//     {
//         returnPointer = LLVMBuildAlloca(builder, LLLType_getLLVMType(&function->returnType), "returnPtr");
//     }

//     LLLVariableStorage vars = {
//         .vars = malloc(sizeof(LLLVariable) * function->params.count),
//         .varCount = function->params.count,
//         .parent = NULL,
//     };

//     for (uint64_t i = 0; i < vars.varCount; i++)
//     {
//         vars.vars[i] = (LLLVariable){
//             .decleration = function->params.decls[i],
//             .pointer = LLVMBuildAlloca(builder, LLLType_getLLVMType(function->params.decls[i].type), "arg"),
//         };
//         LLVMBuildStore(builder, LLVMGetParam(funcVal, i), vars.vars[i].pointer);
//     }

//     LLLStatement_BuildDecls(&function->body, builder, &vars);
//     LLLStatement_Build(&function->body, builder, &vars, funcVal, returnPointer, returnBlock);

//     LLVMBuildBr(builder, returnBlock);

//     LLVMPositionBuilderAtEnd(builder, returnBlock);
//     if (function->returnType.type != LLL_TYPE_VOID)
//     {
//         LLVMValueRef returnVal = LLVMBuildLoad2(builder, LLLType_getLLVMType(&function->returnType), returnPointer, "return");
//         LLVMBuildRet(builder, returnVal);
//     }
//     else
//     {
//         LLVMBuildRetVoid(builder);
//     }

//     if (function->name == NULL)
//     {
//         LLVMSetLinkage(funcVal, LLVMInternalLinkage);
//     }
//     else
//     {
//         LLVMSetLinkage(funcVal, LLVMExternalLinkage);
//     }
// }

// void LLLFunctionDecl_Build(LLLFunctionDecl *decl, LLVMModuleRef module)
// {
//     LLVMAddFunction(module, decl->name, LLLType_getLLVMType(&decl->type));
// }

void LLLGlobalStatement_Build(LLLGlobalStatement *globStmt, LLVMModuleRef module, LLVMBuilderRef builder, LLLVariableStorage *globVars)
{
    LLLType functionType;
    if (globVars->varCount == 0)
    {
        globVars->vars = malloc(sizeof(LLLVariable));
        globVars->varCount = 1;
    }
    else
    {
        globVars->vars = realloc(globVars->vars, sizeof(LLLVariable) * ++globVars->varCount);
    }
    switch (globStmt->type)
    {
    case LLL_GLOB_STMT_FUNCTION:

        // *outType = (LLLType){
        //     .type = LLL_TYPE_FUNC,
        //     .data = {
        //         .func_data = {
        //             .params = malloc(sizeof(LLLType) * globStmt->data.func.params.count),
        //             .returnType = &globStmt->data.func.returnType,
        //         },
        //     },
        // };

        // LLVMTypeRef *children = malloc(sizeof(LLVMTypeRef) * globStmt->data.func.params.count);
        // for (uint64_t i = 0; i < globStmt->data.func.params.count; i++)
        // {
        //     children[i] = LLLType_getLLVMType(globStmt->data.func.params.decls[i].type);
        // }

        functionType = (LLLType){
            .type = LLL_TYPE_FUNC,
            .data = {
                .func_data = {
                    .params = {
                        .count = globStmt->data.func.params.count,
                        .types = malloc(sizeof(LLLType) * globStmt->data.func.params.count),
                    },
                    .returnType = &globStmt->data.func.returnType,
                },
            },
        };

        for (uint64_t i = 0; i < globStmt->data.func.params.count; i++)
        {
            functionType.data.func_data.params.types[i] = globStmt->data.func.params.decls[i].type;
        }

        // LLVMTypeRef functionType_ = LLVMFunctionType(LLLType_getLLVMType(globStmt->data.func.returnType), children, globStmt->data.func.params.count, false);

        globVars->vars[globVars->varCount - 1] = (LLLVariable){
            .decleration = (LLLDecleration){
                .name = globStmt->name,
                .type = functionType,
            },
            .pointer = LLVMAddFunction(module, globStmt->name, LLLType_getLLVMType(functionType)),
        };

        // LLVMValueRef *out;
        // *out = LLVMAddFunction(module, globStmt->name, functionType);

        LLVMBasicBlockRef entry = LLVMAppendBasicBlock(globVars->vars[globVars->varCount - 1].pointer, "entry");
        LLVMBasicBlockRef returnBlock = LLVMAppendBasicBlock(globVars->vars[globVars->varCount - 1].pointer, "return");
        LLVMValueRef returnPointer;

        LLVMPositionBuilderAtEnd(builder, entry);

        if (globStmt->data.func.returnType.type != LLL_TYPE_VOID)
        {
            returnPointer = LLVMBuildAlloca(builder, LLLType_getLLVMType(globStmt->data.func.returnType), "returnPtr");
        }

        LLLVariableStorage vars = {
            .vars = malloc(sizeof(LLLVariable) * globStmt->data.func.params.count),
            .varCount = globStmt->data.func.params.count,
            .parent = globVars,
        };

        for (uint64_t i = 0; i < vars.varCount; i++)
        {
            vars.vars[i] = (LLLVariable){
                .decleration = globStmt->data.func.params.decls[i],
                .pointer = LLVMBuildAlloca(builder, LLLType_getLLVMType(globStmt->data.func.params.decls[i].type), "arg"),
            };
            LLVMBuildStore(builder, LLVMGetParam(globVars->vars[globVars->varCount - 1].pointer, i), vars.vars[i].pointer);
        }

        LLLStatement_BuildDecls(&globStmt->data.func.body, builder, &vars);
        LLLStatement_Build(&globStmt->data.func.body, builder, &vars, globVars->vars[globVars->varCount - 1].pointer, returnPointer, returnBlock);

        LLVMBuildBr(builder, returnBlock);

        LLVMPositionBuilderAtEnd(builder, returnBlock);
        if (globStmt->data.func.returnType.type != LLL_TYPE_VOID)
        {
            LLVMValueRef returnVal = LLVMBuildLoad2(builder, LLLType_getLLVMType(globStmt->data.func.returnType), returnPointer, "return");
            LLVMBuildRet(builder, returnVal);
        }
        else
        {
            LLVMBuildRetVoid(builder);
        }
        break;
    case LLL_GLOB_STMT_FUNCTION_DECL:
        // *outType = globStmt->data.funcDecl;
        // *out = LLVMAddFunction(module, globStmt->name, LLLType_getLLVMType(&globStmt->data.funcDecl));
        globVars->vars[globVars->varCount - 1] = (LLLVariable){
            .decleration = (LLLDecleration){
                .name = globStmt->name,
                .type = globStmt->data.funcDecl,
            },
            .pointer = LLVMAddFunction(module, globStmt->name, LLLType_getLLVMType(globStmt->data.funcDecl)),
        };
        break;
    case LLL_GLOB_STMT_NUM_VAL:
        globVars->vars[globVars->varCount - 1] = (LLLVariable){
            .decleration = (LLLDecleration){
                .name = globStmt->name,
                .type = globStmt->data.numVal.type,
            },
            .pointer = LLVMAddGlobal(module, LLLType_getLLVMType(globStmt->data.numVal.type), globStmt->name),
        };
        // *out = LLVMAddGlobal(module, LLLType_getLLVMType(&globStmt->data.numVal.type), globStmt->name);
        break;
    }
    if (globStmt->public)
    {
        LLVMSetLinkage(globVars->vars[globVars->varCount - 1].pointer, LLVMExternalLinkage);
    }
    else
    {
        LLVMSetLinkage(globVars->vars[globVars->varCount - 1].pointer, LLVMInternalLinkage);
    }
}

int main()
{
    printf("\n ============= input =============\n\n");

    yyparse();
    // printf("1");
}

void LLL_r(LLLGlobal glob)
{
    // printf("1");
    LLVMModuleRef module = LLVMModuleCreateWithName("test");
    // LLVMTypeRef ftype = LLVMFunctionType(LLVMInt16Type(), NULL, 0, false);
    LLVMBuilderRef builder = LLVMCreateBuilder();
    // LLVMValueRef f = LLVMAddFunction(module, "demo", ftype);

    // LLLVariableStorage vars = {
    //     NULL,
    //     0,
    //     NULL,
    // };

    // LLVMBasicBlockRef e = LLVMAppendBasicBlock(f, "entry");
    // LLVMBasicBlockRef r = LLVMAppendBasicBlock(f, "return");
    // LLVMPositionBuilderAtEnd(builder, e);
    // LLVMValueRef retPtr = LLVMBuildAlloca(builder, LLVMInt16Type(), "returnPtr");

    // LLLStatement_BuildDecls(&stmt, builder, &vars);
    // LLLStatement_Build(&stmt, builder, &vars, f, retPtr, r);

    // LLVMBuildBr(builder, r);

    // LLVMPositionBuilderAtEnd(builder, r);
    // LLVMValueRef val = LLVMBuildLoad2(builder, LLVMInt16Type(), retPtr, "returnVal");
    // LLVMBuildRet(builder, val);
    // LLLFunction_Build(&func, builder, module);

    // // for (uint8_t i = 0; i < glob.funcCount; i++)
    // // {
    // //     LLLFunction_Build(&glob.funcs[i], builder, module);
    // // }

    LLLVariableStorage vars = {
        .vars = NULL,
        .varCount = 0,
        .parent = NULL,
    };

    for (uint64_t i = 0; i < glob.count; i++)
    {
        LLLGlobalStatement_Build(&glob.stmts[i], module, builder, &vars);
    }

    printf("\n ============= unopt =============\n\n");

    LLVMDumpModule(module);

    // #ifndef NOPT
    LLVMPassBuilderOptionsRef pbo = LLVMCreatePassBuilderOptions();

    char *triple = LLVMGetDefaultTargetTriple();

    LLVMInitializeNativeTarget();

    LLVMTargetRef target;
    LLVMGetTargetFromTriple(triple, &target, NULL);

    LLVMTargetMachineRef machine = LLVMCreateTargetMachine(target, triple, "generic", "", LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault);

    LLVMErrorRef err = LLVMRunPasses(module, "default<O3>", machine, pbo);

    if (err != LLVMErrorSuccess)
    {
        fprintf(stderr, "ERROR lll.c Just search this string asdfghjkl\n");
        fprintf(stderr, "%s\n", LLVMGetErrorMessage(err));
    }

    // #endif

    printf("\n ============== opt ==============\n\n");

    // printf("%p", err);

    LLVMDumpModule(module);

    printf("\n ============== end ==============\n\n");
}
