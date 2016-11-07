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
#include <windows.h>
using namespace std;

#define citynum 10 //���еĸ���Ϊ10
#define MaxGeneration 20//�����Ĵ���
#define population 10 //��Ⱥ��С
#define mate_p 0.4//Ⱦɫ��佻��ĸ���
#define mutate_p 0.5//Ⱦɫ�����ĸ���
int citydistance[citynum][citynum]; //���м����


struct group{
	int citys[citynum]; //���е�����
	int adapt; //�ó����������Ӧ��
	double p; //�ó���������������
}groups[population],tempgroups[population];

group best;//��õĽ��
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
		if(tempgroups[i].p>tempgroups[best_p].p)
			best_p=i;
	}
	best=tempgroups[best_p];
	cout<<"����Ⱥ��Ⱦɫ����������Ϊ��"<<endl;
	for (i=0;i<population;i++){
		cout<<i<<"  "<<tempgroups[i].p<<endl;
	}
}

//ѡ����,���á����̶ġ���ѡ�񷽷�����������������ݶȸ�����
void choose(){
	double gradient[population]; //�ݶȸ���
	double random[population]; //ѡ��Ⱦɫ����������
	int choice[population];//ѡ�е�Ⱦɫ��
	int i,j;
	//����Ⱦɫ����ݶȸ���--���̵�ɫ��
	gradient[0]=tempgroups[0].p;
	for (i=1;i<population;i++){
		gradient[i]=gradient[i-1]+tempgroups[i].p;
	}
	//�õ�Ⱦɫ��ѡ����������--���̵�ת��
	srand((unsigned)time(NULL));
	double k;
	for(i=0;i<population;i++){
		k=rand()%100;
		random[i]=k/100;
	}
	//��ʼѡ��
	for(i=0;i<population;i++)
		for(j=0;j<population;j++){
			if(random[i]<gradient[j]){
				choice[i]=j;//��j��Ⱦɫ�屻ѡ��
				break;
			}
		}	
	//������Ⱥ,ע��Ӧ�ȿ�����Ⱥ������
	for(i=0;i<population;i++){
		groups[i].adapt=tempgroups[i].adapt;
		groups[i].p=tempgroups[i].p;
		for(j=0;j<citynum;j++){
			groups[i].citys[j]=tempgroups[i].citys[j];
		}
	}
	int temp;
	for(i=0;i<population;i++){
			temp=choice[i];
			tempgroups[i].adapt=groups[temp].adapt;
			tempgroups[i].p=groups[temp].p;
			for(j=0;j<citynum;j++)
				tempgroups[i].citys[j]=groups[temp].citys[j];
	}
}

//�ж�Ⱦɫ�����Ƿ���ڳ�ͻ
bool judge(int temp,int point1,int point2){
	bool result=false;
	int i,j;
	for(i=0;i<point1;i++){
		for(j=point1;j<=point2;j++){
			if(tempgroups[temp].citys[i]==tempgroups[temp].citys[j]){
				result=true;
				break;
			}
		}
		if(result) break;
	}
	for(i=point2+1;i<citynum;i++){
		for(j=point1;j<=point2;j++){
			if(tempgroups[temp].citys[i]==tempgroups[temp].citys[j]){
				result=true;
				break;
			}
		}
		if(result) break;
	}
	return result;
}

