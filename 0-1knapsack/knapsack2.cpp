#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream> 
#include <iostream>
#include <string>
#include <bits/stdc++.h>
//分支限界法、蒙特卡洛来求解0-1背包问题
using namespace std;
std::vector<int> weight;
std::vector<int> value;
int n=0;
int c=0;
int cw=0;
int cp=0;
class Object
{   public:
        int id;
        int weight;
        int price;
        float d;
        float prob;
        static float total_d;
};
float Object::total_d = 0;
Object obj[21];
class MaxHeapQNode
{   public:
        MaxHeapQNode *parent;
        int lchild;
        int upprofit;
        int profit;
        int weight;
        int lev;
};
struct cmp{
    bool operator()(MaxHeapQNode *&a, MaxHeapQNode *&b) const{
        return a->upprofit < b->upprofit;
    }
};
bool compare(const Object &a, const Object &b){
    return a.d >= b.d;
}
void dataload(std::vector<int> &weight, std::vector<int> &value, int &c, int &num) {
    int i=1;
    ifstream file("./testdata/20test.txt");

    string temp,temp2;
    file>>num;file.get();
    file>>c;file.get();
    getline(file,temp);
    istringstream LineBand(temp);//转化成数据流
    weight.push_back(0);
    while(LineBand.fail() == false&&i<=num) //每一行以空格为间隔读数，该行读完则跳出循环
    {   
        LineBand>>temp2;
        weight.push_back(atof(temp2.data()));
        i++;
    }
    i=1;
    getline(file,temp);
    istringstream LineBand2(temp);//转化成数据流
    value.push_back(0);
    while(LineBand2.fail() == false&&i<=num) //每一行以空格为间隔读数，该行读完则跳出循环
    {   
        LineBand2>>temp2;
        value.push_back(atof(temp2.data()));
        i++;
    }
    file.close();
    for(int i = 1; i <= n; ++i){
        obj[i].price=value[i]; 
        obj[i].weight=weight[i];
        obj[i].id = i;
        obj[i].d = 1.0 * obj[i].price / obj[i].weight;
        obj[i].total_d+=obj[i].d;
        }
        sort(obj + 1, obj + n + 1, compare);
    return ;
}
int Bound(int i){
    int tmp_cleft = c - cw;
    int tmp_cp = cp;
    while(tmp_cleft >= obj[i].weight && i <= n){
        tmp_cleft -= obj[i].weight;
        tmp_cp += obj[i].price;
        i++;
    }
    if(i <= n){tmp_cp += tmp_cleft * obj[i].d;}
    return tmp_cp;
}
void AddAliveNode(priority_queue<MaxHeapQNode *, vector<MaxHeapQNode *>, cmp> &q, MaxHeapQNode *E, int up, int wt, int curp, int i, int ch){
    MaxHeapQNode *p = new MaxHeapQNode;
    p->parent = E;
    p->lchild = ch;
    p->weight = wt;
    p->upprofit = up;
    p->profit = curp;
    p->lev = i + 1;
    q.push(p);
}
void BFS_knapsack()
{
    priority_queue<MaxHeapQNode *, vector<MaxHeapQNode *>, cmp > q; // 大顶堆
    MaxHeapQNode *E = NULL;
    static int bestp=0;
    static int bestx[21];
    int i = 1;
    int up = Bound(1); //Bound(i)函数计算的是i还未处理时候的上限值
    while(i != n + 1){
        int wt = cw + obj[i].weight;
        if(wt <= c){
            if(bestp < cp + obj[i].price)
                bestp = cp + obj[i].price;
            AddAliveNode(q, E, up, cw + obj[i].weight, cp + obj[i].price, i, 1);
        }
        up = Bound(i + 1); //注意这里 up != up - obj[i].price而且 up >= up - obj[i].price
        if(up >= bestp) 
        {AddAliveNode(q, E, up, cw, cp, i, 0);} //注意这里必须是大于等于
        E = q.top();
        q.pop();
        cw = E->weight;
        cp = E->profit;
        up = E->upprofit;
        i = E->lev;
    }
    for(int j = n; j > 0; --j){
        bestx[obj[E->lev - 1].id] = E->lchild;
        E = E->parent;
    }
    /*-----------show the best policy-------------*/
    std::cout<<"best value: "<<bestp<<endl;
    std::cout<<"best policy: "<<endl;
    for(int i = 1; i <= n; ++i)
        std::cout<<bestx[i]<<' ';
    std::cout<<endl;
}
void MonteCarlo(){
    
    int bestv_prob[120]={0};
    int bestv =0 ;
    long random=32700;
    for(long epoch = 0;epoch<200000;epoch++){
        int x[21]={0};
        int cw =0;
        int cv=0;
        srand(random);
        for(int i=1; i<=n; i++){
            random = rand();
            x[i] = random%2;
            if(x[i]==1){cw+=weight[i];cv+=value[i];}
        }
        if(cw<=c&&bestv<cv) {bestv = cv;bestv_prob[cv]++;
        for(int i=1;i<=n;i++) std::cout<<x[i]<<" ";
        std::cout<<endl;
        }
    }
   
    std::cout<<"best value: "<<bestv<<endl;
    return ;
}
int main(){
    dataload(weight, value, c, n);
    BFS_knapsack(); std::cout<<"Monte Carlo:"<<endl;
    MonteCarlo();
    getchar();
}