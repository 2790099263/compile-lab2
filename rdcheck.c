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
extern FILE* yyin;
extern char* yytext;

int tok;
void advance() {
    tok = yylex();
    printf("[+]advanced tok: %s\n", yytext);
}
/*function define part begin*/
int analyse_block();
int analyse_blockitems();
// int analyse_blockitem();
/*function define part end*/

/*
 * Block: Y_LBRACKET BlockItems Y_RBRACKET
         | Y_LBRACKET Y_RBRACKET
 */
int analyse_block() {
    if(tok != Y_LBRACKET) {
        printf("ERROR:Expect an LBRACKET.\n");
        return 0;
    }
    advance();
    return 1;
}
/*
 *  BlockItems: BlockItem
              | BlockItem BlockItems
 */
int analyse_blockitems() {
    // analyse_blockitem();
    advance();
    return 1;
}
/*main function*/
int main(int argc, char **argv)
{
    int now = 0;
    while(1)
    {
        advance();now++;
        advance();now++;
    }
    return 0;
}