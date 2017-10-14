#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
typedef struct node
{
    struct node *lf;
    void *ptr;
    struct node *rt;
}NODE;
typedef struct info
{
    int no;
    char nm[15];
    double quan;
    double pri;
    double totpri;
}INFO;
typedef struct snode
{
    INFO data;
    struct snode *next;
}SNODE;
SNODE* CreateSnode(INFO tmp)
{
    SNODE *t=(SNODE *)malloc(sizeof(SNODE));
    t->next=NULL;
    (t->data).no=tmp.no;
    strcpy((t->data).nm,tmp.nm);
    (t->data).quan=tmp.quan;
    (t->data).pri=tmp.pri;
    (t->data).totpri=tmp.totpri;
    return t;
}
void insertNode(INFO *d,NODE **rt)
{
    char stmp[15];
    NODE *ntmp=(NODE *)malloc(sizeof(NODE));
    ntmp->lf=NULL;
    ntmp->rt=NULL;
    ntmp->ptr=d;
    if(*rt==NULL)
        *rt=ntmp;
    else
    {
        NODE *j=*rt;
        while(1)
        {
            strcpy(stmp,((INFO *)(j->ptr))->nm);
            if(strcmp(d->nm,stmp)==-1)
            {
                if(j->lf==NULL)
                {
                    j->lf=ntmp;
                    break;
                }
                else
                    j=j->lf;
            }
            else
            {
                if(j->rt==NULL)
                {
                    j->rt=ntmp;
                    break;
                }
                else
                    j=j->rt;
            }
        }
    }
}
void insert(NODE **root)
{
    int n,b;
    INFO tmp;
    FILE *fp=fopen("data.txt","r");
    if(fp==NULL)
    {
        printf("\nFile not found");
        return;
    }
    INFO *tmp1;
    while(1)
    {
        fscanf(fp,"%d %s %lf %lf %lf\n",&tmp.no,tmp.nm,&tmp.quan,&tmp.pri,&tmp.totpri);
        tmp1=(INFO *)malloc(sizeof(INFO));
        *tmp1=tmp;
        insertNode(tmp1,root);
        if(feof(fp))
            break;
    }
    fclose(fp);
}
void display(void *ptr)
{
    INFO *da=(INFO *)ptr;
    printf("%d   | %-5s |  %.2lf\t        |     %.2lf\t   |      %.2lf\n",da->no,da->nm,da->quan,da->pri,da->totpri);
}
void inorderDisplay(NODE *root)
{
    if(root==NULL)
        return;
    else
    {
        inorderDisplay(root->lf);
        display(root->ptr);
        inorderDisplay(root->rt);
    }
}
NODE* Search(NODE *root,char *name)
{
    NODE *tmp=root;
    INFO *stmp;
    while(tmp!=NULL)
    {
        stmp=(INFO *)(tmp->ptr);
        if(strcmp(name,stmp->nm)==0)
            break;
        else if(strcmp(name,stmp->nm)==-1)
            tmp=tmp->lf;
        else
            tmp=tmp->rt;
    }
    return (tmp);
}
void addEnd(SNODE **t,INFO d)
{
    SNODE *itmp=CreateSnode(d),*a,*b;
    if(*t==NULL)
        *t=itmp;
    else
    {
        a=*t;
        while(a!=NULL)
        {
            b=a;
            a=a->next;
        }
        b->next=itmp;
    }
}
void prepareReceipt(NODE **root)
{
    char name[15];
    NODE *ntmp;
    INFO *stmp;
    SNODE *start=NULL;
    double qua;
    INFO na;
    na.no=na.pri=na.quan=na.totpri=-1;
    printf("\nEnter the customer's name : ");
    scanf("%s",na.nm);
    addEnd(&start,na);
    do
    {
        printf("\nEnter the item name ('Exit' to finish) : ");
        scanf("%s",name);
        if(strcmp(name,"Exit")==0)
            break;
        ntmp=Search(*root,name);
        if(ntmp!=NULL)
        {
            printf("\nEnter the amount you want : ");
            scanf("%lf",&qua);
            if(qua<=0)
                continue;
            stmp=(INFO *)(ntmp->ptr);
            if((stmp->quan)<qua)
                printf("\n   \t%s\tAbove quantity is not available",name);
            else
            {
                (stmp->quan)-=qua;
                (stmp->totpri)=(stmp->pri)*(stmp->quan);
                //printf("\n%d\t%s\t%d\t%lf\t%lf",stmp->no,stmp->nm,qua,stmp->pri,(stmp->pri)*qua);
                INFO sn;
                strcpy(sn.nm,stmp->nm);
                sn.no=stmp->no;
                sn.quan=qua;
                sn.pri=stmp->pri;
                sn.totpri=(stmp->pri)*qua;
                addEnd(&start,sn);
                ntmp->ptr=stmp;
            }
        }
        else
            printf("\n   \t%s\tSorry item do not exists",name);
    }while(1);
    FILE *fr=fopen("Receipt.txt","ab");
    fseek(fr,0,SEEK_END);
    SNODE *sttmp=start;
    while(sttmp!=NULL)
    {
        fwrite(&(sttmp->data),sizeof(INFO),1,fr);
        sttmp=sttmp->next;
    }
    fclose(fr);
}
void displayReceipt()
{
    FILE *fr=fopen("Receipt.txt","rb");
    double cost=0;
    int i=1;
    INFO d;
    fseek(fr,0,SEEK_END);
    while(1)
    {
        fseek(fr,-1*sizeof(INFO),SEEK_CUR);
        fread(&d,sizeof(INFO),1,fr);
        fseek(fr,-1*sizeof(INFO),SEEK_CUR);
        if(d.no==-1)
            break;
    }
    while(1)
    {
        fread(&d,sizeof(INFO),1,fr);
        if(d.no==-1)
        {
            printf("________________________________________________________________________________");
            printf("Customer's Name : %s\n",d.nm);
            printf("________________________________________________________________________________");
            printf("\nCode  | Item  | Quantity(kg)    |  Price(rs/kg)\t   |  Total Price(Rs.)\n");
            printf("________________________________________________________________________________");
            printf("\n");
            continue;
        }
        if(feof(fr))
            break;
        cost+=d.totpri;
        display(&d);
    }
    printf("________________________________________________________________________________");
    printf("\nTotal Amount to pay : Rs. %lf\n",cost);
    printf("________________________________________________________________________________\n\n\n\n\n\n\n\n\n\n\n");
}
void displaySnode(SNODE *tmp)
{
    while(tmp!=NULL)
    {
        display(&(tmp->data));
        tmp=tmp->next;
    }
}
void intoFile(NODE *root,FILE *fp)
{
    INFO *stmp;
    if(root==NULL)
        return;
    else
    {
        stmp=(INFO *)(root->ptr);
        if(stmp->quan!=0)
            fprintf(fp,"%d %s %lf %lf %lf\n",stmp->no,stmp->nm,stmp->quan,stmp->pri,stmp->totpri);
        intoFile(root->lf,fp);
        intoFile(root->rt,fp);
    }
}
int main()
{
    NODE *btr=NULL;
    char ch;
    INFO tmp;
    int flg=0;
    insert(&btr);
    printf("\nStock Available : \n");
    printf("________________________________________________________________________________");
    printf("\nCode  | Item  | Quantity(kg)    |  Price(rs/kg)\t   |  Total Price(Rs.)\n");
    printf("________________________________________________________________________________");
    printf("\n");
    inorderDisplay(btr);
    printf("________________________________________________________________________________");
    while(1)
    {
        prepareReceipt(&btr);
        system("cls");
        printf("\nReceipt : \n");
        displayReceipt();
        printf("\nStock Available : \n");
        printf("________________________________________________________________________________");
        printf("\nCode  | Item  | Quantity(kg)    |  Price(rs/kg)\t   |  Total Price(Rs.)\n");
        printf("________________________________________________________________________________");
        printf("\n");
        inorderDisplay(btr);
        printf("________________________________________________________________________________");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\nDo you want to make the receipt : (Y/N) ");
        scanf(" %c",&ch);
        if(ch=='N')
        {
            FILE *fp=fopen("data.txt","w");
            intoFile(btr,fp);
            fclose(fp);
            break;
        }
        flg=1;
        FILE *fp=fopen("data.txt","w");
        intoFile(btr,fp);
        fclose(fp);
        btr=NULL;
        insert(&btr);
        system("cls");
        printf("\nStock Available : \n");
        printf("________________________________________________________________________________");
        printf("\nCode  | Item  | Quantity(kg)    |  Price(rs/kg)\t   |  Total Price(Rs.)\n");
        printf("________________________________________________________________________________");
        printf("\n");
        inorderDisplay(btr);
        printf("________________________________________________________________________________");
    }
    return 0;
}



