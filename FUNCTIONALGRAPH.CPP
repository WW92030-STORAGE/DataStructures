#include <bits/stdc++.h> 
using namespace std;
#define pb push_back
#define ll int64_t
#define pii pair<ll, ll>

// LIFTING - Given a rooted tree (or successor graph) what is the Nth ancestor of some node?

class SuccGraph {
    public:
    int n;
    vector<int> succ;
    vector<vector<int>> preproc;
    
    const int MAXDEPTH = 32;
    
    SuccGraph(int k) {
        n = k;
        succ = vector<int>(n);
        for (int i = 0; i < n; i++) succ[i] = i;
        preproc = vector<vector<int>>(n, vector<int>(MAXDEPTH, -1));
    }
    
    SuccGraph(vector<int>& v) {
        n = v.size();
        for (int i = 0; i < n; i++) succ.push_back(v[i]);
        preproc = vector<vector<int>>(n, vector<int>(MAXDEPTH));
        
        solve();
    }
    
    void solve() {
        for (int i = 0; i < n; i++) preproc[i][0] = succ[i];
        for (int i = 1; i < MAXDEPTH; i++) {
            for (int j = 0; j < n; j++) preproc[j][i] = preproc[preproc[j][i - 1]][i - 1];
        }
    }
    
    int move(int src, int x) {
        if (x <= 0) return src;
        if (x == 1) return succ[src];
        int res = src;
        for (int i = 0; i < MAXDEPTH; i++) {
            if (x & (1<<i)) res = preproc[res][i];
        }
        return res;
    }
};

int main() // https://www.cses.fi/problemset/task/1750
{
    ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
    int n, q;
    cin >> n >> q;
    vector<int> v(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
        v[i]--;
    }
    SuccGraph succ(v);
    int s, x;
    
    for (int i = 0; i < q; i++) {
        cin >> s >> x;
        cout << 1 + succ.move(s - 1, x) << endl;
    }
    return 0;
}
