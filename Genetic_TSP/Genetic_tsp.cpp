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
using namespace std;

#define citynum 10 //城市的个数为10
#define MaxGeneration 100 //迭代的次数
#define population 10 //种群大小
int citydistance[citynum][citynum]; //城市间距离

struct group{
	int citys[citynum]; //城市的排序
	int adapt; //该城市排序的适应度
	double p; //该城市排序的生存机率
}tempgroups[population],resultgroup[population];

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
		if(tempgroups[i].adapt>tempgroups[best_p].adapt)
			best_p=i;
	}	
	cout<<"该种群中染色体的生存概率为："<<endl;
	for (i=0;i<population;i++){
		cout<<i<<"  "<<tempgroups[i].p<<endl;
	}
}

//选择函数
void choose(){

}
int main(){
	Init();
	GeneProduce();
	evaluate();
	return 0;
}