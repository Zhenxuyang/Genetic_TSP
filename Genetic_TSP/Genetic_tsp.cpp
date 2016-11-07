/*�Ŵ��㷨��Genetic Algorithm����ģ��������������۵���Ȼѡ����Ŵ�ѧ���������������̵ļ���ģ�ͣ���һ��ͨ��ģ����Ȼ���������������Ž�ķ���
�Ŵ��㷨�Ļ�������������£�
a)��ʼ�������ý�������������t=0����������������T���������M��������Ϊ��ʼȺ��P(0)��
b)�������ۣ�����Ⱥ��P(t)�и����������Ӧ�ȡ�
c)ѡ������:��ѡ������������Ⱥ�塣ѡ���Ŀ���ǰ��Ż��ĸ���ֱ���Ŵ�����һ����ͨ����Խ�������µĸ������Ŵ�����һ����ѡ������ǽ�����Ⱥ���и������Ӧ�����������ϵġ�
d)�������㣺����������������Ⱥ�塣��ν������ָ��������������Ĳ��ֽṹ�����滻����������¸���Ĳ������Ŵ��㷨����������õľ��ǽ������ӡ�
e)�������㣺����������������Ⱥ�塣���Ƕ�Ⱥ���еĸ��崮��ĳЩ�������ϵĻ���ֵ���䶯��
Ⱥ��P(t)����ѡ�񡢽��桢��������֮��õ���һ��Ⱥ��P(t 1)��
f)��ֹ�����ж�:��t=T,���Խ������������õ��ľ��������Ӧ�ȸ�����Ϊ���Ž��������ֹ���㡣
*/

/*�����Ŵ��㷨���TSP����*/

#include <iostream>
#include <time.h>
using namespace std;

#define citynum 10 //���еĸ���Ϊ10
#define MaxGeneration 100 //�����Ĵ���
#define population 10 //��Ⱥ��С
int citydistance[citynum][citynum]; //���м����

struct group{
	int citys[citynum]; //���е�����
	int adapt; //�ó����������Ӧ��
	double p; //�ó���������������
}tempgroups[population],resultgroup[population];

//�������м����
void Init(){
	cout<<"------���м�ľ�������ʼ��Ϊ:------"<<endl;
	srand((unsigned)time(NULL));
	int i,j;
	for (i=0;i<citynum;i++)
	{
		for (j=i;j<citynum;j++)
		{
			citydistance[i][j]=rand()%100+1; //����м����Ϊ1~100���������
			citydistance[j][i]=citydistance[i][j];
			cout<<citydistance[i][j]<<"  ";
		}
		cout<<endl;
	}
}

//������Ⱥ
void GeneProduce(){
	//��ʼ��
	int i,j;
	for (i=0;i<population;i++)	{
		for (int j=0;j<citynum;j++){
			tempgroups[i].citys[j]=-1;
		}
	}
	srand((unsigned)time(NULL));
	for (i=0;i<population;i++)	{
		for (j=0;j<citynum;){
			
			int t=rand()%citynum; //�������С�ڳ�������������
			int flag=0; //������ֵĲ���
			for (int k=0;k<citynum;k++){
				if (tempgroups[i].citys[k]==t){
					flag=1;
				}
			}
			if (flag==0){ 
				tempgroups[i].citys[j]=t;
				j++;
			}
		}
	}
	//��ӡ��ʼ����Ⱥ
	for(i=0;i<population;i++){
		for (j=0;j<citynum;j++){
			cout<<tempgroups[i].citys[j]<<"  ";
		}
		cout<<endl;
	}
}

//���ۺ���
void evaluate(){
	int i,j;
	int indi_distance=0,sum_distance=0;
	double sum_p=0;
	//����ÿ����Ⱥ�и������Ӧ��
	for (i=0;i<population;i++){
		indi_distance=0;
		for (j=0;j<citynum-2;j++){
			indi_distance+=citydistance[tempgroups[i].citys[j]][tempgroups[i].citys[j+1]];
		}
		tempgroups[i].adapt=indi_distance;//��Ⱥ��ÿ��Ⱦɫ���·��֮��
		sum_distance+=indi_distance;//������Ⱥ��·���ܺͣ����������������
	}
	//���������
	for (i=0;i<population;i++){
		tempgroups[i].p=1-(double)tempgroups[i].adapt/(double)sum_distance; //·��Խ�̣����漸��Խ��
		sum_p+=tempgroups[i].p; //��ʱsum_p��Ϊ1��
	}
	for (i=0;i<population;i++){
		tempgroups[i].p=tempgroups[i].p/sum_p;//��ʱ�������֮��Ϊ1
	}
	//�ҵ������������Ⱦɫ��
	int best_p=0;
	for(i=0;i<population;i++){
		if(tempgroups[i].adapt>tempgroups[best_p].adapt)
			best_p=i;
	}	
	cout<<"����Ⱥ��Ⱦɫ����������Ϊ��"<<endl;
	for (i=0;i<population;i++){
		cout<<i<<"  "<<tempgroups[i].p<<endl;
	}
}

//ѡ����
void choose(){

}
int main(){
	Init();
	GeneProduce();
	evaluate();
	return 0;
}