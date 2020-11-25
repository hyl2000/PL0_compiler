//
//  main.cpp
//  PL0
//
//  Created by hyl on 2020/11/10.
//

#include<cstdio>
#include<iostream>
#include<cstring>
#include<vector>
#include<fstream>
#include<algorithm>
#include<D:\C++ files\Compile\cmake-build-debug\run.h>
using namespace std;
bool Program(),Sub_program(int father),Const(int father),Const_def(int father),Number(int father),Symbol(int father),Var(int father),Procedure(int father);
bool Sentence(int father),Procedure_head(int father),Eq(int father),If(int father),While(int father),Call(int father),Read(int father),Write(int father),Combain(int father);
bool Expression(int father),Term(int father),Factor(int father);
string s,procedure_now;
string keyword[20];
ifstream in("./source_code-3.txt");
ofstream out("./code.txt");
int length,begin_,top,ip,tree_top,recursion_level,code_top=-1;
vector<string> live_proc;
struct word{
    string w;
    int c;
    int number;
}w[100010];
struct tab{
    string name,procdure;
    int kind, val, level, adr;
};
struct tree{
    string tag;
    int father;
    vector<int> child;
}gramma_tree[100000];

vector<tab> table;
//code_ code[100010];

void make_tab(string name, int kind, int a, int adr, string procure){
    tab temp;
    temp.name=name;
    temp.kind=kind;
    if(kind==0) {
        temp.val = a;
        temp.level = 0;
    }
    else {
        temp.val = -1;
        temp.level = a;
    }
    temp.adr=adr;
    temp.procdure=procure;
    table.push_back(temp);
}

void word_read(string str, int c, int n){
    w[top].w=str;
    w[top].c=c;
    w[top].number=n;
    top++;
    return ;
}

int tree_add(int father, string tag){
    gramma_tree[tree_top].tag=tag;
    gramma_tree[tree_top].father=father;
    gramma_tree[father].child.push_back(tree_top);
    tree_top+=1;
    return tree_top-1;
}

bool is_name(string x){
    int x_len=x.length();
    if (!('a'<=x[0]<='z'))
        return false;
    for(int i=1;i<x_len;i++)
        if (!(('a'<=x[i]<='z')&&('0'<=x[i]<='9')))
            return false;
    if (x_len>10)
        return false;
    return true;
}

int is_keyword(string x){
    for(int i=0;i<14;i++)
        if(x==keyword[i])
            return i;
    return -1;
}

string to_lower_case(string str){
    int len=str.length();
    for (int i=0;i<len;i++)
        str[i] = tolower(str[i]);
    return str;
}

int get_degit(string str){
    int len=str.length();
    int num=0;
    for(int i=0;i<len;i++)
        num=num*10+(str[i]-'0');
    return num;
}

