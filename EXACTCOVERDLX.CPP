#include <set>
#include <vector>
#include <iostream>
#include <queue>

// The hallowed paper that generates the methodologies for this bizarre abstraction
// https://arxiv.org/pdf/cs/0011047.pdf

typedef struct ListNode2 {
    public:
    bool val;
    struct ListNode2* left;
    struct ListNode2* right;
    struct ListNode2* up;
    struct ListNode2* down;
    struct ListNode2* colHead;
    
    int row;
    int col;
    
    int colNodes;
    
    ListNode2() : val(0), colNodes(0), row(-1), col(-1), left(nullptr), right(nullptr), up(nullptr), down(nullptr), colHead(nullptr) {
        
    }
} Node;

class DLX {
    public:
    std::vector<std::vector<Node>> list;
    Node* head;
    int n, m;
    
    std::vector<Node*> sol;
    std::set<int> hiddenCols;
    

    int inc(int x, int mod) {
        return (x + mod + 1) % mod;
    }

    int dec(int x, int mod) {
        return (x + mod + mod - 1) % mod;
    }

    void translate(std::vector<std::vector<bool>>& mat2) {
        n = mat2.size();
        m = mat2[0].size();
        
        std::vector<std::vector<bool>> mat(n + 1, std::vector<bool>(m, true)); // Generate a new matrix with all 1s in the first row
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) mat[i + 1][j] = mat2[i][j];
        } // This only increases the constant factor of the space complexity.
        
        head = new Node();
        head->colHead = head;
        head->colNodes = n + 1;
    
        // Node list[n][m];
        list = std::vector<std::vector<Node>>(n + 1, std::vector<Node>(m));
    
        int x, y;
    
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < m; j++) {
                list[i][j].row = i;
                list[i][j].col = j;
                if (i > 0) list[i][j].val = mat[i - 1][j];
            }
        }
    
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < m; j++) {
                if (!mat[i][j]) continue;
                if (i > 0) list[0][j].colNodes++;
                list[i][j].colHead = &(list[0][j]);
            
            
            
                x = i;
                y = j;
                do {
                    y = dec(y, m);
                } while (!mat[x][y] && y != j);
                list[i][j].left = &(list[x][y]);
            
                x = i;
                y = j;
                do {
                    y = inc(y, m);
                } while (!mat[x][y] && y != j);
                list[i][j].right = &(list[x][y]);
            
                x = i;
                y = j;
                do {
                    x = dec(x, n + 1);
                } while (!mat[x][y] && x != i);
                list[i][j].up = &(list[x][y]);
            
                x = i;
                y = j;
                do {
                    x = inc(x, n + 1);
                } while (!mat[x][y] && x != i);
                list[i][j].down = &(list[x][y]);
            
                /*
                std::cout << "N " << i << "/" << j << "\n";
                std::cout << "L " << list[i][j].left->row << "/" << list[i][j].left->col << "\n";
                std::cout << "U " << list[i][j].up->row << "/" << list[i][j].up->col << " " << "\n";
                std::cout << "R " << list[i][j].right->row << "/" << list[i][j].right->col << " " << "\n";
                std::cout << "D " << list[i][j].down->row << "/" << list[i][j].down->col << " " << "\n\n";
                */
            }
        }
        
        head->right = &(list[0][0]);
        head->left = &(list[0][m - 1]);
        list[0][0].left = head;
        list[0][m - 1].right = head;
    }
    
    void dispInfo(Node* n) {
        std::cout << n->row << "/" << n->col << "\n";
    }
    
    void dispCols() {
        std::cout << "[ ";
        for (int i = 0; i < m; i++) {
            if (hiddenCols.find(i) != hiddenCols.end()) std::cout << "X ";
            else std::cout << list[0][i].colNodes << " ";
        }
        std::cout << "]\n[ ";
        
        Node* node = head->right;
        while (node != head) {
            std::cout << node->colNodes << " ";
            node = node->right;
        }
        
        std::cout << "]\n";
    }

    void dispMat(bool showData) {
        std::queue<Node*> q;
        std::set<Node*> vis;
        
        vis.insert(head);
        q.push(head);
        while (q.size() > 0) {
            Node* node = q.front();
            q.pop();
            
            if (showData) std::cout << node->row << "/" << node->col << " = ";
            
            Node* arr[4] = {node->right, node->down, node->left, node->up};
            for (int i = 0; i < 4; i++) {
                if (arr[i] == nullptr) {
                    std::cout << "[]";
                    continue;
                }
                
                if (showData) std::cout << "[" << arr[i]->row << "/" << arr[i]->col << "]";
                
                if (vis.find(arr[i]) != vis.end()) continue;
                vis.insert(arr[i]);
                q.push(arr[i]);
                
            }
            
            if (showData) std::cout << "\n";
        }
        int res = 0;
        if (!showData) {
            for (auto i : vis) {
                std::cout << i->row << "/" << i->col << " ";
                res++;
                if (res % 16 == 0) std::cout << "\n";
            }
        }
    }
    
    void hide(Node* src) {
        Node* ch = src->colHead;
        
        hiddenCols.insert(ch->col);
        
        ch->left->right = ch->right;
        ch->right->left = ch->left;
        
        for (Node* row = ch->down; row != ch; row = row->down) {
            for (Node* right = row->right; right != row; right = right->right) {
                right->up->down = right->down;
                right->down->up = right->up;
                
                list[0][right->col].colNodes--;
                
                
            }
        }
    }
    
    void show(Node* src) {
        Node* ch = src->colHead;
        
        if (hiddenCols.find(ch->col) != hiddenCols.end()) hiddenCols.erase(hiddenCols.find(ch->col));
        
        for (Node* row = ch->up; row != ch; row = row->up) {
            for (Node* left = row->left; left != row; left = left->left) {
                left->up->down = left;
                left->down->up = left;
                
                list[0][left->col].colNodes++;
            }
        }
        
        ch->left->right = ch;
        ch->right->left = ch;
    }
    
    Node* minCol() {
        Node* node = head;
        Node* res = node->right; // Start to the right because the head node is merely a sentinel
        node = node->right->right;
        
        if (node == head) return res;
        
        do {
            if (node->colNodes < res->colNodes) res = node;
            node = node->right;
        } while (node != head);
        return res;
    }
    
    void dispSol() { // Display solution
        std::cout << "SOLUTION FOUND [ ";
        for (auto i : sol) std::cout << i->row - 1 << " "; // Prints out rows in the ORIGINAL NON-AUGMENTED MATRIX
        std::cout << "]\n";
    }
    
    // Always start with search(0)
    bool solve(int src) {
        if (head->right == head) {
            dispSol();
            return true;
        }
        
        Node* col = minCol();
        // std::cout << "MIN COL " << col->col << "\n";
        hide(col);
        
        bool retval = false;
        
        for (Node* row = col->down; row != col; row = row->down) {
            sol.push_back(row);
            for (Node* right = row->right; right != row; right = right->right) hide(right);
            
            retval = solve(src + 1) | retval;
            
            sol.pop_back();
            
            col = row->colHead;
            
            for (Node* left = row->left; left != row; left = left->left) show(left);
        }
        
        show(col);
        
        return retval;
    }
};

