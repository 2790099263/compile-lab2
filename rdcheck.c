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
#define DEBUG_OUT
// #define DEBUG
#define WHILE
#define DEBUG_LOG

#ifdef DEBUG_LOG
void info(char str[]){
    printf("[INFO]      %s\n",str);
    return ;
}
void path(char strFrom[],char strTo[]){
    printf("[PATH]      %s   --->   %s\n",strFrom,strTo);
    return ;
}
void fail(char strFrom[],char strTo[]){
    printf("[FAIL]      %s   \\->   %s\n",strFrom,strTo);
    return ;
}
void error(char str[]){
    printf("[ERROR]     %s\n",str);
    return ;
}
void unmatch(char str[]){
    printf("[UNMATCH]   %s\n",str);
    return ;
}
void pullin(char str[]){
    printf("[PULLIN]    %s\n",str);
}
#endif

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
        printf("[+]         advance: %d %s\n",pot->val,s_now);
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
#endif
/*function decl part end*/

/*functions program begin*/
int analyse_CompUnit(){
    list *bck;
    bck=pot;
    if(analyse_Decl()) {
        advance();
        analyse_CompUnit();
        path("CompUnit","Decl");
        return 1;
    }
    else {
        fail("CompUnit","Decl");
        rollback(bck);
    }
    if(analyse_FuncDef()) {
        advance();
        analyse_CompUnit();
        path("CompUnit","FuncDef");
        return 1;
    }
    else {
        fail("CompUnit","FuncDef");
        rollback(bck);
    }
    return 0;
}
int analyse_Decl() {
    list *bck;
    bck = pot;
    if(analyse_ConstDecl()){
        path("Decl","ConstDecl");
        return 1;
    }
    else {
        fail("Decl","ConstDecl");
        rollback(bck);
    }
    if(analyse_VarDecl()){
        path("Decl","VarDecl");
        return 1;
    }
    else {
        fail("Decl","VarDecl");
        rollback(bck);
    }
    return 0;
}
int analyse_FuncDef() {
    list *bck;
    bck = pot;
    if(analyse_Type()){
        path("FuncDef","Type");
    }
    else {
        fail("FuncDef","Type");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_ID) {
        unmatch("FuncDef:Y_ID");
        return 0;
    }
    pullin("FuncDef:Y_ID");
    advance();
    bck = pot;
    if(tok!=Y_LPAR) {
        unmatch("FuncDef:Y_LPAR");
        return 0;
    }
    pullin("FuncDef:Y_LPAR");
    advance();
    bck = pot;
    if(analyse_FuncParams()) {
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
        return 0;
    }
    pullin("FuncDef:Y_RPAR");
    advance();
    bck = pot;
    if(analyse_Block()) {
        path("FuncDef","Y_RPAR");
        return 1;
    }
    else {
        rollback(bck);
        fail("FuncDef","Y_RPAR");
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
/*
FuncParams: FuncParam
           | FuncParams Y_COMMA FuncParam
        ====
FuncParams: FuncParam FuncParams'
FuncParams':Y_COMMA FuncParam FuncParams|e
*/
int analyse_FuncParams(){
    list *bck;
    bck = pot;
    if(analyse_FuncParam()){
        printf("[INFO]      advance FuncParam\n");
    }else{
        printf("[UNMATCH]   need FuncParam\n");
        return 0;
    }
    advance();
    bck = pot;
    if(tok!=Y_COMMA){
        rollback(bck->lst);
        printf("[INFO]      advance e\n");
        return 1;
    }else{
        advance();
        bck = pot;
        if(analyse_FuncParam()){
            printf("[INFO]      advance FuncParam\n");
        }else{
            printf("[UNMATCH]   need FuncParam\n");
            return 0;
        }
        advance();
        bck = pot;
        if(analyse_FuncParams()){
            printf("[INFO]      advance FuncParams\n");
        }else{
            printf("[UNMATCH]   need FuncParam\n");
            return 0;
        }
    }
    return 1;
}
int analyse_ConstDef(){
    list *bck;
    bck = pot;
    if(tok != Y_ID){
        printf("[UNMATECH]  need ID\n");
        return 0;
    }
    printf("[INFO]      advance ID\n");
    advance();
    bck = pot;
    if(analyse_ConstExps()){
        printf("[INFO]      advance ConstExps\n");
    }else{
        printf("[UNMATCH]   unmatched ConstExps\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_ASSIGN){
        printf("[UNMATCH]   need ASSIGN\n");
        return 0;
    }
    printf("[INFO]      advance ASSIGN\n");
    advance();
    bck = pot;
    if(analyse_ConstInitVal()){
        printf("[INFO]      advance ConstInitVal\n");
    }else{
        printf("[UNMATCH]   need ConstInitVal\n");
        return 0;
    }
    return 1;
}
int analyse_ConstDefs(){
    list *bck;
    bck = pot;
    if(analyse_ConstDef()){
        printf("[INFO]      advance ConstDef\n");
        advance();
        bck = pot;
        if(tok!=Y_COMMA){
            printf("[UNMATCH]   need Comma\n");
            return 0;
        }
        printf("[INFO]      advance Comma\n");
        advance();
        bck = pot;
        if(analyse_ConstDef()){
            printf("[INFO]      advance ConstDef\n");
        }else{
            printf("[UNMATCH]   need ConstDef\n");
            return 0;
        }
    }else{
        printf("[UNMATCH]   need ConstDef\n");
        return 0;
    }
    advance();
    bck = pot;
    if(tok!=Y_COMMA){
        printf("[INFO]      need Comma\n");
        rollback(bck->lst);
        return 1;
    }
    printf("[INFO]      advance Comma\n");
    advance();
    bck =pot;
    if(analyse_ConstDef()){
        printf("[INFO]      advance ConstDef\n");
    }else{
        printf("[UNMATCH]   need ConstDef\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_ConstDefs()){
        printf("[INFO]      advance ConstDefs\n");
    }else{
        printf("[UNMATCH]   need ConstDefs\n");
        return 0;
    }
    return 1;
}
int analyse_VarDef(){
    list *bck;
    bck = pot;
    if (tok!=Y_ID){
        printf("[INFO]      need ID\n");
        return 0;
    }
    printf("[INFO]      advance ID\n");
    advance();
    bck = pot;
    if(tok == Y_ASSIGN){
        printf("[INFO]      advance Assin\n");
        advance();
        bck = pot;
        if(analyse_InitVal()){
            printf("[INFO]      advance InitVal\n");
        }else{
            printf("[INFO]      need InitVal\n");
            return 0;
        }
        return 1;
    }else{
        printf("[UNMATCH]   unmatch Assign\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(analyse_ConstExps()){
        printf("[INFO]      advance ConstExps\n");
        advance();
        bck = pot;
        if(tok != Y_ASSIGN){
            rollback(bck->lst);
            printf("[UNMATCH]   unmatch Assign\n");
            return 1;
        }
        printf("[INFO]      advance Assign\n");
        advance();
        bck = pot;
        if(analyse_InitVal()){
            printf("[INFO]      advance InitVal\n");
        }else{
            printf("[UNMATCH]   unmatch InitVal\n");
            return 0;
        }
    }else{
        rollback(bck->lst);
        return 1;
    }
    return 1;
}
int analyse_VarDecls(){
    list *bck;
    bck = pot;
    if(tok!=Y_COMMA){
        printf("[UNMATCH]   need a Comma\n");
        return 0;
    }
    printf("[INFO]      advance Comma\n");
    advance();
    bck = pot;
    if(analyse_VarDef()){
        printf("[INFO]      advance VarDef\n");
    }else{
        printf("[INFO]      need VarDef\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_VarDecls()){
        printf("[INFO]      advance VarDecls\n");
    }else{
        printf("[UNMATCH]   unmatch VarDecls\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_BlockItems(){
    list *bck;
    bck = pot;
    if(analyse_BlockItem()){
        printf("[INFO]      advance BlockItem\n");
    }else{
        printf("[UNMATCH]   need BlockItem\n");
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_BlockItems()){
        printf("[INFO]      advance BlockItem\n");
    }else{
        rollback(bck->lst);
        printf("[UNMATCH]   unmatch BlockItem\n");
    }
    return 1;
}
int analyse_FuncParam(){
    list *bck;
    bck = pot;
    if(analyse_Type()){
        printf("[INFO]      advance Type\n");
    }else{
        printf("[UNMATCH]   unmatch Type\n");
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_ID){
        printf("[UNMATCH]   unmatch Y_ID\n");
        return 0;
    }
    printf("[INFO]      advance Y_ID\n");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        printf("[INFO]      advance ArraySubScripts\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch ArraySubScripts\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_LSQUARE){
        printf("[UNMATCH]   unmatch LSquare\n");
        return 1;
    }
    printf("[INFO]      advance LSquare\n");
    advance();
    bck = pot ;
    if(tok!=Y_RSQUARE){
        printf("[UNMATCH]   unmatch RSquare\n");
        return 0;
    }
    printf("[INFO]      advance RSquare\n");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        printf("[INFO]      advance ArraySubscripts\n");
    }else{
        printf("[UNMATCH]   unmatc ArraySubScripts\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstExps(){
    list *bck;
    bck = pot;
    if(tok != Y_LSQUARE){
        printf("[UNMATCH]   unmatch Lsquare\n");
        return 0;
    }
    printf("[INFO]      advance Lsquare\n");
    advance();
    bck = pot;
    if(analyse_ConstExp()){
        printf("[INFO]      advance ConstExp\n");
    }else{
        printf("[UNMATCH]   need ConstExp\n");
        return 0;
    }
    advance();
    bck = pot;
    if(tok!=Y_RSQUARE){
        printf("[UNMATCH]   unmatch Rsquare\n");
        return 0;
    }
    printf("[INFO]      advance Rsquare\n");
    advance();
    bck = pot;
    if(analyse_ConstExps()){
        printf("[INFO]      advance ConstExp\n");
    }else{
        printf("[UNMATCH]   need ConstExps\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstInitVal(){
    list *bck;
    bck = pot;
    if(analyse_ConstExp()){
        printf("[INFO]      advance ConstExp\n");
        return 1;
    }else{
        printf("[UNMATCH]   need ConstExp\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok!=Y_LBRACKET){
        printf("[UNMATCH]   need Lbracket\n");
        return 0;
    }
    printf("[INFO]      advance Lbracket\n");
    advance();
    bck = pot;
    if(analyse_ConstInitVal()){
        printf("[INFO]      advance ConstInitVal\n");
        advance();
        bck = pot;
        if(analyse_ConstInitVals()){
            printf("[INFO]      advance ConstInitVals\n");
        }else{
            printf("[UNMATCH]   need ConstInitVals\n");
            rollback(bck->lst);
        }
    }else{
        printf("[UNMATCH]   unmatch ConstInitVal\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RBRACKET){
        printf("[UNMATCH]   need Rbracket\n");
        return 0;
    }
    return 1;
}
int analyse_InitVal(){
    list *bck;
    bck = pot;
    if(analyse_Exp()){
        printf("[INFO]      advance Exp\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch Exp\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_LBRACKET){
        printf("[UNMATCH]   unmatch Lbracket\n");
        return 0;
    }
    printf("[INFO]      advance Lbracket\n");
    advance();
    bck = pot;
    if(analyse_InitVal()){
        printf("[INFO]      advance InitVal\n");
        advance();
        bck = pot;
        if(analyse_InitVals()){
            printf("[INFO]      advance InitVals\n");
        }else{
            printf("[UNMATCH]   unmatch InitVals\n");
            rollback(bck->lst);
        }
    }else{
        printf("[UNMATCH]   unmatch InitVal\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(tok != Y_RBRACKET){
        printf("[UNMATCH]   unmatch Rbracket\n");
        return 0;
    }
    printf("[INFO]      advance Rbracket\n");
    return 1;
}
int analyse_BlockItem(){
    list *bck ;
    bck = pot;
    if(analyse_Decl()){
        printf("[INFO]      advance Decl\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch Decl\n");
        rollback(bck->lst);
    }
    advance();
    bck = pot;
    if(analyse_Stmt()){
        printf("[INFO]      advance Stmt\n");
        return 1;
    }else{
        printf("[UNMATCH]   need Stmt\n");
        rollback(bck->lst);
    }
    return 0;
}
int analyse_ArraySubscripts(){
    list *bck;
    bck = pot;
    if(tok != Y_LSQUARE){
        printf("[UNMATCH]   unmatch Lsquare\n");
        return 0;
    }
    printf("[INFO]      advance Lsquare\n");
    advance();
    bck = pot;
    if(analyse_Exp()){
        printf("[INFO]      advance Exp\n");
    }else{
        printf("[UNMATCH]   unmatch Exp\n");
        rollback(bck->lst);
        return 0;
    }
    advance();
    bck = pot;
    if(tok != Y_RSQUARE){
        printf("[UNMATCH]   unmatch Rsquare\n");
        return 0;
    }else
    printf("[INFO]advance Rsquare\n");
    if(analyse_ArraySubscripts()){
        printf("[INFO]      advance ArraySubscripts\n");
    }else{
        printf("[UNMATCH]   unmatch ArraySubscripts\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_ConstExp(){
    list *bck;
    bck = pot;
    if(analyse_AddExp()){
        printf("[INFO]      advance AddExp\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch AddExp\n");
        rollback(bck);
        return 0;
    }
}
int analyse_ConstInitVals(){
    list *bck;
    bck = pot;
    if(tok != Y_COMMA){
        printf("[UNMATCH]   unmatch Comma\n");
        return 0;
    }
    printf("[INFO]      advance Comma\n");
    advance();
    bck = pot;
    if(analyse_ConstInitVal()){
        printf("[INFO]      advance ConstInitVal\n");
    }else{
        printf("[UNMATCH]   unmatch ConstInitVal\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_ConstInitVals()){
        printf("[INFO]      advance ConstInitVals\n");
    }else{
        printf("[UNMATCH]   unmatch ConstInitVals\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_Exp(){
    list *bck;
    bck = pot;
    if(analyse_AddExp()){
        printf("[INFO]      advance AddExp\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch AddExp\n");
        rollback(bck);
        return 0;
    }
}
int analyse_InitVals(){
    list *bck ;
    bck = pot;
    if(tok != Y_COMMA){
        printf("[UNMATCH]   unmatch Comma\n");
        return 0;
    }
    printf("[INFO]      advance Comma\n");
    advance();
    bck = pot;
    if(analyse_InitVal()){
        printf("[INFO]      advance InitVal\n");
    }else{
        printf("[UNMATCH]   unmatch InitVal\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(analyse_InitVals()){
        printf("[INFO]      advance InitVals\n");
    }else{
        printf("[UNMATCH]   unmatch InitVals\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_Stmt(){
    list *bck ;
    bck = pot ;
    if(tok == Y_SEMICOLON){
        printf("[INFO]      advance Semicolon\n");
        return 1;
    }
    if(analyse_LVal()){
        printf("[INFO]      advance Lval\n");
        advance();
        bck = pot;
        if(tok != Y_ASSIGN){
            printf("[UNMATCH]   unmatch Assign\n");
            return 0;
        }
        printf("[INFO]      advance Assign\n");
        advance();
        bck = pot;
        if(analyse_Exp()){
            printf("[INFO]      advance Exp\n");
        }else{
            printf("[UNMATCH]   unmatch Exp\n");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            printf("[UNMATCH]   unmatch Semicolon\n");
            return 0;
        }
        printf("[INFO]      advance Semicolon\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch LVal\n");
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
        printf("[INFO]      advance Block \n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch Block\n");
        rollback(bck);
    }
    if(tok == Y_WHILE){
        printf("[INFO]      advance While\n");
        advance();
        bck = pot;
        if(tok != Y_LPAR){
            printf("[UNMATCH]   unmatch LPAR\n");
            return 0;
        }
        printf("[INFO]      advance LPAR\n");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            printf("[INFO]      advance LOrExp\n");
        }else{
            printf("[UNMATCH]   unmatch LorExp\n");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            printf("[UNMATCH]   unmatch RPAR\n");
            return 0;
        }
        printf("[INFO]      advance RPAR\n");
        advance();
        bck = pot;
        if(analyse_Stmt()){
            printf("[INFO]      advance Stmt\n");
        }else{
            printf("[UNMATCH]   unmatch Stmt\n");
            return 0;
        }
        return 1;
    }
    if(tok == Y_IF){
        printf("[INFO]      advance IF\n");
        advance();
        bck = pot ;
        if(tok != Y_LPAR){
            printf("[UNMATCH]   unmatch LPAR\n");
            return 0;
        }
        printf("[INFO]      advance LPAR\n");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            printf("[INFO]      advance LorExp\n");
        }else{
            printf("[UNMATCH]   unmatch LorExp\n");
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            printf("[UNMATCH]   unmatch RPAR\n");
            return 0;
        }
        printf("[INFO]      advance Rpar\n");
        advance();
        if(analyse_Stmt()){
            printf("[INFO]      advance Stmt");
        }else{
            printf("[UNMATCH]   unmatch Stmt\n");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok !=Y_ELSE){
            rollback(bck->lst);
            printf("[UNMATCH]   unmatch Else\n");
            return 1;
        }
        printf("[INFO]      advance Else\n");
        advance();
        bck = pot;
        if(analyse_Stmt()){
            printf("[INFO]      advance Stmt\n");
        }else{
            printf("[UNMATCH]   unmatch Stmt\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    if(tok == Y_BREAK){
        printf("[INFO]      advance Break\n");
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            printf("[UNMATCH]   unmatch Semicolon\n");
            return 0;
        }
        printf("[INFO]      advance semicolon\n");
        return 1;
    }
    if(tok == Y_CONTINUE){
        printf("[INFO]      advance Continue\n");
        advance();
        bck = pot;
        if(tok != Y_SEMICOLON){
            printf("[UNMATCH]   unmatch Semicolon\n");
            return 0;
        }
        printf("[INFO]      advance Semicolon\n");
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
            rollback(bck);
        }
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
        return 1;
    }else{
        fail("AddExp","MulExp");
        rollback(bck);
    }
    if(tok  == Y_ADD){
        pullin("AddExp:Y_ADD");
    }
    else if(tok  == Y_SUB){
        pullin("AddExp:Y_SUB");
    }else{
        unmatch("AddExp:Y_SUB|Y_ADD");
        rollback(bck->lst);
        return 1;
    }
    advance();
    bck = pot;
    if(analyse_MulExp()){
        path("AddExp","MulExp");
        return 1;
    }else{
        fail("AddExp","MulExp");
        rollback(bck);
        return 0;
    }
}
int analyse_LVal(){
    list *bck ;
    bck = pot;
    if(tok != Y_ID){
        printf("[UNMATCH]   unmatch ID\n");
        return 0;
    }
    printf("[INFO]      advance ID");
    advance();
    bck = pot;
    if(analyse_ArraySubscripts()){
        printf("[INFO]      advance ArraySubscripts\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch ArraySubscripts\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_LOrExp(){
    list *bck;
    bck = pot;
    if(analyse_LAndExp()){
        printf("[INFO]      advance LandExp\n");
    }else{
        printf("[UNMATCH]   unmatch LAndExp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok == Y_OR){
        printf("[INFO]      advance OR\n");
        advance();
        bck = pot;
        if(analyse_LOrExp()){
            printf("[INFO]      advance LorExp\n");
        }else{
            printf("[UNMATCH]   need LOrExp\n");
            rollback(bck);
            return 0;
        }
    }else{
        printf("[UNMATCH]   unmatch OR\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_MulExp(){
    list *bck;
    bck = pot;
    if(analyse_UnaryExp()){
        printf("[INFO]      advance UnaryExp\n");
    }else{
        printf("[UNMATCH]   unmatch UnaryExp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok == Y_MUL){
        printf("[INFO]      advance MUL\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 1;
        }
    }else if(tok == Y_DIV){
        printf("[INFO]      advance DIV\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 1;
        }
    }else if(tok == Y_MODULO){
        printf("[INFO]      advance MODULO\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 1;
        }
    }else{
        printf("[UNMATCH]   need MUL|DIV|MODULO\n");
        rollback(bck->lst);
        return 1;
    }
    advance();
    bck = pot;
    if(analyse_MulExp()){
        printf("[INFO]      advance MulExp\n");
    }else{
        printf("[UNMATCH]   unmatch MulExp\n");
        rollback(bck);
        return 0;
    }
    return 1;
}
int analyse_LAndExp(){
    list *bck ;
    bck = pot;
    if(analyse_EqExp()){
        printf("[INFO]      advance EqExp\n");
    }else{
        printf("[UNMATCH]   unmatch EqExp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck =pot;
    if(tok == Y_ADD){
        printf("[INFO]      advance ADD\n");
        advance();
        bck =pot;
        if(analyse_LAndExp()){
            printf("[INFO]      advance LAndExp\n");
        }else{
            printf("[UNMATCH]   unmatch LAndExp\n");
            rollback(bck);
            return 0;
        }
        
    }else{
        printf("[UNMATCH]   unmatch ADD\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_UnaryExp(){
    list *bck ;
    bck = pot;
    if(analyse_PrimaryExp()){
        printf("[INFO]      advance PrimaryExp\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch PrimaryExp\n");
        rollback(bck);
    }
    if(tok == Y_ID){
        printf("[INFO]      advance Y_ID\n");
        advance();
        bck = pot;
        if(tok !=Y_LPAR){
            printf("[UNMATCH]   unmatch LPAR\n");
            return 0;
        }
        printf("[INFO]      advance LPAR\n");
        advance();
        bck = pot;
        if(analyse_CallParams()){
            printf("[INFO]      advance CallParams\n");
        }else{
            printf("[UNMATCH]   unmatch CallParams\n");
            rollback(bck->lst);
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            printf("[UNMATCH]   unmatch Y_RPAR\n");
            return 0;
        }
        printf("[INFO]      advance RPAR\n");
        return 1;
    }
    if(tok == Y_ADD){
        printf("[INFO]      advance Y_ADD\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    if(tok == Y_SUB){
        printf("[INFO]      advance Y_SUB\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    if(tok == Y_NOT){
        printf("[INFO]      advance Y_NOT\n");
        advance();
        bck = pot;
        if(analyse_UnaryExp()){
            printf("[INFO]      advance UnaryExp\n");
        }else{
            printf("[UNMATCH]   unmatch UnaryExp\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    return 0;
}
int analyse_EqExp(){
    list *bck ;
    bck =pot;
    if(analyse_RelExp()){
        printf("[INFO]      advance RelExp\n");
    }else{
        printf("[UNMATCH]   unmatch RelExp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok==Y_EQ){
        printf("[INFO]      advance EQ\n");
        advance();
        bck = pot;
        if(analyse_EqExp()){
            printf("[INFO]      advance EqExp\n");
        }else{
            printf("[UNMATCH]   unmatch EqExp\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    if(tok == Y_NOTEQ){
        printf("[INFO]      advance NOTEQ\n");
        advance();
        bck = pot;
        if(analyse_EqExp()){
            printf("[INFO]      advance EqExp\n");
        }else{
            printf("[UNMATCH]   unmatch EqExp\n");
            rollback(bck);
            return 0;
        }
        return 1;
    }
    return 1;
}
int analyse_PrimaryExp(){
    list *bck ;
    bck = pot;
    if(tok == Y_LPAR){
        printf("[INFO]      advance LPAR\n");
        advance();
        bck =pot;
        if(analyse_Exp()){
            printf("[INFO]      advance Exp\n");
        }else{
            printf("[UNMATCH]   unmatch Exp\n");
            rollback(bck);
            return 0;
        }
        advance();
        bck = pot;
        if(tok != Y_RPAR){
            printf("[UNMATCH]   unmatch RPAR\n");
            return 0;
        }
        printf("[INFO]      advance RPAR\n");
        return 1;
    }
    if(tok == num_INT){
        printf("[INFO]      advance num_INT\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch num_INT\n");
    }
    if(tok == num_FLOAT){
        printf("[INFO]      advance num_FLOAT\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch num_FLOAT\n");
    }
    if(analyse_LVal()){
        printf("[INFO]      advance LVAL\n");
        return 1;
    }else{
        printf("[UNMATCH]   unmatch LVAL\n");
        rollback(bck);
    }
    return 0;
}
int analyse_CallParams(){
    list *bck;
    bck = pot;
    if(analyse_Exp()){
        printf("[INFO]      advance EXP\n");
    }else{
        printf("[UNMATCH]   unmatch Exp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok == Y_COMMA){
        printf("[INFO]  advance COMMA\n");
        advance();
        bck = pot;
        if(analyse_CallParams()){
            printf("[INFO]      advance CallParams\n");
            return 1;
        }else{
            printf("[UNMATCH]   unmatch CallParams\n");
            rollback(bck);
            return 0;
        }
    }else{
        printf("[UNMATCH]   unmatch COMMA\n");
        rollback(bck->lst);
    }
    return 1;
}
int analyse_RelExp(){
    list *bck ;
    bck = pot;
    if(analyse_AddExp()){
        printf("[INFO]      advance AddExp\n");
    }else{
        printf("[UNMATCH]   unmatch AddExp\n");
        rollback(bck);
        return 0;
    }
    advance();
    bck = pot;
    if(tok==Y_LESS||tok == Y_GREAT || tok == Y_LESSEQ || tok == Y_GREATEQ){
        printf("[INFO]      advance GREAT|LESS|LESSEQ|GREATEQ\n");
        advance();
        bck = pot;
        if(analyse_RelExp()){
            printf("[INFO]      advance RelExp\n");
            return 1;
        }else{
            printf("[UNMATCH]   unmatch RelExp\n");
            rollback(bck);
            return 0;
        }
    }else{
        printf("[UNMATCH]   LESS|GREAT|LESSEQ|GREATEQ\n");
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