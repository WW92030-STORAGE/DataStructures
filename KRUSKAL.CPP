#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define pii pair<ll, ll>
#define pb push_back

#define ll long long
#define xii tuple<ll, ll, ll>

xii mx(int a, int b, int c) {
    return make_tuple(a, b, c);
}

ll val(xii a, int b) {
    if (b == 0) return get<0>(a);
    if (b == 1) return get<1>(a);
    if (b == 2) return get<2>(a);
    return get<0>(a);
}

ll search(ll parent[], ll x) {
    while (x != parent[x]) x = parent[x];
    return x;
}

vector<xii> kruskal(vector<xii> graph, ll n) { // weight first second
    ll parent[n];
    ll size[n];
    for (ll i = 0; i < n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
    sort(graph.begin(), graph.end());
    vector<xii> res;
    for (auto i : graph) {
        ll first = val(i, 1);
        ll second = val(i, 2);
        if (search(parent, first) == search(parent, second)) continue;
        ll a = search(parent, first);
        ll b = search(parent, second);
        if (size[a] < size[b]) swap(a, b);
        size[a] += b;
        parent[b] = a;
        res.push_back(i);
    }
    return res;
}

int main()
{
    ll n, m;
    cin >> n >> m;
    vector<xii> graph;
    ll a, b, c;
    for (ll i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graph.push_back(mx(c, a - 1, b - 1));
    }
    
    // for (auto i : graph) cout << val(i, 0) << " : " << val(i, 1) << " " << val(i, 2) << endl;
    // cout << endl;
    
    vector<xii> res = kruskal(graph, n);
    if (res.size() < n - 1) {
        cout << "IMPOSSIBLE" << endl;
        return 0;
    }
    ll sum = 0;
    for (auto i : res) {
        // cout << val(i, 0) << " : " << val(i, 1) << " " << val(i, 2) << endl;
        sum += val(i, 0);
    }
    cout << sum << endl;

    return 0;
}

