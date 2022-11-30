#include <stdio.h>
#include <stdlib.h>
typedef struct list_node
{
    /* data */
    int val;
    struct list *lst;
    struct list *nxt;
}list;
list* listInit(list *lst_node){
    list *l;
    l=(list*)malloc(sizeof(list));
    if(l==NULL){
        printf("ERROR! NO MEMORY!\n");
        exit(0);
    }
    l->nxt=NULL;
    l->lst=lst_node;
    return l;
}
void listPush(int v,list *now){
    list *nx;
    list *po=now;
    while(po->nxt!=NULL)po=po->nxt;
    nx = listInit(po);
    nx->val=v;
    nx->lst=po;
    po->nxt=nx;
    return ;
}
int main(){
    list *bga,*end;
    bga = listInit(NULL);
    for(int i=1;i<=10;i++)
    {
        int a=0;
        scanf("%d",&a);
        listPush(a,bga);
    }
    list *po = bga;
    while(po->nxt!=NULL){
        po=po->nxt;
        printf("%d\n",po->val);
    }
}