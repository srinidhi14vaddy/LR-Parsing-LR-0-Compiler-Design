#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

char terminals[100]={};int no_t;
char non_terminals[100]={};int no_nt;
char goto_table[100][100];
char reduce[20][20];
char follow[20][20];
char fo_co[20][20];
char first[20][20];

struct state{
    int prod_count;
    char prod[100][100]={{}};
};

void add_dots(struct state *I){
    for(int i=0;i<I->prod_count;i++){
        for (int j=99;j>3;j--)
            I->prod[i][j] = I->prod[i][j-1];
        I->prod[i][3]='.';
    }
}

void augument(struct state *S,struct state *I){
    if(I->prod[0][0]=='S')
        strcpy(S->prod[0],"Z->.S");
    else{
        strcpy(S->prod[0],"S->.");
        S->prod[0][4]=I->prod[0][0];
		}
    S->prod_count++;
}

void get_prods(struct state *I){
    cout<<"Enter the number of productions:\n";
    cin>>I->prod_count;
    cout<<"Enter the number of non terminals:"<<endl;
    cin>>no_nt;
    cout<<"Enter the non terminals one by one:"<<endl;
    for(int i=0;i<no_nt;i++)
        cin>>non_terminals[i];
    cout<<"Enter the number of terminals:"<<endl;
    cin>>no_t;
    cout<<"Enter the terminals (single lettered) one by one:"<<endl;
    for(int i=0;i<no_t;i++)
        cin>>terminals[i];
    cout<<"Enter the productions one by one in form (S->ABc):\n";
    for(int i=0;i<I->prod_count;i++){
        cin>>I->prod[i];
    }
}

bool is_non_terminal(char a){
   if (a >= 'A' && a <= 'Z')
        return true;
    else
        return false;
}

bool in_state(struct state *I,char *a){
    for(int i=0;i<I->prod_count;i++){
        if(!strcmp(I->prod[i],a))
            return true;
    }
    return false;
}

char char_after_dot(char a[100]){
    char b;
    for(int i=0;i<strlen(a);i++)
        if(a[i]=='.'){
            b=a[i+1];
            return b;}

}

char* move_dot(char b[100],int len){
    char a[100]={};
    strcpy(a,b);
    for(int i=0;i<len;i++){
        if(a[i]=='.'){
            swap(a[i],a[i+1]);
            break;
        }
    }
    return &a[0];
}

bool same_state(struct state *I0,struct state *I){

    if (I0->prod_count != I->prod_count)
        return false;

    for (int i=0; i<I0->prod_count; i++)
    {
        int flag = 0;
        for (int j=0; j<I->prod_count; j++)
            if (strcmp(I0->prod[i], I->prod[j]) == 0)
                flag = 1;
        if (flag == 0)
            return false;
    }
    return true;

}

void closure(struct state *I,struct state *I0){
    char a={};
    for(int i=0;i<I0->prod_count;i++){
        a=char_after_dot(I0->prod[i]);
        if(is_non_terminal(a)){
            for(int j=0;j<I->prod_count;j++){
                if(I->prod[j][0]==a){
                    if(!in_state(I0,I->prod[j])){
                        strcpy(I0->prod[I0->prod_count],I->prod[j]);
                        I0->prod_count++;
                    }
                }
            }
        }
    }
}

void goto_state(struct state *I,struct state *S,char a){
    int time=1;
    for(int i=0;i<I->prod_count;i++){
        if(char_after_dot(I->prod[i])==a){
                if(time==1){
                    time++;
                }
                strcpy(S->prod[S->prod_count],move_dot(I->prod[i],strlen(I->prod[i])));
                S->prod_count++;
        }
    }
}

void print_prods(struct state *I){
    for(int i=0;i<I->prod_count;i++)
        printf("%s\n",I->prod[i]);
    cout<<endl;
}

bool in_array(char a[20],char b){
    for(int i=0;i<strlen(a);i++)
        if(a[i]==b)
            return true;
    return false;
}



void cleanup_prods(struct state * I){
    char a[100]={};
    for(int i=0;i<I->prod_count;i++)
        strcpy(I->prod[i],a);
    I->prod_count=0;
}

int main(){
    struct state init;
    struct state temp;struct state temp1;
    int state_count=1;
    get_prods(&init);
    temp=init;
    temp1=temp;
    add_dots(&init);

    for(int i=0;i<100;i++)
        for(int j=0;j<100;j++)
            goto_table[i][j]='~';

    struct state I[50];
    augument(&I[0],&init);
    closure(&init,&I[0]);
    cout<<"\nI0:\n";
    print_prods(&I[0]);

    char characters[20]={};
    for(int i=0;i<state_count;i++){
        char characters[20]={};
        for(int z=0;z<I[i].prod_count;z++)
                if(!in_array(characters,char_after_dot(I[i].prod[z])))
                characters[strlen(characters)]=char_after_dot(I[i].prod[z]);


        for(int j=0;j<strlen(characters);j++){
            goto_state(&I[i],&I[state_count],characters[j]);
            closure(&init,&I[state_count]);
            int flag=0;
            for(int k=0;k<state_count-1;k++){
                if(same_state(&I[k],&I[state_count])){
                    cleanup_prods(&I[state_count]);flag=1;
                    cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<k<<".\n";
                    goto_table[i][k]=characters[j];;
                    break;
                }
            }
             if(flag==0){
                state_count++;
                cout<<"I"<<i<<" on reading the symbol "<<characters[j]<<" goes to I"<<state_count-1<<":\n";
                goto_table[i][state_count-1]=characters[j];
                print_prods(&I[state_count-1]);
            }
        }
    }

}


