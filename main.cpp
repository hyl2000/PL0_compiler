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
    int dx=0;//名字分配到的相对地址
    // int tx0; 保留初始tx
    int cx0; //保留初始cx
    //tx0 = table.size()-1; 记录本层的初始位置（符号表中的位置）
    //if(flagg) 如果语法分析没有出现错误，产生转移代码
    //cx0 = code_top;
    cx0 = code.size()-1;
    gen(JMP, 0, 0);
    if(w[ip].c==0)
        flagg=flagg & Const(fa_num);
    if(w[ip].c==1)
        flagg=flagg & Var(fa_num);
    if(table[table.size()-1].adr>0 && table[table.size()-1].kind==1)
        dx = table[table.size() - 1].adr - 2;
    while(w[ip].c==2) //这里可以改回if
        flagg=flagg & Procedure(fa_num);
    //这句结束之后符号表的最高位应该为当前程序名对应的项，最开始生成的无条件跳转代码应该跳转到这里
    //取当前代码位置，回填初始跳转指令a段，生成开辟空间代码
    //code[table[tx0 - 1].adr].pos = cx;  TODO 开始生成当前过程代码，感觉没用？
    //  dx = table[tx0 - 1].adr; 当前层程序名或过程名需要的size
    gen(INT, 0, dx+3);//生成分配内存代码 在运行栈中为被调用的过程开辟a个单元的数据区
    //code[cx0+1].a=code_top+1;
    code[cx0+1].a=code.size()-1;
    flagg=flagg & Sentence(fa_num);
    if (flagg){
        //语句过程执行结束
        //每个过程出口都要使用的释放数据指令
        gen(OPR,0,0);//结束过程调用，返回结束点并退栈
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
    int dx=3; //相对地址从3开始，前3个单元即0、1、2单元分别为 SL:静态链；DL：动态链；RA：返回地址 这里现在不需要管?是执行的时候需要处理的
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
            gen(0,0,0);//防止新定义的标识符需要pop；
    }
    else
        return false;
    return true;
}

bool Procedure(int father){
    int fa_num=tree_add(father, "Procedure");
    bool flagg=true;
    string old_procedure=procedure_now;//知道现在是哪一个，计算level
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
                gen(OPR, 0, 11);//这里交换了大小，因为比较的是次栈顶和栈顶
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
    //cx2=code_top;//回填
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
    gen(JPC,0,0);//a代表地址，需要在后面进行回填
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
        flagg=Expression(fa_num);//也可以write表达式，产生式有误
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
//"const":0\"var":1\"procedure":2\"begin":3\"end":4\"odd":5\"if":6\"then":7\"call":8\"while":9\"do":10\"read":11\"write":12\<标识符>:13
// <加减>:14\<乘除>:15\<关系运算符>:16\(:17\):18\:=:19\;:20\,:21\.:22\<数字>:23
/*
1 LIT：将常数放到运栈顶，a域为常数。
2 LOD：将变量放到栈顶。a域为变量在所说明层中的相对位置，l为调用层与说明层的层差值。
3 STO：将栈顶的内容送到某变量单元中。a,l域的含义与LOD的相同。
4 CAL：调用过程的指令。a为被调用过程的目标程序的入中地址，l为层差。
5 INT：为被调用的过程（或主程序）在运行栈中开辟数据区。a域为开辟的个数。
6 JMP：无条件转移指令，a为转向地址。
7 JPC：条件转移指令，当栈顶的布尔值为非真时，转向a域的地址，否则顺序执行。
8 OPR：关系和算术运算。具体操作由a域给出。运算对象为栈顶和次顶的内容进行运算，结果存放在次顶。a域为0时是退出数据区。
OPR 0 0     	过程结束调用 返回调用点并退栈
OPR 0 1     	栈顶元素取反
OPR 0 2			次栈顶与栈顶相加 退栈两个元素 结果进栈
OPR 0 3			次栈顶减栈顶 退栈两个元素 结果进栈
OPR 0 4			次栈顶乘以栈顶 退栈两个元素 结果进栈
OPR 0 5			次栈顶除以栈顶 退栈两个元素 结果进栈
OPR 0 6			栈顶元素奇偶判断 结果进栈顶
OPR 0 7
OPR 0 8			是否相等 退栈两个元素 结果进栈
OPR 0 9			是否不等 退栈两个元素 结果进栈
OPR 0 10		次栈顶是否小于栈顶 退栈两个元素 结果进栈
OPR 0 11		次栈顶是否大于等于栈顶 退栈两个元素 结果进栈
OPR 0 12		次栈顶是否大于栈顶 退栈两个元素 结果进栈
OPR 0 13		次栈顶是否小于等于栈顶 退栈两个元素 结果进栈
OPR 0 14		栈顶值输出至屏幕
OPR 0 15		屏幕输出换行
OPR 0 16		从命令行读入一个输入至于栈顶
*/