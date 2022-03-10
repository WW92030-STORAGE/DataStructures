#include <bits/stdc++.h> 
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define ll long long
#define ld long double
#define pii pair<ld, ld>
#define pb push_back

ld intersection(pii a, pii b) {
    if (a.first == b.first) return LLONG_MAX;
    // ax + b = cx + d
    // (a - c)x = (d - b)
    return (ld)(b.second - a.second) / (a.first - b.first);
}

ld eval(pii a, ld b) {
    return b * a.first + a.second;
}

vector<pii> gethull(vector<pii> vec) {
    vector<pii> res;
    int n = vec.size();
    sort(vec.begin(), vec.end());
    
    vector<pii> lines;
    for (int i = 0; i < n; i++) {
        while (lines.size() > 0 && lines[lines.size() - 1].first == vec[i].first) lines.pop_back();
        lines.pb(vec[i]);
    }
    n = lines.size();
    
    for (int i = n - 1; i >= 0; i--) {
        while (res.size() > 1) {
            int n2 = res.size();
            double i1 = intersection(lines[i], res[n2 - 1]);
            double i2 = intersection(res[n2 - 1], res[n2 - 2]);
            if (i1 > i2) res.pop_back();
            else break;
        }
        res.pb(lines[i]);
//        cout << "\n" << i << endl;
//        for (auto i : res) cout << i.first << " " << i.second << endl;
    }
    sort(res.begin(), res.end());
    return res;
}

int solve(vector<pii> hull, ld x) {
    int n = hull.size();
    if (x <= intersection(hull[0], hull[1])) return 0;
    if (x >= intersection(hull[n - 1], hull[n - 2])) return n - 1;
    int low = 1;
    int high = n - 2;
    while (low + 10 < high) {
        int mid = (low + high) / 2;
        ld left = intersection(hull[mid], hull[mid - 1]);
        ld right = intersection(hull[mid], hull[mid + 1]);
        bool deficit = x < left;
        bool excess = x > right;
        if (!deficit && !excess) return mid;
        if (deficit) high = mid;
        else low = mid;
    }
    
    for (int i = max(1, low - 10); i <= min(n - 2, high + 10); i++) {
        ld left = intersection(hull[i], hull[i - 1]);
        ld right = intersection(hull[i], hull[i + 1]);
        if (x >= left && x <= right) return i;
    }
    return -1;
}

// Example using this example - https://codeforces.com/blog/entry/63823
int main()
{
	pii a = {2, 4};
	pii b = {1, 2};
	
	vector<pii> lines;
	lines.pb({-2, 1});
	lines.pb({-1, 3});
	// lines.pb({00, 2});
	// lines.pb({1, -2});
	// lines.pb({1, -4});
	// lines.pb({-6, 12});
	
	vector<pii> ch = gethull(lines);
	
	for (auto i : ch) cout << i.first << " " << i.second << endl;
	
	for (int i = 0; i < (int)ch.size() - 1; i++) {
	    cout << intersection(ch[i], ch[i + 1]) << endl;
	}
	
	for (double i = -5.0; i < 5.0; i += 0.5) {
	    int res = solve(ch, i);
	    cout << i << " " << res << " " << eval(ch[res], i) << endl;
	}
	
   	return 0;
}

