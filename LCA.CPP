#include <bits/stdc++.h> 
using namespace std;

class RootedTree { // Input must be a tree
    public:
    int n;
    vector<set<int>> edges;
    vector<int> succ;
    vector<int> depth;
    vector<vector<int>> preproc;
    
    const int MAXDEPTH = 32;
    
    RootedTree() {
        n = 0;
    }
    
    RootedTree(vector<int>& v, int src = 0) {
        n = v.size();
        edges = vector<set<int>>(n, set<int>());
        succ = vector<int>(n, -1);
        depth = vector<int>(n, -1);
        preproc = vector<vector<int>>(n, vector<int>(MAXDEPTH));
        
        for (int i = 0; i < n; i++) {
            if (v[i] < 0) continue;
            edges[i].insert(v[i]);
            edges[v[i]].insert(i);
        }
        
        calc(src);
        solve();
    }
    
    RootedTree(int k, vector<pair<int, int>>& ex, int src = 0) {
        n = k;
        edges = vector<set<int>>(n, set<int>());
        for (auto i : ex) {
            edges[i.first].insert(i.second);
            edges[i.second].insert(i.first);
        }
        succ = vector<int>(n, -1);
        depth = vector<int>(n, -1);
        preproc = vector<vector<int>>(n, vector<int>(MAXDEPTH));
        
        calc(src);
        solve();
    }
    
    void calc(int src = 0) { // Determine parent and depth of each node assuming rooted at src
        succ[src] = -1;
        depth[src] = 0;
        queue<int> q;
        q.push(src);
        while (q.size() > 0) {
            int now = q.front();
            q.pop();
            for (auto i : edges[now]) {
                if (depth[i] >= 0) continue;
                depth[i] = 1 + depth[now];
                succ[i] = now;
                q.push(i);
            }
        }
    }
    
    void solve() { // Determine binary ancestors
        for (int i = 0; i < n; i++) preproc[i][0] = succ[i];
        for (int i = 1; i < MAXDEPTH; i++) {
            for (int j = 0; j < n; j++) {
                if (preproc[j][i - 1] < 0) preproc[j][i] = -1;
                else preproc[j][i] = preproc[preproc[j][i - 1]][i - 1];
            }
        }
    }
    
    int move(int src, int x) {
        if (x <= 0) return src;
        if (x == 1) return succ[src];
        int res = src;
        for (int i = 0; i < MAXDEPTH; i++) {
            if (res < 0) return res;
            if (x & (1<<i)) res = preproc[res][i];
        }
        return res;
    }
    
    int lca(int x1, int x2) {
        int d1 = depth[x1];
        int d2 = depth[x2];
        int dx = abs(d1 - d2);
        if (d1 > d2) x1 = move(x1, dx);
        if (d2 > d1) x2 = move(x2, dx);
        
        if (x1 == x2) return x1;
        
        for (int i = MAXDEPTH - 1; i >= 0; i--) {
            int a1 = preproc[x1][i];
            int a2 = preproc[x2][i];
            if (a1 != a2) {
                x1 = a1;
                x2 = a2;
            }
        }
        return succ[x1];
    }
};

int main() // https://cses.fi/problemset/task/1688/
{
    int n, q, a, b;
    cin >> n >> q;
    
    vector<int> suc(n, -1);
    for (int i = 0; i < n - 1; i++) {
        cin >> suc[i + 1];
        suc[i + 1]--;
    }
    
    RootedTree tree(suc);
   // for (int i = 0; i < n; i++) cout << i << " = " << tree.succ[i] << " " << tree.depth[i] << endl;
    
    for (int i = 0; i < q; i++) {
        cin >> a >> b;
        a--;
        b--;
        cout << tree.lca(a, b) + 1 << endl;
    }
    
    return 0;
}