bool process(string str){
    bool sym_flag=false,num_flag=false,char_flag=false;
    int sub_begin=0,num;
    string temp,sym;
    if(str=="")
        return true;
    int c=is_keyword(str);
    if (c!=-1){
        word_read(str, c, 0);
        return true;
    }
    int len=str.length();
    for(int i=0;i<len;i++){
        if (!(str[i]>='0' && str[i]<='9') && num_flag){
            if(str[i]>='a' && str[i]<='z')
                return false;
            num_flag=false;
            string temp=str.substr(sub_begin,i-sub_begin);
            num=get_degit(temp);
            word_read(temp, 23, num);
        }
        else if (str[i]>='0' && str[i]<='9' && !num_flag && !char_flag){
            num_flag=true;
            sub_begin=i;
        }
        if (!((str[i]>='a' && str[i]<='z')||(str[i]>='0' && str[i]<='9')) && char_flag){
            char_flag=false;
            string temp=str.substr(sub_begin,i-sub_begin);
            int c=is_keyword(temp);
            bool f=is_name(temp);
            if (c!=-1)
                word_read(temp, c, 0);
            else if(f)
                word_read(temp, 13, 0);
            if(!(f || c!=-1))
                return false;
        }
        else if (str[i]>='a' && str[i]<='z' && !char_flag){
            char_flag=true;
            sub_begin=i;
        }
        if(str[i]=='+'||str[i]=='-'){
            temp=str[i];
            word_read(temp, 14, 0);
        }
        if(str[i]=='*'||str[i]=='/'){
            temp=str[i];
            word_read(temp, 15, 0);
        }
        if(str[i]=='('){
            temp=str[i];
            word_read(temp, 17, 0);
        }
        if(str[i]==')'){
            temp=str[i];
            word_read(temp, 18, 0);
        }
        if(str[i]==':' && str[i+1]!='=')
            return false;
        if(str[i]==';'){
            temp=str[i];
            word_read(temp, 20, 0);
        }
        if(str[i]==','){
            temp=str[i];
            word_read(temp, 21, 0);
        }
        if(str[i]=='.'){
            temp=str[i];
            word_read(temp, 22, 0);
        }
        if(str[i]=='='&&str[i-1]==':'){
            temp=":=";
            word_read(temp, 19, 0);
            continue;
        }
        if((sym_flag==false&&str[i]=='=')||(str[i]=='#')){
            temp=str[i];
            word_read(temp,16,0);
        }
        if(sym_flag==true&&str[i]=='='){
            word_read(str.substr(i-1,2),16,0);
            sym_flag=false;
        }
        if(sym_flag==true&&str[i]!='='){
            temp=str[i-1];
            word_read(temp,16,0);
            sym_flag=false;
        }
        if(str[i]=='<'||str[i]=='>') {
            sym_flag = true;
            sym = str[i];
        }
    }
    if (num_flag){
        num_flag=false;
        string temp=str.substr(sub_begin,len-sub_begin);
        num=get_degit(temp);
        word_read(temp, 23, num);
    }
    if (char_flag){
        char_flag=false;
        string temp=str.substr(sub_begin,len-sub_begin);
        int c=is_keyword(temp);
        if (c!=-1){
            word_read(temp, c, 0);
            return true;
        }
        bool f=is_name(temp);
        if(f)
            word_read(temp, 13, 0);
    }
    if(sym_flag)
        word_read(sym,16,0);
    return true;
}

void init_keyword(){
    keyword[0]="const";
    keyword[1]="var";
    keyword[2]="procedure";
    keyword[3]="begin";
    keyword[4]="end";
    keyword[5]="odd";
    keyword[6]="if";
    keyword[7]="then";
    keyword[8]="call";
    keyword[9]="while";
    keyword[10]="do";
    keyword[11]="read";
    keyword[12]="write";
    return ;
}

/*
void block(){
    int flag=-1,lev,dx;
    for(int i=0;i<top;i++){
        if((w[i].c==0)||(w[i].c==1)||(w[i].c==2))
            flag=w[i].c;
        //if(flag!=-1 && w[i].c==13)
    }
}
*/

void gen(int f, int l, int a){
    code_ temp;
    temp.a=a;
    temp.f=f;
    temp.l=l;
    //code_top++;
    //code[code_top]=temp;
    code.push_back(temp);
}

bool Program(){
    procedure_now="main";
    live_proc.push_back(procedure_now);
    int fa_num=tree_add(0, "Program");
    bool flagg=Sub_program(fa_num);
    if(w[ip].c==22){
        tree_add(0, w[ip].w);
        ip++;
        return flagg;
    }
    else
        return false;
}

