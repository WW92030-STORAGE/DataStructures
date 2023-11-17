#include <bits/stdc++.h> 
using namespace std;
#define ll int64_t
#define pii pair<ll, ll>
#define pb push_back

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
        if (a < b) return a;
        return b;
    }
    
    segtree(ll ray[], ll x) {
        n = x;
        size = ceillog(n);
        for (ll i = 0; i < size; i++) arr.pb(0);
        for (ll i = 0; i < 2 * size; i++) tree.pb(0);
        
        for (int i = 0; i < n; i++) arr[i] = ray[i];
	for (int i = 0; i < n; i++) change(i, arr[i]);
    }
    
    ll query(ll a, ll b) {
        a += size;
        b += size;
        
        ll res = LLONG_MAX; // initial value
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

int main() // https://cses.fi/problemset/task/1649/
{
    ll n, q;
    cin >> n >> q;
    ll arr[n];
    for (ll i = 0; i < n; i++) cin >> arr[i];
    
    segtree tree(arr, n);
//    tree.println();
    
    ll c, a, b;
    for (ll i = 0; i < q; i++) {
        cin >> c >> a >> b;
        if (c == 1) {
            tree.update(a - 1, b);
//            tree.println();
        }
        else cout << tree.query(a - 1, b - 1) << endl;
    }
    
    return 0;
}
