#define INF (unsigned)!((int)0)
#define FUNCTION(name,operator) inline void name(int &current, int candidate) {!(current operator candidate) ? current = candidate : false;}
#define io(v) cin>>v
#define toStr(str) (string) "Result ="
#define sz(a) (int)a.size()
#define foreach(v, i) for (int i = 0; i < sz(v); ++i)
#include <iostream>
#include <vector>
using namespace std;

#if !defined toStr || !defined io || !defined FUNCTION || !defined INF
#error Missing preprocessor definitions
#endif 

FUNCTION(minimum, <)
FUNCTION(maximum, >)

int main(){
	int n; cin >> n;
	vector<int> v(n);
	foreach(v, i) {
		io(v)[i];
	}
	int mn = INF;
	int mx = -INF;
	foreach(v, i) {
		minimum(mn, v[i]);
		maximum(mx, v[i]);
	}
	int ans = mx - mn;
	cout << toStr(Result =) <<' '<< ans;
	return 0;

}
