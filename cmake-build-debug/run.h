//
//  run.h
//  PL0
//
//  Created by hyl on 2020/11/24.
//

#ifndef MAIN_CPP_RUN_H
#define MAIN_CPP_RUN_H
#endif //MAIN_CPP_RUN_H

#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

int P,B;
vector<int> stack;
int LIT=1,LOD=2,STO=3, CAL=4,INT=5, JMP=6, JPC=7, OPR=8;

struct code_{
    int f, l, a;
};
vector<code_> code;

void lod(int l, int a){
    int temp_b=B;
    while(l>0){
        l--;
        temp_b=stack[temp_b+2];
    }
    stack.push_back(stack[temp_b+a]);
    P++;
}

void sto(int l,int a){
    int temp_b=B;
    while(l>0){
        l--;
        temp_b=stack[temp_b+2];
    }
    stack[temp_b+a]=stack[stack.size()-1];
    stack.pop_back();
    P++;
}

void cal(int l,int a){
    int old_p=P,old_b=B;
    P=a;
    B=stack.size();
    stack.push_back(old_b);//动态链，每层调用上一个的基地址
    stack.push_back(old_p+1);//返回地址
    if(l>=1)
        stack.push_back(old_b);//静态链，找变量，有层差则直接是当前的基地址
    else
        stack.push_back(stack[old_b+2]);
}

void lit(int a){
    stack.push_back(a);
    P++;
}

void int_(int a){
    int temp=0;
    a-=3;
    for(int i=1;i<=a;i++)
        stack.push_back(temp);
    P++;
}

void jmp(int a){
    P=a;
}

void jpc(int a){
    if(stack[stack.size()-1]==0)
        P=a;
    else
        P++;
}

void opr(int labal){
    int temp;
    if(labal==0){
        int ret=stack[B+1],old_b=B;
        B=stack[B];
        P=ret;
        while(!stack.empty() && stack.size()-1>=old_b)
            stack.pop_back();
        return;
    }
    if(labal==1)
        stack[stack.size()-1]=-stack[stack.size()-1];
    if(labal==2){
        temp=stack[stack.size()-1]+stack[stack.size()-2];
        stack.pop_back();
        stack.pop_back();
        stack.push_back(temp);
    }
    if(labal==3){
        temp=stack[stack.size()-2]-stack[stack.size()-1];
        stack.pop_back();
        stack.pop_back();
        stack.push_back(temp);
    }
    if(labal==4){
        temp=stack[stack.size()-1]*stack[stack.size()-2];
        stack.pop_back();
        stack.pop_back();
        stack.push_back(temp);
    }
    if(labal==5){
        temp=stack[stack.size()-2]/stack[stack.size()-1];
        stack.pop_back();
        stack.pop_back();
        stack.push_back(temp);
    }
    if(labal==6){
        if(stack[stack.size()-1]%2==0){
            stack.pop_back();
            stack.push_back(0);
        }
        else{
            stack.pop_back();
            stack.push_back(1);
        }
    }
    if(labal==8){
        if(stack[stack.size()-1]==stack[stack.size()-2]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
    }
    if(labal==9){
        if(stack[stack.size()-1]==stack[stack.size()-2]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
    }
    if(labal==10){
        if(stack[stack.size()-2]<stack[stack.size()-1]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
    }
    if(labal==11){
        if(stack[stack.size()-2]>=stack[stack.size()-1]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
    }
    if(labal==12){
        if(stack[stack.size()-2]>stack[stack.size()-1]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
    }
    if(labal==13){
        if(stack[stack.size()-2]<=stack[stack.size()-1]){
            stack.pop_back();
            stack.pop_back();
            stack.push_back(1);
        }
        else{
            stack.pop_back();
            stack.pop_back();
            stack.push_back(0);
        }
    }
    if(labal==14)
        cout<<stack[stack.size()-1];
    if(labal==15)
        cout<<endl;
    if(labal==16){
        cin>>temp;
        stack.push_back(temp);
    }
    P++;
}

void running(){
    int len=code.size();
    P=0;
    B=0;
    stack.push_back(0);
    stack.push_back(code.size()-1);
    stack.push_back(0);
    while(P<len and !stack.empty()){
        if(code[P].f==LIT)
            lit(code[P].a);
        else if(code[P].f==LOD)
            lod(code[P].l,code[P].a);
        else if(code[P].f==STO)
            sto(code[P].l,code[P].a);
        else if(code[P].f==CAL)
            cal(code[P].l,code[P].a);
        else if(code[P].f==INT)
            int_(code[P].a);
        else if(code[P].f==JMP)
            jmp(code[P].a);
        else if(code[P].f==JPC)
            jpc(code[P].a);
        else if(code[P].f==OPR)
            opr(code[P].a);
    }
}
/*
 LIT：将常数放到运栈顶，a域为常数。
 LOD：将变量放到栈顶。a域为变量在所说明层中的相对位置，l为调用层与说明层的层差值。
 STO：将栈顶的内容送到某变量单元中。a,l域的含义与LOD的相同。
 CAL：调用过程的指令。a为被调用过程的目标程序的入中地址，l为层差。
 INT：为被调用的过程（或主程序）在运行栈中开辟数据区。a域为开辟的个数。
 JMP：无条件转移指令，a为转向地址。
 JPC：条件转移指令，当栈顶的布尔值为非真时，转向a域的地址，否则顺序执行。
 OPR：关系和算术运算。具体操作由a域给出。运算对象为栈顶和次顶的内容进行运算，结果存放在次顶。a域为0时是退出数据区。

 OPR 0 0         过程结束调用 返回调用点并退栈
 OPR 0 1         栈顶元素取反
 OPR 0 2            次栈顶与栈顶相加 退栈两个元素 结果进栈
 OPR 0 3            次栈顶减栈顶 退栈两个元素 结果进栈
 OPR 0 4            次栈顶乘以栈顶 退栈两个元素 结果进栈
 OPR 0 5            次栈顶除以栈顶 退栈两个元素 结果进栈
 OPR 0 6            栈顶元素奇偶判断 结果进栈顶
 OPR 0 7
 OPR 0 8            是否相等 退栈两个元素 结果进栈
 OPR 0 9            是否不等 退栈两个元素 结果进栈
 OPR 0 10        次栈顶是否小于栈顶 退栈两个元素 结果进栈
 OPR 0 11        次栈顶是否大于等于栈顶 退栈两个元素 结果进栈
 OPR 0 12        次栈顶是否大于栈顶 退栈两个元素 结果进栈
 OPR 0 13        次栈顶是否小于等于栈顶 退栈两个元素 结果进栈
 OPR 0 14        栈顶值输出至屏幕
 OPR 0 15        屏幕输出换行
 OPR 0 16        从命令行读入一个输入至于栈顶
*/