bool Sub_program(int father){
    bool flagg=true;
    int fa_num=tree_add(father, "Sub_program");
    int dx=0;//���ַ��䵽����Ե�ַ
    // int tx0; ������ʼtx
    int cx0; //������ʼcx
    //tx0 = table.size()-1; ��¼����ĳ�ʼλ�ã����ű��е�λ�ã�
    //if(flagg) ����﷨����û�г��ִ��󣬲���ת�ƴ���
    //cx0 = code_top;
    cx0 = code.size()-1;
    gen(JMP, 0, 0);
    if(w[ip].c==0)
        flagg=flagg & Const(fa_num);
    if(w[ip].c==1)
        flagg=flagg & Var(fa_num);
    if(table[table.size()-1].adr>0 && table[table.size()-1].kind==1)
        dx = table[table.size() - 1].adr - 2;
    while(w[ip].c==2) //������ԸĻ�if
        flagg=flagg & Procedure(fa_num);
    //������֮����ű�����λӦ��Ϊ��ǰ��������Ӧ����ʼ���ɵ���������ת����Ӧ����ת������
    //ȡ��ǰ����λ�ã������ʼ��תָ��a�Σ����ɿ��ٿռ����
    //code[table[tx0 - 1].adr].pos = cx;  TODO ��ʼ���ɵ�ǰ���̴��룬�о�û�ã�
    //  dx = table[tx0 - 1].adr; ��ǰ����������������Ҫ��size
    gen(INT, 0, dx+3);//���ɷ����ڴ���� ������ջ��Ϊ�����õĹ��̿���a����Ԫ��������
    //code[cx0+1].a=code_top+1;
    code[cx0+1].a=code.size()-1;
    flagg=flagg & Sentence(fa_num);
    if (flagg){
        //������ִ�н���
        //ÿ�����̳��ڶ�Ҫʹ�õ��ͷ�����ָ��
        gen(OPR,0,0);//�������̵��ã����ؽ����㲢��ջ
        return true;
    }
    else
        return false;
}

