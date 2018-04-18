/**
* title :模拟进程调度---基于多级队列，输出甘特图
* author : liyunhao
* date:2018.04.17
* time : 23:42
*/
//调度的思想，分为三级
#include<iostream>
#include<list>
#include <time.h>
#include <stdlib.h>
using namespace std;

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

    process* findProFromList1AndDoIt(){
        //list<process*>::iterator itor;
        //for(itor = List[0].begin();itor!=List[0].end();itor ++){

                //cout<<(*itor)->pid<<" ";
        //}
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

    bool ajustLevelQueues(){
        cout<<"调整"<<endl;
        process* p[3];
        p[0] = findProFromList(2);//从第二级队列获取最短时间进程
        p[1] = findProFromList(2);//从第二级队列获取最短时间进程
        p[2] = findProFromList(3);//从第三级队列获取最短时间进程
        //process* p3 = findProFromList(3);//从第三极队列中获取最短时间进程
        if( (p[0]==NULL) &&(p[1]==NULL) && (p[2]== NULL)){

            //cout<<"2,3队列已经空了，调整失败"<<endl;
            return false;
        }else if((p[0]==NULL)  && (p[2]!= NULL)){
            p[0] = findProFromList(3);//从第二级队列获取最短时间进程
            p[1] = findProFromList(3);//从第三级队列获取最短时间进程
        }else if((p[0]!=NULL) &&(p[1]==NULL) && (p[2]!= NULL)){
            p[1] = findProFromList(3);//从第二级队列获取最短时间进程
        }else if((p[0]!=NULL) &&(p[1]!=NULL) && (p[2]== NULL)){
            p[2] = findProFromList(2);//从第二级队列获取最短时间进程
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

};
class GanttChart{//输出甘特图的类
public:
    string GamtString;
    GanttChart(){
        GamtString += "|";

    }
    bool add(process *p){
        //for(int i=0 ;i<p->length;i++){
            char temp[10];
            itoa(p->pid,temp,10);
            GamtString += temp;
        //}
        GamtString += "|";
    }
    void print(){
        cout<<"                 该进程调度的甘特图："<<endl;
        cout<<"____________________________________________________________________"<<endl;
        cout<<GamtString<<endl;
        cout<<"￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣"<<endl;
    }

};
int main(){
    cout<<"模拟进程调度---基于多级队列"<<endl;
    srand( (unsigned)time( NULL ) );
    processQueue proqueue;
    process* pro = NULL;
    process* proPrint=NULL;
    GanttChart gant;
    for(int i=1;i<=25;i++){
        //pro = new process(rand()%30,i);
        pro = new process(i,5+i);
        proqueue.processIncoming(pro);
    }
    for(int i = 1;i<=25;i++){
        proPrint=proqueue.findProFromList1AndDoIt();
        gant.add(proPrint);
        if(i%3==0){
            proqueue.ajustLevelQueues();
        }

    }
    gant.print();
    return 0;
}



