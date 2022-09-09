%{
    #include "lll.h"
%}

%%
"+"                     { return ADD; }
"-"                     { return SUB; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"%"                     { return REM; }
">="                    { return LEQ; }
"<="                    { return GEQ; }
"="                     { return EQ; }
"=="                    { return EQEQ; }
"!="                    { return NEQ; }
"<"                     { return LESSER; }
">"                     { return GREATER; }
";"                     { return SEMICOLON; }
[a-zA-Z_][a-zA-Z0-9_]*  { LLL_L_SAVE_STR; return IDENT; }
[ \t\n]                 {}
"$f16"                  { return F16; }
"$f32"                  { return F32; }
"$f64"                  { return F64; }
"$f128"                 { return F128; }
\$u[1-9][0-9]{0,6}      { LLL_L_SAVE_STR; return UINT; }
\$i[1-9][0-9]{0,6}      { LLL_L_SAVE_STR; return INT; }
"$void"                 { return VOID; }
[0-9]+                  { LLL_L_SAVE_STR; return INT_VAL; }
[0-9]*\.[0-9]*          { LLL_L_SAVE_STR; return FLOAT_VAL; }
"["                     { return OPEN_SQUARE_BRACKET; }
"]"                     { return CLOSE_SQUARE_BRACKET; }
"("                     { return OPEN_BRACKET; }
")"                     { return CLOSE_BRACKET; }
"{"                     { return OPEN_CURLY_BRACE; }
"}"                     { return CLOSE_CURLY_BRACE; }
"${"                    { return START_STRUCT; }
","                     { return COMMAR; }
"$("                    { return START_FUNC; }
"->"                    { return ARROW; }
"?"                     { return IF; }
":"                     { return ELSE; }
"°"                     { return LOOP; }
<<EOF>>                 { return YYEOF; }
.                       { printf("Unknown token\n"); yyterminate(); };
%%
