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
		for (int i = size; i < 2 * size; i++) tree[i] = arr[i - size]; // range length 1
		
		for (int i = size - 1; i > 0; i--) {
		    tree[i] = combine(tree[2 * i], tree[2 * i + 1]);
		}
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
