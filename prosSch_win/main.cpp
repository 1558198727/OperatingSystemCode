/**
* title :C++ģ����̵���---���̼߳����ź��������ٽ���
* author : liyunhao
* date:2018.04.24
* time : 21:10
*/
/*
���ȵ�˼�룬��Ϊ�����������С�
ÿ��ֻ�ӵ�һ��������ȡ�����̡�

ÿִ������һ�����еĽ��̷���һ�ζ��е�����

����ִ�У�ǰ�������������ȵ��ȣ����һ�����ȡ���̣�
Ϊ�����õ��������Ľ��̿����л���ִ��

�������Ӷ������к��������зֱ����ȡ������һ������
����һ������������뾺����

ֱ��һ�����п������еĽ���ִ�����

����һ���߳�ר�Ų�������
*/

#include<iostream>
#include<list>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
using namespace std;
int mutex = 0;
class tool{//������
public:
    int timeInterval;
    tool(){
        srand((unsigned)time( NULL ));//���������
        timeInterval = 10;//����ִ�еĵ�λ���
    }

    int getRandom(int a,int b){//����ָ����Χ�������
        int range = b - a;
        if(range==0){
            return 0;
        }else{
            return a + rand()%range;
        }

    }

};

class process{//������
public:
    int pid;    //���̱��
    int length;
    int level;
    process(int pid,int lengthOfProcess ){
        length = lengthOfProcess;
        if(length <= 0){
            cout<<"���̴���ʧ�ܣ�"<<endl;
        }else if(length <= 10){
            level = 1;
            this->pid = pid;
            cout<<"����"<<pid<<"����,����"<<length <<endl;
        }else if(length <= 20){
            level = 2;
            this->pid = pid;
            cout<<"����"<<pid<<"����,����"<<length <<endl;
        }else{
            level = 3;
            this->pid = pid;
            cout<<"����"<<pid<<"����,����"<<length <<endl;
        }
    }
    bool ajustLevel(){//�������̵����ȼ�
        if(length <= 0){
            cout<<"����������ϣ�"<<endl;
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
            cout<<"�����Ѿ�������ϣ�"<<endl;
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
        cout<<"����"<<pid<<"ִ�����"<<endl;
    }

};

class processQueue{
public:
    int level;  //���е����ȼ�
    list<process* > List[3];//������ͬ���ȼ��Ķ���
    processQueue(){
        ;
    }

    bool processIncoming(process*pro){
        int level = pro->level;
        int pid = pro->pid;
        if(level ==1){
            List[0].push_back(pro);
            cout<<"����"<<pid<<"�������"<<level<<endl;
            return true;
        }else if(level ==2){
            List[1].push_back(pro);
            cout<<"����"<<pid<<"�������"<<level<<endl;
            return true;
        }else if(level ==3){
            List[2].push_back(pro);
            cout<<"����"<<pid<<"�������"<<level<<endl;
            return true;
        }else{
            cout<<"δ֪�����ȼ���"<<endl;
            return false;
        }
    }