bool Const(int father){
    bool flagg=true;
    int fa_num=tree_add(father, "Const");
    if(w[ip].c==0){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg=flagg & Const_def(fa_num);
    while(w[ip].c==21){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Const_def(fa_num);
    }
    if(w[ip].c==20){
        tree_add(fa_num, w[ip].w);
        ip++;
        return flagg;
    }
    else
        return false;
}

bool Const_def(int father){
    bool flagg=true;
    int fa_num=tree_add(father, "Const_def");
    flagg=flagg & Symbol(fa_num);
    //code_top-=1;
    code.pop_back();
    string namee=w[ip-1].w;
    if(w[ip].w=="="){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg=flagg & Number(fa_num);
    make_tab(namee,0,w[ip-1].number,-1,procedure_now);
    return flagg;
}

bool Number(int father){
    if(w[ip].c==23){
        tree_add(father, w[ip].w);
        ip++;
    }
    else
        return false;
    return true;
}

bool Var(int father){
    int fa_num=tree_add(father, "Var");
    bool flagg=true;
    if(w[ip].c==1){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg=flagg & Symbol(fa_num);
    //code_top-=1;
    code.pop_back();
    string namee=w[ip-1].w;
    int dx=3; //��Ե�ַ��3��ʼ��ǰ3����Ԫ��0��1��2��Ԫ�ֱ�Ϊ SL:��̬����DL����̬����RA�����ص�ַ �������ڲ���Ҫ��?��ִ�е�ʱ����Ҫ�����
    make_tab(namee,1,recursion_level,dx,procedure_now);
    while(w[ip].c==21){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Symbol(fa_num);
        //code_top-=1;
        code.pop_back();
        namee=w[ip-1].w;
        dx++;
        make_tab(namee,1,recursion_level,dx,procedure_now);
    }
    if(w[ip].c==20){
        tree_add(fa_num, w[ip].w);
        ip++;
        return flagg;
    }
    else
        return false;
}

bool Symbol(int father){
    bool flag=false,not_first=false;
    int l_,a_,level_now=-1;
    if(w[ip].c==13){
        tree_add(father, w[ip].w);
        for (int i = 0; i < table.size(); i++)
            if(w[ip].w==table[i].name && find(live_proc.begin(),live_proc.end(),table[i].procdure)!=live_proc.end() && table[i].level>level_now){
                if(table[i].kind==1) {
                    if(not_first)
                        code.pop_back();
                    else
                        not_first = true;
                    l_=recursion_level - table[i].level;
                    a_=table[i].adr;
                    level_now=table[i].level;
                    gen(LOD, l_, a_);
                }
                else {
                    if(not_first)
                        code.pop_back();
                    else
                        not_first = true;
                    level_now=table[i].level;
                    gen(LIT, 0, table[i].val);
                }
                flag=true;
            }
        ip++;
        if(!flag)
            gen(0,0,0);//��ֹ�¶���ı�ʶ����Ҫpop��
    }
    else
        return false;
    return true;
}

bool Procedure(int father){
    int fa_num=tree_add(father, "Procedure");
    bool flagg=true;
    string old_procedure=procedure_now;//֪����������һ��������level
    flagg=flagg & Procedure_head(fa_num);
    procedure_now=w[ip-2].w;
    live_proc.push_back(procedure_now);
    recursion_level += 1;
    if(recursion_level>3)
        return false;
    flagg=flagg & Sub_program(fa_num);
    recursion_level -= 1;
    live_proc.pop_back();
    if(w[ip].c==20){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    procedure_now=old_procedure;
    //while(w[ip].c==2)
    //    flagg=flagg & Procedure(fa_num);
    return flagg;
}

bool Procedure_head(int father){
    int fa_num=tree_add(father, "Procedure_head");
    bool flagg=true;
    if(w[ip].c==2){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg=flagg & Symbol(fa_num);
    //code_top-=1;
    code.pop_back();
    string namee=w[ip-1].w;
    if(w[ip].c==20){
        //make_tab(namee,2,recursion_level,code_top+1);
        make_tab(namee,2,recursion_level,code.size(),namee);
        tree_add(fa_num, w[ip].w);
        ip++;
        return flagg;
    }
    else
        return false;
}

bool Sentence(int father){
    int fa_num=tree_add(father, "Sentence");
    bool flagg=false;
    int ip_t=ip, tree_top_t=tree_top;
    flagg=Eq(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=If(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=While(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=Call(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=Read(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=Write(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=Combain(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    return true;
}

bool Eq(int father){
    int fa_num=tree_add(father, "Eq"),level_now=-1;
    bool flagg=true,flag_,not_first=false;
    flagg=flagg & Symbol(fa_num);
    if(flagg)
        //code_top-=1;
        code.pop_back();
    string s=w[ip-1].w;
    if(w[ip].c==19){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Expression(fa_num);
        flag_=false;
        for (int i = 0; i < table.size(); i++)
            if(s==table[i].name && table[i].kind==1 && find(live_proc.begin(),live_proc.end(),table[i].procdure)!=live_proc.end() && table[i].level>level_now){
                if(not_first)
                    code.pop_back();
                else
                    not_first = true;
                gen(STO,recursion_level-table[i].level,table[i].adr);
                level_now=table[i].level;
                flag_=true;
            }
        if(!flag_)
            return false;
    }
    else
        return false;
    return flagg;
}

bool Combain(int father){
    int fa_num=tree_add(father, "Combain");
    if(w[ip].c==3){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    bool flagg=true;
    flagg=flagg & Sentence(fa_num);
    while(w[ip].c==20){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Sentence(fa_num);
    }
    if(w[ip].c==4){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    return flagg;
}

bool Rules(int father){
    int fa_num=tree_add(father, "Rules");
    string s;
    bool flagg;
    if(w[ip].c==5){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=Expression(fa_num);
        if(flagg) {
            gen(OPR, 0, 6);
            return true;
        }
        else
            return flagg;
    }
    else {
        flagg = true;
        flagg = flagg & Expression(fa_num);
        if (w[ip].c == 16) {
            s = w[ip].w;
            tree_add(fa_num, w[ip].w);
            ip++;
        } else
            return false;
        flagg = flagg & Expression(fa_num);
        if (flagg){
            if (s == "=")
                gen(OPR, 0, 8);
            if (s == "#")
                gen(OPR, 0, 9);
            if (s == "<")
                gen(OPR, 0, 10);
            if (s == "<=")
                gen(OPR, 0, 13);
            if (s == ">")
                gen(OPR, 0, 12);
            if (s == ">=")
                gen(OPR, 0, 11);//���ｻ���˴�С����Ϊ�Ƚϵ��Ǵ�ջ����ջ��
            return true;
        }
        else
            return flagg;
    }
}

bool Expression(int father){
    int fa_num=tree_add(father, "Expression");
    bool flagg=true,plus=true;
    string s;
    if(w[ip].c==14){
        if(w[ip].w=="-")
            plus=false;
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    flagg=flagg & Term(fa_num);
    if(!plus)
        gen(OPR,0,1);
    while(w[ip].c==14){
        s=w[ip].w;
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Term(fa_num);
        if(s=="-")
            gen(OPR,0,3);
        else
            gen(OPR,0,2);
    }
    return flagg;
}

bool Term(int father){
    bool flagg=true;
    string s;
    int fa_num=tree_add(father, "Term");
    flagg=flagg & Factor(fa_num);
    while(w[ip].c==15){
        s=w[ip].w;
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg=flagg & Factor(fa_num);
        if(s == "*")
            gen(OPR,0,4);
        else
            gen(OPR,0,5);
    }
    return flagg;
}

bool Factor(int father){
    int fa_num=tree_add(father, "Factor");
    if(w[ip].c==17){
        tree_add(fa_num, w[ip].w);
        ip++;
        bool f=Expression(fa_num);
        if(w[ip].c==18){
            tree_add(fa_num, w[ip].w);
            ip++;
            return f;
        }
        else
            return false;
    }
    bool flagg=false;
    int ip_t=ip, tree_top_t=tree_top;
    flagg=Symbol(fa_num);
    if(flagg)
        return true;
    else{
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child.clear();
    }
    flagg=Number(fa_num);
    if(flagg)
        gen(LIT, 0, w[ip-1].number);
    return flagg;
}

bool If(int father){
    bool flagg=true;
    int fa_num=tree_add(father, "If"),cx1,cx2;
    if(w[ip].c==6){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    //cx1=code_top;
    flagg = flagg & Rules(fa_num);
    cx1=code.size()-1;
    gen(JPC,0,0);
    if(w[ip].c==7){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg = flagg & Sentence(fa_num);
    //cx2=code_top;//����
    cx2=code.size()-1;
    code[cx1+1].a=cx2+1;
    return flagg;
}

bool Call(int father){
    int fa_num=tree_add(father, "Call");
    if(w[ip].c==8){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    if(Symbol(fa_num))
        //code_top-=1;
        code.pop_back();
    else
        return false;
    if(w[ip-1].w==procedure_now) {
        for (int i = 0; i < table.size(); i++)
            if (w[ip - 1].w == table[i].name && table[i].kind == 2) {
                gen(CAL, 0, table[i].adr);
                return true;
            }
    }
    else {
        int level_now;
        if(procedure_now=="main")
            level_now=0;
        else
            for (int i = 0; i < table.size(); i++)
                if (procedure_now == table[i].name && table[i].kind == 2) {
                    level_now=table[i].level;
                    break;
                }
        for (int i = 0; i < table.size(); i++)
            if (w[ip - 1].w == table[i].name && table[i].kind == 2) {
                gen(CAL, table[i].level-level_now, table[i].adr);
                return true;
            }
    }
    return false;
}

bool While(int father){
    int fa_num=tree_add(father, "While"), cx1,cx2;
    //cx1=code_top;
    cx1=code.size()-1;
    bool flagg=true;
    if(w[ip].c==9){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg = flagg & Rules(fa_num);
    //cx2=code_top;
    cx2=code.size()-1;
    gen(JPC,0,0);//a�����ַ����Ҫ�ں�����л���
    if(w[ip].c==10){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg = flagg & Sentence(fa_num);
    gen(JMP,0,cx1+1);
    //code[cx2+1].a=code_top+1;
    code[cx2+1].a=code.size();
    return flagg;
}

bool Read(int father){
    int fa_num=tree_add(father, "Read");
    bool flagg=true;
    string s;
    if(w[ip].c==11){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    if(w[ip].c==17){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    flagg = flagg & Symbol(fa_num);
    s=w[ip-1].w;
    //code_top--;
    code.pop_back();
    gen(OPR,0,16);
    int level_now=-1;
    bool not_first=false;
    for (int i = 0; i < table.size(); i++)
        if(s==table[i].name && table[i].kind==1 && find(live_proc.begin(),live_proc.end(),table[i].procdure)!=live_proc.end() && table[i].level>level_now){
            if(not_first)
                code.pop_back();
            else
                not_first = true;
            gen(STO,recursion_level-table[i].level,table[i].adr);
            level_now=table[i].level;
        }
    while(w[ip].c==21){
        tree_add(fa_num, w[ip].w);
        ip++;
        flagg = flagg & Symbol(fa_num);
        s=w[ip-1].w;
        code.pop_back();
        gen(OPR,0,16);
        level_now=-1;
        not_first=false;
        for (int i = 0; i < table.size(); i++)
            if(s==table[i].name && table[i].kind==1 && find(live_proc.begin(),live_proc.end(),table[i].procdure)!=live_proc.end() && table[i].level>level_now){
                if(not_first)
                    code.pop_back();
                else
                    not_first = true;
                gen(STO,recursion_level-table[i].level,table[i].adr);
                level_now=table[i].level;
            }
    }
    if(w[ip].c==18){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    return flagg;
}

bool Write(int father){
    int fa_num=tree_add(father, "Write"),ip_t,tree_top_t;
    vector<int> t_child;
    bool flagg=true;
    if(w[ip].c==12){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    if(w[ip].c==17){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    ip_t=ip;
    tree_top_t=tree_top;
    t_child=gramma_tree[fa_num].child;
    flagg=Symbol(fa_num);
    if(!flagg){
        ip=ip_t;
        tree_top=tree_top_t;
        gramma_tree[fa_num].child=t_child;
        flagg=Expression(fa_num);//Ҳ����write���ʽ������ʽ����
        gen(OPR,0,14);
        if(!flagg)
            return false;
    }
    else
        gen(OPR,0,14);
    while(w[ip].c==21){
        tree_add(fa_num, w[ip].w);
        ip++;
        ip_t=ip;
        tree_top_t=tree_top;
        t_child=gramma_tree[fa_num].child;
        flagg=Symbol(fa_num);
        if(!flagg){
            ip=ip_t;
            tree_top=tree_top_t;
            gramma_tree[fa_num].child=t_child;
            flagg=Expression(fa_num);
            gen(OPR,0,14);
            if(!flagg)
                return false;
        }
        else
            gen(OPR,0,14);
    }
    gen(OPR,0,15);
    if(w[ip].c==18){
        tree_add(fa_num, w[ip].w);
        ip++;
    }
    else
        return false;
    return flagg;
}

bool gramma(){
    tree_top=1;
    gramma_tree[0].tag="Root";
    return Program();
}

void out_put_tree(){
    for(int i=0;i<tree_top;i++){
        out<<i<<", "<<gramma_tree[i].tag.c_str()<<endl;
        int len=gramma_tree[i].child.size();
        for(int j=0;j<len;j++)
            out<<gramma_tree[i].child[j]<<" ";
        out<<endl;
        if(len!=0)
            out<<endl;
    }
    for(int i=0;i<table.size();i++)
        out<<table[i].name<<" "<<table[i].kind<<" "<<table[i].val<<" "<<table[i].level<<" "<<table[i].adr<<endl;
    out<<endl;
    //for(int i=0;i<=code_top;i++) {
    for(int i=0;i<code.size();i++) {
        string str;
        switch(code[i].f){
            case 1:
                str="lit";
                break;
            case 2:
                str="lod";
                break;
            case 3:
                str="sto";
                break;
            case 4:
                str="cal";
                break;
            case 5:
                str="int";
                break;
            case 6:
                str="jmp";
                break;
            case 7:
                str="jpc";
                break;
            case 8:
                str="opr";
                break;
        }
        out << i << ": "<< str << " " << code[i].l << " " << code[i].a << endl;
    }
    out<<endl;
    for(int i=0;i<top;i++)
        out<<"("<<w[i].w<<", "<<w[i].c<<", "<<w[i].number<<")"<<endl;
}

int main(){
    recursion_level=0;
    int count=0;
    string sub_s;
    init_keyword();
    top=0;
    while(getline(in, s)){
        count+=1;
        s=to_lower_case(s);
        length = s.length();
        begin_=0;
        for(int i=0;i<=length;i++){
            if((s[i]==' '||i==length)&&(i-begin_>0)){
                sub_s=s.substr(begin_,i-begin_);
                begin_=i+1;
                bool f=process(sub_s);
                if(!f){
                    out<<"Error at line"<<count<<endl;
                    return 0;
                }
            }
            if(s[i]==' ')
                begin_=i+1;
        }
    }

    if(gramma())
        out_put_tree();
    else {
        out << "Error when Building Tree." << endl;
        return 0;
    }

    running();
    out.close();
    in.close();
    return 0;
}
//"const":0\"var":1\"procedure":2\"begin":3\"end":4\"odd":5\"if":6\"then":7\"call":8\"while":9\"do":10\"read":11\"write":12\<��ʶ��>:13
// <�Ӽ�>:14\<�˳�>:15\<��ϵ�����>:16\(:17\):18\:=:19\;:20\,:21\.:22\<����>:23
/*
1 LIT���������ŵ���ջ����a��Ϊ������
2 LOD���������ŵ�ջ����a��Ϊ��������˵�����е����λ�ã�lΪ���ò���˵����Ĳ��ֵ��
3 STO����ջ���������͵�ĳ������Ԫ�С�a,l��ĺ�����LOD����ͬ��
4 CAL�����ù��̵�ָ�aΪ�����ù��̵�Ŀ���������е�ַ��lΪ��
5 INT��Ϊ�����õĹ��̣���������������ջ�п�����������a��Ϊ���ٵĸ�����
6 JMP��������ת��ָ�aΪת���ַ��
7 JPC������ת��ָ���ջ���Ĳ���ֵΪ����ʱ��ת��a��ĵ�ַ������˳��ִ�С�
8 OPR����ϵ���������㡣���������a��������������Ϊջ���ʹζ������ݽ������㣬�������ڴζ���a��Ϊ0ʱ���˳���������
OPR 0 0     	���̽������� ���ص��õ㲢��ջ
OPR 0 1     	ջ��Ԫ��ȡ��
OPR 0 2			��ջ����ջ����� ��ջ����Ԫ�� �����ջ
OPR 0 3			��ջ����ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 4			��ջ������ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 5			��ջ������ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 6			ջ��Ԫ����ż�ж� �����ջ��
OPR 0 7
OPR 0 8			�Ƿ���� ��ջ����Ԫ�� �����ջ
OPR 0 9			�Ƿ񲻵� ��ջ����Ԫ�� �����ջ
OPR 0 10		��ջ���Ƿ�С��ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 11		��ջ���Ƿ���ڵ���ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 12		��ջ���Ƿ����ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 13		��ջ���Ƿ�С�ڵ���ջ�� ��ջ����Ԫ�� �����ջ
OPR 0 14		ջ��ֵ�������Ļ
OPR 0 15		��Ļ�������
OPR 0 16		�������ж���һ����������ջ��
*/