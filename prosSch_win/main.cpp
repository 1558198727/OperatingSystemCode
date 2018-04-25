/**
* title :C++模拟进程调度---多线程加入信号量控制临界区
* author : liyunhao
* date:2018.04.24
* time : 21:10
*/
/*
调度的思想，分为三级反馈队列。
每次只从第一级队列里取出进程。

每执行三次一级队列的进程发生一次队列调整。

三次执行：前两次遵从最短优先调度，最后一次随机取进程，
为的是让调整上来的进程可以有机会执行

调整：从二级队列和三级队列分别随机取两个和一个进程
放入一级队列里面参与竞争。

直到一级队列空则所有的进程执行完毕

加了一个线程专门产生进程
*/

#include<iostream>
#include<list>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;
int mutex = 0;
class tool{//工具类
public:
    int timeInterval;
    tool(){
        srand((unsigned)time( NULL ));//设置随机数
        timeInterval = 10;//进程执行的单位间隔
    }

    int getRandom(int a,int b){//返回指定范围的随机数
        int range = b - a;
        if(range==0){
            return 0;
        }else{
            return a + rand()%range;
        }

    }

};

class process{//进程类
public:
    int pid;    //进程编号
    int length;
    int level;
    process(int pid,int lengthOfProcess ){
        length = lengthOfProcess;
        if(length <= 0){
            cout<<"进程创建失败！"<<endl;
        }else if(length <= 10){
            level = 1;
            this->pid = pid;
            cout<<"进程"<<pid<<"创建,长度"<<length <<endl;
        }else if(length <= 20){
            level = 2;
            this->pid = pid;
            cout<<"进程"<<pid<<"创建,长度"<<length <<endl;
        }else{
            level = 3;
            this->pid = pid;
            cout<<"进程"<<pid<<"创建,长度"<<length <<endl;
        }
    }
    bool ajustLevel(){//调整进程的优先级
        if(length <= 0){
            cout<<"进程运行完毕！"<<endl;
            return false;
        }else if(length <= 10){
            level = 1;
            return true;
        }else if(length <= 20){
            level = 2;
            return true;
        }else{
            level = 3;
            return true;
        }
    }
    bool doTheProcess(int timeLength){
        if(length <=0 ){
            cout<<"进程已经运行完毕！"<<endl;
            return false;
        }else{
            length -= timeLength;
            ajustLevel();
            if(length < 0){
                length = 0;
                return true;
            }else{
                return true;
            }
        }

    }
    bool finishThePro(){
        cout<<"进程"<<pid<<"执行完毕"<<endl;
    }

};

class processQueue{
public:
    int level;  //队列的优先级
    list<process* > List[3];//三个不同优先级的队列
    processQueue(){
        ;
    }

    bool processIncoming(process*pro){
        int level = pro->level;
        int pid = pro->pid;
        if(level ==1){
            List[0].push_back(pro);
            cout<<"进程"<<pid<<"加入队列"<<level<<endl;
            return true;
        }else if(level ==2){
            List[1].push_back(pro);
            cout<<"进程"<<pid<<"加入队列"<<level<<endl;
            return true;
        }else if(level ==3){
            List[2].push_back(pro);
            cout<<"进程"<<pid<<"加入队列"<<level<<endl;
            return true;
        }else{
            cout<<"未知的优先级！"<<endl;
            return false;
        }
    }

    //从指定队列里面找一个进程，并从原来的队列删除该进程
    process* findProFromList(int listNum){
        listNum--;
        list<process*>::iterator itor;
        list<process*>::iterator delNode;
        int currentMin = 9999;
        process* shortestPro;
        if(List[listNum].size() == 0){
            //cout<<"队列"<<listNum+1<<"的进程已经执行完毕,获取失败"<<endl;
            return NULL;
        }else{
            //cout<<"元素个数"<<List1.size()<<endl;// 返回list中的元素个数
            for(itor = List[listNum].begin();itor!=List[listNum].end();itor ++){
                if((*itor)->length < currentMin){
                    currentMin = (*itor)->length;
                    shortestPro = (*itor);
                    delNode = itor;
                }
            }
            List[listNum].erase(delNode);//从进程队列中删除该进程
            return shortestPro;
        }

    }


