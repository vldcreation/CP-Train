#include <bits/stdc++.h>
using namespace std;


int maxCost(vector<int> cost, vector<string> labels, int dailyCount){
    int maxLegalCost = 0;
    int tempCost = 0;
    int legalCount = 0;
    for(int i=0;i<labels.size();i++){
        tempCost += cost[i];
        if(labels[i] == "legal"){
            legalCount++;
            if(legalCount == dailyCount){
                maxLegalCost = max(maxLegalCost, tempCost);
                legalCount = 0;
                tempCost = 0;
            }
        }
    }
    return maxLegalCost;
}

int main(){
    vector<int> cost = {0,3,2,3,4};
    vector<string> labels = {"legal", "legal", "illegal", "legal", "legal"};
    int dailyCount = 1;
    cout<<maxCost(cost, labels,1)<<endl;
}