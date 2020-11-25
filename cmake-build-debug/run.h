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
    stack.push_back(old_b);//��̬����ÿ�������һ���Ļ���ַ
    stack.push_back(old_p+1);//���ص�ַ
    if(l>=1)
        stack.push_back(old_b);//��̬�����ұ������в����ֱ���ǵ�ǰ�Ļ���ַ
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
 LIT���������ŵ���ջ����a��Ϊ������
 LOD���������ŵ�ջ����a��Ϊ��������˵�����е����λ�ã�lΪ���ò���˵����Ĳ��ֵ��
 STO����ջ���������͵�ĳ������Ԫ�С�a,l��ĺ�����LOD����ͬ��
 CAL�����ù��̵�ָ�aΪ�����ù��̵�Ŀ���������е�ַ��lΪ��
 INT��Ϊ�����õĹ��̣���������������ջ�п�����������a��Ϊ���ٵĸ�����
 JMP��������ת��ָ�aΪת���ַ��
 JPC������ת��ָ���ջ���Ĳ���ֵΪ����ʱ��ת��a��ĵ�ַ������˳��ִ�С�
 OPR����ϵ���������㡣���������a��������������Ϊջ���ʹζ������ݽ������㣬�������ڴζ���a��Ϊ0ʱ���˳���������

 OPR 0 0         ���̽������� ���ص��õ㲢��ջ
 OPR 0 1         ջ��Ԫ��ȡ��
 OPR 0 2            ��ջ����ջ����� ��ջ����Ԫ�� �����ջ
 OPR 0 3            ��ջ����ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 4            ��ջ������ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 5            ��ջ������ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 6            ջ��Ԫ����ż�ж� �����ջ��
 OPR 0 7
 OPR 0 8            �Ƿ���� ��ջ����Ԫ�� �����ջ
 OPR 0 9            �Ƿ񲻵� ��ջ����Ԫ�� �����ջ
 OPR 0 10        ��ջ���Ƿ�С��ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 11        ��ջ���Ƿ���ڵ���ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 12        ��ջ���Ƿ����ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 13        ��ջ���Ƿ�С�ڵ���ջ�� ��ջ����Ԫ�� �����ջ
 OPR 0 14        ջ��ֵ�������Ļ
 OPR 0 15        ��Ļ�������
 OPR 0 16        �������ж���һ����������ջ��
*/
