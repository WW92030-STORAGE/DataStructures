#include <iostream>

using namespace std;

template <typename K, typename V>
class Node {
    public:
    K key;
    V val;
    Node* prev;
    Node* next;
    
    Node() {
    }
    
    Node(K k, V v) {
        key = k;
        val = v;
    }
};

template <typename K, typename V>
class LRUCache {
    public:
    int capacity;
    int size;
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
    
    Node<K, V>* search(K key) {
        Node<K, V>* n = head;
        while (n != nullptr) {
            if (n->key == key) return n;
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
        size++;
    }
    
    void remove(Node<K, V>* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        size--;
    }
};

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

    return 0;
}
