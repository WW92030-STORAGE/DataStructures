#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ll long long
#define pii pair<ll, ll>
#define pb push_back

vector<ll> protogen(string s) { // creates the z array
    ll n = s.length();
    vector<ll> res;
    for (ll i = 0; i < n; i++) res.pb(0);
    res[0] = -1;
    ll x = 0;
    ll y = 0;
    
    for (ll i = 1; i < n; i++) {
        res[i] = max(0LL, min(res[i - x], y - i + 1));
        while (i + res[i] < n && s[res[i]] == s[res[i] + i]) {
            x = i;
            y = i + res[i];
            res[i]++;
        }
    }
    
    return res;
}

vector<ll> search(string s, string p) { // searches for all instances of string p inside string s
    ll n = s.length();
    string spectrum = p + "#" + s; // This symbol should be in neither s nor p
    vector<ll> z = protogen(spectrum);
    vector<ll> res;
    for (ll i = 0; i < n; i++) {
        if (z[i + p.length() + 1] == p.length()) res.pb(i);
    }
    return res;
}

int main()
{
	string s;
	cin >> s;
	vector<ll> z = protogen(s);
	
	ll n = s.length();
//	for (ll i = 0; i < n; i++) cout << z[i] << " ";
	
	string h;
	cin >> h;
	for (auto i : search(s, h)) cout << i << " ";
	cout << endl;
	
   	return 0;
}
