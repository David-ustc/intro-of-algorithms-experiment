#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream> 
#include <iostream>
#include <string>
//穷举法、自顶向下的备忘录法、动态规划算法、回溯法来求解0-1背包问题
using namespace std;
std::vector<int> weight;
std::vector<int> value;
int capacity=0;
int num = 0;

void dataload(std::vector<int> &weight, std::vector<int> &value, int &c, int &num) {
    int i=1;
    ifstream file("./testdata/15test.txt");

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
    return ;
}

int DP(std::vector<int>w, std::vector<int>v, int c, int n) {
    static int m[21][51];
    int jMax = min(w[n]-1, c);
    for(int j=0;j<=jMax;j++) m[n][j]=0;
    for(int j=w[n];j<=c;j++) m[n][j]=v[n];

    for(int i=n-1; i>1;i--){
        jMax=min(w[i]-1, c);
        for(int j=0;j<=jMax;j++) m[i][j]=m[i+1][j];
        for(int j=w[i];j<=c;j++) m[i][j]=max(m[i+1][j], m[i+1][j-w[i]]+v[i]);
    }
    if(c>=w[1]) m[1][c]=max(m[2][c], m[2][c-w[1]]+v[1]);
    else m[1][c]=m[2][c];
    return m[1][c];
}
int memorandom(int i, int current_c){
    static int m[21][51];
    //if(m[i][current_c]) return m[i][current_c];
    int result=0;
    if(i>=num+1) return result;
    if(weight[i]>current_c) m[i+1][current_c]=memorandom(i+1, current_c);
    else result = max(memorandom(i+1, current_c), memorandom(i+1, current_c-weight[i])+value[i]);
    m[i][current_c]=result;
    return  result;
}

int Track(int i){
    static int cv=0;
    static int cw=0;
    static int bestv=0;
    static int x[21];
    if(i>num) {
        if(cv>bestv) std::cout<<"tracking current best policy:"<<x<<" bestv:"<<cv<<endl;
        bestv=max(cv, bestv);
        return cv;}
    else{
        if(cw+weight[i]<=capacity){
            x[i]=1;cw+=weight[i];cv+=value[i];
            Track(i+1);
            cw-=weight[i];cv-=value[i];
        }
        //cw+0<=capacity
        x[i]=0;
        Track(i+1);
    }
}
int BruteForce(int i){
    static int cv=0;
    static int cw=0;
    static int bestv=0;
    static int x[21];
    if(i>num) {
        if(cw<=capacity&&cv>bestv) 
        {std::cout<<"BruteForce current best policy:"<<x<<" bestv:"<<cv<<endl;
        bestv=max(cv, bestv);}
    }
    else{
        x[i]=1;cw+=weight[i];cv+=value[i];
        BruteForce(i+1);
        //cw+0<=capacity
        x[i]=0;cw-=weight[i];cv-=value[i];
        BruteForce(i+1);
    }
}
int main(){
    dataload(weight, value, capacity, num);
    std::cout<<"Dynamic Programming:"<<endl;
    std::cout<<DP(weight, value, capacity, num)<<endl;
    std::cout<<"Memorandom:"<<endl;
    std::cout<<memorandom(1, capacity)<<endl;
    std::cout<<"Tracking:"<<endl;
    Track(1);
    std::cout<<"Brute Force:"<<endl;
    BruteForce(1);
    getchar();
}