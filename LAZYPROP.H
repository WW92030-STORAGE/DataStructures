#ifndef SEGTREE_H
#define SEGTREE_H

#include <vector>
#include <iostream>
#include <string>
#include <cstdint>

template <typename T>
class segtree {
    public:
    std::vector<T> arr;
    std::vector<T> tree;
    std::vector<T> addcache;
    std::vector<T> setcache;
    int64_t n;
    int64_t size;
    
    T identity = 0;
    
    T combine(T a, T b) { // This binary operation is what the tree uses for querying. It is unrelated to any existing operations (including overloaded ones).
        return a + b;
    }
    
    int64_t ceillog(int64_t x) {
        int64_t pwr = 1;
        int64_t ex = 0;
        while (pwr < x) {
            pwr = pwr + pwr;
            ex++;
        }
        return ex;
    }
    
    void reconcile(int x) {
        if (x >= size) {
            arr[x - size] = tree[x];
            return;
        }
        tree[x] = combine(tree[x * 2], tree[x * 2 + 1]);
    }
    
    segtree(std::vector<T> v) {
        n = v.size();
        arr = std::vector<T>(n);
        for (int64_t i = 0; i < n; i++) arr[i] = v[i];
        
        size = 1<<ceillog(n);
        tree = std::vector<T>(2 * size, identity);
        addcache = std::vector<T>(2 * size, identity);
        setcache = std::vector<T>(2 * size, identity);
        for (int64_t i = 0; i < n; i++) tree[size + i] = arr[i];
        for (int64_t i = size - 1; i > 0; i--) reconcile(i);
    }
    
    T queryutil(int64_t a, int64_t b, int64_t k, int64_t x, int64_t y) {
        // std::cout << k << " ";
        if (b < x || a > y) return identity; // If [x, y] is completely outside [a, b] then we return 0
        if (a <= x && y <= b) return tree[k]; // If [x, y] is completely inside [a, b] we simply use the cached value at the tree index
        propagate(k, y - x + 1);
        int64_t d = x + (y - x) / 2;
        return combine(queryutil(a, b, 2 * k, x, d), queryutil(a, b, 2 * k + 1, d + 1, y)); // Otherwise recur on the child nodes
    }
    
    T query(int64_t a, int64_t b) {
        return queryutil(a, b, 1, 0, size - 1);
    }
    
    T update(int64_t x, T k) {
        tree[x + size] = k;
        arr[x] = k;
        
        int64_t i = x + size;
        while (i > 0) {
            i = i / 2;
            reconcile(i);
        }
        
        return k;
    }
    
    void inctree(int64_t x, T v) {
        tree[x] += v;
        if (x >= size) arr[x - size] += v;
    }
    
    void settree(int64_t x, T v) {
        tree[x] = v;
        if (x >= size) arr[x - size] = v;
    }
    
    void propagate(int64_t i, int64_t rangesize) {
        if (i >= size) return;
        
        if (setcache[i] != 0) { // Propagate the set. We simply propagate the setted value to the children.
            setcache[2 * i] = setcache[i];
            setcache[2 * i + 1] = setcache[i]; // propagate to children
            settree(2 * i, (rangesize / 2) * setcache[i]);
            settree(2 * i + 1, (rangesize / 2) * setcache[i]); // because we're blatantly settingvalues we just do this to the node of the rangesize
            addcache[2 * i] = 0;
            addcache[2 * i + 1] = 0; // clear the addcaches
            setcache[i] = 0; // clear the current node
            return;
        }
        
        if (addcache[i] != 0) { // Propagate the add
            for (int index = 0; index < 2; index++) {
                // case 1: if the set cache is zero we simply add
                if (setcache[2 * i + index] == 0) addcache[2 * i + index] += addcache[i];
                else { // case 2: otherwise we update the set cache and clear the add cache
                    setcache[2 * i + index] += addcache[i];
                    addcache[2 * i + index] = 0;
                }
            }
            
            // update the node values
            inctree(2 * i, (rangesize / 2) * addcache[i]);
            inctree(2 * i + 1, (rangesize / 2) * addcache[i]);
            addcache[i] = 0;
        }
    }
    
