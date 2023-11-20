#include <bits/stdc++.h> 
using namespace std;
#define pb push_back
#define ll int64_t
#define pii pair<ll, ll>

class StringHash { // Calculates hashes on strings for a fixed radix and modulus
    public:
    int64_t mod;
    int64_t radix;
    unordered_map<int64_t, int64_t> powers;
    
    StringHash() {
        mod = 1000000007;
        radix = 2017;
        powers.insert({0, 1});
    }
    
    StringHash(int r) {
        mod = 1000000007;
        radix = r;
        powers.insert({0, 1});
    }
    
    StringHash(int r, int m) {
        mod = m;
        radix = r;
        powers.insert({0, 1});
    }
    
    int64_t pow(int e) {
        if (e < 0) return 1;
        if (powers.find(e) != powers.end()) return powers.at(e);
        if (e & 1) {
            int64_t res = pow(e - 1);
            res = (res * radix) % mod;
            powers.insert({e, res});
            return res;
        }
        
        int64_t res = pow(e>>1);
        res = (res * res) % mod;
        powers.insert({e, res});
        return res;
    }
    
    vector<int64_t> phash(string s) {
        int n = s.length();
        vector<int64_t> res(n, 0);
        res[0] = (int)(s[0]);
        for (int i = 1; i < n; i++) {
            res[i] = (radix * res[i - 1] + s[i]) % mod;
        }
        return res;
    }
    
    int64_t subhash(vector<int64_t>& v, int a, int b) { // S[a ... b]
        if (a == 0) return v[b];
        int64_t res = v[b];
        int64_t subscale = v[a - 1] * pow(b - a + 1);
        subscale %= mod;
        res -= subscale;
        while(res < 0) res += mod;
        return res % mod;
    }
};



int main() // https://www.cses.fi/problemset/task/1753/
{
    StringHash h1; // Construct the radix hashing system using two different radices
    StringHash h2(62);
    
    string s, p;
    cin >> s >> p;
    
    int sn = s.length();
    int pn = p.length();
    
    auto s1 = h1.phash(s);
    auto s2 = h2.phash(s);
    
    auto p1 = h1.phash(p);
    auto p2 = h2.phash(p);
    
    int res = 0;
    
    for (int i = 0; i <= sn - pn; i++) {
        if (p1[pn - 1] != h1.subhash(s1, i, i + pn - 1)) continue;
        if (p2[pn - 1] != h2.subhash(s2, i, i + pn - 1)) continue;
        res++;
    }
    
    cout << res << endl;
    
    return 0;
}
