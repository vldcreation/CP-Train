#include <bits/stdc++.h>
using namespace std;


void solve(){
	int N;
	cin>>N;
	while(N--){
		int n, x;
	    cin >> n >> x;
	    vector<string>bocil(n);
	    vector <int> a(n);
	    for (int i = 0; i < n; i++) cin >> a[i];
	    for (int i = 0; i < n; i++) cin >> bocil[i];
	    vector <pair <int, string> > q;
	    for (int i = 0; i < n; i++) {
	      q.push_back({(a[i] + x - 1) / x, bocil[i]});
	    }
	    sort(q.begin(), q.end());
	    vector <string> ord(n);
	    for (int i = 0; i < n; i++) ord[i] = q[i].second;
	    for (string t : ord) cout << t << ' ';
	    cout<<'\n';
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	solve();
	return 0;
}
