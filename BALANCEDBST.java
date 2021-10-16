import java.util.*;

public class BALANCEDBST { // Red-Black Tree that stores integers for now (similar to a treeset)
    static class rbtree {
        class node {
            int value;
            node left;
            node right;
            node parent;
            int state;
            
            public node(int x) {
                super();
                value = x;
                left = null;
                right = null;
                parent = null;
                state = 1; // 0 black 1 red
            }
        }
        
        private node root;
        
        public rbtree() {
            super();
            root = null;
        }
        
        node lambda(node n) {
            node x = n.right;
            node y = x.left;
            x.left = n;
            n.right = y;
            n.parent = x;
            if (y != null) y.parent = n;
            return x;
        }
        
        node rho(node n) {
            node x = n.left;
            node y = x.right;
            x.right = n;
            n.left = y;
            n.parent = x;
            if (y != null) y.parent = n;
            return x;
        }
        
        private boolean ll, lr, rl, rr;
        
        node insert(node root, int x) {
            boolean trustedthepillars = false;
            if (root == null) return new node(x);
            else if (x < root.value) {
                root.left = insert(root.left, x);
                root.left.parent = root;
                if (root != this.root) {
                    if (root.state == 1 && root.left.state == 1) trustedthepillars = true;
                }
            }
            else {
                root.right = insert(root.right, x);
                root.right.parent = root;
                if (root != this.root) {
                    if (root.state == 1 && root.right.state == 1) trustedthepillars = true;
                }
            }
            
            if (ll) {
                root = lambda(root);
                root.state = 0;
                root.left.state = 1;
                ll = false;
            }
            else if (rr) {
                root = rho(root);
                root.state = 0;
                root.right.state = 1;
                rr = false;
            }
            else if (rl) {
                root.right = rho(root.right);
                root.right.parent = root;
                root = lambda(root);
                root.state = 0;
                root.left.state = 1;
                rl = false;
            }
            else if (lr) {
                root.left = lambda(root.left);
                root.left.parent = root;
                root = rho(root);
                root.state = 0;
                root.right.state = 1;
                lr = false;
            }
            
            if (trustedthepillars) {
                if (root.parent.right == root) {
                    if (root.parent.left == null || root.parent.left.state == 0) {
                        if (root.left != null && root.left.state == 1) rl = true;
                        else if (root.right != null && root.right.state == 1) ll = true;
                    }
                    else {
                        root.parent.left.state = 0;
                        root.state = 0;
                        if (root.parent != this.root) root.parent.state = 1;
                    }
                }
                else {
                    if (root.parent.right == null || root.parent.right.state == 0) {
                        if (root.left != null && root.left.state == 1) rr = true;
                        else if (root.right != null && root.right.state == 1) lr = true;
                    }
                    else {
                        root.parent.right.state = 0;
                        root.state = 0;
                        if (root.parent != this.root) root.parent.state = 1;
                    }
                }
                trustedthepillars = false;
            }
            return root;
        }
        
        public int add(int x) {
            if (this.root == null) {
                this.root = new node(x);
                this.root.state = 0;
            }
            else this.root = insert(this.root, x);
            
            return x;
        }
        
        // here we go into the abyss
        
        public node leftmost(node n) { // leaf node in subtree
            node temp = n;
            while (temp.left != null) temp = temp.left;
            return temp;
        }
        
        public int count(node n) {
            int count = 0;
            if (n.left != null) count++;
            if (n.right != null) count++;
            return count;
        }
        
        public int redcount(node n) {
            int count = 0;
            if (n.left != null) count += n.left.state;
            if (n.right != null) count += n.right.state;
            return count;
        }
        
        node adjacent(node n) {
            if (n.parent == null) return null;
            if (n == n.parent.left) return n.parent.right;
            return n.parent.left;
        }
        
        node replacement(node n) { // what node replaces n if it is deleted?
            if (count(n) == 2) return leftmost(n.right);
            if (n.left != null) return n.left;
            if (n.right != null) return n.right;
            return null;
        }
        
