/**
* title :ģ����̵���---���ڶ༶���У��������ͼ
* author : liyunhao
* date:2018.04.17
* time : 23:42
*/
//���ȵ�˼�룬��Ϊ����
#include<iostream>
#include<list>
#include <time.h>
#include <stdlib.h>
using namespace std;

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
        cout<<"����"<<endl;
        process* p[3];
        p[0] = findProFromList(2);//�ӵڶ������л�ȡ���ʱ�����
        p[1] = findProFromList(2);//�ӵڶ������л�ȡ���ʱ�����
        p[2] = findProFromList(3);//�ӵ��������л�ȡ���ʱ�����
        //process* p3 = findProFromList(3);//�ӵ����������л�ȡ���ʱ�����
        if( (p[0]==NULL) &&(p[1]==NULL) && (p[2]== NULL)){

            //cout<<"2,3�����Ѿ����ˣ�����ʧ��"<<endl;
            return false;
        }else if((p[0]==NULL)  && (p[2]!= NULL)){
            p[0] = findProFromList(3);//�ӵڶ������л�ȡ���ʱ�����
            p[1] = findProFromList(3);//�ӵ��������л�ȡ���ʱ�����
        }else if((p[0]!=NULL) &&(p[1]==NULL) && (p[2]!= NULL)){
            p[1] = findProFromList(3);//�ӵڶ������л�ȡ���ʱ�����
        }else if((p[0]!=NULL) &&(p[1]!=NULL) && (p[2]== NULL)){
            p[2] = findProFromList(2);//�ӵڶ������л�ȡ���ʱ�����
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

};
class GanttChart{//�������ͼ����
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
        cout<<"                 �ý��̵��ȵĸ���ͼ��"<<endl;
        cout<<"____________________________________________________________________"<<endl;
        cout<<GamtString<<endl;
        cout<<"��������������������������������������������������������������������"<<endl;
    }

};
int main(){
    cout<<"ģ����̵���---���ڶ༶����"<<endl;
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



