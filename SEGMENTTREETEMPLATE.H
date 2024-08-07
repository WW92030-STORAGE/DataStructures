#ifndef SEGMENT_TREE_TEMPLATE_H
#define SEGMENT_TREE_TEMPLATE_H

// SEGMENT TREE HEADER FILE AND TEMPLATE CLASS (C++)

#include <iostream>
#include <functional>
#include <vector>

template<typename T> class segtree {
    public:
    int n;
    int size;
    std::vector<T> arr;
    std::vector<T> tree;
    std::function<T(T, T)> op; // monoid function
    T identity; // identity element of the function
    
    int ceillog(int x);
    T combine(T a, T b);
    segtree(T ray[], int x, std::function<T(T, T)> func, T id);
    T query(int a, int b);
    T change(int i, T x);
    T update(int i, T x);
    void print();
    void println();
};
    
    template <typename T> int segtree<T>::ceillog(int x) {
        int count = 0;
	    int p = 1;
	    while (p * 2 <= x) {
		    p *= 2;
		    count++;
	    }
	    if (p == x) return p;
	    return p * 2;
    }
    
    template <typename T> T segtree<T>::combine(T a, T b) {
        return op(a, b);
    }
    
    template <typename T> segtree<T>::segtree(T ray[], int x, std::function<T(T, T)> func, T id) {
        op = func;
        identity = id;
        n = x;
        size = ceillog(n);
        
        for (int i = 0; i < size; i++) arr.push_back(0);
        for (int i = 0; i < 2 * size; i++) tree.push_back(0);
        
        for (int i = 0; i < n; i++) arr[i] = ray[i];
	for (int i = 0; i < n; i++) change(i, arr[i]);
    }
    
    template <typename T> T segtree<T>::query(int a, int b) {
        a += size;
        b += size;
        
        T res = identity; // initial value
        while (a <= b) {
            if (a % 2 == 1) res = combine(res, tree[a++]);
            if (b % 2 == 0) res = combine(res, tree[b--]);
            a /= 2;
            b /= 2;
        }
        
        return res;
    }
    
    template <typename T> T segtree<T>::change(int i, T x) {
        arr[i] = combine(arr[i], x);
        i += size;
        tree[i] = combine(tree[i], x);
        for (int k = i / 2; k >= 1; k /= 2) tree[k] = combine(tree[2 * k], tree[2 * k + 1]);
	    
    	return x;
    }
    
    template <typename T> T segtree<T>::update(int i, T x) {
        arr[i] = x;
        i += size;
        tree[i] = x;
        for (int k = i / 2; k >= 1; k /= 2) tree[k] = combine(tree[2 * k], tree[2 * k + 1]);
	    
    	return x;
    }
    
    template <typename T> void segtree<T>::print() {
        std::cout << "[" << arr[0];
        for (int i = 1; i < size; i++) std::cout << " " << arr[i];
        std::cout << "] = ";
        
        std::cout << "[" << tree[0];
        for (int i = 1; i < 2 * size; i++) std::cout << " " << tree[i];
        std::cout << "]";
    }
    
    template <typename T> void segtree<T>::println() {
        print();
        std::cout << std::endl;
    }


/*

// Example Program (https://cses.fi/problemset/task/1648)

#include <iostream>
#include <functional>
#include <vector>
#include "segmenttree.h"

int add(int a, int b) {
    return a + b;
}

int operation(int a, int b) {
    return (a < b) ? a : b;
}

int main()
{
    int n, q;
    std::cin >> n >> q;
    int arr[n];
    for (int i = 0; i < n; i++) std::cin >> arr[i];
    
    segtree<int> tree(arr, n, &add, 0);
    tree.println();
    
    int a, b, c;
    for (int i = 0; i < q; i++) {
        std::cin >> a >> b >> c;
        if (a == 1) {
            tree.update(b - 1, c);
            tree.println();
        }
        else std::cout << tree.query(b - 1, c - 1) << std::endl;
    }
    
    return 0;
}

*/


#endif