    process* findProFromListByRandom(int listNum){
        listNum--;
        list<process*>::iterator itor;
        int currentMin = 9999;
        process* shortestPro;
        tool atool;

        if(List[listNum].size() == 0){
            //cout<<"队列"<<listNum+1<<"的进程已经执行完毕,获取失败"<<endl;
            return NULL;
        }else{
            itor = List[listNum].begin();
            int queLen = List[listNum].size();
            //cout<<"queLen :"<<queLen<<endl;
            int randomNum = atool.getRandom(0,queLen-1);
            //cout<<"randomNum :"<<randomNum<<endl;
            //cout<<"随机选取进程"<<randomNum<<endl;
            advance(itor,randomNum);
            shortestPro = (*itor);
            List[listNum].erase(itor);//从进程队列中删除该进程
            //printQue(1);
            return shortestPro;
        }


    }
    process* selectFromList1AndDoIt(){

        process* shortestPro;
        shortestPro = findProFromList(1);
        if(shortestPro!= NULL){
            shortestPro->finishThePro();
            //ajustLevelQueues();
            return shortestPro;
        }else{
            return NULL;
        }

    }
    process* selectList1ByRondomAndDoIt(){

        process* RondomPro;
        RondomPro = findProFromListByRandom(1);
        if(RondomPro!= NULL){
            RondomPro->finishThePro();
            //ajustLevelQueues();
            return RondomPro;
        }else{
            return NULL;
        }

    }
    bool ajustLevelQueues(){
        cout<<"调整"<<endl;
        Sleep(500);
        process* p[3];
        p[0] = findProFromListByRandom(2);//从第二级队列随机获取进程
        p[1] = findProFromListByRandom(2);//从第二级队列随机获取进程
        p[2] = findProFromListByRandom(3);//从第三级队列随机获取进程
        //process* p3 = findProFromList(3);//从第三极队列中获取最短时间进程
        if( (p[0]==NULL) &&(p[1]==NULL) && (p[2]== NULL)){

            //cout<<"2,3队列已经空了，调整失败"<<endl;
            return false;
        }else if((p[0]==NULL)  && (p[2]!= NULL)){
            p[0] = findProFromListByRandom(3);//从第二级队列获取最短时间进程
            p[1] = findProFromListByRandom(3);//从第三级队列获取最短时间进程
        }else if((p[0]!=NULL) &&(p[1]==NULL) && (p[2]!= NULL)){
            p[1] = findProFromListByRandom(3);//从第二级队列获取最短时间进程
        }else if((p[0]!=NULL) &&(p[1]!=NULL) && (p[2]== NULL)){
            p[2] = findProFromListByRandom(2);//从第二级队列获取最短时间进程
        }


        if(p[0]){
            p[0]->level = 1;
            cout<<"调整"<<p[0]->pid<<endl;
            List[0].push_back(p[0]);//放入第一级队列
        }
        if(p[1]){
            p[1]->level = 1;
            cout<<"调整"<<p[1]->pid<<endl;
            List[0].push_back(p[1]);//放入第一级队列
        }
        if(p[2]){
            p[2]->level = 1;
            cout<<"调整"<<p[2]->pid<<endl;
            List[0].push_back(p[2]);//放入第一级队列
        }
        return true;
    }
    void printQue(int listNum){
        listNum--;
        list<process*>::iterator itor;
        for(itor = List[listNum].begin();itor!=List[listNum].end();itor ++){

                cout<<(*itor)->pid<<" ";
        }
        cout<<endl;
    }

};
class GanttChart{//输出甘特图的类
public:
    string GamtString;
    int proNum;
    GanttChart(){
        GamtString += "|";
        proNum = 0;

    }
    bool add(process *p){
        //for(int i=0 ;i<p->length;i++){
            char temp[10];
            itoa(p->pid,temp,10);
            GamtString += temp;
        //}
        GamtString += "|";
        proNum ++;
        if(proNum!=0 && proNum % 25 ==0){
            GamtString ="";//每隔25个清空一次甘特图
        }
    }
    void print(){
        cout<<"                 该进程调度的甘特图："<<endl;
        cout<<"____________________________________________________________________"<<endl;
        cout<<GamtString<<endl;
        cout<<"￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣"<<endl;
    }

};
//主函数与线程的共享变量
int currPro;
tool tool1;
processQueue proqueue;
process *pro ;
process *proPrint;
GanttChart gant;
int timeInterval;

class index{
public:


    index(){
        currPro = 0;

        timeInterval = tool1.timeInterval;
    }

    void IndexGo(){
        int proCountForPrint = 0;
        while(1){
            while(mutex==1);
            mutex=1;//加锁保护临界区
            cout<<"在主函数里面"<<endl;
            for(int i=0;i<3;i++){
                if(proqueue.List[0].size()>0){
                    proPrint=proqueue.selectFromList1AndDoIt();
                    Sleep(proPrint->length * timeInterval);
                    gant.add(proPrint);
                    proCountForPrint++;
                }
            }

            proqueue.ajustLevelQueues();
            if(proCountForPrint!=0 && proCountForPrint % 24==0){
               gant.print();
            }

            mutex=0;//解锁
            Sleep(1000);//便于观察
        }


    }
};


DWORD WINAPI ThreadProc(LPVOID lpParameter)
{


    while(1){
        while(mutex==1);
        mutex = 1;          //加锁保护临界区
        cout<<"在线程里面"<<endl;
        int RandomPronum = tool1.getRandom(1,5);
        for(int i = 0;i < RandomPronum;i++){//随机生成不同（1-5个）的进程
            int Len = tool1.getRandom(1,50);
            pro = new process(currPro,Len);
            proqueue.processIncoming(pro);
            currPro++;
        }

        mutex = 0;//解锁
        Sleep(1000);//便于观察

    }

    return 0;
}
int main(){
    cout<<"模拟进程调度---基于多级队列"<<endl;
    index inde;
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    inde.IndexGo();


    return 0;
}



