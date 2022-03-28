#include <bits/stdc++.h>
using namespace std;

vector<long> findSum(vector<int> numbers, vector<vector<int>> queries){
    int qlen = queries.size();
    vector<long> temp(qlen);
    for(int i=0;i<qlen;i++){
        long sum = 0;
        bool ok = true;
        int start = queries[i][0];
        int end = queries[i][1];
        int inc = queries[i][2];
        for(int j=start;j<=end;j++){
            sum += numbers[j-1];
            if(ok){
              if(numbers[j-1] == 0){
                  sum += inc;
                  ok = false;
              }
            }
        }
        temp[i] = sum;
    }
    return temp;
}

int main(){
    int n = 2;
    vector<int> numbers{-5,0};
    int q = 2;
    int ql = 3;
    vector<vector<int>> queries{{2,2,20},{1,2,10}};
    vector<long> result = findSum(numbers, queries);
    for(int i=0;i<result.size();i++){
        cout<<result[i]<<endl;
    }
}