    // [a ... b] is the added range. [x ... y] is the range of the node T[k]
    T addrangeutil(int64_t a, int64_t b, int64_t k, int64_t x, int64_t y, T v) {
        if (b < x || a > y) return v; // do nothing if [x, y] is completely outside [a, b]
        if (a <= x && y <= b) { // If [x, y] is completely inside [a, b] we cache the value so the tree knows that every element in this range needs to be changed
            inctree(k, v * (y - x + 1));
            // Interaction step: we might also have to change the setcache
            if (setcache[k] == 0) addcache[k] += v;
            else setcache[k] += v;
            return v;
        }
        
        // Otherwise [x, y] intersects [a, b] but is not completely inside it.
        propagate(k, y - x + 1); // send the values downwards
        int64_t d = x + (y - x) / 2;
        addrangeutil(a, b, 2 * k, x, d, v);
        addrangeutil(a, b, 2 * k + 1, d + 1, y, v);
        reconcile(k);
        return v;
    }
    
    T setrangeutil(int64_t a, int64_t b, int64_t k, int64_t x, int64_t y, T v) {
        if (b < x || a > y) return v; // do nothing if [x, y] is completely outside [a, b]
        if (a <= x && y <= b) { // If [x, y] is completely inside [a, b] we cache the value so the tree knows that every element in this range needs to be changed
            settree(k, v * (y - x + 1));
            // Interaction step: we might also have to change the caches
            setcache[k] = v;
            addcache[k] = 0;
            return v;
        }
        
        // Otherwise [x, y] intersects [a, b] but is not completely inside it.
        propagate(k, y - x + 1); // send the values downwards
        int64_t d = x + (y - x) / 2;
        setrangeutil(a, b, 2 * k, x, d, v);
        setrangeutil(a, b, 2 * k + 1, d + 1, y, v);
        reconcile(k);
        return v;
    }
    
    T addrange(int64_t a, int64_t b, T v) { return addrangeutil(a, b, 1, 0, size - 1, v); }
    
    T setrange(int64_t a, int64_t b, T v) { return setrangeutil(a, b, 1, 0, size - 1, v); }
    
    void flush() { // Clears all caches
        while (true) {
        int64_t left = size * 2;
        int64_t right = -1;
        for (auto i : addcache) {
            left = std::min(left, i);
            right = std::max(right, i);
        }
        for (auto i : setcache) {
            left = std::min(left, i);
            right = std::max(right, i);
        }
        if (right < 0) return;
        addrange(left, right, 0);
        }
    }
    
    std::string toString() {
        std::string res = "[";
        for (int64_t i = 0; i < tree.size(); i++) {
            if (i > 0) res = res + " ";
            res = res + std::to_string(tree[i]);
            if (i == size - 1) res = res + " |";
        }
        res = res + "][";
        for (int64_t i = 0; i < addcache.size(); i++) {
            if (i > 0) res = res + " ";
            res = res + std::to_string(addcache[i]);
        }
        res = res + "][";
        for (int64_t i = 0; i < setcache.size(); i++) {
            if (i > 0) res = res + " ";
            res = res + std::to_string(setcache[i]);
        }
        res = res + "]";
        return res;
    }
    
    void disp() {
        std::cout << toString() << "\n";
    }
};

/*

#include <bits/stdc++.h>

using namespace std;

void csesprob() {
    int64_t n, q;
    cin >> n >> q;
    
    vector<int64_t> v(n);
    for (int64_t i = 0; i < n; i++) cin >> v[i];
    
    segtree<int64_t> tree(v);
    tree.identity = (int64_t)(0);
    
    // tree.disp();
    
    int64_t x, a, b, c;
    for (int64_t i = 0; i < q; i++) {
        cin >> x >> a >> b;
        a--;
        b--;
        // cout << x << " " << a << " " << b << " = ";
        if (x == 1) {
            cin >> c;
            tree.addrange(a, b, c);
        }
        else if (x == 2) {
            cin >> c;
            tree.setrange(a, b, c);
        }
        else cout << tree.query(a, b) << endl;
        
        // tree.disp();
    }
    
    return;
}

int main()
{
    csesprob();

    return 0;
}

*/

#endif
