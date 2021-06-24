#include<bits/stdc++.h>
using namespace std;

bool checkJD(string s1, string s2){
	int ck=0;
	for(int i=0;i<s1.length();++i){
		if(s2.find(s1[i])!=string::npos)
			ck++;
	}
	if(ck)
		return 1;
	return 0;
}

int LCA(string s1, string s2){

	int n1 = s1.length();
	int n2 = s2.length();
	int dp[n1+1][n2+1];

	for(int i=0;i<=n1;i++){
		for(int j=0;j<=n2;j++){
			if(i == 0 or j == 0) dp[i][j] = 0;
			else if(s1[i-1] == s2[j-1]) dp[i][j] = 1 + dp[i-1][j-1];
			else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
		}
	}
	return dp[n1][n2];
}

int minCost(string s1, string s2, int cost1, int cost2){

	int n1 = s1.length();
	int n2 = s2.length();

	int lca = LCA(s1,s2);

	int min_cost = cost1 * (n1 - lca) + cost2 * (n2 - lca);

	return min_cost; 
}

int main(){

	string s1, s2;

	int cost1, cost2;
	int N;
	cin>>N;
	cin>>cost1>>cost2;
	for(int i = 0;i<N;++i){
		cin>>s1>>s2;
		if(checkJD(s1,s2)){
			cout << minCost(s1,s2,cost1,cost2)<<'\n';
		}
		else
			cout<<"Dia bukan jodoh Anda :')\n";
	}

	return 0;
}
