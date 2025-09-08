#include <cstdint>
#include <vector>
#include <string>

class RollingHash {
    public:
    // both values are ideally prime. mod must be prime.
    int64_t base = 2017;
    int64_t mod = 1000000007;

    RollingHash(int64_t b = 2017, int64_t m = 1000000007) {
        base = b;
        mod = m;
    }

    inline int64_t rem(int64_t a, int64_t m) {
        a %= m;
        while (a < 0) a += m;
        return a % m;
    }

    int64_t modexp(int64_t e) {
        if (e <= 0) return 1;
        if (e & 1) {
            int64_t res = base * modexp(e - 1);
            return rem(res, mod);
        }

        int64_t res = modexp(e>>1);
        res = (res * res) % mod;
        return rem(res, mod);
    }

    std::vector<int64_t> rh(std::string s, int k) {
        k = std::min(k, (int)(s.size()));
        int64_t greatest = modexp(k - 1); // highest power of the base

        std::vector<int64_t >res;
        int64_t rolling = 0;
        for (int i = 0; i < k; i++) {
            rolling = rolling * base + s[i];
            rolling = rem(rolling, mod);
        }

        res.push_back(rolling);
        for (int i = k; i < s.size(); i++) {
            rolling -= rem(greatest * s[i - k], mod);
            rolling = rem(rolling, mod);
            rolling = rolling * base + s[i];
            rolling = rem(rolling, mod);
            res.push_back(rolling);
        }
        return res;
    }
};

#include <iostream>
using namespace std;
int main() {
    string s = "protogenxxprotobeanprotogenprotoproto";
    
    RollingHash rh;
    
    vector<int64_t> v = rh.rh(s, 5);
    for (auto i : v) cout << i << " ";
}
