#include <complex>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::complex<double>> uroots(int n) {
    if (n <= 0) return std::vector<std::complex<double>>();
    std::vector<std::complex<double>> roots(n);
    for (int i = 0; i < n; i++) {
        double theta = -2 * M_PI * i / n;
        roots[i] = std::complex<double>(cos(theta), sin(theta));
    }
    return roots;
}

// The purpose of this function is to evaluate the degree n polynomial (v) at the n roots of unity.
// The resultant vector is the result of evaluating the polynomial at the points
// Specifically res[x] = P(exp(i * x * 2pi / n))

std::vector<std::complex<double>> FFTUtil(std::vector<std::complex<double>>& v) { // v must have power of 2 length
    int n = v.size();
    if (n <= 0) return std::vector<std::complex<double>>();
    if (n == 1) return std::vector<std::complex<double>>(1, v[0]);
    
    
    std::vector<std::complex<double>> roots = uroots(n); // collect nth roots of unity
    
    std::vector<std::complex<double>> evens(n>>1);
    std::vector<std::complex<double>> odds(n>>1);
    for (int i = 0; i < n>>1; i++) {
        evens[i] = v[i<<1];
        odds[i] = v[(i<<1) + 1];
    }
    
    std::vector<std::complex<double>> eres = FFTUtil(evens);
    std::vector<std::complex<double>> ores = FFTUtil(odds);
    
    std::vector<std::complex<double>> res(n);
    for (int i = 0; i < n>>1; i++) {
        res[i] = eres[i] + roots[i] * ores[i];
        res[i + (n>>1)] = eres[i] - roots[i] * ores[i];
    }
    
    return res;
}

// The purpose of this function is to recover the original polynomial v = v[0] + v[1]x + v[2]x^2 + ... 
// From the FFT res[x] = P(root[x]) (root[x] = exp(i * x * 2pi / n))
// It can be realized that if v is the polynomial we seek
// then v[x] = (1 / n) res(root[n - x])
// We can simply call FFT on the FFT vector again and manipulate the resulting values into the coefficient array
// It's not as simple as a reverse and pointwise divide
// This function takes in an FFT vector of power of 2 size

std::vector<std::complex<double>> invFFTUtil(std::vector<std::complex<double>> v) {\
    int n = v.size();
    std::vector<std::complex<double>> huh = FFTUtil(v);
    
    std::vector<std::complex<double>> res(n);
    for (int i = 1; i < n; i++) res[n - i] = huh[i] * (1.0 / n);
    res[0] = huh[0] * (1.0 / n);
    return res;
}

std::vector<std::complex<double>> FFT(std::vector<std::complex<double>> v) {
    int n = v.size();
    int bound = 1;
    while (bound < n) bound = bound<<1;
    while (v.size() < bound) v.push_back(std::complex<double>(0, 0));
    return FFTUtil(v);
}

std::vector<std::complex<double>> convolve(std::vector<std::complex<double>> a, std::vector<std::complex<double>> b) {
    int na = a.size();
    int nb = b.size();
    int n = std::max(a.size(), b.size());
    while (a.size() < 2 * n) a.push_back(std::complex<double>(0, 0));
    while (b.size() < 2 * n) b.push_back(std::complex<double>(0, 0));
    
    std::vector<std::complex<double>> FA = FFT(a); // Furaffinity
    std::vector<std::complex<double>> FB = FFT(b);
    
    std::vector<std::complex<double>> FF(FA.size()); // FFT of the resultant vector
    for (int i = 0; i < FA.size(); i++) FF[i] = FA[i] * FB[i];
    std::vector<std::complex<double>> res = invFFTUtil(FF);
    while (res.size() > na + nb - 1) res.pop_back(); // don't need those padded 0s
    return res;
}

#include <bits/stdc++.h>
using namespace std;
typedef complex<double> cd;

void disp(vector<cd> v) {
    cout << "[";
    for (auto i : v) cout << i;
    cout << "]\n";
}

int main()
{
    vector<cd> a{1, 2, 1}; // All vectors are in complex numbers. Small errors might be present.
    disp(a);
    auto n = convolve(a, a); // Discrete convolution is equivalent to polynomial multiplication
    disp(n); // (1 + 2x + x^2)^2 = (1 + 4x + 6x^2 + 4x + 1)
    
    
    vector<cd> p1{5, 2, 3, 1, 3, 5, 6};
    vector<cd> p2{3, 1, 4, 1, 5};
    disp(convolve(p1, p2)); 
    // (5 + 2x + 3x^2 + 1x^3 + 3x^4 + 5x^5 + 6x^6)(3 + 1x + 4x^2 + 1x^3 + 5x^4)
    // = 15 + 11x + 31x^2 + 19x^3 + 49x^4 + 35x^5 + 51x^6 + 34x^7 + 44x^8 + 31x^9 + 30x^10
    
    return 0;
}
