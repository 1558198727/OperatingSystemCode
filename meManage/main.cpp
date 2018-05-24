/**
* title :C++模拟动态内存管理
* author : liyunhao
* date:2018.05.17
* time : 0:26
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;
/////公用函数
//控制输出的位置
void gotoxy(short x, short y)
{
    static COORD cd;
    HANDLE g_hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    cd.X = (short)(x << 1);
    cd.Y = y;
    SetConsoleCursorPosition(g_hConsoleOutput, cd);
}

int getRandom(int a,int b){//返回指定范围的随机数
    int range = b - a;
    if(range<=0){
        return 0;
    }else{
        return a + rand()%range;
    }

}


class Memory{
public:
    int *p;
    int len;
    int CurrPos;
    Memory(int n=500){
        len = n;
        CurrPos = 0;
        p = new int [n];
        memset(p,0,4*n);
    }
    /////////
    int findAPos(int proLen){
        int pos = CurrPos;
        int start;
        int end;
        start = pos;
        while(p[start]==1 && start < len){
            start++;
        }
        end = start;
        while(p[end]==0){
            end++;
        }
        if(end-start > proLen && end <len){
            return start;
        }

    }
    bool MemoCanBeWrote(int s,int len){
        for(int i=s;i<s+len;i++){
            if(p[i]){
                return false;
            }
        }
        return true;
    }

    bool writeMemo(int s,int len){

        for(int i=s;i<s+len;i++){
            p[i]=1;
        }

        CurrPos = s+len;
        CurrPos %= this->len;
    }

    bool clearMemo(int s,int len){
        for(int i=s;i<s+len;i++){
            p[i]=0;
        }
    }

    void disp(){

        gotoxy(1,1);
        for(int i=0;i<len;i++){

            cout<<p[i];
            if(i!=0 && (i+1)%100==0){
                cout<<endl;
                gotoxy(1,i/100+2);
            }
        }
        cout<<endl;
    }
};

class Process{
public:
    int pid;
    int meSize;
    int meStart;
    int time;
    int nextPro;
    Process(int pid,int meSize,int time){
        this->pid = pid;
        this->meSize = meSize;
        this->time = time;
    }
};

class MemoMange{
public:
    Memory * Memory1;
    vector<Process *> ProcVector;
    MemoMange(){
        Memory1 = new Memory;
    }
    int FindRoomForPro(int proLen){
        vector<Process *>::iterator iter;
        if(ProcVector.size() ==0 && proLen<Memory1->len){
            return 0;
        }
        iter =ProcVector.end()-1;
        if(Memory1->MemoCanBeWrote( (*iter)->nextPro,proLen )){
            return (*iter)->nextPro;
        }else{

            for(iter=ProcVector.begin();iter!=ProcVector.end()-1;iter++){
                if(Memory1->MemoCanBeWrote( (*iter)->nextPro,proLen )){
                return (*iter)->nextPro;
                }
            }
            return -1;
        }
    }
    bool ProInComing(Process * pro){
        if(pro){
            int startOfNewPro = FindRoomForPro(pro->meSize);
            if(startOfNewPro >= 0){

                Memory1->writeMemo(startOfNewPro, pro->meSize);
                pro->meStart = startOfNewPro;
                pro->nextPro = startOfNewPro + pro->meSize;
                pro->nextPro %= Memory1->len;
                //Memory1->CurrPos += pro->meSize;
                ProcVector.push_back(pro);
                gotoxy(1,7);
                cout<<"进程"<<pro->pid<<"被创建"<<"并且成功写入内存"<<endl;
            }else{
                gotoxy(1,7);
                cout<<"进程"<<pro->pid<<"被创建"<<"但是无法写入内存"<<endl;
            }
            Memory1->disp();
        }
        else{
            cout<<"未知的进程"<<endl;
        }
    }

    bool ProLeaving(Process * pro){
         //在vector中查找指定元素
        vector<Process *>::iterator iter = find(ProcVector.begin(),ProcVector.end(),pro);
        //cout<<"pro->meStart"<<pro->meStart<<"pro->meSize"<<pro->meSize<<endl;
        Memory1->clearMemo(pro->meStart,pro->meSize);
        //删除指定元素
        gotoxy(1,8);
        cout<<"进程"<<pro->pid<<"被移出内存"<<endl;
        if(iter!=ProcVector.end())ProcVector.erase(iter);
        Memory1->disp();

    }

    bool ShortestProLeaving(){
         //在vector中查找指定元素

        vector<Process *>::iterator iter;
        vector<Process *>::iterator shortestIter;
        if(ProcVector.size() <= 0){
            return false;
        }
        iter = ProcVector.begin();
        shortestIter = iter;
        int shortest = (*iter)->time;

        while(iter != ProcVector.end()){
            if( (*iter)->time < shortest){
                shortestIter = iter;
                shortest = (*iter)->time;
            }
            iter++;
        }
        ProLeaving(*shortestIter);
        return true;
    }
    void disp(){
        vector<Process *>::iterator iter=ProcVector.begin();
        while(iter!=ProcVector.end())
        cout<<*iter++<<" ";
        cout<<endl;
    }
};

int main()
{
    MemoMange MemoMange1;
    Process * p;
    gotoxy(20,0);
    cout<<"C++模拟动态内存管理"<<endl;
    int i=0;
    while(1){

        int randLen = getRandom(10,30);
        int action = getRandom(0,10);
        int randTime = getRandom(10,30);
        if(action<=7){
            p = new Process(i,randLen,randTime);
            MemoMange1.ProInComing(p);
        }else{
            MemoMange1.ShortestProLeaving();

        }
        Sleep(1000);
        i++;
    }

    return 0;
}
