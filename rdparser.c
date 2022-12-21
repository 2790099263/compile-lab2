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
    char s[3005];
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
    // printf("%s\n",ss);
    // nx->s = (char*)malloc(sizeof(ss)+16);
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
past analyse_AddExpDot();
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
past analyse_CompUnit(){
    list *bck;
    bck=pot;
    past CompUnit_node = newAstNode(TRANSLATION_UNIT,NULL,NULL);
    CompUnit_node->left = analyse_Decl();
    if(judge(CompUnit_node->left)) {
        advance();
        past t = analyse_CompUnit();
        if(judge(t)){
            path("CompUnit","CompUnit");
            CompUnit_node->left->next = t->left;
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
        past t = analyse_CompUnit();
        if(judge(t)){
            path("CompUnit","CompUnit");
            CompUnit_node->left->next = t->left;
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
    Free(CompUnit_node);
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
    past t = analyse_Type();
    if(judge(t)){
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
    if(judge(FuncDef_node->right)) {
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
    VarDecl_node->left->next = analyse_VarDecls();
    if(judge(VarDecl_node->left->next)) {
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
    if(judge(Block_node->left)){
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
    return FuncParamsDot_node;
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
    if(judge(ConstDefs_node->next)){
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
past analyse_FuncParam(){
    list *bck;
    bck = pot;
    if(analyse_Type()!=NULL){
        path("FuncParam","Type");
    }else{
        fail("FuncParam","Type");
        rollback(bck);
        return NULL;
    }
    past FuncParam_node = newAstNode(PARM_DECL,NULL,NULL);
    advance();
    bck = pot;
    if(tok != Y_ID){
        unmatch("FuncParam:Y_ID");
        return NULL;
    }
    pullin("FuncParam:Y_ID");
    FuncParam_node->svalue = pot->s;
    advance();
    bck = pot;
    past t = analyse_ArraySubscripts();
    if(judge(t)){
        path("FuncParam","ArraySubscripts");
        Free(t);
        return FuncParam_node;
    }else{
        fail("FuncParam","ArraySubscripts");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_LSQUARE){
        unmatch("FuncParam:Y_LSQUARE");
        rollback(bck->lst);
        return FuncParam_node;
    }
    pullin("FuncParam:Y_LSQUARE");
    advance();
    bck = pot ;
    if(tok!=Y_RSQUARE){
        unmatch("FuncParam:Y_REQUARE");
        Free(FuncParam_node);
        return NULL;
    }
    pullin("FuncParam:Y_REQUARE");
    advance();
    bck = pot;
    t = analyse_ArraySubscripts();
    if(judge(t)){
        path("FuncParam","ArraySubscripts");
    }else{
        fail("FuncParam","ArraySubscripts");
        rollback(bck->lst);
    }
    return FuncParam_node;
}
past analyse_ConstExps(){
    list *bck;
    bck = pot;
    past ConstExps_node = newAstNode(ARRAY_SUBSCRIPT_EXPR,NULL,NULL);
    if(tok != Y_LSQUARE){
        unmatch("ConstExps:Y_LSQUARE");
        Free(ConstExps_node);
        return NULL;
    }
    pullin("ConstExps:Y_LSQUARE");
    advance();
    bck = pot;
    ConstExps_node->right = analyse_ConstExp();
    if(judge(ConstExps_node->right)){
        path("ConstExps","ConstExp");
    }else{
        fail("ConstExps","ConstExp");
        rollback(bck);
        Free(ConstExps_node);
        return NULL;
    }
    advance();
    bck = pot;
    if(tok!=Y_RSQUARE){
        unmatch("ConstExps:Y_RSQUARE");
        Free(ConstExps_node);
        return NULL;
    }
    pullin("ConstExps:Y_RSQUARE");
    advance();
    bck = pot;
    ConstExps_node->left = analyse_ConstExps();
    if(judge(ConstExps_node->left)){
        path("ConstExps","ConstExps");
    }else{
        fail("ConstExps","ConstExps");
        rollback(bck->lst);
    }
    return ConstExps_node;
}
past analyse_ConstInitVal(){
    list *bck;
    bck = pot;
    past ConstInitVal_node = analyse_ConstExp();
    if(judge(ConstInitVal_node)){
        path("ConstInitVal","ConstExp");
        return ConstInitVal_node;
    }else{
        fail("ConstInitVal","ConstExp");
        rollback(bck->lst);
    }
    ConstInitVal_node = newAstNode(INIT_LIST_EXPR,NULL,NULL);
    advance();
    bck = pot;
    if(tok!=Y_LBRACKET){
        unmatch("ConstInitVal:Y_LBRACKET");
        Free(ConstInitVal_node);
        return NULL;
    }
    pullin("ConstInitVal:Y_LBRACKET");
    advance();
    bck = pot;
    ConstInitVal_node->left = analyse_ConstInitVal();
    if(judge(ConstInitVal_node->left)){
        path("ConstInitVal","ConstInitVal");
        advance();
        bck = pot;
        ConstInitVal_node->left->next = analyse_ConstInitVals();
        if(judge(ConstInitVal_node->left->next)){
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
        Free(ConstInitVal_node);
        return NULL;
    }
    pullin("ConstInitVal:Y_BRACKET");
    return ConstInitVal_node;
}
past analyse_InitVal(){
    list *bck;
    bck = pot;
    past InitVal_node = analyse_Exp();
    if(judge(InitVal_node)){
        path("InitVal","Exp");
        return InitVal_node;
    }else{
        fail("InitVal","Exp");
        rollback(bck->lst);
    }
    InitVal_node = newAstNode(INIT_LIST_EXPR,NULL,NULL);
    advance();
    bck = pot;
    if(tok != Y_LBRACKET){
        unmatch("InitVal:Y_LBRACKET");
        Free(InitVal_node);
        return NULL;
    }
    pullin("InitVal:Y_LBRACKET");
    advance();
    bck = pot;
    InitVal_node->left = analyse_InitVal();
    if(judge(InitVal_node->left)){
        path("InitVal","InitVal");
        advance();
        bck = pot;
        InitVal_node->left->next = analyse_InitVals();
        if(judge(InitVal_node->left->next)){
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
        Free(InitVal_node);
        return NULL;
    }
    pullin("InitVal:Y_RBRACKET");
    return InitVal_node;
}
past analyse_BlockItem(){
    list *bck ;
    bck = pot;
    past BlockItem_node = analyse_Decl();
    if(judge(BlockItem_node)){
        path("BlockItem","Decl");
        return BlockItem_node;
    }else{
        fail("BlockItem","Decl");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    BlockItem_node = analyse_Stmt();
    if(judge(BlockItem_node)){
        path("BlockItem","Stmt");
        return BlockItem_node;
    }else{
        fail("BlockItem","Stmt");
        rollback(bck->lst);
    }
    Free(BlockItem_node);
    return NULL;
}
past analyse_ArraySubscripts(){
    list *bck;
    bck = pot;
    if(tok != Y_LSQUARE){
        unmatch("ArraySubscripts:Y_LSQUARE");
        return NULL;
    }
    pullin("ArraySubscripts:Y_Lsquare");
    advance();
    bck = pot;
    past ArraySubscripts_node = newAstNode(ARRAY_SUBSCRIPT_EXPR,NULL,NULL);
    ArraySubscripts_node->right = analyse_Exp();
    if(judge(ArraySubscripts_node->right)){
        path("ArraySubscripts","Exp");
    }else{
        fail("ArraySubscripts","Exp");
        rollback(bck->lst);
        Free(ArraySubscripts_node);
        return NULL;
    }
    advance();
    bck = pot;
    if(tok != Y_RSQUARE){
        unmatch("ArraySubscripts:Y_RSQUARE");
        Free(ArraySubscripts_node);
        return NULL;
    }
    pullin("ArraySubscripts:Y_RSQUARE");
    advance();
    bck = pot;
    past t = analyse_ArraySubscripts();
    if(judge(t)){
        path("ArraySubscripts","ArraySubscripts");
        past s = t;
        while(s->left!=NULL)s=s->left;
        s->left = ArraySubscripts_node;
        ArraySubscripts_node = t;
    }else{
        fail("ArraySubscripts","ArraySubscripts");
        rollback(bck->lst);
    }
    return ArraySubscripts_node;
}
past analyse_ConstExp(){
    list *bck;
    bck = pot;
    past ConstExp_node = analyse_AddExp();
    if(judge(ConstExp_node)){
        path("ConstExp","AddExp");
        return ConstExp_node;
    }else{
        fail("ConstExp","AddExp");
        rollback(bck);
        Free(ConstExp_node);
        return NULL;
    }
}
past analyse_ConstInitVals(){
    list *bck;
    bck = pot;
    if(tok != Y_COMMA){
        unmatch("ConstInitVals:Y_COMMA");
        return NULL;
    }
    pullin("ConstInitVals:Y_COMMA");
    advance();
    bck = pot;
    past ConstInitVals_node = analyse_ConstInitVal();
    if(judge(ConstInitVals_node)){
        path("ConstInitVals","ConstInitVal");
    }else{
        fail("ConstInitVals","ConstInitVal");
        rollback(bck);
        Free(ConstInitVals_node);
        return NULL;
    }
    advance();
    bck = pot;
    ConstInitVals_node->next = analyse_ConstInitVals();
    if(judge(ConstInitVals_node->next)){
        path("ConstInitVals","ConstInitVals");
    }else{
        fail("ConstInitVals","ConstInitVals");
        rollback(bck->lst);
    }
    return ConstInitVals_node;
}
past analyse_Exp(){
    list *bck;
    bck = pot;
    past Exp_node = analyse_AddExp();
    if(judge(Exp_node)){
        path("Exp","AddExp");
        return Exp_node;
    }else{
        fail("Exp","AddExp");
        rollback(bck);
        Free(Exp_node);
        return NULL;
    }
}
past analyse_InitVals(){
    list *bck ;
    bck = pot;
    if(tok != Y_COMMA){
        unmatch("InitVals:Y_COMMA");
        return NULL;
    }
    pullin("InitVals:Y_COMMA");
    advance();
    bck = pot;
    past InitVals_node = analyse_InitVal();
    if(judge(InitVals_node)){
        path("InitVals","InitVal");
    }else{
        fail("InitVals","InitVal");
        rollback(bck);
        Free(InitVals_node);
        return NULL;
    }
    advance();
    bck = pot;
    InitVals_node->next = analyse_InitVals();
    if(judge(InitVals_node->next)){
        path("InitVals","InitVals");
    }else{
        fail("InitVals","InitVals");
        rollback(bck->lst);
    }
    return InitVals_node;
}
past analyse_Stmt(){
    list *bck,*bck2;
    bck = pot ;
    bck2= pot;
    past Stmt_node = newAstNode(BINARY_OPERATOR,NULL,NULL);
    Stmt_node->left = analyse_LVal();
    if(judge(Stmt_node->left)){
        path("Stmt","LVal");
        advance();
        bck = pot;
        if(tok != Y_ASSIGN){
            unmatch("Stmt:Y_ASSIGN");
            rollback(bck2->lst);
            Stmt_node ->left=NULL;

        }
        else pullin("Stmt:Y_ASSIGN");
        Stmt_node->svalue = pot->s;
        advance();
        bck = pot;
        Stmt_node->right = analyse_Exp();
        if(judge(Stmt_node->right)){
            path("Stmt","Exp");
        }else{
            fail("Stmt","Exp");
            rollback(bck);
            Free(Stmt_node);
            return NULL;
        }
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_SEMICOLON");
        if(Stmt_node->left==NULL)return Stmt_node->right;
        else return Stmt_node;
    }else{
        fail("Stmt","LVal");
        rollback(bck);
    }
    // if(analyse_Exp()){
    //     path("Stmt","Exp");
    //     advance();
    //     bck = pot;
    //     if(tok != Y_SEMICOLON){
    //         unmatch("Stmt:SEMICOLON");
    //         return 0;
    //     }
    //     pullin("Stmt:SEMICOLON");
    //     return 1;
    // }else{
    //     fail("Stmt","Exp");
    //     rollback(bck);
    // }
    Stmt_node = analyse_Block();
    if(judge(Stmt_node)){
        path("Stmt","Block");
        return Stmt_node;
    }else{
        fail("Stmt","Block");
        rollback(bck);
    }
    if(tok == Y_SEMICOLON){
        pullin("Stmt:Y_SEMICOLON");
        Stmt_node = newAstNode(NULL_STMT,NULL,NULL);
        return Stmt_node;
    }
    else if(tok == Y_WHILE){
        pullin("Stmt:Y_WHILE");
        advance();
        bck = pot;
        if(tok != Y_LPAR){
            unmatch("Stmt:Y_LPAR");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_LPAR");
        advance();
        bck = pot;
        Stmt_node = newAstNode(WHILE_STMT,NULL,NULL);
        Stmt_node->left = analyse_LOrExp();
        if(judge(Stmt_node->left)){
            path("Stmt","LOrExp");
        }else{
            fail("Stmt","LOrExp");
            rollback(bck);
            Free(Stmt_node);
            return NULL;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("Stmt:Y_RPAR");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_RPAR");
        advance();
        bck = pot;
        Stmt_node->right = analyse_Stmt();
        if(judge(Stmt_node->right)){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            Free(Stmt_node);
            return NULL;
        }
        return Stmt_node;
    }
    else if(tok == Y_IF){
        pullin("Stmt:Y_IF");
        advance();
        bck = pot ;
        if(tok != Y_LPAR){
            unmatch("Stmt:Y_LPAR");
            return NULL;
        }
        pullin("Stmt:Y_LPAR");
        advance();
        bck = pot;
        Stmt_node = newAstNode(IF_STMT,NULL,NULL);
        Stmt_node->if_cond = analyse_LOrExp();
        if(judge(Stmt_node->if_cond)){
            path("Stmt","LOrExp");
        }else{
            fail("Stmt","LOrExp");
            Free(Stmt_node);
            return NULL;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("Stmt:Y_RPAR");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_RPAR");
        advance();
        Stmt_node ->left = analyse_Stmt();
        if(judge(Stmt_node->left)){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            rollback(bck);
            Free(Stmt_node);
            return NULL;
        }
        advance();
        bck = pot;
        if(tok !=Y_ELSE){
            rollback(bck->lst);
            unmatch("Stmt:Y_ELSE");
            return Stmt_node;
        }
        pullin("Stmt:Y_ELSE");
        advance();
        bck = pot;
        Stmt_node->right = analyse_Stmt();
        if(judge(Stmt_node->right)){
            path("Stmt","Stmt");
        }else{
            fail("Stmt","Stmt");
            rollback(bck);
            Free(Stmt_node);
            return NULL;
        }
        return Stmt_node;
    }
    if(tok == Y_BREAK){
        pullin("Stmt:Y_BREAK");
        advance();
        Stmt_node = newAstNode(BREAK_STMT,NULL,NULL);
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_SEMICOLON");
        return Stmt_node;
    }
    if(tok == Y_CONTINUE){
        pullin("Stmt:Y_CONTINUE");
        advance();
        bck = pot;
        Stmt_node = newAstNode(CONTINUE_STMT,NULL,NULL);
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_SEMICOLON");
        return Stmt_node;
    }
    if(tok == Y_RETURN){
        pullin("Stmt:Y_RETURN");
        advance();
        bck = pot;
        Stmt_node = newAstNode(RETURN_STMT,NULL,NULL);
        Stmt_node->left = analyse_Exp();
        if(judge(Stmt_node->left)){
            path("Stmt","Exp");
        }else{
            fail("Stmt","Exp");
            rollback(bck->lst);
        }
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            unmatch("Stmt:Y_SEMICOLON");
            Free(Stmt_node);
            return NULL;
        }
        pullin("Stmt:Y_SEMICOLON");
        return Stmt_node;
    }
    Free(Stmt_node);
    return NULL;
}
past analyse_AddExp(){
    list *bck ;
    bck = pot;
    past AddExp_node = analyse_MulExp();
    if(judge(AddExp_node)){
        path("AddExp","MulExp");
    }else{
        fail("AddExp","MulExp");
        rollback(bck);
        Free(AddExp_node);
        return NULL;
    }
    advance();
    bck = pot;
    past t = analyse_AddExpDot();
    if(judge(t)){
        path("AddExp","AddExpDot");
        past s = t;
        while(s->left != NULL)s=s->left;
        s->left = AddExp_node;
        AddExp_node = t;
    }else{
        fail("AddExp","AddExpDot");
        rollback(bck->lst);
    }
    return AddExp_node;
}
past analyse_AddExpDot(){
    list *bck;
    bck = pot;
    if(tok == Y_ADD){
        pullin("AddExpDot:Y_ADD");
    }else if(tok == Y_SUB){
        pullin("AddExpDot:Y_SUB");
    }else{
        unmatch("AddExpDot:Y_ADD||Y_SUB");
        rollback(bck);
        return NULL;
    }
    past AddExpDot_node = newAstNode(BINARY_OPERATOR,NULL,NULL);
    AddExpDot_node->svalue = pot->s;
    advance();
    bck = pot;
    AddExpDot_node->right=analyse_MulExp();
    if(judge(AddExpDot_node->right)){
        path("AddExpDot","MulExp");
    }else{
        fail("AddExpDot","MulExp");
        rollback(bck);
        Free(AddExpDot_node);
        return NULL;
    }
    advance();
    bck = pot;
    past t = analyse_AddExpDot();
    if(judge(t)){
        path("AddExpDot","AddExpDot");
        past s = t;
        while(s->left!=NULL)s=s->left;
        s->left = AddExpDot_node;
        AddExpDot_node = t;
    }else{
        fail("AddExpDot","AddExpDot");
        rollback(bck->lst);
    }
    return AddExpDot_node;
}
past analyse_LVal(){
    list *bck ;
    bck = pot;
    if(tok != Y_ID){
        unmatch("LVal:Y_ID");
        return NULL;
    }
    past LVal_node = newAstNode(DECL_REF_EXPR,NULL,NULL);
    LVal_node->svalue = pot->s;
    pullin("LVal:Y_ID");
    advance();
    bck = pot;
    past t = analyse_ArraySubscripts();
    if(judge(t)){
        path("LVal","ArraySubScripts");
        past s = t;
        while(s->left!=NULL){s=s->left;}
        s->left = LVal_node;
        LVal_node = t;
        return LVal_node;
    }else{
        fail("LVal","ArraySubScripts");
        rollback(bck->lst);
    }
    return LVal_node;
}
past analyse_LOrExp(){
    list *bck;
    bck = pot;
    past LOrExp_node = analyse_LAndExp();
    if(judge(LOrExp_node)){
        path("LOrExp","LAndExp");
    }else{
        fail("LOrExp","LAndExp");
        rollback(bck);
        Free(LOrExp_node);
        return NULL;
    }
    advance();
    bck = pot;
    while(tok == Y_OR){
        pullin("LOrExp:Y_OR");
        past t = newAstNode(BINARY_OPERATOR,LOrExp_node,NULL);
        t->svalue = pot->s;
        advance();
        bck = pot;
        t->right = analyse_LAndExp();
        LOrExp_node = t;
        advance();
        bck = pot;
    }
    rollback(pot->lst);
    return LOrExp_node;
}
past analyse_MulExp(){
    list *bck;
    bck = pot;
    past MulExp_node = analyse_UnaryExp();
    if(judge(MulExp_node)){
        path("MulExp","UnaryExp");
    }else{
        fail("MuilExp","UnaryExp");
        rollback(bck);
        Free(MulExp_node);
        return NULL;
    }
    advance();
    bck = pot;
    past t =analyse_MulExpDot();
    if(judge(t)){
        path("MulExp","MulExpDot");
        past s = t;
        while(s->left!=NULL)s=s->left;
        s->left = MulExp_node;
        MulExp_node = t;
    }else{
        fail("MulExp","MulExpDot");
        rollback(bck->lst);
        Free(t);
    }
    // printf("\ntest\n");
    return MulExp_node;
}
past analyse_MulExpDot(){
    list *bck;
    bck = pot;
    past MulExpDot_node = newAstNode(BINARY_OPERATOR,NULL,NULL);
    MulExpDot_node->svalue = pot->s;
    if(tok == Y_MUL||tok == Y_DIV || tok == Y_MODULO){
        pullin("MulExpDot:Y_ML||Y_DIV||Y_MODLO");
    }else{
        unmatch("MulExpDot:Y_ML||Y_DIV||Y_MODLO");
        Free(MulExpDot_node);
        return NULL;
    }
    advance();
    bck = pot;
    MulExpDot_node->right = analyse_UnaryExp();
    if(judge(MulExpDot_node->right)){
        path("MulExpDot","UnaryExp");
    }else{
        fail("MulExpDot","UnaryExp");
        rollback(bck);
        Free(MulExpDot_node);
        return NULL;
    }
    advance();
    bck = pot;
    past t= analyse_MulExpDot();
    if(judge(t)){
        path("MulExpDot","MulExpDot");
        past s = t;
        while (s->left!=NULL)s=s->left;
        s->left = MulExpDot_node ;
        MulExpDot_node = t;
    }else{
        fail("MulExpDot","MulExpDot");
        rollback(bck->lst);
        Free(t);
    }
    return MulExpDot_node;
}
past analyse_LAndExp(){
    list *bck ;
    bck = pot;
    past LAndExp_node = analyse_EqExp();
    if(judge(LAndExp_node)){
        path("LAndExp","EqExp");
    }else{
        fail("LAndExp","EqExp");
        rollback(bck);
        Free(LAndExp_node);
        return NULL;
    }
    advance();
    bck =pot;
    while(tok == Y_AND){
        past t = newAstNode(BINARY_OPERATOR,LAndExp_node,NULL);
        t->svalue = pot->s;
        advance();
        bck = pot;
        t->right = analyse_EqExp();
        LAndExp_node = t;
        advance();
        bck = pot;
    }
    rollback(pot->lst);
    return LAndExp_node;
}
past analyse_UnaryExp(){
    list *bck,*nxt;
    bck = pot;
    nxt = bck->nxt;
    past UnaryExp_node = NULL;
    if(tok == Y_ID&&(nxt->val==Y_LPAR)){
        pullin("UnaryExp:Y_ID");
        UnaryExp_node = newAstNode(CALL_EXPR,NULL,NULL);
        UnaryExp_node->left = newAstNode(DECL_REF_EXPR,NULL,NULL);
        UnaryExp_node->left->svalue = pot->s;
        advance();
        bck = pot;
        if(tok !=Y_LPAR){
            unmatch("UnaryExp:Y_LPAR");
            Free(UnaryExp_node);
            return NULL;
        }
        pullin("UnaryExp:Y_LPAR");
        advance();
        bck = pot;
        UnaryExp_node->right = analyse_CallParams();
        if(judge(UnaryExp_node->right)){
            path("UnaryExp","CallParams");
        }else{
            fail("UnaryExp","CallParams");
            rollback(bck->lst);
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("UnaryExp:Y_RPAR");
            Free(UnaryExp_node);
            return NULL;
        }
        pullin("UnaryExp:Y_RPAR");
        return UnaryExp_node;
    }
    else if(tok == Y_SUB || tok == Y_NOT || tok == Y_ADD){
        UnaryExp_node = newAstNode(UNARY_OPERATOR,NULL,NULL);
        UnaryExp_node->svalue = pot->s;
        pullin("UnaryExp:Y_SUB");
        advance();
        bck = pot;
        UnaryExp_node->right = analyse_UnaryExp();
        if(judge(UnaryExp_node->right)){
            path("UnaryExp","UnaryExp");
        }else{
            fail("UnaryExp","UnaryExp");
            rollback(bck);
            Free(UnaryExp_node);
            return NULL;
        }
        return UnaryExp_node;
    }
    else{
        UnaryExp_node = analyse_PrimaryExp();
        if(judge(UnaryExp_node)){
            path("UnaryExp","PrimaryExp");
            return UnaryExp_node;
        }else{
            fail("UnaryExp","PrimaryExp");
            rollback(bck);
        }
    }
    Free(UnaryExp_node);
    return NULL;
}
past analyse_EqExp(){
    list *bck ;
    bck =pot;
    past EqExp_node = analyse_RelExp();
    if(judge(EqExp_node)){
        path("EqExp","RelExp");
    }else{
        fail("EqExp","RelExp");
        rollback(bck);
        Free(EqExp_node);
        return NULL;
    }
    advance();
    bck = pot;
    while(tok == Y_EQ || tok == Y_NOTEQ){
        past t = newAstNode(BINARY_OPERATOR,EqExp_node,NULL);
        t->svalue = pot->s;
        advance();
        bck = pot;
        t->right = analyse_RelExp();
        EqExp_node = t;
        advance();
        bck = pot;
    }
    rollback(pot->lst);
    return EqExp_node;
}
past analyse_PrimaryExp(){
    list *bck ;
    bck = pot;
    past PrimaryExp_node = NULL;
    if(tok == Y_LPAR){
        pullin("PrimaryExp:Y_LPAR");
        advance();
        bck =pot;
        PrimaryExp_node = analyse_Exp();
        if(judge(PrimaryExp_node)){
            path("PrimaryExp","Exp");
        }else{
            fail("PrimaryExp","Exp");
            rollback(bck);
            Free(PrimaryExp_node);
            return NULL;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            unmatch("PrimaryExp:Y_RPAR");
            Free(PrimaryExp_node);
            return NULL;
        }
        pullin("ParmaryExp:Y_RPAR");
        return PrimaryExp_node;
    }
    if(tok == num_INT){
        PrimaryExp_node = newAstNode(INTEGER_LITERAL,NULL,NULL);
        PrimaryExp_node->svalue = pot->s;
        PrimaryExp_node->ivalue = atoi(pot->s);
        pullin("ParmaryExp:num_INT");
        return PrimaryExp_node;
    }else{
        unmatch("ParmaryExp:num_INT");
    }
    if(tok == num_FLOAT){
        PrimaryExp_node = newAstNode(FLOATING_LITERAL,NULL,NULL);
        PrimaryExp_node->svalue = pot->s;
        PrimaryExp_node->fvalue = atof(pot->s);
        pullin("ParmaryExp:num_FLOAT");
        return PrimaryExp_node;
    }else{
        unmatch("ParmaryExp:num_FLOAT");
    }
    PrimaryExp_node = analyse_LVal();
    if(judge(PrimaryExp_node)){
        path("ParamaryExp","LVal");
        return PrimaryExp_node;
    }else{
        fail("ParamaryExp","LVal");
        rollback(bck);
    }
    Free(PrimaryExp_node);
    return NULL;
}
past analyse_CallParams(){
    list *bck;
    bck = pot;
    past CallParams_node = NULL;
    CallParams_node = analyse_Exp();
    if(judge(CallParams_node)){
        path("CallParams","Exp");
    }else{
        fail("CallParams","Exp");
        rollback(bck);
        Free(CallParams_node);
        return NULL;
    }
    advance();
    bck = pot;
    if(tok == Y_COMMA){
        pullin("CallParams:Y_COMMA");
        advance();
        bck = pot;
        CallParams_node->next = analyse_CallParams();
        if(judge(CallParams_node->next)){
            path("CallParams","CallParams");
            return CallParams_node;
        }else{
            fail("CallParams","CallParams");
            rollback(bck);
            Free(CallParams_node);
            return NULL;
        }
    }else{
        unmatch("CallParams:Y_COMMA");
        rollback(bck->lst);
    }
    return CallParams_node;
}
past analyse_RelExp(){
    list *bck ;
    bck = pot;
    past RelExp_node = analyse_AddExp();
    if(judge(RelExp_node)){
        path("RelExp","AddExp");
    }else{
        fail("RelExp","AddExp");
        rollback(bck);
        Free(RelExp_node);
        return NULL;
    }
    advance();
    bck = pot;
    while(tok == Y_LESS||tok == Y_GREAT || tok == Y_LESSEQ || tok ==Y_GREATEQ){
        pullin("Y_LESS|Y_GREAT|Y_LESSEQ|Y_GREATEQ");
        past t = newAstNode(BINARY_OPERATOR,RelExp_node,NULL);
        t->svalue = pot->s;
        advance();
        bck = pot;
        t->right = analyse_AddExp();
        RelExp_node = t;
        advance();
        bck = pot;
    }
    rollback(pot->lst);
    return RelExp_node;
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