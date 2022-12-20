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
#include "node_type.h"

#define DECL_PART
// #define DEBUG_OUT
// #define DEBUG
#define WHILE
#define DEBUG_LOG

/*Linked List part begin*/
typedef struct list_node
{
    /* data */
    int val;
    char s[10];
    struct list *lst;
    struct list *nxt;
}list;
list *list_head,*pot;
list* listInit(){
    list *l;
    l=(list*)malloc(sizeof(list));
    if(l==NULL){
        printf("ERROR! NO MEMORY!\n");
        exit(0);
    }
    l->val=NULL;
    l->nxt=NULL;
    l->lst=NULL;
    return l;
}
void listPush(list *now,int v,char *ss){
    list *nx;
    list *po=now;
    while(po->nxt!=NULL)po=po->nxt;
    nx = (list*)malloc(sizeof(list));
    if(nx == NULL){
        error("no memory");
        exit(0);
    }
    strcpy(nx->s,ss);
    nx->val=v;
    nx->lst=po;
    po->nxt=nx;
    return ;
}
/*Link List part end*/

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
extern char *yytext;
char s_now[10];
int tok;

void advance(){
    if(pot->nxt != NULL) {
        pot=pot->nxt;
        strcpy(s_now,pot->s);
        #ifdef DEBUG_LOG
        printf("[+]         advance: %d %s\n",pot->val,pot->s);
        #endif
        tok=pot->val;
        // s_now = pot->s;
        return ;
    }
    #ifdef DEBUG_LOG
    printf("[WARNING]   No advance!\n");
    #endif
    tok=-114514;
    return ;
}
void rollback(list *p){
    pot = p;
    tok = p->val;
    return ;
}

void info(char str[]){
    #ifdef DEBUG_LOG
    printf("[INFO]      %s  %s\n",str,s_now);
    #endif
    return ;
}
void path(char strFrom[],char strTo[]){
    #ifdef DEBUG_LOG
    printf("[PATH]      %s   --->   %s  %s\n",strFrom,strTo,s_now);
    #endif
    return ;
}
void fail(char strFrom[],char strTo[]){
    #ifdef DEBUG_LOG
    printf("[FAIL]      %s   \\->   %s  %s\n",strFrom,strTo,s_now);
    #endif
    return ;
}
void error(char str[]){
    #ifdef DEBUG_LOG
    printf("[ERROR]     %s\n",str);
    #endif
    return ;
}
void unmatch(char str[]){
    #ifdef DEBUG_LOG
    printf("[UNMATCH]   %s  %s\n",str,s_now);
    #endif
    return ;
}
void pullin(char str[]){
    #ifdef DEBUG_LOG
    printf("[PULLIN]    %s  %s\n",str,s_now);
    #endif
}

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	int ivalue;
	float fvalue;
	char* svalue;
	node_type nodeType;
	past left;
	past right;
	past if_cond;
	past next;
};


/*function decl part begin*/
#ifdef DECL_PART
/*
if below part is right way ,return 1;
else return 0;
*/
past analyse_Decl();
past analyse_CompUnit();
past analyse_FuncDef();
past analyse_ConstDecl();
past analyse_VarDecl();
past analyse_Type();
past analyse_ConstDefs();
past analyse_ConstExps();
past analyse_ConstExp();
past analyse_ConstInitVals();
past analyse_ConstInitVal();
past analyse_VarDecls();
past analyse_VarDef();
past analyse_InitVal();
past analyse_Exp();
past analyse_InitVals();
past analyse_FuncParams();
past analyse_FuncParam();
past analyse_Subscripts();
past analyse_Block();
past analyse_BlockItems();
past analyse_BlockItem();
past analyse_Stmt();
past analyse_LVal();
past analyse_LOrExp();
past analyse_AddExp();
past analyse_ArraySubscripts();
past analyse_PrimaryExp();
past analyse_UnaryExp();
past analyse_CallParams();
past analyse_MulExp();
past analyse_RelExp();
past analyse_EqExp();
past analyse_LAndExp();
past analyse_ConstDefsDot();
past analyse_MulExpDot();
past analyse_FuncParamsDot();
past newAstNode(node_type nodetype,past left,past right);
#endif
/*function decl part end*/

