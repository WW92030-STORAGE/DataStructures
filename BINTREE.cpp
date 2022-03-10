#include <bits/stdc++.h> 
using namespace std;
#define ll long
#define pii pair<ll, ll>
#define pb push_back

class bintree {
    public:
    ll n;
    vector<ll> arr;
    vector<ll> tree;
    
    ll psum(ll index) {
        if (index < 0) return 0;
        ll res = 0;
        index++;
        
        while (index > 0) {
            res += tree[index];
            index -= (index & (-1 * index));
        }
        return res;
    }
    
    ll query(ll a, ll b) {
        return psum(b) - psum(a - 1);
    }
    
    ll change(ll i, ll x) {
        arr[i] += x;
        i++;
        while (i <= n) {
    	    tree[i] += x;
            i += (i & (-1 * i));
    	}
    	return x;
    }
    
    ll update(ll i, ll x) {
    	ll prev = arr[i];
    	change(i, x - prev);
    	return x;
    }
    
    bintree(ll ray[], ll x) {
        n = x;
        for (ll i = 0; i < n; i++) arr.pb(0);
        for (ll i = 0; i < n + 1; i++) tree.pb(0);
        for (ll i = 0; i < n; i++) change(i, ray[i]);
    }
    
    void print() {
        cout << "[" << arr[0];
        for (ll i = 1; i < n; i++) cout << " " << arr[i];
        cout << "] = ";
        
        cout << "[" << tree[0];
        for (ll i = 1; i <= n; i++) cout << " " << tree[i];
        cout << "]";
    }
    
    void println() {
        print();
        cout << endl;
    }
};

int main() // https://cses.fi/problemset/task/1648
{
    ll n, q;
    cin >> n >> q;
    ll arr[n];
    for (ll i = 0; i < n; i++) cin >> arr[i];
    
    bintree tree(arr, n);
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
