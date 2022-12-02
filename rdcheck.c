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

#define DECL_PART
// #define DEBUG_OUT
#define DEBUG
// #define WHILE

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
        printf("[ERROR]     NO MEMORY!\n");
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

int tok;

void advance(){
    printf("[INFO]      pot->nxt : %p\n",pot->nxt);
    if(pot->nxt != NULL) {
        pot=pot->nxt;
        printf("[+]         advance: %d %s\n",pot->val,pot->s);
        tok=pot->val;
        return ;
    }
    printf("[WARNING]   No advance!\n");
    tok=-114514;
    return ;
}
void rollback(list *p){
    pot = p;
    tok = p->val;
    return ;
}
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
int analyse_block();
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
int analyse_AddExp();
int analyse_RelExp();
int analyse_EqExp();
int analyse_RelExp();
int analyse_LAndExp();
int analyse_LOrExp();
int analyse_ConstExp();
#endif
/*function decl part end*/

/*functions program begin*/
int analyse_CompUnit(){
    list *bck;
    bck=pot;
    if(analyse_Decl()) {
        advance();
        analyse_CompUnit();
        printf("[INFO]      advance Decl\n");
        return 1;
    }
    else {
        rollback(bck);
    }
    if(analyse_FuncDef()) {
        advance();
        analyse_CompUnit();
        printf("[INFO]      advance CompUnit\n");
        return 1;
    }
    else {
        rollback(bck);
    }
    return 0;
}
int analyse_Decl() {
    list *bck;
    bck = pot;
    if(analyse_ConstDecl()){
        printf("[INFO]      advance ConstDecl\n");
        return 1;
    }
    else {
        rollback(bck);
    }
    if(analyse_VarDecl()){
        printf("[INFO]      advance VarDecl\n");
        return 1;
    }
    else {
        rollback(bck);
    }
    return 0;
}
int analyse_FuncDef() {
    list *bck;
    bck = pot;
    if(analyse_Type()){
        printf("[INFO]      advance Type\n");
    }
    else {
        rollback(bck);
        printf("[UNMATCH]   need a Type\n");
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_ID) {
        printf("[UNMATCH]   need a Y_ID\n");
        return 0;
    }
    printf("[INFO]      advance Y_ID");
    advance();
    bck = pot;
    if(tok!=Y_LPAR) {
        printf("[UNMATCH]   need a Y_LPAR\n");
        return 0;
    }
    printf("[INFO]      advance Y_LPAR");
    advance();
    bck = pot;
    if(analyse_FuncParams()) {
        printf("[INFO]      advance FuncParams\n");
    }
    else {
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RPAR) {
        printf("[UNMATCH]   need a Y_RPAR\n");
        return 0;
    }
    printf("[INFO]      advance Y_RPAR");
    advance();
    bck = pot;
    if(analyse_Block()) {
        printf("[INFO]      advance Block\n");
        return 1;
    }
    else {
        rollback(bck);
        printf("[UNMATCH]   need a Block\n");
        return 0;
    }
}
int analyse_ConstDecl() {
    list *bck;
    bck = pot;
    if(tok != Y_CONST){
        printf("[UNMATCH]   need a Y_CONST\n");
        return 0;
    }
    printf("[INFO]      advance Const\n");
    advance();
    bck = pot;
    if(analyse_Type()) {
        printf("[INFO]      advance Type\n");
    }else{
        printf("[UNMATCH]   need a Type\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_ConstDef()){
        printf("[INFO]      advance ConstDef\n");
    }else {
        printf("[UNMATCH]   need ConstDef\n");
        rollback(bck);
        if(analyse_ConstDefs()){
            printf("[INFO]      advace ConstDefs\n");
        }
        else {
            printf("[UNMATCH]   need ConstDefs\n");
            return 0;
        }
    }
    advance();
    bck = pot;
    if(tok!=Y_SEMICOLON){
        printf("[UNMATCH]   need Semicolon\n");
        return 0;
    }else{
        printf("[INFO]      advance Semicolon\n");
        return 1;
    }
}
int analyse_VarDecl() {
    list *bck;
    bck = pot;
    if(analyse_Type()){
        printf("[INFO]      advance Type\n");
    }else{
        printf("[UNMATCH]   need a Type\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_VarDef()) {
        printf("[INFO]      advance Vardef\n");
    }else{
        printf("[UNMATCH]   need a Vardef\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_VarDecls()) {
        printf("[INFO]      advance VarDecls\n");
    }else{
        printf("[UNMATCH]   need a VarDecls\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_SEMICOLON){
        printf("[UNMATCH]   need a Semicolon\n");
        return 0;
    }
    printf("[INFO]      advance semicolon\n");
    return 1;
}
int analyse_Type() {
    switch (tok)
    {
    case Y_INT:
        printf("[INFO]      advance Int\n");
        break;
    case Y_FLOAT:
        printf("[INFO]      advance Float\n");
        break;
    case Y_VOID:
        printf("[INFO]      advance void\n");
        break;
    default:
        printf("[UNMATCH]   need INT | FLOAT | VOID\n");
        return 0;
        break;
    }
    return 1;
}
int analyse_Block() {
    list *bck;
    bck = pot;
    if(tok!=Y_LBRACKET){
        printf("[UNMATCH]   need LBracket\n");
        return 0;
    }
    printf("[INFO]      advance LBracket\n");
    advance();
    bck = pot;
    if(analyse_BlockItems()){
        printf("[INFO]  advance blockitems\n");
    }else{
        rollback(bck->lst);
        printf("[UNMATCH]   unmatch blockitems\n");
    }
    advance();
    bck = pot;
    if(tok!=Y_RBRACKET){
        printf("[UNMATCH]   need a Rbracket\n");
        return 0;
    }
    return 1;
}
int analyse_FuncParams(){
    if(tok == 301)return 1;
    else return 0;
}
int analyse_ConstDef(){
    if(tok == 302)return 1;
    else return 0;
}
int analyse_ConstDefs(){
    if(tok == 303)return 1;
    else return 0;
}
int analyse_VarDef(){
    if(tok == 304)return 1;
    else return 0;
}
int analyse_VarDecls(){
    if(tok == 305)return 1;
    else return 0;
}
int analyse_BlockItems(){
    if(tok == 306)return 1;
    else return 0;
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
            // printf("[+]push : %d %s\n",tok,yytext);
            listPush(list_head,tok,yytext);
        }
    }
    #endif

    #ifdef DEBUG
    freopen("debug.txt","r",stdin);
    for(int i=1;i<=5;i++){
        int a=0;
        char ss[5]="test";
        scanf("%d",&a);
        listPush(list_head,a,ss);
    }
    #endif
    pot = list_head;
    advance();
    int res = analyse_Block();
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