/*
***************************************************
@FILENAME   : LAB2_ANALYSE_C
@AUTHER     : lsc
@INITTIME   : 22-11-18
@ Copyright (c) 2022 lsc All rights reserved.
***************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
    num_INT = 258,
    num_FLOAT = 259,

    Y_ID = 260,

    Y_INT = 261,
    Y_VOID = 262,
    Y_CONST = 263,
    Y_IF = 264,
    Y_ELSE = 265,
    Y_WHILE = 266,
    Y_BREAK = 267,
    Y_CONTINUE = 268,
    Y_RETURN = 269,

    Y_ADD = 270,
    Y_SUB = 271,
    Y_MUL = 272,
    Y_DIV = 273,
    Y_MODULO = 274,
    Y_LESS = 275,
    Y_LESSEQ = 276,
    Y_GREAT = 277,
    Y_GREATEQ = 278,
    Y_NOTEQ = 279,
    Y_EQ = 280,
    Y_NOT = 281,
    Y_AND = 282,
    Y_OR = 283,
    Y_ASSIGN = 284,

    Y_LPAR = 285,
    Y_RPAR = 286,
    Y_LBRACKET = 287,
    Y_RBRACKET = 288,
    Y_LSQUARE = 289,
    Y_RSQUARE = 290,
    Y_COMMA = 291,
    Y_SEMICOLON = 292,

    Y_FLOAT = 293
};
extern int yylex();
extern int yylval;
extern char* yytext;

int tok;
void advance() {
    tok = yylex();
    printf("[+]read tok: %s\n", yytext);
}
/*
 * Block: Y_LBRACKET BlockItems Y_RBRACKET
         | Y_LBRACKET Y_RBRACKET
 */
void analyse_block() {
    if(tok != Y_LBRACKET) {
        printf("ERROR:Expect an LBRACKET.\n");
    }
    advance();
    if(tok == Y_RBRACKET) {
        
    }
}
int main(int argc, char **argv)
{
    while(1)
    {
        printf("input expression, 'q' to exit>");
        advance();
        int r = expr();
        printf("result: %d\n", r);

        //past rr = astExpr();
        //showAst(rr, 0);
    }
    return 0;
}