#include <bits/stdc++.h>
using namespace std;

class Trie {
    class Node {
        public:
        char val;
        bool terminal;
        Node* next[256];
        
        Node() {
            val = 0;
            terminal = false;
            for (int i = 0; i < 256; i++) next[i] = nullptr;
        }
    };
    
    public:
    Node* root;
    Trie() {
        root = new Node();
    }
    
    void insert(string s) {
        Node* n = root;
        for (int i = 0; i < s.length(); i++) {
            if (n->next[s[i]] == nullptr) n->next[s[i]] = new Node();
            n = n->next[s[i]];
        }
        n->terminal = true;
    }
    
    bool contains(string s) {
        Node* n = root;
        for (int i = 0; i < s.length(); i++) {
            if (n->next[s[i]] == nullptr) return false;
            n = n->next[s[i]];
        }
        return n->terminal;
    }
    
    void remove(string s) {
        Node* n = root;
        for (int i = 0; i < s.length(); i++) {
            if (n->next[s[i]] == nullptr) return;
            n = n->next[s[i]];
        }
        n->terminal = false;
    }
};

int main()
{
    Trie trie;
    string keys[] = {"proto", "protogen", "primagen", "species", "terrain", "terraria", "terra", "terrarium", "prototype", "spectral", "deception", "darkness"};
    string queries[] = {"proto", "pro", "prime", "specs", "species", "terrarian", "terra", "spectrum", "deceptive", "protogen", "deceive", "dark"};
    
    for (auto i : keys) trie.insert(i);
    for (auto q : queries) cout << q << " = " << trie.contains(q) << endl;

    return 0;
}
