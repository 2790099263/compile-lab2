/*
list struct test program;
*/

#include <stdio.h>
#include <stdlib.h>
typedef struct list_node
{
    /* data */
    int val;
    struct list *lst;
    struct list *nxt;
}list;
list *head;
list* listInit(){
    list *l;
    l=(list*)malloc(sizeof(list));
    if(l==NULL){
        printf("ERROR! NO MEMORY!\n");
        exit(0);
    }
    l->nxt=NULL;
    l->lst=NULL;
    return l;
}
void listPush(list *now,int v){
    list *nx;
    list *po=now;
    while(po->nxt!=NULL)po=po->nxt;
    nx = (list*)malloc(sizeof(list));
    if(nx == NULL){
        printf("ERROR! NO MEMORY!\n");
        exit(0);
    }
    nx->val=v;
    nx->lst=po;
    po->nxt=nx;
    return ;
}
int main(){
    head = listInit();
    for(int i=1;i<=10;i++)
    {
        int a=0;
        scanf("%d",&a);
        listPush(head,a);
    }
    list *lin;
    lin=head;
    while (lin->nxt!=NULL)
    {
        lin=lin->nxt;
        printf("%d\n",lin->val);
    }
    

}