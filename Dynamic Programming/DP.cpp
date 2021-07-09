#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

using namespace std;

vector<long> dataload() {
    std::vector<long> csvData;

    long i,datalen=0;
    long price=0;
    ifstream file("./prices.txt");
    while( ! file.eof() ){
        file>>price;
        file.get();
        csvData.push_back(price);
        datalen++;
    }
    file.close();
    std::cout<<datalen<<endl;
    return csvData;
}

long maxProfit(vector<long>& prices, long fee) {
    long n = prices.size();
    if(!n) return 0;

    vector<long>dp0(n, 0);
    vector<long>dp1(n, 0);
    dp1[0] = - prices[0]; // i = 0
    for(long i = 1; i < n; i++){
         dp0[i] = max(dp0[i-1], dp1[i-1] + prices[i] - fee); 
         dp1[i] = max(dp1[i-1], dp0[i-1] - prices[i]);
     }
     return dp0[n-1];
    /**
    long dp0 = 0, dp1 = - prices[0];
    for(long i = 1; i < n; i++){
        long pre_dp0 = dp0;
        dp0 = max(dp0, dp1 + prices[i] - fee); 
        dp1 = max(dp1, pre_dp0 - prices[i]);
    }
    return dp0;
    **/
}
long greedy_maxProfit(vector<long>& prices, long fee) {
        long result = 0;
        long minPrice = prices[0]; // 记录最低价格
        for (long i = 1; i < prices.size(); i++) {
            // 买入
            if (prices[i] < minPrice) minPrice = prices[i]; 

            // 因为价格还在涨，但是涨的又不多，不够一次手续费，不卖；或者是没有净利润，不需要算进总收益中
            if (prices[i] >= minPrice && prices[i] <= minPrice + fee) {
                continue;
            }

            if (prices[i] > minPrice + fee) {
                result += prices[i] - minPrice - fee; // 加上今天卖出比之前某一天卖出多的利润
                minPrice = prices[i] - fee; 
            }
        }
        return result;
}

int main(){
    vector<long> prices = dataload();
    long fee = 260;
    std::cin>>fee;
    getchar();
    std::cout<<maxProfit(prices, fee)<<endl;
    std::cout<<greedy_maxProfit(prices, fee)<<endl;
    getchar();
}