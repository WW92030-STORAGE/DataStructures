#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ll int
#define pii pair<ll, ll>
#define pb push_back

// https://cses.fi/problemset/task/1135

class segtree {
    public:
    ll n;
    ll size;
    vector<ll> arr;
    vector<ll> tree;
    
    ll ceillog(ll x) {
        ll count = 0;
	    ll p = 1;
	    while (p * 2 <= x) {
		    p *= 2;
		    count++;
	    }
	    if (p == x) return p;
	    return p * 2;
    }

    ll combine(ll a, ll b) { // example combine function (must satisfy commutative + associative + identity)
        return min(a, b);
    }
    
    segtree(vector<ll> ray, ll x) {
        n = x;
        size = ceillog(n);
        for (ll i = 0; i < size; i++) arr.pb(0);
        for (ll i = 0; i < 2 * size; i++) tree.pb(0);
        
        for (int i = 0; i < n; i++) arr[i] = ray[i];
		for (int i = size; i < 2 * size; i++) tree[i] = arr[i - size]; // range length 1
		
		for (int i = size - 1; i > 0; i--) {
		    tree[i] = combine(tree[2 * i], tree[2 * i + 1]);
		}
    }
    
    ll query(ll a, ll b) {
        if (a > b) swap(a, b);
        a += size;
        b += size;
        
        ll res = INT_MAX; // initial value
        while (a <= b) {
            if (a % 2 == 1) res = combine(res, tree[a++]);
            if (b % 2 == 0) res = combine(res, tree[b--]);
            a /= 2;
            b /= 2;
        }
        
        return res;
    }
    
    ll change(ll i, ll x) {
        arr[i] += x;
        i += size;
        tree[i] += x;
        for (int k = i / 2; k >= 1; k /= 2) tree[k] = combine(tree[2 * k], tree[2 * k + 1]);
	    
    	return x;
    }
    
    ll update(ll i, ll x) {
        ll prev = arr[i];
        change(i, x - prev);
        return x;
    }
    
    void print() {
        cout << "[" << arr[0];
        for (ll i = 1; i < size; i++) cout << " " << arr[i];
        cout << "] = ";
        
        cout << "[" << tree[0];
        for (ll i = 1; i < 2 * size; i++) cout << " " << tree[i];
        cout << "]";
    }
    
    void println() {
        print();
        cout << endl;
    }
};

// The actual program.

vector<vector<ll>> edges;
vector<ll> dfs;
vector<ll> depth;
vector<ll> nodeindices;
vector<ll> dfsdepths;

vector<bool> vis;

void preprocess(ll src) {
    vis[src] = true;
    dfs.pb(src);
    for (auto i : edges[src]) {
        if (vis[i]) continue;
        if (depth[i] < 0) depth[i] = depth[src] + 1;
        preprocess(i);
        dfs.pb(src);
    }
}

void disp(vector<ll> v) {
    cout << "[ ";
    for (auto i : v) cout << i << " ";
    cout << "]\n";
}

int main()
{
    ll n, q;
    cin >> n >> q;
    for (ll i = 0; i < n; i++) edges.pb(vector<int>());
    
    depth = vector<ll>(n, -1);
    nodeindices = vector<ll>(n, 0);
    vis = vector<bool>(n, false);
    
    int a, b;
    for (int i = 0; i < n - 1; i++) {
        cin >> a >> b;
        a--;
        b--;
        edges[a].pb(b);
        edges[b].pb(a);
    }
    
    depth[0] = 0;
    
    preprocess(0);
    for (auto i : dfs) dfsdepths.pb(depth[i]);
    for (int i = 0; i < dfs.size(); i++) nodeindices[dfs[i]] = i;
    
    // disp(depth);
    // disp(dfs);
    // disp(dfsdepths);
    // disp(nodeindices);
    
    segtree tree(dfsdepths, dfsdepths.size());
    // disp(tree.tree);
    
    for (int i = 0; i < q; i++) {
        cin >> a >> b;
        a--;
        b--;
        int lca = tree.query(nodeindices[a], nodeindices[b]);
        // cout << "|" << a << " " << b << " " << nodeindices[a] << " " << nodeindices[b] << " " << lca << "|\n";
        cout << depth[a] + depth[b] - 2 * lca << endl;
    }
    
    return 0;
}


/*

1 --- 2
|
|
|
3 --- 4
|
|
|
5

*/



