#include <iostream>

// Based on CLRS

template <typename T>
class RBTree {
    public:
    class Node {
        public:
        bool color; // FALSE = BLACK
        T val;
        Node* left;
        Node* right;
        Node* p;
        
        Node() {
            color = false;
            val = 0;
            left = nullptr;
            right = nullptr;
            p = nullptr;
        }
        
        Node(Node* L, Node* R, Node* P) {
            color = false;
            val = 0;
            left = L;
            right = R;
            p = P;
        }
    };
    
    Node* NIL;
    Node* root;
    
    RBTree() {
        NIL = new Node();
    }
    
    bool isNull(Node* x) {
        return (x == nullptr) || (x == NIL);
    }
    
    bool isLeftChild(Node* x) {
        if (isNull(x->p)) return false;
        return x == x->p->left;
    }
    
    bool isRightChild(Node* x) {
        if (isNull(x->p)) return false;
        return x == x->p->right;
    }
    
    void lambda(Node* x) { // LEFT ROTATE X
        Node* y = x->right;
        x->right = y->left;
        if (!isNull(y->left)) y->left->p = x;
        y->p = x->p;
        if (isNull(x->p)) root = y;
        else if (x == x->p->left) x->p->left = y;
        else x->p->right = y;
        y->left = x;
        x->p = y;
    }
    
    void rho(Node* x) { // RIGHT ROTATE X
        Node* y = x->left;
        x->left = y->right;
        if (!isNull(y->right)) y->right->p = x;
        y->p = x->p;
        if (isNull(x->p)) root = y;
        else if (x == x->p->right) x->p->right = y;
        else x->p->left = y;
        y->right = x;
        x->p = y;
    }
    
    void transplant(Node* u, Node* v) { // Places subtrees under each other
        if (isNull(u->p)) root = v;
        else if (isLeftChild(u)) u->p->left = v;
        else u->p->right = v;
        v->p = u->p;
    }
    
    Node* find(T val) {
        Node* n = root;
        while (!isNull(n)) {
            if (n->val == val) {
                return n;
            }
            if (val < n->val) n = n->left;
            else n = n->right;
        }
        
        return NIL;
    }
    
    Node* minimum(Node* n) { 
        while (!isNull(n->left)) n = n->left;
        return n;
    }
    
    Node* successor(Node* n) {
        if (!isNull(n->right)) return minimum(n->right);
        Node* y = n->p;
        while (!isNull(y) && n == y->right) {
            n = y;
            y = y->p;
        }
        return y;
    }
    
    void disp() {
        std::cout << "[";
        dispUtil(root, 0);
        std::cout << "]";
    }
    
    void dispUtil(Node* x, int h) {
        if (isNull(x)) return;
        dispUtil(x->left, h + 1);
        std::cout << "(" << x->val << "/" << x->color << ":" << h << ")";
        dispUtil(x->right, h + 1);
    }
    
    void insert(T val) { // Insert a node first then rectify the errors
        Node* z = new Node(NIL, NIL, NIL);
        z->val = val;
        
        Node* y = NIL;
        Node* x = root;
        while (!isNull(x)) {
            y = x;
            if (z->val < x->val) x = x->left;
            else x = x->right;
        }
        z->p = y;
        if (isNull(y)) root = z;
        else if (z->val < y->val) y->left = z;
        else y->right = z;
        z->color = true;
        
        solve(z);
    }
    
    void solve(Node* z) {
        while (z->p->color) { // While the parent is red
            if (isLeftChild(z->p)) {
                Node* y = z->p->p->right; // Z's "uncle" is the node Y
                if (y->color) { // Uncle is red so we can just swap the colors of the previous 2 generations
                    z->p->color = false;
                    y->color = false;
                    z->p->p->color = true;
                    z = z->p->p; // Fixed stuff now go to grandparent
                }
                else {
                    if (!isLeftChild(z)) { // Rotate around z to fix the problem
                        z = z->p;
                        lambda(z);
                    }
                    // Then rotate around the grandparent to fix that problem
                    z->p->color = false;
                    z->p->p->color = true;
                    rho(z->p->p);
                }
            }
            else { // isRightChild(z->p)
                Node* y = z->p->p->left; // Uncle
                if (y->color) {
                    z->p->color = false;
                    y->color = false;
                    z->p->p->color = true;
                    z = z->p->p;
                }
                else {
                    if (!isRightChild(z)) {
                        z = z->p;
                        rho(z);
                    }
                    z->p->color = false;
                    z->p->p->color = true;
                    lambda(z->p->p);
                }
            }
        }
        
        root->color = false; // We can do this without any repercussions
        NIL->color = false; // Just in case
    }
    
    void erase(T val) {
        Node* n = find(val);
        if (isNull(n)) return;
        remove(n);
    }
    
    void remove(Node* z) {
        Node* y = z;
        y = z;
        bool y_original_color = y->color;
        Node* x;
        if (isNull(z->left)) {
            x = z->right;
            transplant(z, z->right);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->p == z) x->p = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (!y_original_color) resolve(x);
    }
    
    void resolve(Node* x) {
        while (x != root && !x->color) {
            if (isLeftChild(x)) {
                Node* w = x->p->right;
                if (w->color) { // Sibling red
                    w->color = false;
                    x->p->color = true;
                    lambda(x->p);
                    w = x->p->right;
                }
                if (!w->left->color && !w->right->color) { // We can swap here
                    w->color = true;
                    x = x->p;
                }
                else {
                    if (!w->right->color) { // Need to rotate
                        w->left->color = false;
                        w->color = true;
                        rho(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = false;
                    w->right->color = false;
                    lambda(x->p);
                    x = root;
                }
            }
            else {
                Node* w = x->p->left;
                if (w->color) { // Sibling red
                    w->color = false;
                    x->p->color = true;
                    rho(x->p);
                    w = x->p->left;
                }
                if (!w->right->color && !w->left->color) { // We can swap here
                    w->color = true;
                    x = x->p;
                }
                else {
                    if (!w->left->color) { // Need to rotate
                        w->right->color = false;
                        w->color = true;
                        lambda(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = false;
                    w->left->color = false;
                    rho(x->p);
                    x = root;
                }
            }
            x->color = false;
            root->color = false;
            NIL->color = false;
        }
    }
};


int main() {
    RBTree<int> protogen;
    for (int i = 0; i < 64; i++) protogen.insert(i);
    protogen.disp();
    std::cout << "\n";
    for (int i = 2; i < 64; i += 4) protogen.erase(i);
    protogen.disp();
    std::cout << "\n";
}