void cover_matdisp(std::vector<std::vector<bool>>& mat, std::set<int>& rows, std::set<int>& cols) {
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat[i].size(); j++) {
            if (rows.find(i) != rows.end()) std::cout << "X";
            else if (cols.find(j) != cols.end()) std::cout << "X";
            else std::cout << mat[i][j];
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::vector<std::vector<bool>> mat;
    DLX dlx;
    std::set<int> s;
    
    mat = std::vector<std::vector<bool>> (6, std::vector<bool>(7, false)); // Example from GeeksForGeeks
    
    mat[0][2] = mat[0][4] = mat[0][5] = true;
    mat[1][0] = mat[1][3] = mat[1][6] = true;
    mat[2][1] = mat[2][2] = mat[2][5] = true;
    mat[3][0] = mat[3][3] = true;
    mat[4][1] = mat[4][6] = true;
    mat[5][3] = mat[5][4] = mat[5][6] = true;
    
    cover_matdisp(mat, s, s);
    dlx.translate(mat);
    // dlx.dispMat(true);
    // dlx.dispCols();
    std::cout << dlx.solve(0) << "\n";
    
    mat = std::vector<std::vector<bool>> (6, std::vector<bool>(7, false)); // Another example from GeeksForGeeks
    mat[0][0] = mat[0][3] = mat[0][6] = true;
    mat[1][0] = mat[1][3] = true;
    mat[2][3] = mat[2][4] = mat[2][6] = true;
    mat[3][2] = mat[3][4] = mat[3][5] = true;
    mat[4][1] = mat[4][2] = mat[4][5] = mat[4][6] = true;
    mat[5][1] = mat[5][6] = true;
    
    cover_matdisp(mat, s, s);
    dlx.translate(mat);
    // dlx.dispMat(true);
    // dlx.dispCols();
    std::cout << dlx.solve(0) << "\n";
    
    mat = std::vector<std::vector<bool>>(4, std::vector<bool>(4, false)); // One per row
    for (int i = 0; i < 4; i++) mat[i][(i + 1) % 4] = 1;
    
    cover_matdisp(mat, s, s);
    dlx.translate(mat);
    // dlx.dispMat(true);
    // dlx.dispCols();
    std::cout << dlx.solve(0) << "\n";
    
    mat = std::vector<std::vector<bool>>(2, std::vector<bool>(4, true)); // Checkerboard
    mat[1][1] = mat[0][2] = false;
    mat[1][3] = mat[0][0] = false;
    
    cover_matdisp(mat, s, s);
    dlx.translate(mat);
    // dlx.dispMat(true);
    // dlx.dispCols();
    std::cout << dlx.solve(0) << "\n";
    
    
    std::cout << "END\n";

    return 0;
}

