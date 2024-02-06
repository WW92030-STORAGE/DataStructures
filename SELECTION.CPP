#include <vector>
#include <iostream>

int select(std::vector<int> v, int k) { // not space efficient
    if (k <= 0 || k > v.size()) return 0;
    std::vector<int> v1;
    std::vector<int> v2;
    int pivindex = rand() % v.size();
    int pivot = v[pivindex];
    for (int i = 0; i < v.size(); i++) {
        if (i == pivindex) continue;
        if (v[i] < pivot) v1.push_back(v[i]);
        else v2.push_back(v[i]);
    }
    
    if (v1.size() == k - 1) return pivot;
    if (k <= v1.size()) return select(v1, k);
    return select(v2, k - v1.size() - 1);
}

int main()
{
    std::vector<int> v;
    int sz = 10000019;
    for (int i = 0; i < sz; i++) v.push_back((i * 193 + 138) % sz);
    std::cout << select(v, 999999);

    return 0;
}
