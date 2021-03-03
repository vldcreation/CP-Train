#include <bits/stdc++.h>
using namespace std;

void solve(){
	int n;
	int a,tmp=1,tmp2=0;
	cin>>a>>n;
	int i=1;
	do{
		for(int j=1;j<=a;++j){
			cout<<tmp;
			if(j<a)
				cout<<' ';
			tmp++;
			tmp2++;
		}
		tmp -= tmp2;
		tmp += a;
		tmp2 = 0;
		cout<<'\n';
		i++;
	}while(tmp<=n);
}

int main(){
	solve();
}