/*functions program begin*/
int judge(past x){
    if(x!=NULL)return 1;
    else return 0;
}
past newAstNode(node_type nodetype,past left,past right){
    past node = malloc(sizeof(ast));
    if(node == NULL){
        printf("Run out of Memory!\n");
        exit(0);
    }
    memset(node,0,sizeof(ast));
    node->nodeType  =   nodetype;
    node->left      =   left    ;
    node->right     =   right   ;
    return node;
}
void Free(past x){
    if(x==NULL)return ;
    Free(x->left);
    Free(x->right);
    Free(x->next);
    free(x);
    return ;
}
past _CompUnit(){
	past _CompUnit_node = NULL;
	if(tok == Y_CONST){
		_CompUnit_node = Decl();
	}else{
		_CompUnit_node = FuncDef();
	}
	
}
past CompUnit(){
	past CompUnit_node = newAstNode(TRANSLATION_UNIT,NULL,NULL);
	CompUnit_node->left = _CompUnit();
	if(judge(CompUnit_node->left)){
		past p =CompUnit_node->left;
		while(tok){
			p->next = _CompUnit();
			if(judge(p->next))p=p->next;
			else break;
		}
		p->next = NULL;
	}
	return CompUnit_node;
}
void print(past cur) {
	enum yytokentype type = cur->nodeType;
	switch (type) {
		case COMPOUND_STMT: printf("COMPOUND_STMT"); break;
		case RETURN_STMT: printf("RETURN_STMT"); break;
		case DECL_REF_EXPR: printf("DECL_REF_EXPR %s", cur->svalue); break;
		case CALL_EXPR: printf("CALL_EXPR"); break;
		case INTEGER_LITERAL: printf("INTEGER_LITERAL %d", cur->ivalue); break;
		case FLOATING_LITERAL: printf("FLOATING_LITERAL %f", cur->fvalue); break;
		case UNARY_OPERATOR: printf("UNARY_OPERATOR %s", cur->svalue); break;
		case ARRAY_SUBSCRIPT_EXPR: printf("ARRAY_SUBSCRIPT_EXPR"); break;
		case BINARY_OPERATOR: printf("BINARY_OPERATOR %s", cur->svalue); break;
		case IF_STMT: printf("IF_STMT"); break;
		case WHILE_STMT: printf("WHILE_STMT"); break;
		case CONTINUE_STMT: printf("CONTINUE_STMT"); break;
		case BREAK_STMT: printf("BREAK_STMT"); break;
		case FUNCTION_DECL: printf("FUNCTION_DECL %s", cur->svalue); break;
		case VAR_DECL: printf("VAR_DECL %s", cur->svalue); break;
		case PARM_DECL: printf("PARM_DECL %s", cur->svalue); break;
		case INIT_LIST_EXPR: printf("INIT_LIST_EXPR"); break;
		case DECL_STMT: printf("DECL_STMT"); break;
		case TRANSLATION_UNIT: printf("TRANSLATION_UNIT"); break;
		case NULL_STMT: printf("NULL_STMT"); break;
		default: printf("Unknown node type!"); break;
	}
}
void showAst(past node, int nest) {
	if(node == NULL) return;
	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	//printf("%d\n", node->nodeType);
	print(node);
	puts("");
	showAst(node->if_cond, nest + 1);
	showAst(node->left, nest + 1);
	showAst(node->right, nest + 1);
	showAst(node->next, nest);
}

int main(int argc, char **argv)
{
    /*init the list*/
    list_head = listInit();
    /*push input yyval & yytext into list;*/
    #ifdef WHILE
    while(tok = yylex())
    {
        if (tok!=1){
            #ifdef DEBUG_OUT
            printf("[+]push : %d %s\n",tok,yytext);
            #endif
            listPush(list_head,tok,yytext);
        }
    }
    #endif

    pot = list_head;
    advance();
    past res = analyse_CompUnit();
    showAst(res,0);
    /*debug out lin->val lin->s part*/
    #ifdef DEBUG_OUT
    list *lin;
    lin=list_head;
    while(lin->nxt!=NULL){
        lin=lin->nxt;
        printf("[*]int list : tok: %d yytext: %s\n",lin->val,lin->s);
    }
    #endif
    return 0;
}