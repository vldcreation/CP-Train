#include <bits/stdc++.h>
using namespace std;

const string her = "HeR";
void solve(){
	// ubah char h atau e atau r (Upper dan Lower) menjadi huruf di string her
	string str;
	cin>>str;
	int idx = 0;
	for(int i=0;i<str.size();++i){
		if(str[i]=='h'||str[i]=='H'||str[i]=='e'||str[i]=='E'||str[i]=='r'||str[i]=='R'){
			str[i]=her[idx];
			++idx;
			if(idx==2)
				idx=0;
		}
	}
	cout<<str<<'\n';
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int N;
	cin>>N;
//	cout<<"\n\nOutput\n\n";
	for(int i=0;i<N;++i)
		solve();
	return 0;
}
