#include <iostream>
#include <map>
#include <unordered_map>


template <typename K, typename V>
class Node {
    public:
    K key;
    V val;
    Node* prev;
    Node* next;
    
    Node() {
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
class LRUCache {
    public:
    int capacity;
    int size;
    
    std::map<K, Node<K, V>*> hashMap; // can replace with unordered_map if your types are hashable
    
    Node<K, V>* head = new Node<K, V>();
    Node<K, V>* tail = new Node<K, V>();
    LRUCache(int n) {
        size = 0;
        capacity = n;
        head->next = tail;
        tail->prev = head;
    }
    
    V get(K key) {
        Node<K, V>* node = search(key);
        if (node == nullptr) return -1;
        
        remove(node);
        add(node);
        return node->val;
    }
    
    void put(K key, V val) {
        Node<K, V>* node = search(key);
        if (node != nullptr) remove(node);
        
        node = new Node<K, V>(key, val);
        add(node);
        
        if (size > capacity) {
            Node<K, V>* del = head->next;
            remove(del);
            
        }
    }
    
    Node<K, V>* search(K k) {
        if (hashMap.find(k) != hashMap.end()) return hashMap.at(k);
        Node<K, V>* n = head->next;
        while (n != tail) {
            if (n->key == k) return n;
            n = n->next;
        }
        return nullptr;
    }
    
    void add(Node<K, V>* node) {
        Node<K, V>* prevEnd = tail->prev;
        prevEnd->next = node;
        node->prev = prevEnd;
        node->next = tail;
        tail->prev = node;
        hashMap.insert({node->key, node});
        size++;
    }
    
    void remove(Node<K, V>* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        hashMap.erase(hashMap.find(node->key));
        size--;
    }
};

#include <iostream>
using namespace std;

int main() // Leetcode sample testcase
{
    LRUCache<int, int> lRUCache(2);
    lRUCache.put(1, 1); // cache is {1=1}
    lRUCache.put(2, 2); // cache is {1=1, 2=2}
    cout << lRUCache.get(1) << endl;    // return 1
    lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
    cout << lRUCache.get(2) << endl;    // returns -1 (not found)
    lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
    cout << lRUCache.get(1) << endl;    // return -1 (not found)
    cout << lRUCache.get(3) << endl;    // return 3
    cout << lRUCache.get(4) << endl;    // return 4
    
    
    LRUCache<int, int> cache(10000);
    
    cout << "SECOND CACHE TEST\n";
    
    for (int i = 0; i < 100000; i += 7) cache.put(i, i);
    
    for (int i = 0; i < 100000; i++) {
        if (cache.get(i) >= 0) cout << i << " " << i % 7 << endl;
    }
    
    cout << "END OF LRU TEST\n";
    
    return 0;
}