//���亯��
void generate(){
	int i,j;
	double mate[population];//���������Ⱦɫ�彻��ĸ���
	int flag[population]={0};//Ⱦɫ���Ƿ��ܽ���ı��
	int num=0;//�ܹ������Ⱦɫ����Ŀ
	int temp1,temp2;//���������Ⱦɫ��
	int point1,point2;//����������˵�
	//���ѡ���ɽ����Ⱦɫ��
	srand((unsigned)time(NULL));
	for(i=0;i<population;i++){
		mate[i]=rand()%100;
		mate[i]=mate[i]/100;
		if(mate[i]<mate_p){
			flag[i]=1;
			num++;
		}
	}
	//��֤�����Ⱦɫ����Ŀ��ż��
	num=num/2*2;
	//Ⱦɫ���������䣬������t/2��
	for (i=0;i<num/2;i++){
		//�ҳ��˴ν��������Ⱦɫ��
		for(j=0;j<population;j++){
			if(flag[j]==1){
				temp1=j;
				break;
			}
		}
		for(j=temp1+1;j<population;j++){
			if(flag[j]==1){
				temp2=j;
				break;
			}
		}
		//�������Ⱦɫ�彻��Ķ˵�
		srand((unsigned)time(NULL));
		point1=rand()%citynum;
		point2=rand()%citynum;
		if(point1>point2){
			int t;
			t=point2;
			point2=point1;
			point1=t;
		}
		//�����˵���Ⱦɫ��
		int copy1[citynum]={0},copy2[citynum]={0};
		for(i=point1;i<=point2;i++){
			int t;
			copy1[i]=tempgroups[temp1].citys[i];
			copy2[i]=tempgroups[temp2].citys[i];
			t=tempgroups[temp1].citys[i];
			tempgroups[temp1].citys[i]=tempgroups[temp2].citys[i];
			tempgroups[temp2].citys[i]=t;
		}
		
		//���ܲ�����ͻ��������ͻ
		bool conflict1=false,conflict2=false;//�ж�����Ⱦɫ�����Ƿ��г�ͻ
		conflict1=judge(temp1,point1,point2);
		conflict2=judge(temp2,point1,point2);
		while (conflict1){
			for(i=0;i<point1;i++){
				for(j=point1;j<=point2;j++){
					if(tempgroups[temp1].citys[i]==tempgroups[temp1].citys[j]){
						tempgroups[temp1].citys[i]=copy1[j];
						break;
					}
				}
			}
			for(i=point2;i<citynum;i++){
				for(j=point1;j<=point2;j++){
					if(tempgroups[temp1].citys[i]==tempgroups[temp1].citys[j]){
						tempgroups[temp1].citys[i]=copy1[j];
						break;
					}
				}
			}
			conflict1=judge(temp1,point1,point2);
		}
		while(conflict2){
			for(i=0;i<point1;i++){
				for(j=point1;j<=point2;j++){
					if(tempgroups[temp2].citys[i]==tempgroups[temp2].citys[j]){
						tempgroups[temp2].citys[i]=copy2[j];
						break;
					}
				}
			}
			for(i=point2;i<citynum;i++){
				for(j=point1;j<=point2;j++){
					if(tempgroups[temp2].citys[i]==tempgroups[temp2].citys[j]){
						tempgroups[temp2].citys[i]=copy2[j];
						break;
					}
				}
			}
			conflict2=judge(temp1,point1,point2);
		}
		//��temp1��temp2��Ϊ���ɽ���״̬
		flag[temp1]=0;
		flag[temp2]=0;
	}
}



//���캯��
void mutate(){
	int i,t;
	double p;
	int flag[population]={0};//Ⱦɫ���Ƿ��ܱ���ı��
	srand((unsigned)time(NULL));
	for(i=0;i<population;i++){
		t=rand()%100;
		p=(double)t/100;
		if(p<mutate_p) flag[i]=1;
	}
	//�������
	int point1,point2;
	srand((unsigned)time(NULL));
	for(i=0;i<population;i++){
		if(flag[i]==1){
			point1=rand()%citynum;
			point2=rand()%citynum;
			t=tempgroups[i].citys[point1];
			tempgroups[i].citys[point1]=tempgroups[i].citys[point2];
			tempgroups[i].citys[point2]=t;
		}
	}
}
int test(){
	int i;
	Init();//��ʼ��
	GeneProduce();//������һ����Ⱥ
	evaluate();//���۵�һ����Ⱥ
	for(i=0;i<MaxGeneration;i++){
		choose();
		//generate();
		mutate();
		evaluate();
	}
	cout<<"����"<<MaxGeneration<<"������ѵ�Ⱦɫ��Ϊ"<<endl;
	for(i=0;i<citynum;i++){
		cout<<best.citys[i]<<" ";
	}
	cout<<endl;
	cout<<"�������Ϊ"<<best.p<<endl;
	return 0;
}

void main(){
	for (int i=0;i<10;i++)
	{
		test();
		Sleep(1000);
	}
}