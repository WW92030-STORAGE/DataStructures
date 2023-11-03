#include <bits/stdc++.h> 
using namespace std;
#define pb push_back
#define ll int64_t
#define pii pair<ll, ll>

class DSU {
    public:
    vector<int> parent;
    vector<int> size;
    int n;
    DSU(int x) {
        n = x;
        parent = vector<int>(n, 0);
        size = vector<int>(n, 1);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int root(int x) {
        if (x < 0 || x >= n) return -1;
        while (x != parent[x]) x = parent[x];
        return x;
    }
    
    int card(int x) {
        if (x < 0 || x >= n) return -1;
        while (x != parent[x]) x = parent[x];
        return size[x];
    }
    
    bool same(int a, int b) {
        if (a < 0 || a >= n) return false;
        if (b < 0 || b >= n) return false;
        return root(a) == root(b);
    }
    
    void unite(int a, int b) {
        if (a < 0 || a >= n) return;
        if (b < 0 || b >= n) return;
        
        a = root(a);
        b = root(b);
        if (size[a] < size[b]) swap(a, b);
        size[a] += size[b];
        parent[b] = a;
    }
};

// https://www.cses.fi/problemset/task/1676/

int main()
{
    int n, m;
    cin >> n >> m;
    DSU dsu(n);
    
    int a, b;
    
    int maxsize = 1;
    int numcomps = n;
    
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        a--;
        b--;
        
        if (!dsu.same(a, b)) {
            dsu.unite(a, b);
            numcomps--;
            maxsize = max(maxsize, max(dsu.card(a), dsu.card(b)));
        }
        
        cout << numcomps << " " << maxsize << "\n";
    }
    
    return 0;
}