        void solve(node n) { // solve the changing black height of the nodes
            if (n == root) return;
            
            node other = adjacent(n);
            node parent = n.parent;
            
            if (other == null) solve(parent);
            else {
                if (other.state == 1) {
                    parent.state = 1;
                    other.state = 0;
                    if (other == other.parent.left) rho(parent);
                    else lambda(parent);
                    solve(n);
                }
                else {
                    if (redcount(other) > 0) {
                        if (other.left != null && other.left.state == 1) {
                            if (other == other.parent.left) {
                                other.left.state = other.state;
                                other.state = parent.state;
                                rho(parent);
                            }
                            else {
                                other.left.state = parent.state;
                                rho(other);
                                lambda(parent);
                            }
                        }
                        else {
                            if (other == other.parent.left) {
                                other.right.state = parent.state;
                                lambda(other);
                                rho(parent);
                            }
                            else {
                                other.right.state = other.state;
                                other.state = parent.state;
                                lambda(parent);
                            }
                        }
                        parent.state = 0;
                    }
                    else {
                        other.state = 1;
                        if (parent.state == 0) solve(parent);
                        else parent.state = 0;
                    }
                }
            }
        }
        
        void delete(node n) {
            node rep = replacement(n);
            node parent = n.parent;
            
            boolean b2 = (n.state == 0) && (rep == null || rep.state == 0);
            
            if (rep == null) { // implying that n is a leaf node
                if (n == root) root = null;
                else {
                    if (b2) solve(n);
                    else {
                        node per = adjacent(n);
                        if (per != null) per.state = 1;
                    }
                    
                    if (n == parent.left) parent.left = null;
                    else parent.right = null;
                }
                return;
            }
            
            // at this point we have ruled out the case where n is the leaf node
            
            if (count(n) == 1) {
                if (n == root) {
                    n.value = rep.value;
                    n.left = n.right = null;
                }
                else {
                    if (n == parent.left) parent.left = rep;
                    else parent.right = rep;
                    rep.parent = parent;
                    if (b2) solve(rep);
                    else rep.state = 0;
                }
                return;
            }
            
            int temp = n.value;
            n.value = rep.value;
            rep.value = temp;
            
            delete(rep);
        }
        
        node search(int x) {
            node res = root;
            while (res != null) {
                node l = res.left;
                node r = res.right;
                if (x < res.value) {
                    if (l == null) break;
                    res = l;
                }
                else if (x > res.value) {
                    if (r == null) break;
                    res = r;
                }
                else break;
            }
            
            return res;
        }
        
        public boolean contains(int x) {
            return search(x).value == x;
        }
        
        public int remove(int x) {
            if (root == null) return x;
            node end = search(x);
            if (end.value == x) delete(end);
            return x;
        }
        
        public void inorder(node n) {
            if (n == null) return;
            inorder(n.left);
            System.out.print("[" + n.value + " | " + n.state + "] ");
            inorder(n.right);
        }
        
        public void print() {
            inorder(root);
            System.out.print("\n");
        }
    }
    
    public static void main(String[] args) {
        rbtree tree = new rbtree();
        ArrayList<Integer> list = new ArrayList<Integer>();
        TreeSet<Integer> set = new TreeSet<Integer>();
        
        while (list.size() < 20) {
            int rand = (int)(Math.random() * 1024);
            if (!set.contains(rand)) {
                tree.add(rand);
                list.add(rand);
            }
        }
        
    //    System.out.println(list);
    //    tree.print();
        
        long start = System.nanoTime();
        
        tree = new rbtree();
        
        int n = 200000;
        
        for (int i = 0; i < n; i++) tree.add(i); // adding some values
        
        System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
        
        for (int i = 0; i < n; i += 3) tree.remove(i); // removing some values
        
        System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
        
    //    tree.print();
    
        System.out.println(tree.contains(600)); // Multiples of 3 should be eliminated ... hence 600 is not in the set however 601 is.
        System.out.println(tree.contains(600 + 1)); // testing for some values
        
        System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
    }
}




