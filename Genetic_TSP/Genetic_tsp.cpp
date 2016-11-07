/*遗传算法（Genetic Algorithm）是模拟达尔文生物进化论的自然选择和遗传学机理的生物进化过程的计算模型，是一种通过模拟自然进化过程搜索最优解的方法
遗传算法的基本运算过程如下：
a)初始化：设置进化代数计数器t=0，设置最大进化代数T，随机生成M个个体作为初始群体P(0)。
b)个体评价：计算群体P(t)中各个个体的适应度。
c)选择运算:将选择算子作用于群体。选择的目的是把优化的个体直接遗传到下一代或通过配对交叉产生新的个体再遗传到下一代。选择操作是建立在群体中个体的适应度评估基础上的。
d)交叉运算：将交叉算子作用于群体。所谓交叉是指把两个父代个体的部分结构加以替换重组而生成新个体的操作。遗传算法中起核心作用的就是交叉算子。
e)变异运算：将变异算子作用于群体。即是对群体中的个体串的某些基因座上的基因值作变动。
群体P(t)经过选择、交叉、变异运算之后得到下一代群体P(t 1)。
f)终止条件判断:若t=T,则以进化过程中所得到的具有最大适应度个体作为最优解输出，终止计算。
*/

/*利用遗传算法求解TSP问题*/

#include <iostream>
#include <time.h>
#include <windows.h>
using namespace std;

#define citynum 10 //城市的个数为10
#define MaxGeneration 20//迭代的次数
#define population 10 //种群大小
#define mate_p 0.4//染色体间交配的概率
#define mutate_p 0.5//染色体变异的概率
int citydistance[citynum][citynum]; //城市间距离


struct group{
	int citys[citynum]; //城市的排序
	int adapt; //该城市排序的适应度
	double p; //该城市排序的生存机率
}groups[population],tempgroups[population];

group best;//最好的结果
//给出城市间距离
void Init(){
	cout<<"------城市间的距离矩阵初始化为:------"<<endl;
	srand((unsigned)time(NULL));
	int i,j;
	for (i=0;i<citynum;i++)
	{
		for (j=i;j<citynum;j++)
		{
			citydistance[i][j]=rand()%100+1; //令城市间距离为1~100间随机整数
			citydistance[j][i]=citydistance[i][j];
			cout<<citydistance[i][j]<<"  ";
		}
		cout<<endl;
	}
}

