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
// #define DEBUG_LOG

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
        // strcpy(s_now,pot->s);
        #ifdef DEBUG_LOG
        printf("[+]         advance: %d %s\n",pot->val,yytext);
        #endif
        tok=pot->val;
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
int analyse_Decl();
int analyse_CompUnit();
int analyse_FuncDef();
int analyse_ConstDecl();
int analyse_VarDecl();
int analyse_Type();
int analyse_ConstDefs();
int analyse_ConstExps();
int analyse_ConstExp();
int analyse_ConstInitVals();
int analyse_ConstInitVal();
int analyse_VarDecls();
int analyse_VarDef();
int analyse_InitVal();
int analyse_Exp();
int analyse_InitVals();
int analyse_FuncParams();
int analyse_FuncParam();
int analyse_Subscripts();
int analyse_Block();
int analyse_BlockItems();
int analyse_BlockItem();
int analyse_Stmt();
int analyse_LVal();
int analyse_LOrExp();
int analyse_AddExp();
int analyse_ArraySubscripts();
int analyse_PrimaryExp();
int analyse_UnaryExp();
int analyse_CallParams();
int analyse_MulExp();
int analyse_RelExp();
int analyse_EqExp();
int analyse_LAndExp();
int analyse_ConstDefsDot();
int analyse_MulExpDot();
int analyse_FuncParamsDot();
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
past analyse_CompUnit(){
    list *bck;
    bck=pot;
    past CompUnit_node = newAstNode(TRANSLATION_UNIT,NULL,NULL);
    CompUnit_node->left = analyse_Decl();
    if(judge(CompUnit_node->left)) {
        advance();
        CompUnit_node->next = analyse_CompUnit();
        if(judge(CompUnit_node->next)){
            path("CompUnit","CompUnit");
        }
        else{
            fail("CompUnit","CompUnit");
        }
        path("CompUnit","Decl");
        return CompUnit_node;
    }
    else {
        fail("CompUnit","Decl");
        rollback(bck);
    }
    CompUnit_node->left = analyse_FuncDef();
    if(judge(CompUnit_node->left)) {
        advance();
        CompUnit_node->next = analyse_CompUnit();
        if(judege(CompUnit_node->next)){
            path("CompUnit","CompUnit");
        }else{
            fail("CompUnit","CompUnit");
        }
        path("CompUnit","FuncDef");
        return CompUnit_node;
    }
    else {
        fail("CompUnit","FuncDef");
        rollback(bck);
    }
    return NULL;
}
past analyse_Decl() {
    list *bck;
    bck = pot;
    past Decl_node = NULL;
    Decl_node = analyse_ConstDecl();
    if(judge(Decl_node)){
        path("Decl","ConstDecl");
        return Decl_node;
    }
    else {
        fail("Decl","ConstDecl");
        rollback(bck);
    }
    Decl_node = analyse_VarDecl();
    if(judge(Decl_node)){
        path("Decl","VarDecl");
        return Decl_node;
    }
    else {
        fail("Decl","VarDecl");
        rollback(bck);
    }
    return NULL;
}
past analyse_FuncDef() {
    list *bck;
    bck = pot;
    past FuncDef_node = newAstNode(FUNCTION_DECL,NULL,NULL);
    FuncDef_node = analyse_Type();
    if(judge(FuncDef_node)){
        path("FuncDef","Type");
    }
    else {
        fail("FuncDef","Type");
        rollback(bck);
        Free(FuncDef_node);
        return NULL;
    }
    advance();
    bck = pot;
    if(tok != Y_ID) {
        unmatch("FuncDef:Y_ID");
        Free(FuncDef_node);
        return NULL;
    }
    pullin("FuncDef:Y_ID");
    FuncDef_node->svalue = pot->s;
    advance();
    bck = pot;
    if(tok!=Y_LPAR) {
        unmatch("FuncDef:Y_LPAR");
        Free(FuncDef_node);
        return NULL;
    }
    pullin("FuncDef:Y_LPAR");
    advance();
    bck = pot;
    FuncDef_node->left = analyse_FuncParams();
    if(judge(FuncDef_node->left)) {
        path("FuncDef","FuncParams");
    }
    else {
        fail("FuncDef","FuncParams");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RPAR) {
        unmatch("FuncDef:Y_RPAR");
        Free(FuncDef_node);
        return NULL;
    }
    pullin("FuncDef:Y_RPAR");
    advance();
    bck = pot;
    FuncDef_node->right = analyse_Block();
    if(judge(FuncDef_node)) {
        path("FuncDef","Y_RPAR");
        return FuncDef_node;
    }
    else {
        rollback(bck);
        fail("FuncDef","Y_RPAR");
        Free(FuncDef_node);
        return NULL;
    }
}
past analyse_ConstDecl() {
    list *bck;
    bck = pot;
    past ConstDecl_node =newAstNode(DECL_STMT,NULL,NULL);
    if(tok != Y_CONST){
        unmatch("ConstDecl:Y_CONST");
        return 0;
    }
    pullin("ConstDecl:Y_CONST");
    advance();
    bck = pot;
    if(analyse_Type()!=NULL) {
        path("ConstDecl","Type");
    }else{
        fail("ConstDecl","Type");
        rollback(bck);
        Free(ConstDecl_node);
        return NULL;
    }
    advance();
    bck = pot;
    ConstDecl_node->left = analyse_ConstDefs();
    if(judge(ConstDecl_node->left)){
        path("ConstDecl","ConstDefs");
    }else{
        fail("ConstDecl","ConstDefs");
        rollback(bck);
        Free(ConstDecl_node);
        return NULL;
        // if(analyse_ConstDef()){
        //     path("ConstDecl","ConstDef");
        // }else{
        //     fail("ConstDecl","ConstDefs|ConstDef");
        //     rollback(bck);
        //     return 0;
        // }
    }
    advance();
    bck = pot;
    if(tok!=Y_SEMICOLON){
        unmatch("ConstDecl:Y_SEMICOLON");
        Free(ConstDecl_node);
        return NULL;
    }else{
        pullin("ConstDecl:Y_SEMICOLON");
        return ConstDecl_node;
    }
}
past analyse_VarDecl() {
    list *bck;
    bck = pot;
    past VarDecl_node = newAstNode(DECL_STMT,NULL,NULL);
    if(analyse_Type()!=NULL){
        path("VarDecl","Type");
    }else{
        fail("VarDecl","Type");
        rollback(bck);
        Free(VarDecl_node);
        return NULL;
    }
    advance();
    bck = pot;
    VarDecl_node->left = analyse_VarDef();
    if(judge(VarDecl_node->left)) {
        path("VarDecl","VarDef");
    }else{
        fail("VarDecl","VarDef");
        rollback(bck);
        Free(VarDecl_node);
        return NULL;
    }
    advance();
    bck = pot;
    VarDecl_node->next = analyse_VarDecls();
    if(judge(VarDecl_node->next)) {
        path("VarDecl","VarDecls");
    }else{
        fail("VarDecl","VarDecls");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_SEMICOLON){
        unmatch("VarDecl:Y_SEMICILON");
        Free(VarDecl_node);
        return NULL;
    }
    pullin("VarDecl:Y_SEMICILON");
    return VarDecl_node;
}
past analyse_Type() {
    past Type_node = newAstNode(0,NULL,NULL);
    switch (tok)
    {
    case Y_INT:
        pullin("Type:Y_INT");
        break;
    case Y_FLOAT:
        pullin("Type:Y_FLOAT");
        break;
    case Y_VOID:
        pullin("Type:Y_VOID");
        break;
    default:
        unmatch("Type: INT|FLOAT|VOID");
        return NULL;
        break;
    }
    return Type_node;
}
past analyse_Block() {
    list *bck;
    bck = pot;
    past Block_node = newAstNode(COMPOUND_STMT,NULL,NULL);
    if(tok!=Y_LBRACKET){
        unmatch("Block:Y_LBRACKET");
        Free(Block_node);
        return NULL;
    }
    pullin("Block:Y_LBRACKET");
    advance();
    bck = pot;
    Block_node->left = analyse_BlockItems();
    if(judge(Block_node)){
        path("Block","BlockItems");
    }else{
        rollback(bck->lst);
        fail("Block","BlockItems");
    }
    advance();
    bck = pot;
    if(tok!=Y_RBRACKET){
        unmatch("Block:Y_RBRACKET");
        Free(Block_node);
        return NULL;
    }
    pullin("Block:Y_RBRACKET");
    return Block_node;
}
past analyse_FuncParams(){
    list *bck;
    bck = pot;
    past FuncParams_node = analyse_FuncParam();
    if(judge(FuncParams_node)){
        path("FuncParams","FuncParam");
    }else{
        fail("FuncParams","FuncParam");
        rollback(bck);
        Free(FuncParams_node);
        return NULL;
    }
    advance();
    bck = pot;
    FuncParams_node->next = analyse_FuncParamsDot();
    if(judge(FuncParams_node->next)){
        path("FuncParams","FuncParamsDot");
    }else{
        fail("FuncParams","FuncParamsDot");
        rollback(bck->lst);
    }
    return FuncParams_node;
}
past analyse_FuncParamsDot(){
    list *bck ;
    bck = pot;
    if(tok == Y_COMMA){
        pullin("FuncParamsDot:Y_COMMA");
    }else{
        unmatch("FuncParamsDot:Y_COMMA");
        return NULL;
    }
    advance();
    bck = pot;
    past FuncParamsDot_node = analyse_FuncParam();
    if(judge(FuncParamsDot_node)){
        path("FuncParamsDot","FuncParam");
    }else{
        fail("FuncParamsDot","FuncParam");
        rollback(bck);
        Free(FuncParamsDot_node);
        return NULL;
    }
    advance();
    bck = pot;
    FuncParamsDot_node->next = analyse_FuncParamsDot();
    if(judge(FuncParamsDot_node->next)){
        path("FuncParamsDot","FuncParamsDot");
    }else{
        fail("FuncParamsDot","FuncParamsDot");
        rollback(bck->lst);
    }
    return 1;
}
past analyse_ConstDef(){
    list *bck;
    bck = pot;
    past ConstDef_node = newAstNode(VAR_DECL,NULL,NULL);
    if(tok != Y_ID){
        unmatch("ConstDef:Y_ID");
        Free(ConstDef_node);
        return NULL;
    }
    pullin("ConstDef:Y_ID");
    ConstDef_node->svalue = pot->s;
    advance();
    bck = pot;
    past tt = analyse_ConstExps();
    if(judge(tt)){
        path("ConstDef","ConstExps");
        advance();
        bck = pot;
    }else{
        fail("ConstDef","ConstExps");
        rollback(bck);
    }
    Free(tt);
    if(tok!=Y_ASSIGN){
        unmatch("ConstDef:Y_ASSIGN");
        Free(ConstDef_node);
        return NULL;
    }
    pullin("ConstDef:Y_ASSIGN");
    advance();
    bck = pot;
    ConstDef_node->left = analyse_ConstInitVal();
    if(judge(ConstDef_node->left)){
        path("ConstDef","ConstInitVal");
    }else{
        fail("ConstDef","ConstInitVal");
        rollback(bck);
        Free(ConstDef_node);
        return NULL;
    }
    return ConstDef_node;
}
//need to change
past analyse_ConstDefs(){
    list *bck;
    bck = pot;
    past ConstDefs_node = analyse_ConstDef();
    if(judge(ConstDefs_node)){
        path("ConstDefs","ConstDef");
    }else{
        fail("ConstDefs","ConstDef");
        rollback(bck);
        Free(ConstDefs_node);
        return NULL;
    }
    advance();
    bck = pot;
    ConstDefs_node->next = analyse_ConstDefsDot();
    if(judge(ConstDefs_node)){
        path("ConstDefs","ConstDefsDot");
    }else{
        fail("ConstDefs","ConstDefsDot");
        rollback(bck->lst);
    }
    return ConstDefs_node;
}
//need to change
past analyse_ConstDefsDot(){
    list *bck ;
    bck = pot;
    if(tok != Y_COMMA){
        unmatch("ConstDefsDot:Y_COMMA");
        return NULL;
    }
    pullin("ConstDefsDot:Y_COMMA");
    advance();
    bck = pot;
    past ConstDefsDot_node = analyse_ConstDef();
    if(judge(ConstDefsDot_node)){
        path("ConstDefsDot","ConstDef");
    }else{
        fail("ConstDefsDot","ConstDef");
        rollback(bck);
        Free(ConstDefsDot_node);
        return NULL;
    }
    advance();
    bck = pot;
    ConstDefsDot_node->next = analyse_ConstDefsDot();
    if(judge(ConstDefsDot_node->next)){
        path("ConstDefsDot","ConstDefsDot");
    }else{
        fail("ConstDefsDot","ConstDefsDot");
        rollback(bck->lst);
    }
    return ConstDefsDot_node;
}
past analyse_VarDef(){
    list *bck;
    bck = pot;
    past VarDef_node = newAstNode(VAR_DECL,NULL,NULL);
    if (tok!=Y_ID){
        unmatch("VarDef:Y_ID");
        Free(VarDef_node);
        return NULL;
    }
    pullin("VarDef:Y_ID");
    VarDef_node->svalue = pot->s;
    advance();
    bck = pot;
    if(tok == Y_ASSIGN){
        pullin("VarDef:Y_ASSIGN");
        advance();
        bck = pot;
        VarDef_node->left = analyse_InitVal();
        if(judge(VarDef_node->left)){
            path("VarDef","InitVal");
        }else{
            fail("VarDef","InitVal");
            Free(VarDef_node);
            return NULL;
        }
        return VarDef_node;
    }else{
        unmatch("VarDef:Y_ASSIGN");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    past t = analyse_ConstExps();
    if(judge(t)){
        path("VarDef","ConstExps");
        advance();
        bck = pot;
        if(tok != Y_ASSIGN){
            rollback(bck->lst);
            unmatch("Vardef:Y_ASSIGN");
            Free(t);
            return VarDef_node;
        }
        pullin("VarDef:Y_ASSIGN");
        Free(t);
        advance();
        bck = pot;
        VarDef_node->left = analyse_InitVal();
        if(judge(VarDef_node->left)){
            path("VarDef","InitVal");
        }else{
            fail("VarDef","InitVal");
            rollback(bck);
            Free(VarDef_node);
            return VarDef_node;
        }
    }else{
        fail("VarDef","ConstExps");
        rollback(bck->lst);
        return VarDef_node;
    }
    return VarDef_node;
}
past analyse_VarDecls(){
    list *bck;
    bck = pot;
    if(tok!=Y_COMMA){
        unmatch("VarDecls:Y_COMMA");
        return NULL;
    }
    pullin("VarDecls:Y_COMMA");
    advance();
    bck = pot;
    past VarDecls_node = analyse_VarDef();
    if(VarDecls_node){
        path("Vardecls","VarDef");
    }else{
        fail("VarDecls","VarDef");
        rollback(bck);
        Free(VarDecls_node);
        return NULL;
    }
    advance();
    bck = pot;
    VarDecls_node->next = analyse_VarDecls();
    if(judge(VarDecls_node->next)){
        path("VarDecls","VarDecls");
    }else{
        fail("VarDecls","VarDecls");
        rollback(bck->lst);
    }
    return VarDecls_node;
}
past analyse_BlockItems(){
    list *bck;
    bck = pot;
    past BlockItems_node = analyse_BlockItem();
    if(judge(BlockItems_node)){
        path("BlockItems","BlockItem");
    }else{
        fail("BlockItems","BlockItem");
        rollback(bck);
        return NULL;
    }
    advance();
    bck = pot;
    BlockItems_node->next = analyse_BlockItems();
    if(judge(BlockItems_node->next)){
        path("BockItems","BlockItems");
    }else{
        rollback(bck->lst);
        fail("BlockItems","BlockItems");
    }
    return BlockItems_node;
}
//this line
int analyse_FuncParam(){
    list *bck;
    bck = pot;
    if(analyse_Type()){
        path("FuncParam","Type");
    }else{
        fail("FuncParam","Type");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_ID){
        unmatch("FuncParam:Y_ID");
        return 0;
    }
    pullin("FuncParam:Y_ID");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        path("FuncParam","ArraySubscripts");
        return 1;
    }else{
        fail("FuncParam","ArraySubscripts");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_LSQUARE){
        unmatch("FuncParam:Y_LSQUARE");
        rollback(bck->lst);
        return 1;
    }
    pullin("FuncParam:Y_LSQUARE");
    advance();
    bck = pot ;
    if(tok!=Y_RSQUARE){
        unmatch("FuncParam:Y_REQUARE");
        return 0;
    }
    pullin("FuncParam:Y_REQUARE");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        path("FuncParam","ArraySubscripts");
    }else{
        fail("FuncParam","ArraySubscripts");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstExps(){
    list *bck;
    bck = pot;
    if(tok != Y_LSQUARE){
        unmatch("ConstExps:Y_LSQUARE");
        return 0;
    }
    pullin("ConstExps:Y_LSQUARE");
    advance();
    bck = pot;
    if(analyse_ConstExp()){
        path("ConstExps","ConstExp");
    }else{
        fail("ConstExps","ConstExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok!=Y_RSQUARE){
        unmatch("ConstExps:Y_RSQUARE");
        return 0;
    }
    pullin("ConstExps:Y_RSQUARE");
    advance();
    bck = pot;
    if(analyse_ConstExps()){
        path("ConstExps","ConstExps");
    }else{
        fail("ConstExps","ConstExps");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstInitVal(){
    list *bck;
    bck = pot;
    if(analyse_ConstExp()){
        path("ConstInitVal","ConstExp");
        return 1;
    }else{
        fail("ConstInitVal","ConstExp");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_LBRACKET){
        unmatch("ConstInitVal:Y_LBRACKET");
        return 0;
    }
    pullin("ConstInitVal:Y_LBRACKET");
    advance();
    bck = pot;
    if(analyse_ConstInitVal()){
        path("ConstInitVal","ConstInitVal");
        advance();
        bck = pot;
        if(analyse_ConstInitVals()){
            path("ConstInitVal","ConstInitVals");
        }else{
            fail("ConstInitVal","ConstInitVal");
            rollback(bck->lst);
        }
    }else{
        fail("ConstInitVal","ConstInitVal");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RBRACKET){
        unmatch("ConstInitVal:Y_BRACKET");
        return 0;
    }
    pullin("ConstInitVal:Y_BRACKET");
    return 1;
}
int analyse_InitVal(){
    list *bck;
    bck = pot;
    if(analyse_Exp()){
        path("InitVal","Exp");
        return 1;
    }else{
        fail("InitVal","Exp");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_LBRACKET){
        unmatch("InitVal:Y_LBRACKET");
        return 0;
    }
    pullin("InitVal:Y_LBRACKET");
    advance();
    bck = pot;
    if(analyse_InitVal()){
        path("InitVal","InitVal");
        advance();
        bck = pot;
        if(analyse_InitVals()){
            path("InitVal","InitVals");
        }else{
            fail("InitVal","InitVals");
            rollback(bck->lst);
        }
    }else{
        fail("InitVal","InitVal");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RBRACKET){
        unmatch("InitVal:Y_RBRACKET");
        return 0;
    }
    pullin("InitVal:Y_RBRACKET");
    return 1;
}
int analyse_BlockItem(){
    list *bck ;
    bck = pot;
    if(analyse_Decl()){
        path("BlockItem","Decl");
        return 1;
    }else{
        fail("BlockItem","Decl");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(analyse_Stmt()){
        path("BlockItem","Stmt");
        return 1;
    }else{
        fail("BlockItem","Stmt");
        rollback(bck->lst);
    }
    return 0;
}
int analyse_ArraySubscripts(){
    list *bck;
    bck = pot;
    if(tok != Y_LSQUARE){
        unmatch("ArraySubscripts:Y_LSQUARE");
        return 0;
    }
    pullin("ArraySubscripts:Y_Lsquare");
    advance();
    bck = pot;
    if(analyse_Exp()){
        path("ArraySubscripts","Exp");
    }else{
        fail("ArraySubscripts","Exp");
        rollback(bck->lst);
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_RSQUARE){
        unmatch("ArraySubscripts:Y_RSQUARE");
        return 0;
    }
    pullin("ArraySubscripts:Y_RSQUARE");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        path("ArraySubscripts","ArraySubscripts");
    }else{
        fail("ArraySubscripts","ArraySubscripts");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstExp(){
    list *bck;
    bck = pot;
    if(analyse_AddExp()){
        path("ConstExp","AddExp");
        return 1;
    }else{
        fail("ConstExp","AddExp");
        rollback(bck);
        return 0;
    }
}
int analyse_ConstInitVals(){
    list *bck;
    bck = pot;
    if(tok != Y_COMMA){
        unmatch("ConstInitVals:Y_COMMA");
        return 0;
    }
    pullin("ConstInitVals:Y_COMMA");
    advance();
    bck = pot;
    if(analyse_ConstInitVal()){
        path("ConstInitVals","ConstInitVal");
    }else{
        fail("ConstInitVals","ConstInitVal");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_ConstInitVals()){
        path("ConstInitVals","ConstInitVals");
    }else{
        fail("ConstInitVals","ConstInitVals");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_Exp(){
    list *bck;
    bck = pot;
    if(analyse_AddExp()){
        path("Exp","AddExp");
        return 1;
    }else{
        fail("Exp","AddExp");
        rollback(bck);
        return 0;
    }
}
int analyse_InitVals(){
    list *bck ;
    bck = pot;
    if(tok != Y_COMMA){
        unmatch("InitVals:Y_COMMA");
        return 0;
    }
    pullin("InitVals:Y_COMMA");
    advance();
    bck = pot;
    if(analyse_InitVal()){
        path("InitVals","InitVal");
    }else{
        fail("InitVals","InitVal");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_InitVals()){
        path("InitVals","InitVals");
    }else{
        fail("InitVals","InitVals");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_Stmt(){
    list *bck,*bck2;
    bck = pot ;
    bck2= pot;
    if(analyse_LVal()){
        path("Stmt","LVal");
        advance();
        bck = pot;
        if(tok != Y_ASSIGN){
            unmatch("Stmt:Y_ASSIGN");
            rollback(bck2->lst);
        }
        else pullin("Stmt:Y_ASSIGN");
        advance();
        bck = pot;
        if(analyse_Exp()){
            path("Stmt","Exp");
        }else{
            fail("Stmt","Exp");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            return 0;
        }
        pullin("Stmt:Y_SEMICOLON");
        return 1;
    }else{
        fail("Stmt","LVal");
        rollback(bck);
    }
    if(analyse_Exp()){
        path("Stmt","Exp");
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:SEMICOLON");
            return 0;
        }
        pullin("Stmt:SEMICOLON");
        return 1;
    }else{
        fail("Stmt","Exp");
        rollback(bck);
    }
    if(analyse_Block()){
        path("Stmt","Block");
        return 1;
    }else{
        fail("Stmt","Block");
        rollback(bck);
    }
    if(tok == Y_SEMICOLON){
        pullin("Stmt:Y_SEMICOLON");
        return 1;
    }
    else if(tok == Y_WHILE){
        pullin("Stmt:Y_WHILE");
        advance();
        bck = pot;
        if(tok != Y_LPAR){
            unmatch("Stmt:Y_LPAR");
            return 0;
        }
        pullin("Stmt:Y_LPAR");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            path("Stmt","LOrExp");
        }else{
            fail("Stmt","LOrExp");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("Stmt:Y_RPAR");
            return 0;
        }
        pullin("Stmt:Y_RPAR");
        advance();
        bck = pot;
        if(analyse_Stmt()){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            return 0;
        }
        return 1;
    }
    else if(tok == Y_IF){
        pullin("Stmt:Y_IF");
        advance();
        bck = pot ;
        if(tok != Y_LPAR){
            unmatch("Stmt:Y_LPAR");
            return 0;
        }
        pullin("Stmt:Y_LPAR");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            path("Stmt","LOrExp");
        }else{
            fail("Stmt","LOrExp");
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("Stmt:Y_RPAR");
            return 0;
        }
        pullin("Stmt:Y_RPAR");
        advance();
        if(analyse_Stmt()){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok !=Y_ELSE){
            rollback(bck->lst);
            unmatch("Stmt:Y_ELSE");
            return 1;
        }
        pullin("Stmt:Y_ELSE");
        advance();
        bck = pot;
        if(analyse_Stmt()){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    if(tok == Y_BREAK){
        pullin("Stmt:Y_BREAK");
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            return 0;
        }
        pullin("Stmt:Y_SEMICOLON");
        return 1;
    }
    if(tok == Y_CONTINUE){
        pullin("Stmt:Y_CONTINUE");
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            return 0;
        }
        pullin("Stmt:Y_SEMICOLON");
        return 1;
    }
    if(tok == Y_RETURN){
        pullin("Stmt:Y_RETURN");
        advance();
        bck = pot;
        if(analyse_Exp()){
            path("Stmt","Exp");
        }else{
            fail("Stmt","Exp");
            rollback(bck->lst);
        }
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            return 0;
        }
        pullin("Stmt:Y_SEMICOLON");
        return 1;
    }
    return 0;
}
int analyse_AddExp(){
    list *bck ;
    bck = pot;
    if(analyse_MulExp()){
        path("AddExp","MulExp");
    }else{
        fail("AddExp","MulExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_AddExpDot()){
        path("AddExp","AddExpDot");
    }else{
        fail("AddExp","AddExpDot");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_AddExpDot(){
    list *bck;
    bck = pot;
    if(tok == Y_ADD){
        pullin("AddExpDot:Y_ADD");
    }else if(tok == Y_SUB){
        pullin("AddExpDot:Y_SUB");
    }else{
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_MulExp()){
        path("AddExpDot","MulExp");
    }else{
        fail("AddExpDot","MulExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_AddExpDot()){
        path("AddExpDot","AddExpDot");
    }else{
        fail("AddExpDot","AddExpDot");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_LVal(){
    list *bck ;
    bck = pot;
    if(tok != Y_ID){
        unmatch("LVal:Y_ID");
        return 0;
    }
    pullin("LVal:Y_ID");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        path("LVal","ArraySubScripts");
        return 1;
    }else{
        fail("LVal","ArraySubScripts");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_LOrExp(){
    list *bck;
    bck = pot;
    if(analyse_LAndExp()){
        path("LOrExp","LAndExp");
    }else{
        fail("LOrExp","LAndExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok == Y_OR){
        pullin("LOrExp:Y_OR");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            path("LOrExp","LOrExp");
        }else{
            fail("LOrExp","LOrExp");
            rollback(bck);
            return 0;
        }
    }else{
        unmatch("LOrExp:Y_OR");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_MulExp(){
    list *bck;
    bck = pot;
    if(analyse_UnaryExp()){
        path("MulExp","UnaryExp");
    }else{
        fail("MuilExp","UnaryExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_MulExpDot()){
        path("MulExp","MulExpDot");
    }else{
        fail("MulExp","MulExpDot");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_MulExpDot(){
    list *bck;
    bck = pot;
    if(tok == Y_MUL||tok == Y_DIV || tok == Y_MODULO){
        pullin("MulExpDot:Y_ML||Y_DIV||Y_MODLO");
    }else{
        unmatch("MulExpDot:Y_ML||Y_DIV||Y_MODLO");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_UnaryExp()){
        path("MulExpDot","UnaryExp");
    }else{
        fail("MulExpDot","UnaryExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_MulExpDot()){
        path("MulExpDot","MulExpDot");
    }else{
        fail("MulExpDot","MulExpDot");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_LAndExp(){
    list *bck ;
    bck = pot;
    if(analyse_EqExp()){
        path("LAndExp","EqExp");
    }else{
        fail("LAndExp","EqExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck =pot;
    if(tok == Y_AND){
        pullin("LAndExp:Y_AND");
        advance();
        bck =pot;
        if(analyse_LAndExp()){
            path("LandExp","LandExp");
        }else{
            fail("LandExp","LandExp");
            rollback(bck);
            return 0;
        }
        
    }else{
        unmatch("LAndExp:Y_AND");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_UnaryExp(){
    list *bck,*nxt;
    bck = pot;
    nxt = bck->nxt;
    if(tok == Y_ID&&(nxt->val==Y_LPAR)){
        pullin("UnaryExp:Y_ID");
        advance();
        bck = pot;
        if(tok !=Y_LPAR){
            unmatch("UnaryExp:Y_LPAR");
            return 0;
        }
        pullin("UnaryExp:Y_LPAR");
        advance();
        bck = pot;
        if(analyse_CallParams()){
            path("UnaryExp","CallParams");
        }else{
            fail("UnaryExp","CallParams");
            rollback(bck->lst);
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("UnaryExp:Y_RPAR");
            return 0;
        }
        pullin("UnaryExp:Y_RPAR");
        return 1;
    }
    else if(tok == Y_ADD){
        pullin("UnaryExp:Y_ADD");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            path("UnaryExp","UnaryExp");
        }else{
            fail("UnaryExp","UnaryExp");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    else if(tok == Y_SUB){
        pullin("UnaryExp:Y_SUB");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            path("UnaryExp","UnaryExp");
        }else{
            fail("UnaryExp","UnaryExp");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    else if(tok == Y_NOT){
        pullin("UnaryExp:Y_NOT");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            path("UnaryExp","UnaryExp");
        }else{
            fail("UnaryExp","UnaryExp");
            rollback(bck);
            return 0;
        }
        return 1;
    }else{
        if(analyse_PrimaryExp()){
            path("UnaryExp","PrimaryExp");
            return 1;
        }else{
            fail("UnaryExp","PrimaryExp");
            rollback(bck);
        }
    }
    return 0;
}
int analyse_EqExp(){
    list *bck ;
    bck =pot;
    if(analyse_RelExp()){
        path("EqExp","RelExp");
    }else{
        fail("EqExp","RelExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok==Y_EQ){
        pullin("EqExp:Y_EQ");
        advance();
        bck = pot;
        if(analyse_EqExp()){
            path("EqExp","EqExp");
        }else{
            fail("EqExp","EqExp");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    else if(tok == Y_NOTEQ){
        pullin("EqExp:Y_NOTEQ");
        advance();
        bck = pot;
        if(analyse_EqExp()){
            path("EqExp","EqExp");
        }else{
            fail("EqExp","EqExp");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    else {
        unmatch("EqExp:Y_EQ||Y_NOTEQ");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_PrimaryExp(){
    list *bck ;
    bck = pot;
    if(tok == Y_LPAR){
        pullin("PrimaryExp:Y_LPAR");
        advance();
        bck =pot;
        if(analyse_Exp()){
            path("PrimaryExp","Exp");
        }else{
            fail("PrimaryExp","Exp");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("PrimaryExp:Y_RPAR");
            return 0;
        }
        pullin("ParmaryExp:Y_RPAR");
        return 1;
    }
    if(tok == num_INT){
        pullin("ParmaryExp:num_INT");
        return 1;
    }else{
        unmatch("ParmaryExp:num_INT");
    }
    if(tok == num_FLOAT){
        pullin("ParmaryExp:num_FLOAT");
        return 1;
    }else{
        unmatch("ParmaryExp:num_FLOAT");
    }
    if(analyse_LVal()){
        path("ParamaryExp","LVal");
        return 1;
    }else{
        fail("ParamaryExp","LVal");
        rollback(bck);
    }
    return 0;
}
int analyse_CallParams(){
    list *bck;
    bck = pot;
    if(analyse_Exp()){
        path("CallParams","Exp");
    }else{
        fail("CallParams","Exp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok == Y_COMMA){
        pullin("CallParams:Y_COMMA");
        advance();
        bck = pot;
        if(analyse_CallParams()){
            path("CallParams","CallParams");
            return 1;
        }else{
            fail("CallParams","CallParams");
            rollback(bck);
            return 0;
        }
    }else{
        unmatch("CallParams:Y_COMMA");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_RelExp(){
    list *bck ;
    bck = pot;
    if(analyse_AddExp()){
        path("RelExp","AddExp");
    }else{
        fail("RelExp","AddExp");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok==Y_LESS||tok == Y_GREAT || tok == Y_LESSEQ || tok == Y_GREATEQ){
        pullin("Y_LESS|Y_GREAT|Y_LESSEQ|Y_GREATEQ");
        advance();
        bck = pot;
        if(analyse_RelExp()){
            path("RelExp","RelExp");
            return 1;
        }else{
            fail("RelExp","RelExp");
            rollback(bck);
            return 0;
        }
    }else{
        unmatch("Y_LESS|Y_GREAT|Y_LESSEQ|Y_GREATEQ");
        rollback(bck->lst);
    }
    return 1;
}
/*functions program end*/


/*main function*/
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

    #ifdef DEBUG
    freopen("debug.txt","r",stdin);
    for(int i=1;i<=7;i++){
        int a=0;
        char ss[5]="test";
        scanf("%d",&a);
        listPush(list_head,a,ss);
    }
    #endif
    pot = list_head;
    advance();
    int res = analyse_CompUnit();
    printf("res: %d\n",res);
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