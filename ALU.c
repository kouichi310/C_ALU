#include <stdio.h>
#define DATA_SELECT_BIT 3
#define ALL_BIT 4

int nand(int x,int y){
    //2入力nand
    return !(x & y);
}
int not_nand(int x){
    //2入力not
    return nand(x,x);
}
int and_nand(int x,int y){
    //2入力and
    return not_nand(nand(x,y));
}
int three_and_nand(int x,int y,int z){
    //3入力and
    return and_nand(x,(and_nand(y,z)));
}
int or_nand(int x,int y){
    //2入力or
    return nand(not_nand(x),not_nand(y));
}
int four_or_nand(int w,int x,int y,int z){
    //4入力or
    return or_nand(or_nand(w,x),or_nand(y,z));
}
int xor_nand(int x,int y){
    //2入力xor
    return and_nand(or_nand(x,y),or_nand(not_nand(x),not_nand(y)));
}
void ha_nand(int x,int y,int *s,int *c){
    *s=xor_nand(x,y);
    *c=and_nand(x,y);
}
void fa_nand(int a,int b,int c_i,int *s_re,int *c_re){
    int s=0,c1=0,c2=0;
    ha_nand(a,b,&s,&c1);
    ha_nand(c_i,s,&s,&c2);
    *s_re=s;
    *c_re=or_nand(c1,c2);
}
int data_select(int d1,int d2,int d3,int d4,int select[DATA_SELECT_BIT]){
    //     |a|      |b|        |s|      //
    //      0        0         d1       //
    //      0        1         d2       //
    //      1        0         d3       //
    //      1        1         d4       //
    //論理式:d1*~a*~b+d2*~a*b+d3*a*~b+d4*a*b
    int a=select[1],b=select[2];
    return four_or_nand(three_and_nand(d1,not_nand(a),not_nand(b)),three_and_nand(d2,not_nand(a),b),three_and_nand(d3,a,not_nand(b)),three_and_nand(d4,a,b));
}
void alu(int a,int b,int c_in,int d[DATA_SELECT_BIT],int *ans,int *c_out){
    int fa_ans=0,fa_c=0;
    fa_nand(a,b,c_in,&fa_ans,&fa_c);
    *ans=data_select(not_nand(a),and_nand(a,b),or_nand(a,b),fa_ans,d);
    *c_out=fa_c;
}
void str_for_int(char str[ALL_BIT],int re_i[ALL_BIT]){
    //文字列を数列に変える関数
    int i;
    for(i=0;i<ALL_BIT;i++){
        re_i[i]=str[i]-'0';
    }
}
int string_length(char a[]){
    //文字列の長さを返す関数
    int n;
    for(n=0;a[n]!='\0';n++){
        if(a[n]!='0'&&a[n]!='1')return 1;
    }
    for(n=0;a[n]!='\0';n++);
    return n;
}

int main(){
    int c=0,i,d_min=0;
    char sd_select[DATA_SELECT_BIT]="",a_c[ALL_BIT]="",b_c[ALL_BIT]="";
    int check=0,a[ALL_BIT]={},b[ALL_BIT]={},s[ALL_BIT]={},d_select[DATA_SELECT_BIT]={0,0,0},minus[ALL_BIT]={0,0,0,1};
    printf("ALU mode select\n000 ='not',001 ='and',010='or',011='+',111='-'\n");
    scanf("%s",sd_select);
    while(string_length(sd_select)!=DATA_SELECT_BIT){
        printf("There is no mode!\n");
        scanf("%s",sd_select);
    }
    str_for_int(sd_select,d_select);
    printf("Input A\nex) 0001\n");
    scanf("%s",a_c);
    while(string_length(a_c)!=ALL_BIT){
        printf("There is no range!\n");
        scanf("%s",a_c);
    }
    str_for_int(a_c,a);
    if(d_select[0]!=0 || d_select[1]!=0 || d_select[2]!=0){
        printf("Input B\nex) 1111\n");
        scanf("%s",b_c);
        while(string_length(b_c)!=ALL_BIT){
            printf("There is no range!\n");
            scanf("%s",b_c);
        }
        str_for_int(b_c,b);
    }
    if(d_select[0]==1 || b[0]!=0&& b[1]!=0&& b[2]!=0&& b[3]!=0){
        for(i=0;i<ALL_BIT;i++)b[i]=not_nand(b[i]);
        for(i=ALL_BIT;i>0;i--)alu(b[i-1],minus[i-1],c,d_select,&b[i-1],&c);
    }
    for(i=ALL_BIT;i>0;i--)alu(a[i-1],b[i-1],c,d_select,&s[i-1],&c);
    printf("\nAnswer = ");
    if(d_select[0]==0&&d_select[1]==1&&d_select[2]==1)printf("%d",c);
    for(i=0;i<ALL_BIT;i++)printf("%d",s[i]);
    printf("\n");
}