//产生种群
void GeneProduce(){
	//初始化
	int i,j;
	for (i=0;i<population;i++)	{
		for (int j=0;j<citynum;j++){
			tempgroups[i].citys[j]=-1;
		}
	}
	srand((unsigned)time(NULL));
	for (i=0;i<population;i++)	{
		for (j=0;j<citynum;){
			
			int t=rand()%citynum; //随机生成小于城市数量的整数
			int flag=0; //标记数字的产生
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
	//打印初始的种群
	for(i=0;i<population;i++){
		for (j=0;j<citynum;j++){
			cout<<tempgroups[i].citys[j]<<"  ";
		}
		cout<<endl;
	}
}

//评价函数
void evaluate(){
	int i,j;
	int indi_distance=0,sum_distance=0;
	double sum_p=0;
	//计算每个种群中个体的适应度
	for (i=0;i<population;i++){
		indi_distance=0;
		for (j=0;j<citynum-2;j++){
			indi_distance+=citydistance[tempgroups[i].citys[j]][tempgroups[i].citys[j+1]];
		}
		tempgroups[i].adapt=indi_distance;//种群中每条染色体的路径之和
		sum_distance+=indi_distance;//整个种群的路径总和，用来计算生存概率
	}
	//求生存概率
	for (i=0;i<population;i++){
		tempgroups[i].p=1-(double)tempgroups[i].adapt/(double)sum_distance; //路径越短，生存几率越大
		sum_p+=tempgroups[i].p; //此时sum_p不为1！
	}
	for (i=0;i<population;i++){
		tempgroups[i].p=tempgroups[i].p/sum_p;//此时生存概率之和为1
	}
	//找到生存概率最大的染色体
	int best_p=0;
	for(i=0;i<population;i++){
		if(tempgroups[i].p>tempgroups[best_p].p)
			best_p=i;
	}
	best=tempgroups[best_p];
	cout<<"该种群中染色体的生存概率为："<<endl;
	for (i=0;i<population;i++){
		cout<<i<<"  "<<tempgroups[i].p<<endl;
	}
}

//选择函数,采用“轮盘赌”的选择方法，即随机概率落在梯度概率内
void choose(){
	double gradient[population]; //梯度概率
	double random[population]; //选择染色体的随机概率
	int choice[population];//选中的染色体
	int i,j;
	//计算染色体的梯度概率--轮盘的色块
	gradient[0]=tempgroups[0].p;
	for (i=1;i<population;i++){
		gradient[i]=gradient[i-1]+tempgroups[i].p;
	}
	//得到染色体选择的随机概率--轮盘的转针
	srand((unsigned)time(NULL));
	double k;
	for(i=0;i<population;i++){
		k=rand()%100;
		random[i]=k/100;
	}
	//开始选择
	for(i=0;i<population;i++)
		for(j=0;j<population;j++){
			if(random[i]<gradient[j]){
				choice[i]=j;//第j条染色体被选中
				break;
			}
		}	
	//更新种群,注意应先拷贝种群！！！
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

//判断染色体中是否存在冲突
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

//交配函数
void generate(){
	int i,j;
	double mate[population];//随机产生各染色体交配的概率
	int flag[population]={0};//染色体是否能交配的标记
	int num=0;//能够交配的染色体数目
	int temp1,temp2;//交配的两条染色体
	int point1,point2;//交配的两个端点
	//随机选出可交配的染色体
	srand((unsigned)time(NULL));
	for(i=0;i<population;i++){
		mate[i]=rand()%100;
		mate[i]=mate[i]/100;
		if(mate[i]<mate_p){
			flag[i]=1;
			num++;
		}
	}
	//保证交配的染色体数目是偶数
	num=num/2*2;
	//染色体两两交配，共交配t/2次
	for (i=0;i<num/2;i++){
		//找出此次交配的两条染色体
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
		//随机产生染色体交配的端点
		srand((unsigned)time(NULL));
		point1=rand()%citynum;
		point2=rand()%citynum;
		if(point1>point2){
			int t;
			t=point2;
			point2=point1;
			point1=t;
		}
		//交换端点间的染色体
		int copy1[citynum]={0},copy2[citynum]={0};
		for(i=point1;i<=point2;i++){
			int t;
			copy1[i]=tempgroups[temp1].citys[i];
			copy2[i]=tempgroups[temp2].citys[i];
			t=tempgroups[temp1].citys[i];
			tempgroups[temp1].citys[i]=tempgroups[temp2].citys[i];
			tempgroups[temp2].citys[i]=t;
		}
		
		//可能产生冲突，消除冲突
		bool conflict1=false,conflict2=false;//判断两条染色体中是否有冲突
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
		//将temp1和temp2置为不可交配状态
		flag[temp1]=0;
		flag[temp2]=0;
	}
}



//变异函数
void mutate(){
	int i,t;
	double p;
	int flag[population]={0};//染色体是否能变异的标记
	srand((unsigned)time(NULL));
	for(i=0;i<population;i++){
		t=rand()%100;
		p=(double)t/100;
		if(p<mutate_p) flag[i]=1;
	}
	//变异操作
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
	Init();//初始化
	GeneProduce();//产生第一代种群
	evaluate();//评价第一代种群
	for(i=0;i<MaxGeneration;i++){
		choose();
		//generate();
		mutate();
		evaluate();
	}
	cout<<"经过"<<MaxGeneration<<"代后最佳的染色体为"<<endl;
	for(i=0;i<citynum;i++){
		cout<<best.citys[i]<<" ";
	}
	cout<<endl;
	cout<<"生存概率为"<<best.p<<endl;
	return 0;
}

void main(){
	for (int i=0;i<10;i++)
	{
		test();
		Sleep(1000);
	}
}