    //��ָ������������һ�����̣�����ԭ���Ķ���ɾ���ý���
    process* findProFromList(int listNum){
        listNum--;
        list<process*>::iterator itor;
        list<process*>::iterator delNode;
        int currentMin = 9999;
        process* shortestPro;
        if(List[listNum].size() == 0){
            //cout<<"����"<<listNum+1<<"�Ľ����Ѿ�ִ�����,��ȡʧ��"<<endl;
            return NULL;
        }else{
            //cout<<"Ԫ�ظ���"<<List1.size()<<endl;// ����list�е�Ԫ�ظ���
            for(itor = List[listNum].begin();itor!=List[listNum].end();itor ++){
                if((*itor)->length < currentMin){
                    currentMin = (*itor)->length;
                    shortestPro = (*itor);
                    delNode = itor;
                }
            }
            List[listNum].erase(delNode);//�ӽ��̶�����ɾ���ý���
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
            //cout<<"����"<<listNum+1<<"�Ľ����Ѿ�ִ�����,��ȡʧ��"<<endl;
            return NULL;
        }else{
            itor = List[listNum].begin();
            int queLen = List[listNum].size();
            //cout<<"queLen :"<<queLen<<endl;
            int randomNum = atool.getRandom(0,queLen-1);
            //cout<<"randomNum :"<<randomNum<<endl;
            //cout<<"���ѡȡ����"<<randomNum<<endl;
            advance(itor,randomNum);
            shortestPro = (*itor);
            List[listNum].erase(itor);//�ӽ��̶�����ɾ���ý���
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
        cout<<"����"<<endl;
        Sleep(500);
        process* p[3];
        p[0] = findProFromListByRandom(2);//�ӵڶ������������ȡ����
        p[1] = findProFromListByRandom(2);//�ӵڶ������������ȡ����
        p[2] = findProFromListByRandom(3);//�ӵ��������������ȡ����
        //process* p3 = findProFromList(3);//�ӵ����������л�ȡ���ʱ�����
        if( (p[0]==NULL) &&(p[1]==NULL) && (p[2]== NULL)){

            //cout<<"2,3�����Ѿ����ˣ�����ʧ��"<<endl;
            return false;
        }else if((p[0]==NULL)  && (p[2]!= NULL)){
            p[0] = findProFromListByRandom(3);//�ӵڶ������л�ȡ���ʱ�����
            p[1] = findProFromListByRandom(3);//�ӵ��������л�ȡ���ʱ�����
        }else if((p[0]!=NULL) &&(p[1]==NULL) && (p[2]!= NULL)){
            p[1] = findProFromListByRandom(3);//�ӵڶ������л�ȡ���ʱ�����
        }else if((p[0]!=NULL) &&(p[1]!=NULL) && (p[2]== NULL)){
            p[2] = findProFromListByRandom(2);//�ӵڶ������л�ȡ���ʱ�����
        }


        if(p[0]){
            p[0]->level = 1;
            cout<<"����"<<p[0]->pid<<endl;
            List[0].push_back(p[0]);//�����һ������
        }
        if(p[1]){
            p[1]->level = 1;
            cout<<"����"<<p[1]->pid<<endl;
            List[0].push_back(p[1]);//�����һ������
        }
        if(p[2]){
            p[2]->level = 1;
            cout<<"����"<<p[2]->pid<<endl;
            List[0].push_back(p[2]);//�����һ������
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
class GanttChart{//�������ͼ����
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
            GamtString ="";//ÿ��25�����һ�θ���ͼ
        }
    }
    void print(){
        cout<<"                 �ý��̵��ȵĸ���ͼ��"<<endl;
        cout<<"____________________________________________________________________"<<endl;
        cout<<GamtString<<endl;
        cout<<"��������������������������������������������������������������������"<<endl;
    }

};
//���������̵߳Ĺ������
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
            mutex=1;//���������ٽ���
            cout<<"������������"<<endl;
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

            mutex=0;//����
            Sleep(1000);//���ڹ۲�
        }


    }
};


DWORD WINAPI ThreadProc(LPVOID lpParameter)
{


    while(1){
        while(mutex==1);
        mutex = 1;          //���������ٽ���
        cout<<"���߳�����"<<endl;
        int RandomPronum = tool1.getRandom(1,5);
        for(int i = 0;i < RandomPronum;i++){//������ɲ�ͬ��1-5�����Ľ���
            int Len = tool1.getRandom(1,50);
            pro = new process(currPro,Len);
            proqueue.processIncoming(pro);
            currPro++;
        }

        mutex = 0;//����
        Sleep(1000);//���ڹ۲�

    }

    return 0;
}
int main(){
    cout<<"ģ����̵���---���ڶ༶����"<<endl;
    index inde;
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    inde.IndexGo();


    return 0;
}



