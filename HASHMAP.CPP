template <typename K, typename V>
class Node {
    public:
    K key;
    V val;
    Node* prev;
    Node* next;
    
    Node() {
        key = 0;
        val = 0;
        prev = nullptr;
        next = nullptr;
    }
    
    Node(K k, V v) {
        key = k;
        val = v;
        prev = nullptr;
        next = nullptr;
    }
};

template <typename K, typename V>
class HashMap {
    public:
    static const int HASHES = 10007;
    Node<K, V>* buckets[HASHES];
    
    HashMap() {
        for (int i = 0; i < HASHES; i++) buckets[i] = nullptr;
    }
    
    unsigned long xhash(K key) {
        unsigned long res = (unsigned long)(reinterpret_cast<int>(key));
        res = (82589933 * res) + 290832;
        
        return res % HASHES;
    }
    
    void insert(K k, V v) {
        int position = xhash(k);
        
        Node<K, V>* node = buckets[position];
        while (node != nullptr) {
            if (node->key == k) {
                node->val = v;
                return;
            }
            if (node->next == nullptr) break;
            node = node->next;
        }
        node = buckets[position];
        Node<K, V>* newnode = new Node<K, V>(k, v);
        newnode->next = node;
        if (node != nullptr) node->prev = newnode;
        buckets[position] = newnode;
    }
    
    void add(K key, V val) {
        insert(key, val);
    }
    
    void put(K key, V val) {
        insert(key, val);
    }
    
    bool containsKey(K k) {
        int position = xhash(k);
        
        Node<K, V>* node = buckets[position];
        while (node != nullptr) {
            if (node->key == k) return true;
            node = node->next;
        }
        return false;
    }
    
    V get(K k) {
        int position = xhash(k);
        
        Node<K, V>* node = buckets[position];
        while (node != nullptr) {
            if (node->key == k) return node->val;
            node = node->next;
        }
        return 0;
    }
    
    bool remove(K k) {
        int position = xhash(k);
        
        Node<K, V>* node = buckets[position];
        while (node != nullptr) {
            if (node->key == k) {
                if (node->prev == nullptr) {
                    if (node->next != nullptr) node->next->prev = nullptr;
                    buckets[position] = node->next;
                    delete node;
                    return true;
                }
                if (node->prev != nullptr) node->prev->next = node->next;
                if (node->next != nullptr) node->next->prev = node->prev;
                delete node;
                return true;
            }
            node = node->next;
        }
        return false;
    }
};

#include <iostream>
using namespace std;

int main()
{    
    HashMap<int, int> hm;
    int N = 300000;
    
    for (int i = 0; i < N; i++) {
        hm.put(i, 100 * i);
    }
    
    long res = 0;
    
    
    
    for (int i = 0; i < N; i++) res += hm.get(i);
    cout << res / 100 << endl;
    
    for (int i = 0; i < N; i++) {
        if (i % 100 == 0) continue;
        hm.remove(i);
    }
    
    for (int i = 0; i < N; i++) {
        if (hm.containsKey(i)) cout << hm.get(i) << " ";
    }
    

    return 0;
}
